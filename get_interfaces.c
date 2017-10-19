#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <linux/if_link.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define malloc_and_copy(type,dest,src,var) \
	dest->var = malloc(sizeof(type)); \
	memset(dest->var,0,sizeof(type)); \
	memcpy(dest->var,src->var,sizeof(type));

int copy_ifaddrs(struct ifaddrs *dest, struct ifaddrs *src)
{
	if (!dest || !src) return -1;
	if (dest == src) return 0;

	dest->ifa_name = malloc(strlen(src->ifa_name) +1);
	memset(dest->ifa_name,0,strlen(src->ifa_name) +1);
	memcpy(dest->ifa_name,src->ifa_name,strlen(src->ifa_name));

	dest->ifa_flags = src->ifa_flags;

	malloc_and_copy(struct sockaddr, dest,src,ifa_addr);
	malloc_and_copy(struct sockaddr, dest,src,ifa_netmask);

	// TODO: use ifa_flags to select correct union var
	//malloc_and_copy(struct sockaddr, dest,src,ifa_ifu);

}

struct ifaddrs *get_interfaces(int type)
{
	int family;
	struct ifaddrs *intfs = NULL, *intf = NULL,
		       *intfret = NULL, *intfiter = NULL;

	if (getifaddrs(&intfs) < 0) {
		printf("Failed to retrieve interfaces\n");
		return NULL;
	}

	for (intf = intfs; intf->ifa_next != NULL; intf = intf->ifa_next) {
		family = intf->ifa_addr->sa_family;
		if (intf->ifa_addr  && family == type) {
			struct ifaddrs *tmp = malloc(sizeof(struct ifaddrs));
			memset(tmp,0,sizeof(struct ifaddrs));

			copy_ifaddrs(tmp,intf);
			if (!intfret){
				intfret = tmp;
				intfiter = intfret;
			} else {
				intfiter->ifa_next = tmp;
				intfiter = intfiter->ifa_next;
			}
		}
	}
	freeifaddrs(intfs);
	return intfret;
}

void retrieve_ip_address(struct ifaddrs *intf)
{
	int family;
	char host[NI_MAXHOST];

	getnameinfo(intf->ifa_addr, (family == AF_INET) ?
		    sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
		    host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
	printf("%s\n",host);
}

void cleanup_list(struct ifaddrs *head)
{
	struct ifaddrs *delete,*tmp;
	for (delete = head; delete != NULL; delete = tmp) {
		tmp = delete->ifa_next;
		free(delete->ifa_name);
		free(delete->ifa_addr);
		free(delete->ifa_netmask);
		free(delete);
	}
	head = NULL;
}

int main(void) {
	struct ifaddrs *intf = NULL, *ret = NULL, *ret2 = NULL;
	printf("IPv4\n");
	ret = get_interfaces(AF_INET);
	for (intf = ret; intf != NULL; intf = intf->ifa_next) {
		printf("%s: ",intf->ifa_name);
		retrieve_ip_address(intf);
	}
	cleanup_list(ret);
	if (!ret) printf("List cleaned\n");

	printf("IPv6\n");
	ret = get_interfaces(AF_INET6);
	for (intf = ret; intf != NULL; intf = intf->ifa_next) {
		printf("%s: ",intf->ifa_name);
		retrieve_ip_address(intf);
	}
	cleanup_list(ret);
}
