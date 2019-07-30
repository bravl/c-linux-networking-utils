/**
 * @file server.c
 * @brief Small TCP example 
 * @note Compilation: gcc -o server server.c
 * @author Bram Vlerick <bram.vlerick@openpixelsystems.org>
 * @version v0.0
 * @date 2019-07-30
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>

#define TCP_PORT 4000

int main()
{
	printf("Running server\n");
	int sock = 0;
	int opt = 1;
	int oplen = sizeof(opt);
	struct sockaddr_in serv;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("Fail");
		return -1;
	}

	/* Allow TCP to reconnect to a port (Otherwise you'll need 
	 * to wait for the timeout which is annoying */
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, oplen);
	
	memset(&serv, 0, sizeof(struct sockaddr_in));
	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = htonl(INADDR_ANY); // Allow all addresses to connect
	serv.sin_port = htons(TCP_PORT); // Port 4000

	/* Bind our socket to the information provided above*/
	int error = bind(sock, (struct sockaddr *)&serv, sizeof(struct sockaddr));
	if (error < 0) {
		printf("Failed to bind to port\n");
		return -1;
	}

	/* Allow a max of 10 connections */
	error = listen(sock, 10);
	if (error < 0) {
		printf("Failed to listen");
		return -1;
	}

	/* Start accepting clients here */
	for (;;) {
		/* client will contain our connection data */
		struct sockaddr_in client;
		socklen_t len = sizeof(struct sockaddr);
		memset(&client, 0, sizeof(struct sockaddr_in));
		int new_sock = accept(sock, (struct sockaddr*)&client, &len);
		if (new_sock < 0) {
			printf("Accept failed\n");
			continue;
		}

		/* Following code should theoretically be pushed to either a thread or forked if
		 * you want concurrent connections. */
		
		printf("Connection from: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

		/* Read / Write here */
		
		close(new_sock);
	}
}
