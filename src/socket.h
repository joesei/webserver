
#ifndef _SOCKET_
#define _SOCKET_

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2def.h>
#else
  #include <netinet/in.h>
  #include <stdint.h>       // uint64_t
  #include <sys/socket.h>
  #include <unistd.h>
#endif

#include <iostream>
#include <string>

// TODO: Use smart pointers instead
// TODO: Init and Quit could not be static
//       Set static flag instead
// TODO: Check type cast for SOCKET linux
class Socket {
 public:

  #ifdef _WIN32
    static const SOCKET kInvalid = INVALID_SOCKET;
    static const int kError = SOCKET_ERROR;
  #else
    typedef uint64_t SOCKET;
    static const SOCKET kInvalid = SOCKET(~0);
    static const int kError = (-1);
  #endif

  Socket();
  Socket(SOCKET socket);
  Socket(int socket);
  Socket(int af, int type, int protocol);
  ~Socket();
  int static SocketInit();
  int static SocketQuit();
  Socket Accept(struct sockaddr* addr, int* addrlen);

  int Bind(const struct sockaddr* addr, int addrlen);
  int Close();
  int Listen(int backlog);
  int Recv(std::string* msg, int len, int flags);
  int Send(std::string* msg, int len, int flags);


 private:
  SOCKET socket_;
};



#endif   