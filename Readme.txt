myShell

About:
Its a Unix command-line interpretor which runs similar to the Linux Shell, allowing us to execute basic commands like ls, pwd etc.

How to run:

Method 1:

- Put all the files to a folder.
- Open a terminal and go to that directory using 'cd'.
- Compile the program by using the Makefile. To compile, just type 'make' and press <enter>.
- Run the output file myShell. To run, just type './myShell' in the terminal and press <enter>
- You will see a new prompt: '[myShell]: '. Type your commands there.
- To exit myShell, press ctrl+D.

Method 2:

- Put all the files to a folder.
- Open a terminal and go to that directory using 'cd'.
- type 'chmod +x myShell.sh' and press <enter>.
- type './myShell' and press <enter>.
- You will see a new prompt: '[myShell]: '. Type your commands there.
- To exit myShell, press ctrl+D.


Note:

- The directory commands are not working at present.
- There is a function in the file myShell.c named 'printPrompt', which changes the prompt to a new prompt of the format: '<userName>@<hostName>@myShell:'. This function works correctly on some devices and doesn't work on some others. You can try it on your device. The changes which are required to get the new prompt are commented in the code(4 changes required in total).

Tested on Linux(Ubuntu) and Mac OS X.