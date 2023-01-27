#include<windows.networking.h>
#include<iostream>
#include<string>
#include<bitset>
#include<thread>
#include<vector>
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
	unsigned int bin = 0;
	string currentNum = "";
	for (auto c : ip) {
		if (c != '.') {
			currentNum += c;
		}
		else {
			string currentBin = bitset<8>(stoi(currentNum)).to_string();
			for (auto b : currentBin) {
				bin <<= 1;
				if (b == '1')
					bin += 1;
			}
			currentNum = "";
		}
	}
	return bin;
}


int main() {

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 1;
	}

	struct sockaddr_in server_info = { 0 };

	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = htonl(iptoint("192.168.56.1."));
	server_info.sin_port = htons(42069);
	int server_info_len = sizeof(server_info);


	SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket == -1) {
		perror("Server socket");
		return -1;
	}


	if (connect(server_socket, (struct sockaddr*)&server_info, server_info_len) < 0) {
		perror("Bind");
		return -1;
	}
	cout << "Connected to the server!" << endl;

	char inputText[1500];
	string outputText;

	while (1)
	{
		cout << ">";
		getline(cin, outputText);
		memset(&inputText, 0, sizeof(inputText));//clear the buffer
		strcpy_s(inputText, outputText.c_str());
		if (outputText == "exit")
		{
			send(server_socket, (char*)&inputText, strlen(inputText), 0);
			break;
		}
		send(server_socket, (char*)&inputText, strlen(inputText), 0);
		cout << "Awaiting server response..." << endl;
		memset(&inputText, 0, sizeof(inputText));//clear the buffer
		recv(server_socket, (char*)&inputText, sizeof(inputText), 0);
		if (!strcmp(inputText, "exit"))
		{
			cout << "Server has quit the session" << endl;
			break;
		}
		cout << "Server: " << inputText << endl;
	}

	closesocket(server_socket);
	cout << "Connection closed." << endl;

}