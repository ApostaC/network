#include "mydns.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>

/*
 * HELPER FUNCTION DEFINITIONS
 */
size_t addr2dns(const char * addr, char * dst); 
size_t dns2addr(const char * dns, char * addr);




/*
 * =================================
 *			IMPLEMENTATIONS
 * =================================
 */

/**
 * HELPER FUNCTION addr2dns(addr,dst)
 *
 * convert an address (such as www.baidu.com) to fit the
 * dns packet format
 * 
 * @param addr	the specified address
 * @param dst	the destination, can be a buffer or the packet
 *
 * @returns	the size of the dns_string if successed
 *			else return 0 [ERROR OCCURED]
 */
size_t addr2dns(const char * addr, char * dst)
{
	if(!addr) return 0;		//INVALID ADDRESS STRING
	if(!dst) return 0;		//INVALID DESTINATION BUFFER

	size_t paddr=0,pdst=0;
	u_char currlen=0;
	size_t length = strlen(addr);	//addr = "www.baidu.com0"
	memset(dst,0,length+2);			//dst = "000000000000000"
	strcpy(dst + 1, addr);	//dst = "0www.baidu.com0"
	while(paddr < length)
	{
		if (addr[paddr] == '.')
		{
			dst[pdst] = currlen;
			paddr++;
			pdst = paddr;
			currlen = 0;
		}
		else
		{
			currlen ++;
			paddr++;
		}
	}								//dst = "3www5baidu.com0"
	dst[pdst]=currlen;				//dst = "3www5baidu3com0"
	return length+2;
}

/**
 * HELPER FUNCTION dns2addr(dns,addr)
 * convert dns payload into address (such as www.baidu.com)
 *
 * @param dns	the dns packet, starts at the first bit of "QNAME"
 *				field
 * @param addr	the buffer to fill
 *
 * @returns the length of dns packet parsed
 */
size_t dns2addr(const char * dns, char * addr)
{
	if(dns == NULL || addr == NULL) return 0;
	size_t ret = 0,templen = 0;
	while(*dns)
	{
		templen = (size_t)(*dns);	//length 
		dns++;						//point to next
		memcpy(addr,dns,templen);	//copy
		addr+=templen;dns+=templen;	//update pointers
		*addr='.'; addr++;			//add '.'
		ret += templen+1;			//update ret
	}
	*(--addr) = '\0';				//add '\0'
	return ret+1;					//last '\0' in dns
}


/**
 * IMPLEMENTATION OF fill_request(re_num,addrs,payload)
 */
size_t fill_request(int req_num, const char * req_addr[], int id, char * payload)
{
	int ret = 0;
	if(req_num < 0 || req_addr == NULL || payload == NULL) 
		return 0;
	//FILL HEADER
	struct DNS_header_t * pheader;
	memset(payload,0,sizeof(struct DNS_header_t));
	pheader = (struct DNS_header_t *) payload;
	pheader->ID = htons(id);
	pheader->tag = htons(REQUEST_TAG);
	pheader->QDcount = htons(req_num);
	
	ret += sizeof(struct DNS_header_t);
	payload += sizeof(struct DNS_header_t);


	//Fill body
	size_t tempsize = 0;
    struct DNS_request_t *requset_header;	
	for(int i=0;i<req_num;i++)
	{
		tempsize = addr2dns(req_addr[i], payload);	//FILL QNAME FIELD
		ret+=tempsize;
		payload += tempsize;

		requset_header = (struct DNS_request_t *) payload;
		requset_header->qtype = htons(1);
		requset_header->qclass = htons(1);
		ret += sizeof(struct DNS_request_t);
		payload += sizeof(struct DNS_request_t);
	}
	return ret;
}

size_t fill_response(int req_num, const char * req_addrs[], int id, const struct in_addr * res_addrs[], char * payload, int flag)
{
	if(req_num < 0 || req_addrs == NULL ||
		   (res_addrs == NULL && flag == RESPOND_TAG)
		   || payload == NULL)
		return 0;
	size_t ret = 0;
	// FILL HEADER
	memset(payload,0,sizeof(struct DNS_header_t));
	struct DNS_header_t * pheader = (struct DNS_header_t *)payload;
	pheader->ID = htons(id);
	pheader->tag = htons(flag);
	pheader->ANcount = htons(req_num);
	ret += sizeof(struct DNS_header_t);
	payload += sizeof(struct DNS_header_t);
	

	//FILL BODY
	if(flag == RESPOND_TAG)
	{
		size_t tempsize = 0;
		struct DNS_respond_t * res_header;
		for(int i=0;i<req_num;i++)
		{
			//fill name
			tempsize = addr2dns(req_addrs[i],payload);
			ret+=tempsize; payload+=tempsize;

			//fill DNS_respond_t
			res_header = (struct DNS_respond_t *)payload;
			res_header->rtype = htons(1);
			res_header->rclass = htons(1);
			res_header->ttl = 0;
			res_header->rd_length = htons(sizeof(struct in_addr));
			ret += sizeof(struct DNS_respond_t);
			payload += sizeof(struct DNS_respond_t);

			//fill RDATA
			//TODO: this line here is not good ...
			*(uint32_t*)payload = htonl(res_addrs[i]->s_addr);
			tempsize=sizeof(struct in_addr);
			ret+=tempsize; payload+=tempsize;
		}
	}
	return ret;
}

size_t resolve_request_info(char * payload, char *(*dst)[])
{
	if(dst==NULL || payload == NULL) return 0;

	const size_t BUFFER_MAX_SIZE = 50;
	struct DNS_header_t *pheader = (struct DNS_header_t *)payload;
	uint16_t cnt = ntohs(pheader->QDcount);
	if(cnt == 0) return 0;
	
	payload += sizeof(struct DNS_header_t);
	size_t templen=0;
	for(int i=0;i<cnt;i++)
	{	
		(*dst)[i] = (char*)malloc(BUFFER_MAX_SIZE);
		templen = dns2addr(payload, (*dst)[i]);
//		fprintf(stdout,"%s\n",(*dst)[i]);
		if(templen == 0)		//parse failed
			return 0;			//*dst[i] is null or payload is null
		payload += templen;
		payload += sizeof (struct DNS_request_t);
	}
	return cnt;

}
