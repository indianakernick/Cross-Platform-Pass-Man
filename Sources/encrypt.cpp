//
//  encrypt.cpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "encrypt.hpp"

#include <random>

namespace {
  using File = std::unique_ptr<std::FILE, decltype(&std::fclose)>;

  File openFile(const char *path, const char *options) {
    std::FILE *file = std::fopen(path, options);
    if (file == nullptr) {
      throw std::runtime_error(std::string("Failed to open file \"") + path + "\"");
    } else {
      return {file, &std::fclose};
    }
  }
}

std::string decryptFile(
  const uint64_t key,
  const std::experimental::string_view path
) {
  File file = openFile(path.data(), "rb");
  
  std::mt19937_64 gen(key);
  std::uniform_int_distribution<uint8_t> dist;
  
  std::string str;
  
  std::fseek(file.get(), 0, SEEK_END);
  const size_t fileSize = std::ftell(file.get());
  str.reserve(fileSize);
  std::rewind(file.get());
  
  int b = std::fgetc(file.get());
  while (b != EOF) {
    str.push_back(dist(gen) ^ uint8_t(b));
    b = std::fgetc(file.get());
  }
  
  if (str.size() != fileSize) {
    throw std::runtime_error("File read error");
  }
  
  //possible unaligned read
  const size_t strHash = *reinterpret_cast<const size_t *>(str.data() + str.size() - sizeof(size_t));
  int i = sizeof(size_t);
  while (i--) str.pop_back();
  
  //Confirm MAC
  std::hash<std::experimental::string_view> hasher;
  
  if (hasher(str) != strHash) {
    throw std::runtime_error("Decryption authentication failed");
  }
  
  return str;
}

void encryptFile(
  const uint64_t key,
  const std::experimental::string_view path,
  const std::experimental::string_view str
) {
  File file = openFile(path.data(), "wb");
  
  std::mt19937_64 gen(key);
  std::uniform_int_distribution<uint8_t> dist;
  
  for (const char c : str) {
    std::fputc(dist(gen) ^ c, file.get());
  }
  
  //MAC - authenticate then encrypt is secure when used with a stream cipher
  std::hash<std::experimental::string_view> hasher;
  const size_t hash = hasher(str);
  const uint8_t *hashBytes = reinterpret_cast<const uint8_t *>(&hash);
  const uint8_t *const hashBytesEnd = hashBytes + sizeof(size_t);
  
  while (hashBytes != hashBytesEnd) {
    std::fputc(dist(gen) ^ *hashBytes, file.get());
    ++hashBytes;
  }
}

uint64_t generateKey(const std::experimental::string_view phrase) {
  const std::hash<std::experimental::string_view> hasher;
  return hasher(phrase);
}

namespace {
  constexpr char NUM_CHARS = 10;
  constexpr char ALPHA_CHARS = 26;
  
  char mapChar(const char c) {
    if (c < NUM_CHARS) {
      return '0' + c;
    } else if (c < NUM_CHARS + ALPHA_CHARS) {
      return 'A' + (c - NUM_CHARS);
    } else { // c < NUM_CHARS + 2 * ALPHA_CHARS
      return 'a' + (c - NUM_CHARS - ALPHA_CHARS);
    }
  }
}

std::string generatePassword(const size_t size) {
  static std::random_device gen;
  std::uniform_int_distribution<char> dist(0, NUM_CHARS + 2 * ALPHA_CHARS);
  std::string password;
  while (password.size() < size) {
    password.push_back(mapChar(dist(gen)));
  }
  return password;
}