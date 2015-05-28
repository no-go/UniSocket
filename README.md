# UniSocket

UniSocket is an universal Socket (Wrapper) in C++ and works 
on Linux, OS X and Windows (VisualC++). An example and
some shell and batch scripts for compilation and linking
are included.

## Features

- makes a c++ object orientated version of the Socket API
- actually only TCP
- polling for *recv()* is an option
- clean splitting of potentially OS dependent Code
- uses a header with the size of the transmitted bytes
- header is seperated by 2 newlines

## The Classes

### UniServerSocket Class

- makes a port listening and set the queueSize of parallel connections
- *accept()* gives you a UniSocket which represents the 
  connction to the client

### UniSocket Class

- make a connection to IP-address and port of UniServerSocket
- *send()*, *recv()* and *close()*
- *getIp()* to get the IP-address of your connection partner

### UniSocketException Class

This object is thrown on errors or if you use polling with
the *recv(true)* method. You get the thrown message as
string from the public attribute *_msg*.

### SocketWrapper Class

This class protects you from using OS dependent implemetations
and pretends to be OS indendent. For a nice and clean developing
please do not use this Class; only the UniSocket classes should
fulfil your desire!

## Hints

- polling: *recv(true)* only runs on header receiving!
  After the header is readed, the *revc(true)* is waiting
  until the expected size is received.
- sorry about my strange english

