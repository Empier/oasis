#include <winsock2.h>
#include <Windows.h>

#include <stdio.h>
#include <mmsystem.h> //Sound


#ifdef __cplusplus
extern "C" {
#endif
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#ifdef __cplusplus
}
#endif
#pragma comment(lib,"lua53.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"iphlpapi.lib")
#pragma comment(lib, "winmm.lib")


SOCKET server_sock;
SOCKET server_sock2;

int channel_port = 10100;
SOCKADDR_IN server_addr;


int start_socket()
{
	int buflen = 0x3FFFF;
	char buf[0x3FFFF] = { 0, };
	char buf2[0x3FFFF] = { 0x00, };
	int ret = 0;
	int datalen = 0;
	WSADATA wsaData;

	if (WSAStartup(0x202, &wsaData) == -1)
	{
		printf("WSAStartup error!\n");
		return -1;
	}


	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&server_addr, 0, sizeof(SOCKADDR_IN));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(channel_port);
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		printf("bind error!\n");
		closesocket(server_sock);
		return -1;
	}
	
	listen(server_sock, 1000);
	int acceptlen = sizeof(server_addr);

	
	server_sock2 = accept(server_sock, (SOCKADDR*)(&server_addr), &acceptlen);

	setsockopt(server_sock2, SOL_SOCKET, SO_RCVBUF, (char *)&buflen, sizeof(buflen));
	setsockopt(server_sock2, SOL_SOCKET, SO_SNDBUF, (char *)&buflen, sizeof(buflen));

	//u_long arg = 1;
	//ioctlsocket(server_sock2, FIONBIO, &arg);



	if (server_sock2 == SOCKET_ERROR)
	{
		printf("accept error!\n");
		closesocket(server_sock2);
		closesocket(server_sock);
		return -1;
	}

	datalen = recv(server_sock2, buf, 0x3FFFF, 0);
	send(server_sock2, "ASDF", 4, 0);





	closesocket(server_sock2);
	closesocket(server_sock);
}

int start_db()
{
	//mysql
	return 0;
}


int load_config()
{
	//lua
	return 0;
}

int main()
{
	load_config();
	start_db();
	start_socket();
	return 0;
}