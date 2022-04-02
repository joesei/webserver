// webserver.h : Include file for standard system include files,
// or project specific include files.

#ifndef _WEB_SERVER_
#define _WEB_SERVER_

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "clientsession.h"
#include "socket.h"

class WebServer {
 public:
  WebServer();
  ~WebServer();
  void Start();
  
 private:
  std::mutex session_mutex_;
  std::vector<ClientSession> client_sessions_;
  Socket server_socket_;

  void ListenerThread();
  void WorkingThread();
};



#endif