//
//  parse.cpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "parse.hpp"

/*

file
  entry
    password name
    0
    password
    0

*/

Passwords readPasswords(std::experimental::string_view decryptedFile) {
  Passwords passwords;
  
  constexpr size_t npos = std::experimental::string_view::npos;
  
  auto getStr = [&decryptedFile] () -> std::experimental::string_view {
    const size_t nullChar = decryptedFile.find_first_of('\0');
    if (nullChar == npos) {
      return {};
    }
    const auto str = decryptedFile.substr(0, nullChar);
    decryptedFile.remove_prefix(nullChar + 1);
    return str;
  };
  
  while (true) {
    const auto key = getStr();
    if (key.empty()) break;
    const auto val = getStr();
    if (val.empty()) throw std::runtime_error("Parse failed");
    
    passwords.emplace(key.to_string(), val.to_string());
  }
  
  return passwords;
}

std::string writePasswords(const Passwords &passwords) {
  std::string decryptedFile;
  
  const auto end = passwords.cend();
  for (auto p = passwords.cbegin(); p != end; ++p) {
    decryptedFile.append(p->first);
    decryptedFile.push_back('\0');
    decryptedFile.append(p->second);
    decryptedFile.push_back('\0');
  }
  
  return decryptedFile;
}
