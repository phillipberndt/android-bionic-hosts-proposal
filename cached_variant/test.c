#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <strings.h>
#include <utime.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct addrinfo pai;
extern int _cached_hosts_lookup(const char *name, const struct addrinfo *pai, struct addrinfo **retval);

int main(int argc, char *argv[]) {
	if (argc < 2) return;

	char host[255];
	struct addrinfo *retval;
	if(_cached_hosts_lookup(argv[1], &pai, &retval) == 0 && retval) {
		getnameinfo(retval->ai_addr, retval->ai_addrlen, host, 255, NULL, 0, NI_NUMERICHOST);
		printf("%s\n", host);
	}
}
