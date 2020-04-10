//client.h
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#include <iostream>

class Connection {
public:
	Connection(const std::string& name): name(name) {
		inet_aton("127.0.0.1",&addr.sin_addr);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(34253);
	}
	~Connection() {
		close(sock);
	}
	void Send(std::string mess) {
		const char * buf = mess.c_str();
		int n = 0;
		for(size_t total = 0; total < mess.size(); total+=n)
		{
			n = send(sock, buf+total, mess.size()-total, 0);
			if (n == -1){close(sock); exit(0);} ;
		}
	}

	std::string Recv() 
	{
		char buf[150];
		size_t size_buf = 150;
		int n = 0;
			n = recv(sock, buf, size_buf, 0);
			if (n == 0) {close(sock); exit(0);}
			buf[n-1] = '\0';
		return std::string(buf, n);
	}
	void Create() 
	{
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (connect(sock, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
			std::cerr << " connect error" << std::endl;
			exit(1);
		}
	}


private:
	std::string name;
	int sock;
	struct sockaddr_in addr;
};

