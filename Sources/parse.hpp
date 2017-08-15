//
//  parse.hpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef parse_hpp
#define parse_hpp

#include "passwords.hpp"
#include <experimental/string_view>

Passwords readPasswords(std::experimental::string_view);
std::string writePasswords(const Passwords &);

#endif
