//
//  write to clipboard.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write to clipboard.hpp"

#include "../dependencies/clip/clip.h"

void writeToClipboard(const std::string &string) {
  if (!clip::set_text(string)) {
    throw std::runtime_error("Failed to write to clipboard");
  }
}