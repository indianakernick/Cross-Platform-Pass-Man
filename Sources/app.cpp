//
//  app.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "app.hpp"

#include <ctime>
#include <thread>
#include <iostream>
#include "interpret commands.hpp"

//The number of seconds of inactivity before the database is closed
constexpr uint64_t TIMEOUT_SEC = 2 * 60;

uint64_t getTimeSec() {
  return static_cast<uint64_t>(std::time(nullptr));
}

void runApp() {
  std::cin.exceptions(0xFF);
  std::cout.exceptions(0xFF);

  //the time that the last command was executed
  std::atomic<uint64_t> lastCommandTime(0);
  //the database is being closed on the helper thread
  std::atomic<bool> closingDatabase(false);
  CommandInterpreter interpreter;
  char command[1024];
  
  std::thread([
    &lastCommandTime,
    &closingDatabase,
    &interpreter
  ] () {
    try {
      while (true) {
        while (getTimeSec() - lastCommandTime < TIMEOUT_SEC) {
          std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        closingDatabase = true;
        interpreter.sessionExpired();
        closingDatabase = false;
      }
    } catch (std::exception &e) {
      std::cout << e.what() << '\n';
    }
  }).detach();
  
  do {
    interpreter.prefix();
    std::cin.getline(command, sizeof(command));
    //wait until the helper thread has finished closing the database
    while (closingDatabase);
    lastCommandTime = getTimeSec();
    
    try {
      interpreter.interpret(command);
    } catch (std::exception &e) {
      std::cout << e.what() << '\n';
    }
  } while (interpreter.shouldContinue());
}
