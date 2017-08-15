//
//  interpret commands.cpp
//  Pass Man
//
//  Created by Indi Kernick on 10/8/17.
//  Copyright © 2017 Indi Kernick. All rights reserved.
//

#include "interpret commands.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include "parse.hpp"
#include "encrypt.hpp"
#include "generate key.hpp"
#include "write to clipboard.hpp"
#include "generate password.hpp"

namespace {
  const char HELP_TEXT[] =
R"(help
  Prints a list of all commands and what they do.

gen_key
  Generates an encryption key

gen_key_copy
  Generates an encryption key and copies it to the clipboard

open <key> <file>
  Opens a file and decrypts it. Once opened, the file can be manipulated. If the
  file either doesn't exist or is empty, then a new database is created.

clear
  Removes every entry from the database.

flush
  Writes all changes to the file (if it exists).

quit
  Writes all changes to the file (if it exists) and exits.

quit_no_flush
  Exits without flushing changes.

dump <file>
  Writes all passwords into a file WITHOUT ENCRYPTING them. This command is
  for changes to the tool that may break existing databases.

search <sub_string>
  Searchs for passwords by name.

list
  Lists the names of every password.

count
  Prints the number of passwords in the database.

gen <length>
  Prints a randomly generated string

create <name> <password>
  Creates a new entry in the database.

create_gen <name> <length>
  Generates a passwords and puts it into the database.

change <name> <new_password>
  If name is an unambiguous substring then that password is changed.

change_s <index> <new_password>
  The password in the most recent search with that index is changed.

rename <name> <new_name>
  If name is an unambiguous substring then that password is renamed.

rename_s <index> <new_name>
  The password in the most recent search with that index is renamed.
  
get <name>
  If name is an unambiguous substring then that password is printed.

get_s <index>
  The password in the most recent search with that index is printed.

copy <name>
  If name is an unambiguous substring then that password is copied to
  the clipboard.

copy_s <index>
  The password in the most recent search with that index is copied to
  the clipboard.

rem <name>
  If name is an unambiguous substring then that password is removed from
  the database.

rem_s <index>
  The password in the most recent search with that index is removed from
  the database.
)";

  bool commandIs(const std::experimental::string_view command, const std::experimental::string_view name) {
    if (command.size() < name.size()) {
      return false;
    }
    
    const size_t space = command.find_first_of(' ');
    constexpr size_t npos = std::experimental::string_view::npos;
    const size_t compareLength = space == npos ? command.size() : space;
    
    if (compareLength != name.size()) {
      return false;
    }
    
    return command.compare(0, compareLength, name) == 0;
  }
  
  void unknownCommand(const std::experimental::string_view command) {
    std::cout << "Unknown command \"";
    const size_t space = command.find_first_of(' ');
    std::cout.write(
      command.data(),
      space == std::experimental::string_view::npos
      ? command.size()
      : space
    );
    std::cout << "\"\n";
  }

  void helpCommand() {
    std::cout << HELP_TEXT;
  }
  
  void genKeyCommand() {
    std::cout << "Encryption key: " << generateKey() << '\n';
  }
  
  void genKeyCopyCommand() {
    writeToClipboard(std::to_string(generateKey()));
    std::cout << "An encryption key was copied to the clipboard\n";
  }
}

CommandInterpreter::CommandInterpreter() {
  std::cout << "Welcome to PassMan!\n";
  std::cout << "Type \"help\" for a list of commands.\n";
  std::cout << '\n';
}

CommandInterpreter::~CommandInterpreter() {
  //goodbye message?
};

void CommandInterpreter::prefix() {
  std::cout << "> ";
}

constexpr std::experimental::string_view operator""_sv(const char *data, const size_t size) {
  return {data, size};
}

