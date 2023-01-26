#include<windows.networking.h>
#include<iostream>
#include<string>
#include<bitset>
#pragma comment(lib, "Ws2_32.lib")

//port 42069
//local ip is 10.17.68.58
// 00001010000100010100010000111010
// 
//declan ip is 10.17.68.53
// 0b00001010000100010100010000110101
// 168903733
// 
//cory ip is 10.17.68.??
//aidan ip is 10.17.68.59

using namespace std;


unsigned int iptoint(string ip) {
	string pt1 = "";
	string pt2 = "";
	string pt3 = "";
	string pt4 = "";
	int pt = 0;
	for (auto c : ip) {
		if (c != '.') {
			switch (pt) {
			case 0:
				pt1 += c;
				break;
			case 1:
				pt2 += c;
				break;
			case 2:
				pt3 += c;
				break;
			case 3:
				pt4 += c;
				break;
			}
		}
		else
			pt++;
	}
	string binip = "";

	binip += bitset<8>(stoi(pt1)).to_string();
	binip += bitset<8>(stoi(pt2)).to_string();
	binip += bitset<8>(stoi(pt3)).to_string();
	binip += bitset<8>(stoi(pt4)).to_string();

	return stoi(binip, 0, 2);
}


int main() {

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 1;
	}

	struct sockaddr_in server_info = { 0 };

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = htonl(iptoint("10.17.68.59"));
	server_info.sin_port = htons(42069);
	int server_info_len = sizeof(server_info);


	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0) {
		perror("Server socket");
		return -1;
	}


	if (connect(server_socket, (struct sockaddr*)&server_info, server_info_len) < 0) {
		perror("Bind");
		return -1;
	}

	const int buffer_len = 1024;
	char buffer[buffer_len];

	int recvd = recv(server_socket, buffer, buffer_len - 1, 0);
	cout << buffer << endl;

	string uin;

	closesocket(server_socket);

}