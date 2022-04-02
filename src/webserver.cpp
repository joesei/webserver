
#include "webserver.h"

using namespace std;

WebServer::WebServer() {
  Socket::SocketInit();
}

WebServer::~WebServer() {
  Socket::SocketQuit();
}

void WebServer::Start() {
  std::thread listener(&WebServer::ListenerThread, this);
  WorkingThread();
  listener.join();
}

void WebServer::ListenerThread() {
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;

  std::cout << "Entered Listener Thread\n";

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(80);

  server_socket_ = Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  server_socket_.Bind((struct sockaddr*)&server_addr, sizeof(server_addr));
  while (true) {
    server_socket_.Listen(SOMAXCONN);

    struct sockaddr_in client_addr;
    int client_addr_len = 0;
    Socket client_socket = server_socket_.Accept((struct sockaddr*)&client_addr, 
                                                  &client_addr_len);

    session_mutex_.lock();
    client_sessions_.push_back(ClientSession(client_socket, client_addr, 
                                             client_addr_len));

    session_mutex_.unlock();
  }

}

void WebServer::WorkingThread() {
  // TODO: Make (recv_len) constant?
  int recv_len = 200;
  std::cout << "Entered Working Thread\n";
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

