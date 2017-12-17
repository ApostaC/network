#include "mydns.c"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#define MAXDATASIZE 1500
void printTo(FILE *f, int len, char * buf)
{
	for(int i=0;i<len;i++)
	{
		fprintf(f,"%c",buf[i]);
	}
	fprintf(f,"\n");
}
int main(int argc, char * argv[])
{
	char buf[300],buf2[100];
	const char *req_addrs[3];
	memset(req_addrs,0,sizeof(req_addrs));
	req_addrs[0] = "video.pku.edu.cn";
	req_addrs[1] = "video.pku.edu.cn";
	req_addrs[2] = "pku.edu.cn";
	struct in_addr *res_addrs[3];
	uint32_t u1=0x31323334,u2=0x41424344,u3=0x51525354;
	res_addrs[0] = (struct in_addr *)(&u1);
	res_addrs[1] = (struct in_addr *)(&u2);
	res_addrs[2] = (struct in_addr *)(&u3);
	int len = fill_request(3,req_addrs,0x1111,buf);
	//len = dns2addr(buf+sizeof(struct DNS_header_t),buf2);
	//char *dstaddr[3],addr1[100],addr2[100],addr3[100];
	//dstaddr[0] = addr1;
	//dstaddr[1] = addr2;
	//dstaddr[2] = addr3;
//	len = resolve_request_info(buf,&dstaddr);
//	for(int i=0;i<3;i++) fprintf(stdout,"%s\n",dstaddr[i]);
	FILE * f = fopen("test.out","w");
	printTo(f,len,buf);
	fclose(f);
	f=fopen(argv[2],"w");

	int sockfd, num;
	struct hostent *he;
	struct sockaddr_in server,peer;

	if (argc !=4)
	{
		printf("Usage: %s <IP Address><file><client ip address>\n",argv[0]);
		exit(1);
	}

	if ((he=gethostbyname(argv[1]))==NULL)
	{
		printf("gethostbyname()error\n");
		exit(1);
	}

	if ((sockfd=socket(AF_INET, SOCK_DGRAM,0))==-1)
	{
		printf("socket() error\n");
		exit(1);
	}

	struct sockaddr_in me;
	me.sin_family = AF_INET;
	me.sin_port = htons(1234);
	inet_aton(argv[3],&(me.sin_addr));
	bind(sockfd,(struct sockaddr *)&me, sizeof me);

	bzero(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(1234);
	server.sin_addr= *((struct in_addr *)he->h_addr);
	printTo(stdout,len,buf);
	fflush(stdout);
	sendto(sockfd, buf,len,0,(struct sockaddr *)&server,sizeof(server));
	socklen_t  addrlen;
	addrlen=sizeof(server);
	while (1)
	{
		if((num=recvfrom(sockfd,buf,MAXDATASIZE,0,(struct sockaddr *)&peer,&addrlen))== -1)
		{
			printf("recvfrom() error\n");
			exit(1);
		}
		if (addrlen != sizeof(server) ||memcmp((const void *)&server, (const void *)&peer,addrlen) != 0)
		{
			printf("Receive message from otherserver.\n");
			continue;
		}

		buf[num]='\0';
		printTo(f,num,buf);
		fflush(f);
		printf("get msg! ");
		struct addrinfo * p;
		size_t temp2 = resolve_respond_info(buf,&p);
		fprintf(stderr,"%ld p=%s\n",temp2,inet_ntoa(((struct sockaddr_in*)(p->ai_addr))->sin_addr));
	}

	close(sockfd);

	return 0;
}