void CommandInterpreter::interpret(const std::experimental::string_view command) {
  #define COMMAND_IS(COMMAND_NAME) \
    const auto name = #COMMAND_NAME##_sv; \
    commandIs(command, name)
  
  if (COMMAND_IS(help)) {
    helpCommand();
  } else if (COMMAND_IS(gen_key)) {
    genKeyCommand();
  } else if (COMMAND_IS(gen_key_copy)) {
    genKeyCopyCommand();
  } else if (COMMAND_IS(open)) {
    openCommand(command.substr(name.size()));
  } else if (COMMAND_IS(clear)) {
    clearCommand();
  } else if (COMMAND_IS(flush)) {
    flushCommand();
  } else if (COMMAND_IS(quit)) {
    quitCommand();
  } else if (COMMAND_IS(quit_no_flush)) {
    quitNoFlushCommand();
  } else if (COMMAND_IS(dump)) {
    dumpCommand(command.substr(name.size()));
  } else if (COMMAND_IS(search)) {
    searchCommand(command.substr(name.size()));
  } else if (COMMAND_IS(list)) {
    listCommand();
  } else if (COMMAND_IS(count)) {
    countCommand();
  } else if (COMMAND_IS(gen)) {
    genCommand(command.substr(name.size()));
  } else if (COMMAND_IS(create)) {
    createCommand(command.substr(name.size()));
  } else if (COMMAND_IS(create_gen)) {
    createGenCommand(command.substr(name.size()));
  } else if (COMMAND_IS(change)) {
    changeCommand(command.substr(name.size()));
  } else if (COMMAND_IS(change_s)) {
    changeSCommand(command.substr(name.size()));
  } else if (COMMAND_IS(rename)) {
    renameCommand(command.substr(name.size()));
  } else if (COMMAND_IS(rename_s)) {
    renameSCommand(command.substr(name.size()));
  } else if (COMMAND_IS(get)) {
    getCommand(command.substr(name.size()));
  } else if (COMMAND_IS(get_s)) {
    getSCommand(command.substr(name.size()));
  } else if (COMMAND_IS(copy)) {
    copyCommand(command.substr(name.size()));
  } else if (COMMAND_IS(copy_s)) {
    copySCommand(command.substr(name.size()));
  } else if (COMMAND_IS(rem)) {
    remCommand(command.substr(name.size()));
  } else if (COMMAND_IS(rem_s)) {
    remSCommand(command.substr(name.size()));
  } else {
    unknownCommand(command);
  }
  
  std::cout.flush();
  
  #undef COMMAND_IS
}

bool CommandInterpreter::shouldContinue() const {
  return !quit;
}

namespace {
  unsigned long long readNumber(std::experimental::string_view &args) {
    if (args.size() == 0) {
      throw std::runtime_error("Expected number");
    }
    char *end;
    const unsigned long long arg = std::strtoull(args.data(), &end, 0);
    if (errno == ERANGE) {
      throw std::runtime_error("Number out of range");
    }
    if (arg == 0 && end[-1] != '0') {
      throw std::runtime_error("Invalid number");
    }
    args.remove_prefix(end - args.data());
    return arg;
  }

  std::string readString(std::experimental::string_view &args) {
    if (args.size() == 0) {
      throw std::runtime_error("Expected string");
    }
    
    size_t begin = 0;
    for (; begin != args.size(); ++begin) {
      if (args[begin] != ' ') {
        break;
      }
    }
    
    if (begin == args.size()) {
      throw std::runtime_error("Expected string");
    }
    
    size_t end = 0;
    std::string arg;
    bool prevBackSlash = false;
    
    for (size_t i = begin; i != args.size(); ++i) {
      end = i;
      const char c = args[i];
      if (c == ' ') {
        if (prevBackSlash) {
          arg.push_back(' ');
        } else {
          break;
        }
      } else if (c == '\\') {
        if (prevBackSlash) {
          arg.push_back('\\');
          prevBackSlash = false;
        } else {
          prevBackSlash = true;
        }
      } else {
        arg.push_back(c);
        prevBackSlash = false;
      }
    }
    
    args.remove_prefix(end);
    
    return arg;
  }

  bool fileExists(const char *const path) {
    std::unique_ptr<std::FILE, decltype(&std::fclose)> file(
      fopen(path, "r"),
      &std::fclose
    );
    return bool(file);
  }

  void nextArg(std::experimental::string_view &args, const char *signature) {
    if (args.size() == 0 || args[0] != ' ') {
      throw std::runtime_error(std::string("Command signature is:\n") + signature);
    }
    args.remove_prefix(1);
  }
}

void CommandInterpreter::openCommand(
  std::experimental::string_view arguments
) {
  nextArg(arguments, "open <key> <file>");
  const uint64_t newKey = readNumber(arguments);
  
  nextArg(arguments, "open <key> <file>");
  const std::string newFile = readString(arguments);
  
  if (!fileExists(newFile.c_str())) {
    std::FILE *fileStream = std::fopen(newFile.c_str(), "w");
    if (fileStream == nullptr) {
      std::cout << "Failed to create file \"" << newFile.c_str() << "\"\n";
      return;
    } else {
      std::cout << "Created a new file named \"" << newFile.c_str() << "\"\n";
    }
    std::fclose(fileStream);
    encryptFile(newKey, newFile, "");
  }
  
  if (passwords) {
    flushCommand();
  }
  
  passwords.emplace(readPasswords(decryptFile(newKey, newFile)));
  searchResults.clear();
  key = newKey;
  file = std::move(newFile);
  
  std::cout << "Success!\n";
}

