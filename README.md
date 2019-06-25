# onigumo TCP/IP Server
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
This server exposes a traditional TCP/IP-based command-and-control protocol.  The protocol consists of the client sending commands with names that are four uppercase characters.  Optional arguments may sometimes be specified after a space that always follows the command's name.  Not all commands have arguments.  A command string is terminated by a single ```<LF>``` character.  The server will respond with a distinctive 3-digit number that is customized for each response.  This is the command's status code.  The 3-digit number will be followed by a space and some helpful text.  All commands have their own unique set of possible return status codes.  All server responses will be terminated by a newline (```<LF>```).  

All server responses will begin with a single line that has a status code, a space, optional text, and then a terminating ```<LF>``` character.  If the server needs to issue a multiline response, the server will continue sending lines of text, with each line terminating with a single ```<LF>``` character.  

The message sent by the server will terminate, as a whole, when a line containing a single period (```.```) and a ```<LF>``` character only is sent.  All multiline command input or response, whether sent by the server to the client, or the reverse, will work in this way.   Status codes are matched to identical error conditions.  For example, a ```501``` code always indicates the server cannot parse input it is given as an integer.  

If the client needs to send multiline data, the client must first issue the proper command to the server.  The server will respond.  If the response has the proper status code, then the client may reply by sending its input, again as text strings terminated by newlines, one by one, until the input is done.  When the client is done transmitting lines, the client will send one file line consisting only of a  period (```.```) and a ```<LF>``` character.  This shall signal the server that the client is  finished transmitting.
## IV.  Status Code Ranges
Even though there are distinct status codes for each response to each command, the status codes will all fall within the numeric ranges given below:
### 1. ```200```-```299```:   Command completed successfully.  No further input required for this command.
Other commands may need to come next.
### 2. ```300```-```399```:  Command completed successfully, but further input (such as data transmission) is required.
The server must receive more input from the client before the server can act.
### 3. ```400```-```499```: Command received but its arguments or other data is in an invalid format, or are outside a valid range.
The client (or the client's user) will need to correct the error and try again.
### 4. ```500```-```599```:  Command invalid or the action that is required is impossible.
Sometimes the server will send a error code ```5xx``` message to the client without the client having sent a command; typically, this in the case that a user of the machine on which the server is based has killed the server's process; or, the operating system installed on the host machine has denied access due to the permissions assigned to the user account under which the server process is running.
## V. List of Commands
The following sections list the available protocol commands for use in communicating with the server.
### 1. Case-sensitivity of commands
#### a. All four-character command names must be supplied as all-caps strings.  Command names in any other case are ignored.
### 2. Supported commands
The commands supported by the onigumo server are:
#### a. ```HELO```
#### b. ```CODE```
#### c. ```EXEC```
#### d. ```LIST```
#### e. ```PURG```
#### f. ```INFO```
#### g. ```QUIT```
Documentation of each command follows.
### 3. Server replies
Once a command string is transmitted, the server (who is listening for data from clients) responds with a message.  
#### a. General Error Response
At any time, the server could send the following string to the client:
##### i. ```503 Server forcibly shut down by its operator.```
If this string is received by the client, the client must immediately terminate the connection with the server and/or release TCP endpoint resources back to the operating system, as this indicates that the executable process that the server is implemented with has been terminated.
### 4. ```HELO```
Activates this particular client's session with the server.  The server marks this client as being permitted to send further commands.  Except for QUIT, no other commands in this specification shall be accepted by the server until the HELO command has been sent.
#### a. Command
##### i. ```HELO<LF>```
The HELO command should be issued immediately following successful socket connection.  The word "HELO", all caps, should be sent, followed by an ```<LF>``` character.

Nothing else should be transmitted.  If any string other than the above is sent, such as HELO <text>```<LF>```, the command will be ignored, or will be considered as part of a multiline data transmission.
#### b. Responses
##### i. ```200 OK.  Welcome to the onigumo server.  Be careful what you wish for!<LF>```
Indicates that a new session has been activated successfully for the client by the server.
##### ii. ```501 Failed.  The maximum count of connected clients has been exceeded.<LF>```
There is a set maximum number (i.e., 20) clients that may be connected to the onigumo server at any one time.  If there are already 20 other clients connected with the current client issues ```HELO```, this error response will be returned and the client's TCP endpoint connection shuold be severed by the server.
##### iii. ```errno == EPIPE``` or ```errno == EBADF```
Occurs if the client is attempting to connect to a machine that is known to host an instance of the onigumo server and the server process is not operational on the machine (either the exploit was discovered and/or the process was otherwise terminated).
### 5. ```CODE```
To be followed by one or more lines of shellcode to be executed on the target machine.  The target machine is the machine on which the server process itself is running.  When this command is received, servers shall store the data sent after this command in a buffer for later use.  The ```CODE``` command has a single required argument.  The argument must be a positive integer that gives the number of shellcode bytes that the client wants to send.
#### a. Command
##### i. ```CODE <number of Base64-encoded shellcode bytes><LF>```
#### b. Responses
##### i. ```301 OK. Send your shellcode.  Terminate with a ```.``` on a line by itself.<LF>```
Indicates that the server is ready to receive the shellcode bytes.  Only ASCII characters are acceptable.  Clients should Base64-encode the opcodes before sending them.  Servers should Base64-decode the opcodes when receiving.  More than one line of data may be submitted to the server by clients.  Each line of data can not exceed 255 characters (excluding the terminating ```<LF>```).  If the Base64-encoded version of the shellcode is longer than this limit, the data must be broken up into multiple lines.

Each line must termiante with a ```<LF>```.  When the client is finished sending data, clients should indicate this to the server by sending a period (```.```) on a line by itself, followed by ```<LF>```.  This command's sole argument must be a nonzero, positive integer that specifies the total number of Base64-encoded bytes it's transmitting. 

After clients receive the ```301``` response and send the shellcode, clients should issue the ```EXEC``` command (see below).  The ```EXEC``` command orders the server to execute any shellcode it has received. 
##### ii. ```201 OK. X bytes of shellcode received.<LF>```
This response is sent to clients after clients send the terminating ```.``` line.  This is to confirm that the server received all the shellcode.  ```X``` is the number of bytes of shellcode processed, minus the ```<LF>``` characters and the period at the end. 
##### ii. ```401 Failed.  Cannot parse input as 32-bit signed integer.<LF>```
The argument passed to ```CODE``` is not able to be converted from an ASCII string representation of a 32-bit integer into a numeric value.
##### iii. ```402 Failed.  Integer argument must be a positive, 32-bit quanity.<LF>```
```CODE``` must be called followed by a space, and then the ASCII representation of a positive integer. This response code indicates that the number passed is parsable, but it is either zero or negative, which is invalid. Since the argument encodes a length, and since we do not know what to do with, e.g., ```-5``` bytes of shellcode, such a quantity must always be a positive integer.
### 6. ```EXEC```
Upon receiving this command, servers shall execute the shellcode having previously been transmitted by ```CODE```.  The shellcode shall take a single 32-bit integer as its sole argument.  The value to be passed to the shellcode is the sole argument of this protocol command.  A status message shall be sent back to the client upon successful execution of the shellcode containing the value returned by the shellcode.  Since shellcode executed on the server by the ```EXEC``` command may force the host machine to shut down or reboot, thus ending the server's process, clients must be prepared for a broken pipe error on further sends to the socket maintained by clients.
#### a. Command
##### i. ```EXEC <32-bit signed integer argument to pass to shellcode as a parameter><LF>```
To tell the server to execute the shellcode you have just sent it, send the server the ```EXEC``` command followed by the numeric argument (a 32-bit integer) that is to be passed to the shellcode as a function parameter, followed by a ```<LF>```.  The shellcode is called using the [64-bit x86 C Calling Convention](https://aaronbloomfield.github.io/pdr/book/x86-64bit-ccc-chapter.pdf), that is, provided the server is compiled with gcc and appropriate settings.  This basically means that the parameter value will be placed in CPU register ```RDI``` prior to the shellcode getting executed.
#### b. Responses
##### i. ```202 Shellcode executed.  Return value: <32-bit signed integer>.<LF>```
Indicates that the instructions in the shellcode ran successfully on the destination machine without problems.  The value of ```<32-bit integer>``` in this case will generally be ```0```.
##### ii. ```401 Failed.  Cannot parse input as 32-bit signed integer.<LF>```
The argument passed to ```EXEC``` is not able to be converted from an ASCII string representation of a 32-bit signed integer into a numeric value.
##### ii. ```504 Failed.<LF>```
Due to either a remote OS error or problem with your shellcode, execution failed.
##### iii. ```<nothing> ```
Perhaps stack-smashing in the shellcode caused an OS fault or some other general-protection condition, and perhaps led the server itself to crash. 
### 6. ```LIST```
Returns a list of processes (obtained using the system command 'ps -a') running on the machine that is hosting the server's process.
#### a. Command
##### i. LIST```<LF>```
The ```LIST``` command is simply sent by itself with a terminating newline.
#### b. Responses
##### i. ```203 OK. List of processes running on this machine follows.<LF>```
This message is sent to indicate that the server is about to tranmit a multiline response to the client.  The end of the multiline repsonse will be denoted by the transmission of a single period (```.```) on a line by itself.  Each line of the response will be the PID of a process, a tab, then the command utilized to launch the process, and a linefeed (LF) character.
##### iii. ```505 Host operating system denied us access to the list of processes.<LF>```
Exactly as it says on the tin -- the onigumo server process attempted to gather information on the processes running on its host machine, but access was denied to the onigumo process to run this command.  Could also indicate that the user account under which the ```onigumo``` server's process is executing does not have sufficient access to the host system.
### 7. ```QUIT```
Ends a client's session with the server.  After this command has been processed, the server will sever the TCP/IP connection with the client and reset its internal state variables.  The client will be marked as no longer being permitted to send commands or data.  
#### a. Command
##### i. ```QUIT```<LF>```
The ```QUIT``` command is four uppercase-only characters and a terminating LF. This command terminates the client's session with the server.  Clients must issue this command when they are finished talking with the server.
#### b. Response
##### ii. ```204 Goodbye.<LF>```
Indicates that the server has processed the QUIT command successfully. The client's session with the server is now over.
### 8. ```USRS```
Causes the server to return a list of currently-logged-in users on the target machine.
#### a. Command
##### i. ```USRS<LF>```
Clients issue the ```USRS``` command as just the command name followed by a linefeed (LF). This causes the server to query the host for the list of all the currently logged in users.
#### b. Response
##### i. ```205 OK. List of local users follows.<LF>```
This response indicates that the command succeeded and that the server is about to tranmit a multiline response.  Each line of the response consists of each user's username, followed by a LF character.  The end of the response is signaled by the server transmitting a period (```.```) on a line by itself.
##### ii. ```506 Failed. Host operating system denied us access to the list of users.<LF>```
Exactly as it says on the tin -- the ongiumo server attempted to obtain the list of currently active users; however, the operating system of the host machine denied the user account under which onigumo's server process is running access to the list of users.

Note, the server also sends the following response any time a client action has resulted in a heretofore-unknown failure on the server's part:
##### iii. ```550 Failed. Unknown server or OS error.<LF>```
