

#include "socket.h"

// TODO: More informational error messages

Socket::Socket(int socket) {
  socket_ = socket;
}

// TODO: Modify for TCP only sockets
Socket::Socket(int af, int type, int protocol) {
#ifdef _WIN32
  if ((socket_ = socket(af, type, protocol)) == INVALID_SOCKET) {
    std::cout << "Socket creation failed. Code: " << WSAGetLastError() << 
      std::endl;
  }
#else
  if ((socket_ = socket(af, type, 0)) < 0) {
    std::cout << "Socket creation failed." << std::endl;
  }
#endif
}

Socket::~Socket() {

}

int Socket::SocketInit() {
  #ifdef _WIN32
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
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

Socket Socket::Accept(int socket, struct sockaddr* addr, int* addrlen) {
  return Socket(accept(socket, addr, addrlen));
}

int Socket::Bind(const struct sockaddr* addr, int addrlen) {
  int status = 0;
#ifdef _WIN32
  status = bind(socket_, addr, addrlen);
  if (status == SOCKET_ERROR) {
    std::cout << "Socket bind failed. Code: " << WSAGetLastError() << 
      std::endl;
  }
#else
  status = bind(socket_, addr, addrlen);
  if (status < 0) {
    std::cout << "Socket bind failed." << std::endl;
  }
#endif
  return status;
}

int Socket::Close() {
  int status = 0;
  #ifdef _WIN32
    status = shutdown(socket_, SD_BOTH);
    if (status == 0) {
      status = closesocket(socket_);
    }
  #else
    status = shutdown(socket_, SHUT_RDWR);
    if (status == 0) {
      status = close(socket_);
    }
  #endif
  return status;
}

// TODO: Research/Testing for case (len < 0)
int Socket::Recv(std::string* msg, int len, int flags) {

  int status = -1;
  if (len > 0) {
    char* char_msg = (char*)malloc(sizeof(char) * len);
    status = recv(socket_, char_msg, len, flags);

  #ifdef _WIN32
    if (status == SOCKET_ERROR) {
      std::cout << "Failed to receive message. Code: " << WSAGetLastError() <<
        std::endl;
      return status;
    }
  #else
    if (status == -1) {
      std::cout << "Failed to receive message." << std::endl;
      return status;
    }
  #endif 

    msg->assign(char_msg, len);
    free(char_msg);
  }
  return status;
}

int Socket::Send(std::string* msg, int len, int flags) {
  int status = -1;
  if (len > 0) {
    status = send(socket_, msg->c_str(), len, flags);
    
  #ifdef _WIN32
    if (status == _WIN32) {
      std::cout << "Failed to send message. Code: " << WSAGetLastError() <<
        std::endl;
    }
  #else
    if (status == -1) {
      std::cout << "Failed to send message." << std::endl;
    }
  #endif 
  }
  return status;
}