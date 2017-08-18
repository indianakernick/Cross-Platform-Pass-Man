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
#include "parse.hpp"
#include <experimental/optional>
#include <experimental/string_view>

class CommandInterpreter {
public:
  CommandInterpreter();
  ~CommandInterpreter();
  
  void prefix();
  void interpret(std::experimental::string_view);
  bool shouldContinue() const;
  void timeout();

private:
  size_t key = 0;
  std::string file;
  std::experimental::optional<Passwords> passwords;
  std::vector<std::string> searchResults;
  bool quit = false;
  
  void openCommand(std::experimental::string_view);
  void closeCommand();
  void clearCommand();
  void flushCommand() const;
  void quitCommand();
  
  void quitNoFlushCommand();
  void dumpCommand(std::experimental::string_view);
  void unDumpCommand(std::experimental::string_view);
  
  void expectInit() const;
  
  void searchCommand(std::experimental::string_view);
  void listCommand() const;
  void countCommand() const;
  void genCommand(std::experimental::string_view) const;
  
  Passwords::iterator uniqueSearch(std::experimental::string_view);
  Passwords::iterator getFromIndex(size_t);
  
  Passwords::iterator create(const std::string &, std::string &&);
  void change(Passwords::iterator, std::string &&);
  
  void createCommand(std::experimental::string_view);
  void createGenCommand(std::experimental::string_view);
  void createGenCopyCommand(std::experimental::string_view);
  void changeCommand(std::experimental::string_view);
  void changeSCommand(std::experimental::string_view);
  
  void rename(Passwords::iterator, std::string &&);
  void get(Passwords::const_iterator) const;
  
  void renameCommand(std::experimental::string_view);
  void renameSCommand(std::experimental::string_view);
  void getCommand(std::experimental::string_view);
  void getSCommand(std::experimental::string_view);
  
  void copy(Passwords::const_iterator) const;
  void rem(Passwords::iterator);
  
  void copyCommand(std::experimental::string_view);
  void copySCommand(std::experimental::string_view);
  void remCommand(std::experimental::string_view);
  void remSCommand(std::experimental::string_view);
};

#endif
