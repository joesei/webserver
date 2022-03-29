

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2def.h>
#else
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <unistd.h>
#endif

#include <iostream>
#include <string>

// TODO: Use smart pointers instead

class Socket {
 public:
  int socket_;
  
  Socket(int socket);
  Socket(int af, int type, int protocol);
  ~Socket();
  int static SocketInit();
  int static SocketQuit();
  Socket static Accept(int socket, struct sockaddr* addr, int* addrlen);

  int Bind(const struct sockaddr* addr, int addrlen);
  int Close();
  int Recv(std::string* msg, int len, int flags);
  int Send(std::string* msg, int len, int flags);


 private:
};