void CommandInterpreter::clearCommand() {
  if (passwords) {
    passwords->clear();
    searchResults.clear();
    std::cout << "Database cleared\n";
  }
}

void CommandInterpreter::flushCommand() const {
  if (passwords) {
    encryptFile(key, file, writePasswords(*passwords));
    std::cout << "Database flushed\n";
  }
}

void CommandInterpreter::quitCommand() {
  flushCommand();
  quit = true;
}

void CommandInterpreter::quitNoFlushCommand() {
  quit = true;
}

void CommandInterpreter::dumpCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "dump <file>");
  const std::string filePath = readString(arguments);
  
  std::ofstream file(filePath, std::ofstream::binary);
  if (!file.is_open()) {
    std::cout << "File open error\n";
    return;
  }
  file.exceptions(0xFFFF);
  
  for (auto p = passwords->cbegin(); p != passwords->cend(); ++p) {
    file << p->first << "\n    " << p->second << '\n';
  }
  
  std::cout << "Database dumped to \"" << filePath << "\"\n";
}

void CommandInterpreter::expectInit() const {
  if (!passwords) {
    throw std::runtime_error("Database is uninitialized. Use the open command to initialize");
  }
}

void CommandInterpreter::searchCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "search <sub_string>");
  const std::string subString = readString(arguments);
  
  searchResults.clear();
  
  for (auto p = passwords->begin(); p != passwords->end(); ++p) {
    if (p->first.find(subString) != std::experimental::string_view::npos) {
      std::cout.width(4);
      std::cout << searchResults.size() << " - " << p->first << '\n';
      searchResults.push_back(p->first);
    }
  }
  
  if (searchResults.empty()) {
    std::cout << "No password names where found containing the substring:\n\"";
    std::cout << subString << "\"\n";
  }
}

void CommandInterpreter::listCommand() const {
  expectInit();
  
  if (passwords->empty()) {
    std::cout << "Database is empty\n";
  } else {
    for (auto p = passwords->cbegin(); p != passwords->cend(); ++p) {
      std::cout << p->first << '\n';
    }
  }
}

void CommandInterpreter::countCommand() const {
  expectInit();
  
  if (passwords->empty()) {
    std::cout << "Database is empty\n";
  } else if (passwords->size() == 1) {
    std::cout << "Database contains 1 password\n";
  } else {
    std::cout << "Database contains " << passwords->size() << " passwords\n";
  }
}

void CommandInterpreter::genCommand(std::experimental::string_view arguments) const {
  nextArg(arguments, "gen <length>");
  const uint64_t size = readNumber(arguments);
  
  std::cout << "Random password: " << generatePassword(size) << '\n';
}

Passwords::iterator CommandInterpreter::uniqueSearch(
  const std::experimental::string_view substring
) {
  expectInit();
  
  Passwords::iterator iter = passwords->end();
  
  for (auto p = passwords->begin(); p != passwords->end(); ++p) {
    if (p->first.find(substring.data(), 0, substring.size()) != std::string::npos) {
      if (iter == passwords->end()) {
        iter = p;
      } else {
        throw std::runtime_error("More than one password name contains the substring \"" + substring.to_string() + "\"");
      }
    }
  }
  
  if (iter == passwords->end()) {
    throw std::runtime_error("No password name contains the substring \"" + substring.to_string() + "\"");
  } else {
    return iter;
  }
}

void CommandInterpreter::createCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "create <name> <new_password>");
  const std::string name = readString(arguments);
  
  nextArg(arguments, "create <name> <new_password>");
  const std::string password = readString(arguments);
  
  if (!passwords->emplace(std::move(name), std::move(password)).second) {
    std::cout << "Entry was not created. A password for \"" << name << "\" already exists\n";
    return;
  }
  
  std::cout << "Created \"" << name << "\" password\n";
}

void CommandInterpreter::createGenCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "create <name> <length>");
  const std::string name = readString(arguments);
  
  nextArg(arguments, "create <name> <length>");
  const size_t length = readNumber(arguments);
  
  if (length == 0) {
    throw std::runtime_error("Invalid password length");
  }
  const std::string password = generatePassword(length);
  if (!passwords->emplace(std::move(name), std::move(password)).second) {
    std::cout << "Entry was not created. A password for \"" << name << "\" already exists\n";
    return;
  }
  
  std::cout << "Created \"" << name << "\" password\n";
}

