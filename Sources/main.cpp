//
//  main.cpp
//  Pass Man
//
//  Created by Indi Kernick on 9/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include <iostream>
#include "app.hpp"

int main(int, const char **) {
  try {
    runApp();
  } catch (std::exception &e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return 0;
}
