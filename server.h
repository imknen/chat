//server.h
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include <unordered_map>
#include <utility>
#include <cstring>
#include <deque>
#include <iostream>
class Server {
public:
	Server()
	{
	listener = socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family = AF_INET;
	inet_aton("127.0.0.1",&addr.sin_addr);
	addr.sin_port = htons(34253);

	bind(listener, (struct sockaddr *) &addr, sizeof(addr));
	}
	~Server()
	{
		close(listener);
		for (auto [key,value] : sock_to_client) {
			close(key);
		}
	}
	std::string GetMessage(int sock)
	{
		char buf[150];
    size_t size_buf = 150;
    int n = 0;
    n = recv(sock, buf, size_buf, 0);
    buf[n] = '\0';

	 return std::string(buf, n);
	}
	void Send(int s, std::string mess) 
	{
		auto str = mess.c_str();
		int n = 0;
    for(size_t total = 0; total < mess.size(); total+=n)
    { 
      n = send(s, str+total, mess.size()-total, 0);
      if (n == -1) break;
    }
	}
	void SendAll(int s, std::string message)
	{
		auto & user = sock_to_client[s];
		std::string mess = user + ": " + message; 
		for (const auto& [sock, name] : sock_to_client) {
			Send(sock, move(mess));
		}
	}
	//       new sock   name
	std::pair<int, std::string> GetName(int sock)
	{
		char name[20];
		int n = recv(sock, name, 20, 0);
		name[n] = '\0';
		SendAll(sock, std::string(name, n));

		return {sock, std::string(name, n)};
	}
	void AddUser(int sock)
	{

		auto [newsock, name] = GetName(sock);
		sock_to_client[newsock] = name;
		Send(newsock, std::string("Welcome "+name));
		FD_SET(newsock, &sockets);
	}
	void Listen() 
	{

		listen(listener, 1);
		addrs.push_back({});
		auto & remaddr = addrs.back();
		int cs;
		cs = accept(listener, (struct sockaddr *) &remaddr.addr, &remaddr.remadrs);
		if (cs != -1)
		{
			AddUser(cs);
		}
	}
	int QuantityClients()
	{
		return sock_to_client.size();
	}           

	const std::unordered_map<int, std::string>& GetSockets() {
		
		return sock_to_client;
	}

private:
	struct Conn {
	public:
		Conn() {remadrs = sizeof(addr);}
		struct sockaddr_in addr;
		socklen_t remadrs;
	};

	struct sockaddr_in addr;
	int listener;
	std::unordered_map<int, std::string> sock_to_client;
	fd_set sockets;
	std::deque<Conn> addrs;
};
