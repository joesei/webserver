#include "webserver.h"

// TODO: Check the process of the accept socket to look for error

int main() {
	auto server = new WebServer();
	server->Start();
	delete server;
	return 0;
}