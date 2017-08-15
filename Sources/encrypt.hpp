//
//  encrypt.hpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef encrypt_hpp
#define encrypt_hpp

#include <string>
#include <experimental/string_view>

std::string decryptFile(uint64_t, std::experimental::string_view);
void encryptFile(uint64_t, std::experimental::string_view, std::experimental::string_view);

#endif
