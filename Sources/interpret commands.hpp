//
//  interpret commands.hpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#ifndef interpret_commands_hpp
#define interpret_commands_hpp

#include <vector>
#include "passwords.hpp"
#include <experimental/optional>
#include <experimental/string_view>

class CommandInterpreter {
public:
  CommandInterpreter();
  ~CommandInterpreter();
  
  void prefix();
  void interpret(std::experimental::string_view);
  bool shouldContinue() const;

private:
  size_t key = 0;
  std::string file;
  std::experimental::optional<Passwords> passwords;
  std::vector<std::string> searchResults;
  bool quit = false;
  
  void openCommand(std::experimental::string_view);
  void clearCommand();
  void flushCommand() const;
  void quitCommand();
  
  void quitNoFlushCommand();
  void dumpCommand(std::experimental::string_view);
  
  void expectInit() const;
  
  void searchCommand(std::experimental::string_view);
  void listCommand() const;
  void countCommand() const;
  void genCommand(std::experimental::string_view) const;
  
  Passwords::iterator uniqueSearch(std::experimental::string_view);
  
  void createCommand(std::experimental::string_view);
  void createGenCommand(std::experimental::string_view);
  void changeCommand(std::experimental::string_view);
  void changeSCommand(std::experimental::string_view);
  
  Passwords::iterator getFromIndex(size_t);
  
  void renameCommand(std::experimental::string_view);
  void renameSCommand(std::experimental::string_view);
  void getCommand(std::experimental::string_view);
  void getSCommand(std::experimental::string_view);
  
  void copyCommand(std::experimental::string_view);
  void copySCommand(std::experimental::string_view);
  void remCommand(std::experimental::string_view);
  void remSCommand(std::experimental::string_view);
};

#endif
