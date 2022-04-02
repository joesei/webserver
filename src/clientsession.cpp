

#include "clientsession.h"

ClientSession::ClientSession(Socket socket, struct sockaddr_in addr, 
                             int addr_len) {

  socket_ = socket;
  addr_ = addr;
  addr_len_ = addr_len;

}

Socket ClientSession::GetSocket() {
  return socket_;
}