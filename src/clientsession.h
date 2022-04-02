
#ifndef _CLIENT_SESSION_
#define _CLIENT_SESSION_

#include "socket.h"

// TODO: Make into struct possible

class ClientSession {
 public:
  ClientSession() = default;
  ClientSession(Socket socket, struct sockaddr_in addr, int addr_len);
  Socket GetSocket();

 private:
  Socket socket_;
  struct sockaddr_in addr_;
  int addr_len_;
};


#endif