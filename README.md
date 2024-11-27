# CPP_Chat
A simple console baed chat application written in C++. Please note that this application is currently only support on Debian based systems unless otherwise mentioned.
# Compatability
The chat sever is only compatible on Linux systems.
THe chat client currently support Linux systems. Compatability with Windows will be added in a later revision.
# Building the project
This project uses cmake to compile the project.
See more instructions on how to build the project [here](./BUILD.md)
# Running the project
See information about running the project [here](./RUN.md)
# Application Requirements
 - [X] Use of control structure
 - [X] Use of functions
 - [X] Use of structs or enumeration
 - [X] Classes with Object Oriented Design
 - [X] Arrays
 - [X] Pointers
 - [X] templates
 - [X] file streams
 - [ ] class relations 
## Project Specifications
 - [X] Create a server to recive TCP sockets and transmit messages
 - [X] Create a client to send messages and view the chatlog
 - [ ] Encrypt the traffic between client and server
   - Decide what encryption standard to use
   - Decide to use symetric or asymetric encryption
 - Authentication
   - [X] Have users connect with a username
   - [ ]Have users connect with provided credentials
 - [X] Use file IO to keep a log of the chat
 - [X] Allow the server admin to determin what port to launch the server on on start
 -  [ ] Add server commands such as whisper
 - TBD
 - Maybes
   - [ ] Cross-platform compatability bewteen Windows and Linux

# Project Structure
```
chat_app/
├── inc/
│   ├── openssl/     # OpenSSL headers
├── lib/
│   ├── libssl.a     # OpenSSL library
│   ├── libcrypto.a
├── src/
│   ├── server.cpp   # Server implementation
│   ├── client.cpp   # Client implementation
│   ├── encryption.cpp  # Encryption functions
│   ├── encryption.h    # Encryption header
│   ├── common.h     # Shared constants/functions
├── Makefile         # Makefile for building
```

# Reference Materials
[netinet/in.h](https://man7.org/linux/man-pages/man0/netinet_in.h.0p.html) <br>
[unistd.h](https://www.man7.org/linux/man-pages/man0/unistd.h.0p.html)