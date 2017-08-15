//
//  app.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app.hpp"

#include <iostream>
#include "interpret commands.hpp"

void runApp() {
  std::cin.exceptions(0xFF);
  std::cout.exceptions(0xFF);

  CommandInterpreter interpreter;
  char command[1024];
  do {
    interpreter.prefix();
    std::cin.getline(command, sizeof(command) / sizeof(char));
    try {
      interpreter.interpret(command);
    } catch (std::exception &e) {
      std::cout << e.what() << '\n';
    }
  } while (interpreter.shouldContinue());
}
