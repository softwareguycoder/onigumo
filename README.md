# onigumo TCP/IP Server (this Readme is WIP)
(C/64-bit/Linux) Server to enable the remote execution of shellcode and to gain knowledge of the user's system.

## I.  Introduction
onigumo is a server that facilitates the execution of shellcode on a remote machine.  Up to 20 clients can connect simulatenously to an instance of an ongiumo server.

Onigumo is the name of a character from the anime "[InuYasha](https://en.wikipedia.org/wiki/Inuyasha)," one of the author's favorite shows.  In the show, the bandit Onigumo was so evil, he was burned and maimed by his fellow bandits.  Nursed by the Shinto priestess Kikyo, he soon fell in love with her.  However, a love triangle ensured.  Because Kikyo loved Inuyasha (the show's titular character) instead, Onigumo grew jealous, and longed to have a healed body that could walk again.  In his effort to walk again, he called forth a great number of evil demons, who came and possessed his body, making it able to walk again.  However, instead of Onigumo getting to fulfill his wish of winning Kikyo's hart, the demons had other plans.  Double-crossing Onigumo, the demons conspired to make him kill Kikyo -- AND set up Inuyasha -- in a most brutal fashion.

As was the case with Onigumo, the villian in the show, the onigumo server is capable of being utilized by several clients at once.  The onigumo server calls forth client connections (with clients playing the role of the show's "demons," as they tell the onigumo server to do some bad-ass stuff on the host machine) and these clients can send the server shellcode bytes to be then executed on the destination machine, in a sense, "possessing" the machine the server is being executed on and causing the machine to either do things it is not supposed to do etc.  as the shellcode directs, such as killing processes or taking over the machine.

## II.  Dependencies
This program depends on the following repositories, with links to follow each:
* api_core, at: https://github.com/softwareguycoder/api_core
* common_core, at: https://github.com/softwareguycoder/common_core
* console_core, at: https://github.com/softwareguycoder/console_core
* conversion_core, at: https://github.com/softwareguycoder/conversion_core
* debug_core, at: https://github.com/softwareguycoder/debug_core
* exceptions_core, at: https://github.com/softwareguycoder/exceptions_core
* file_core, at: https://github.com/softwareguycoder/file_core
* inetsock_core, at: https://github.com/softwareguycoder/inetsock_core
* list_core, at: https://github.com/softwareguycoder/list_core
* mutex_core, at: https://github.com/softwareguycoder/mutex_core
* naraku, at: https://github.com/softwareguycoder/naraku
* threading_core, at: https://github.com/softwareguycoder/threading_core
* uuid_core, at: https://github.com/softwareguycoder/uuid_core

The thinking in listing these is, that if you clone or fork this repo, it will be necessary to also clone and fork these other repos and ensure that the code is compiled/referenced by the server project in order for it to build.

## III.  Protocol Summary
This server exposes a traditional TCP/IP-based command-and-control protocol.  The protocol consists of the client sending commands with names that are four uppercase characters.  Optional arguments may sometimes be specified after a space that always follows the command's name.  Not all commands have arguments.  A command string is terminated by a single ```\n``` character.  The server will respond with a distinctive 3-digit number that is customized for each response.  This is the command's status code.  The 3-digit number will be followed by a space and some helpful text.  All commands have their own unique set of possible return status codes.  All server responses will be terminated by a newline (```\n```).  

All server responses will begin with a single line that has a status code, a space, optional text, and then a terminating ```\n``` character.  If the server needs to issue a multiline response, the server will continue sending lines of text, with each line terminating with a single ```\n``` character.  

The message sent by the server will terminate, as a whole, when a line containing a single period (```.```) and a ```\n``` character only is sent.  All multiline command input or response, whether sent by the server to the client, or the reverse, will work in this way.   Status codes are matched to identical error conditions.  For example, a 501 code al- ways indicates the server cannot parse input it is given as an integer.  

If the client needs to send multiline data, the client must first issue the proper command to the server.  The server will respond.  If the response has the proper status code, then the client may reply by sending its input, again as text strings terminated by newlines, one by one, until the input is done.  When the client is done transmitting lines, the client will send one file line consisting only of a  period (```.```) and a ```\n``` character.  This shall signal the server that the client is  finished transmitting.

## IV.  Status Code Ranges

Even though there are distinct status codes for each response to each command, the status codes will all fall within the numeric ranges given below:

### a. ```200```-```299```:   Command completed successfully.  No further input required for this command.
Other commands may need to come next.
### b. ```300```-```399```:  Command completed successfully, but further input (such as data transmission) is required.
The server must receive more input from the client before the server can act.
### c. ```400```-```499```: Command received but its arguments or other data is in an invalid format, or are outside a valid range.
The client (or the client's user) will need to correct the error and try again.
### d. ```500```-```599```:  Command invalid or the action that is required is impossible.
Sometimes the server will send a error code ```5xx``` message to the client without the client having sent a command; typically, this in the case that a user of the machine on which the server is based has killed the server's process; or, the operating system installed on the host machine has denied access due to the permissions assigned to the user account under which the server process is running.
