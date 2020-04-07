#include "server.h"
#include <iostream>
#include <thread>

using namespace std;


int main ()
{
	Server server;
	cerr << "server is created " << endl;

	
	
	cerr << "second thread " << endl;

	thread get_messages ([&] {
															for (;;) {
																timeval timeout;
																timeout.tv_sec = 15;
																timeout.tv_usec = 0;
																
																fd_set sockets;
																for (const auto [sock, name] : server.GetSockets()) {
																	FD_SET(sock, &sockets);
																}

																int n = select (server.QuantityClients()+1, &sockets, NULL, NULL, &timeout);
																if (n == 0) { continue;}
																server.SendAll(n, server.GetMessage(n));
															}
	});
	get_messages.detach();

	for (;;) {
		server.Listen();
	}

	return 0;
}
