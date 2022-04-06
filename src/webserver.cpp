
#include "webserver.h"

using namespace std;

WebServer::WebServer() {
  Socket::SocketInit();
}

WebServer::~WebServer() {
  for (ClientSession client : client_sessions_) {
    client.GetSocket().Close();
  }
  Socket::SocketQuit();
}

void WebServer::Start() {
  std::thread listener(&WebServer::ListenerThread, this);
  WorkingThread();
  listener.join();
}

void WebServer::ListenerThread() {
  server_socket_ = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (server_socket_.IsInvalid()) {
    std::cout << "Error creating server socket" << std::endl;
    return;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(80);
  int status = server_socket_.Bind((struct sockaddr*)&server_addr, 
                                    sizeof(server_addr));

  if (status == Socket::kError) {
    std::cout << "Error binding server socket" << std::endl;
    return;
  } else {
    std::cout << "Server socket binding successful" << std::endl;
  }

  while (true) {
    status = server_socket_.Listen(SOMAXCONN);
    if (status == Socket::kError) {
      std::cout << "Error listening event on server socket" << std::endl;
      return;
    }

    sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    Socket client_socket = server_socket_.Accept((struct sockaddr*)&client_addr, 
                                                  &client_addr_len);

    if (!client_socket.IsInvalid()) {
      session_mutex_.lock();
      client_sessions_.push_back(ClientSession(client_socket, client_addr,
                                               client_addr_len));
      session_mutex_.unlock();
    } 
  }

}

void WebServer::WorkingThread() {
  // TODO: Make (recv_len) constant?
  // Process:
  //   Get message
  //   Parse message into keywords
  //     GET: 
  //   Response
  int recv_len = 200;
  while (true) {
    std::string msg;
    for (ClientSession client : client_sessions_) {
      client.GetSocket().Recv(&msg, recv_len, 0);
      if (msg.size() > 0) {
        std::cout << msg << endl;
      }
    }
  }
}

