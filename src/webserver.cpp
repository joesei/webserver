// webserver.cpp : Defines the entry point for the application.
//

#include "webserver.h"

#include <winsock2.h>
#include <thread> 

using namespace std;

WebServer::WebServer() {

}

WebServer::~WebServer() {

}






void listener(SOCKET server, sockaddr_in service) {

  int s = sizeof(service);
  struct sockaddr_in client;
  SOCKET socket;

  while (true) {
    if (listen(server, SOMAXCONN) == SOCKET_ERROR) {
      cout << "Listen function failed. Code: " << WSAGetLastError() << endl;
    }
    


    // Client connected to server 
    socket = accept(server, (struct sockaddr*)&client, &s);
    if (socket == INVALID_SOCKET) {
      cout << "Client connection failed. Code: " << WSAGetLastError() << endl;
      closesocket(socket);
    }
    else {
      cout << "Client connected successfully" << endl;
      char message[200];
      int result = 1;

      // Receiving messages from client
      do {
        if ((result = recv(socket, message, 200, 0)) == SOCKET_ERROR) {
          cout << "Failed to receive message from client. Code: " << 
            WSAGetLastError() << endl;
        }
        else {

          // result = # of bytes 

          // Split message into lines
          // Get key-pair of each line


          cout << message << endl;
        }
      } while (result > 0);

    }

  }
}

int main()
{
  const int port = 80;

  WSADATA wsa;
  SOCKET server;

  sockaddr_in service;
  service.sin_family = AF_INET;
  service.sin_addr.s_addr = htonl(INADDR_ANY);
  service.sin_port = htons(80);

  cout << "Winsock startup" << endl;
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    cout << "Winsock startup failed. Code: " << WSAGetLastError() << endl;
    return 1;
  }

  cout << "Winsock Initialized" << endl;

  cout << "Creating server socket" << endl;
  if ((server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
    cout << "Server socket creation failed. Code: " << WSAGetLastError() << 
      endl;
    WSACleanup();
    return 1;
  }

  cout << "Server socket created" << endl;

  cout << "Binding server socket to port 80" << endl;
  if (bind(server, (struct sockaddr*)&service, sizeof(service)) == SOCKET_ERROR) {
    cout << "Server socket binding failed. Code: " << WSAGetLastError() << endl;
    WSACleanup();
    return 1;
  }

  thread listener_thread(listener, server, service);



  listener_thread.join();

  closesocket(server);
  WSACleanup();

  return 0;
}
