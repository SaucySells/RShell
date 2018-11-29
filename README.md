# RShell

This program is RShell. It's a command shell written in C++. It has the functionalities of a normal
command shell, including comments and the ability to chain commands (with ;, &&, or ||).

The program works like a typical command shell. It outputs a prompt ($) to ask for a command to be input.

Commands are input as follows:

cmd		= executable [ argumentList ] [ connector] cmd . . .
connector	= || or && or ;

Where executable is the command and argumentList is all of the arguments for the command. The connectors go
in between commands and work as follows: a command coming after a ";" will always run. A command following a "&&" 
will only run if the preceding command ran successfully. A command following a "||" will only run if the preceding
command did not run succcessfully.

Additionally, you can use parenthesis as such: (echo A && echo B) && echo C, to set precedence for determining how
commands are run. For example, in the case you just saw, without the parenthesis, echo C would normally output 
depending on if echo B ran successfully. With the parenthesis added, both echo A and echo B have to complete
successfully before echo C can run successfully.

It also supports piping ( | ) and redirection ( <<. <, >, >> ) for both input and output. 

# Known Issues/Bugs

Multiple pipes in one command chain does not work sometimes.
