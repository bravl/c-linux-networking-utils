#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <unistd.h>
#include <stdlib.h>

struct ifaddrs *get_interfaces(int type)
{
	int family;
	struct ifaddrs *intfs, *intf, *intfret;
	if (getifaddrs(&intfs) < 0) {
		printf("Failed to retrieve interfaces\n");
		return NULL;
	}
	for (intf = intfs; intf->ifa_next != NULL; intf = intf->ifa_next) {
		family = intf->ifa_addr->sa_family;
		if (intf->ifa_addr  && family == type)
			printf("%s\n", intf->ifa_name);
	}
}

int main(void) {
	get_interfaces(AF_INET);
	get_interfaces(AF_INET6);
}
