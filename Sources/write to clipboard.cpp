//
//  write to clipboard.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write to clipboard.hpp"

#include <SDL2/SDL_clipboard.h>

void writeToClipboard(const std::string &string) {
  if (SDL_SetClipboardText(string.c_str()) == -1) {
    throw std::runtime_error("Failed to write to clipboard");
  }
}