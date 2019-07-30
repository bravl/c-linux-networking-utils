/**
 * @file client.c
 * @brief  Simple tcp client
 * @note Compile: gcc -o client client.c
 * @author Bram Vlerick <bram.vlerick@openpixelsystems.org>
 * @version v0.0.
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

#define TCP_IP_ADDR "127.0.0.1"
#define TCP_PORT 4000

int main()
{
	printf("Running client\n");
	int sock = 0;
	int opt = 1;
	int oplen = sizeof(opt);
	struct sockaddr_in conn;
	struct hostent *lookup;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		printf("Fail\n");
		return -1;
	}

	memset(&conn, 0, sizeof(struct sockaddr_in));

	/* Retrieve host details */
	lookup = gethostbyname(TCP_IP_ADDR);	
	if (!lookup) {
		printf("Could not find server\n");
		return -1;
	}

	/* Copy to our connection */
	bcopy(lookup->h_addr, &conn.sin_addr, lookup->h_length);
	conn.sin_family = AF_INET;
	conn.sin_port = htons(TCP_PORT);

	/* Connect to server */
	int error = connect(sock, (struct sockaddr *)&conn, sizeof(struct sockaddr));
	if (error < 0) {
		printf("Failed to connect to port\n");
		return -1;
	}

	printf("connected!");

	sleep(1);
	close(sock);
}
