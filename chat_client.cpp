#include "client.h"
#include <iostream>
#include <future>

using namespace std;


int main ()
{
	cout << "Enter you name:  ";
	string name;
 	cin >> name;

	Connection conn(name);
	conn.Create();

	std::ios::sync_with_stdio(false);

	thread read_data(     [&]{
														for (string mess;getline(cin, mess);) {
																conn.Send(move(mess));
	}
	});
	read_data.detach();
										for (;;) {
															cout << conn.Recv() << endl;;
														}
	

	return 0;
}
