//
//  write to clipboard.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "write to clipboard.hpp"

void writeToClipboard(const std::experimental::string_view string) {
  std::unique_ptr<std::FILE, decltype(&pclose)> pipe(popen("pbcopy", "w"), &pclose);
  if (pipe == nullptr) {
    throw std::runtime_error("Failed to open pipe to pbcopy");
  }
  if (std::fwrite(string.data(), string.size(), 1, pipe.get()) != 1) {
    throw std::runtime_error("Failed to write string to pbcopy");
  }
}
