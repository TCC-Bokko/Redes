#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <iostream>

int main(int argc, char** argv){

	struct addrinfo hints;
	memset((void*) &hints,'\0', sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;

	struct addrinfo* res;
	int rc = getaddrinfo(argv[1], argv[2], &hints, &res);
	if(rc != 0){
		std::cout << "Error: " << gai_strerror(rc) << std::endl;
		return -1;
	}

	for(struct addrinfo* tmp = res; tmp != 0; tmp = tmp->ai_next){
		char host[NI_MAXHOST];
		char serv [NI_MAXSERV];
		getnameinfo(tmp->ai_addr, tmp->ai_addrlen, 
			host, NI_MAXHOST, serv, NI_MAXSERV, NI_NUMERICHOST | NI_NUMERICSERV);
		
		std::cout << host << "   " << tmp->ai_family << "   " << tmp->ai_socktype << std::endl;
		
	}
	freeaddrinfo(res);
	return 0;
}