#include "mydns.h"
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
int main(int argc, char *argv[])
{
	//./test2 <client_ip> <dns_ip> <dns_port> <logFile>
	if(argc!=5)
	{
		fprintf(stderr,"Usage ./test2 <client_ip> <dns_ip> <dns_port> <logFile>\n");
		exit(-1);
	}

	char buf1[] = "video.pku.edu.cn";
	char buf2[] = "pku.edu.cn";

	init_mydns(argv[2],atoi(argv[3]),argv[1]);
	struct addrinfo * p;
	resolve(buf1,argv[3],NULL,&p);
	fprintf(stderr,"1 ok\n");
	resolve(buf2,argv[3],NULL,&p);
	fprintf(stderr,"2 ok\n");
}
