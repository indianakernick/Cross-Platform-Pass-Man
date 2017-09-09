# Cross-Platform-Pass-Man
A cross platform command-line password manager

## Example
    
Here is an example of opening a database, decrypting the database and retrieving a password. 

    $ passman
    Welcome to PassMan!
    Type "help" for a list of commands.

    > open 123456789 my_passwords.txt
    Success!
    > copy face
    Password for "Facebook" has been copied to the clipboard
    > quit
    Flushing database
    Goodbye!
    $

## Features

The database is encrypted with an XOR stream cipher and authenticated with an encrypted `std::hash` of the unencrypted data. There are many commands for generating encryption keys, generating passwords and manipulating the database. The latest help text is at the beginning of "interpret commands.cpp".

Before I created this tool, I had a big file with all my passwords in it. So anyone could just find the file and read all my passwords. To create a new password, I would mash the keyboard! Now I use this tool to store all of my passwords and I'm glad I did! I trust this tool with my passwords so you know it must be well tested.

## Contributing

To contribute to this project, simply open a pull request or an issue and we'll discuss the changes. When modifying or adding new code, please be sure to maintain the style of the existing code so that the entire code base is consistent.
