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

SOCKET sock;
SOCKET server_sock;

int channel_port = 10100;
SOCKADDR_IN server_addr;


int sock_num;
int max_sock;

SOCKET clnt_sock;
SOCKET client[4000];



fd_set read_socks, all_socks;
struct timeval timeout;
int clnt_num = 0;
int clnt_len = 0;
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
	



	setsockopt(server_sock, SOL_SOCKET, SO_RCVBUF, (char *)&buflen, sizeof(buflen));
	setsockopt(server_sock, SOL_SOCKET, SO_SNDBUF, (char *)&buflen, sizeof(buflen));
	u_long arg = 1;
	ioctlsocket(server_sock, FIONBIO, &arg);

	max_sock = server_sock;

	memset(client, -1, sizeof(client));

	FD_ZERO(&read_socks);
	FD_SET(server_sock, &read_socks);
	


	timeout.tv_sec = 3;
	timeout.tv_usec = 0;


	while (1)
	{
		all_socks = read_socks;
		
		sock_num = select(max_sock + 1, &all_socks, (fd_set *)0, (fd_set *)0, &timeout);
		//printf("3");
		if (FD_ISSET(server_sock, &all_socks))
		{
			int acceptlen = sizeof(server_addr);
			clnt_sock = accept(server_sock, (SOCKADDR*)(&server_addr), &acceptlen);

			
			if (clnt_num > 4000)
			{
				closesocket(clnt_sock);
			}
			else
			{
				for (int i = 0; i < 4000; i++)
				{
					if (int(client[i]) < 0)
					{
						client[i] = clnt_sock;
						printf("New Connect : %d\n", client[i]);
						break;
					}
				}
		
				FD_SET(clnt_sock, &read_socks);

				if (clnt_sock > max_sock)
					max_sock = clnt_sock;

				clnt_num++;


			}

		
		}
		

		for (int i = 0; i < 4000; i++)
		{
			if ((sock = int(client[i])) == -1)
				continue;
		
			if (FD_ISSET(sock, &all_socks))
			{
				clnt_len = recv(sock, buf, 0x3FFF - 1, 0); //¿¬°á²÷±â¸é -1°¡Á®¿È
				
				
				if (clnt_len <= 0 || !strncmp(buf, "bye", 3))
				{
					send(sock, "bye", 3, 0);
					printf("disconnect\n");
					closesocket(sock);
					FD_CLR(sock, &read_socks);
					client[i] = -1;
					clnt_num--;
				}
				else
				{
					send(sock, "hehe", 4, 0);
				}

			}
		}

		
	}

	
	



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