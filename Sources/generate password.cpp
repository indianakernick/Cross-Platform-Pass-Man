//
//  generate password.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "generate password.hpp"

#include <random>

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
