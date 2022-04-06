
#include "socket.h"

Socket::Socket() {
  socket_ = kInvalid;
}

Socket::Socket(SOCKET socket) {
  socket_ = socket;
}

Socket::Socket(int socket) {
  socket_ = socket;
}

// TODO: Modify for TCP only sockets
Socket::Socket(int af, int type, int protocol) {
#ifdef _WIN32
  socket_ = socket(af, type, protocol);
#else
  int socket = socket(af, type, 0);
  if (socket == -1) {
    socket_ = kInvalid;
  } else {
    socket_ = static_cast<SOCKET>(socket);
  }
#endif
}

// TODO: Check open status before closing
Socket::~Socket() {

}

int Socket::SocketInit() {
  #ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
      std::cout << "Winsock startup failed. Code: " << WSAGetLastError() << 
        std::endl;
      return 1;
    }
  #endif
  return 0;
}

int Socket::SocketQuit() {
  #ifdef _WIN32
    if (WSACleanup() != 0) {
      std::cout << "Winsock cleanup failed. Code: " << WSAGetLastError() <<
        std::endl;
      return 1;
    }
  #endif
  return 0;
}

Socket Socket::Accept(struct sockaddr* addr, int* addrlen) {
  Socket socket;
  #ifdef _WIN32
    SOCKET s = accept(socket_, addr, addrlen);
    if (s == INVALID_SOCKET) {
      socket = Socket();
    } else {
      socket = Socket(s);
    }
  #else
    int s = accept(static_cast<int>(socket_), addr, addrlen);
    if (s == -1) {
      socket = Socket();
    } else {
      socket = Socket(static_cast<SOCKET>(s));
    }
  #endif 
  return socket;
}

bool Socket::IsInvalid() {
  return (socket_ == kInvalid);
}

// Assumes return for Windows and Linux bind functions on error is (kError)
int Socket::Bind(const struct sockaddr* addr, int addrlen) {
  #ifdef _WIN32
    return bind(socket_, addr, addrlen);
  #else
    return bind(static_cast<int>(socket_), addr, addrlen);
  #endif 
}

// Assumes return for Windows and Linux close functions on error is (kError) 
int Socket::Close() {
  int status = kError;
  #ifdef _WIN32
    status = shutdown(socket_, SD_BOTH);
    if (status == 0) {
      status = closesocket(socket_);
    }
  #else
    status = shutdown(static_cast<int>(socket_), SHUT_RDWR);
    if (status == 0) {
      status = close(static_cast<int>(socket_));
    }
  #endif
  return status;
}

// Assumes return for Windows and Linux listen functions on error is (kError)
int Socket::Listen(int backlog) {
#ifdef _WIN32
  return listen(socket_, backlog);
#else
  return listen(static_cast<int>(socket_), backlog);
#endif  
}

// TODO: Research/Testing for case (len < 0)
int Socket::Recv(std::string* msg, int len, int flags) {
  int status = kError;
  if (len > 0 && msg != NULL) {
    char* char_msg = (char*)malloc(sizeof(char) * len);

    // If statement to remove warning: "(char_msg) could be (0)"
    if (char_msg != NULL) {
      #ifdef _WIN32
        status = recv(socket_, char_msg, len, flags);
        if (status == SOCKET_ERROR) {
          return kError;
        }
      #else
        status = recv(static_cast<int>(socket_), char_msg, len, flags);
        if (status == -1) {
          return kError;
        }
      #endif 
      msg->assign(char_msg, len);
      free(char_msg);
     }
  }
  return status;
}

int Socket::Send(std::string* msg, int len, int flags) {
  int status = kError;
  if (len > 0) {   
    #ifdef _WIN32
      status = send(socket_, msg->c_str(), len, flags);
      if (status == SOCKET_ERROR) {
        return kError;
      }
    #else
      status = send(static_cast<int>(socket_), msg->c_str(), len, flags);
      if (status == -1) {
        return kError;
      }
    #endif 
  }
  return status;
}