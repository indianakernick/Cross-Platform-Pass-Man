//
//  parse.hpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef parse_hpp
#define parse_hpp

#include <unordered_map>
#include <experimental/string_view>

using Passwords = std::unordered_map<std::string, std::string>;

Passwords readPasswords(std::experimental::string_view);
std::string writePasswords(const Passwords &);

#endif
