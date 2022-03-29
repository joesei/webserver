// webserver.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include <mutex>
#include <vector>

#include "clientsession.h"

class WebServer {
 public:
  std::mutex session_mutex_;
  std::vector<ClientSession> client_sessions_;

  WebServer();
  ~WebServer();
  void static ListenerThread();
  void static WorkingThread();




 private:

};