void CommandInterpreter::changeCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "change <name> <new_password>");
  const std::string name = readString(arguments);
  
  nextArg(arguments, "change <name> <new_password>");
  const std::string password = readString(arguments);
  
  const auto entry = uniqueSearch(name);
  entry->second = std::move(password);
  
  std::cout << "Changed \"" << entry->first << "\" password\n";
}

void CommandInterpreter::changeSCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "change_s <index> <new_password>");
  const size_t index = readNumber(arguments);
  
  nextArg(arguments, "change_s <index> <new_password>");
  const std::string password = readString(arguments);
  
  const auto entry = getFromIndex(index);
  entry->second = std::move(password);
  
  std::cout << "Changed \"" << entry->first << "\" password\n";
}

Passwords::iterator CommandInterpreter::getFromIndex(const size_t index) {
  if (index >= searchResults.size()) {
    throw std::runtime_error("Index out of range\n");
  }
  
  auto iter = passwords->find(searchResults[index]);
  if (iter == passwords->end()) {
    throw std::runtime_error("Password for \"" + searchResults[index] + "\" has been removed since the search\n");
  }
  
  return iter;
}

void CommandInterpreter::renameCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "rename <name> <new_name>");
  const std::string name = readString(arguments);
  
  nextArg(arguments, "rename <name> <new_name>");
  const std::string newName = readString(arguments);
  
  const auto entry = uniqueSearch(name);
  auto newEntry = passwords->find(newName);
  if (newEntry != passwords->end()) {
    std::cout << "Cannot rename \"" << entry->first << "\" to \"" << newName << "\" because that name is taken\n";
    return;
  }
  
  std::cout << "Renamed \"" << entry->first << "\" to \"" << newName << "\"\n";
  
  passwords->emplace(std::move(newName), std::move(entry->second));
  passwords->erase(entry);
}

void CommandInterpreter::renameSCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "rename_s <index> <new_name>");
  const size_t index = readNumber(arguments);
  
  nextArg(arguments, "rename_s <index> <new_name>");
  const std::string newName = readString(arguments);
  
  const auto entry = getFromIndex(index);
  auto newEntry = passwords->find(newName);
  if (newEntry != passwords->end()) {
    std::cout << "Cannot rename \"" << entry->first << "\" to \"" << newName << "\" because that name is taken\n";
    return;
  }
  
  std::cout << "Renamed \"" << entry->first << "\" to \"" << newName << "\"\n";
  
  passwords->emplace(std::move(newName), std::move(entry->second));
  passwords->erase(entry);
}

void CommandInterpreter::getCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "get <name>");
  const std::string name = readString(arguments);
  
  const auto entry = uniqueSearch(name);
  
  std::cout << "Password for \"" << entry->first << "\" is:\n" << entry->second << '\n';
}

void CommandInterpreter::getSCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "get <index>");
  const size_t index = readNumber(arguments);
  
  const auto entry = getFromIndex(index);
  
  std::cout << "Password for \"" << entry->first << "\" is:\n" << entry->second << '\n';
}

void CommandInterpreter::copyCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "copy <name>");
  const std::string name = readString(arguments);
  
  const auto entry = uniqueSearch(name);
  writeToClipboard(entry->second);
  
  std::cout << "Password for \"" << entry->first << "\" was copied to the clipboard\n";
}

void CommandInterpreter::copySCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "copy_s <index>");
  const size_t index = readNumber(arguments);
  
  const auto entry = getFromIndex(index);
  writeToClipboard(entry->second);
  
  std::cout << "Password for \"" << entry->first << "\" was copied to the clipboard\n";
}

void CommandInterpreter::remCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "rem <name>");
  const std::string name = readString(arguments);
  
  const auto entry = uniqueSearch(name);
  std::cout << "Password for \"" << entry->first << "\" was removed from the database\n";
  passwords->erase(entry);
}

void CommandInterpreter::remSCommand(std::experimental::string_view arguments) {
  expectInit();
  
  nextArg(arguments, "rem_s <index>");
  const size_t index = readNumber(arguments);
  
  const auto entry = getFromIndex(index);
  std::cout << "Password for \"" << entry->first << "\" was removed from the database\n";
  passwords->erase(entry);
}
