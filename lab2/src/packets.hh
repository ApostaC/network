/*
typedef enum {
	DATA = 0,
	HELLO,
	ACK 
} packet_types;
*/
#define FIN 0x1
#define SYN 0x2
#define RST 0x4
#define ACK 0x8
#define ECE 0x10
#define CWR 0x20
#define NS 0x40
#define HELLO 0x80
#define LSP 0x100

#define MAXTTL 10

/* It would be much more convenient to put TCP header and IP header together,
   given that we are not using other protocols */
struct Pkthdr {
	/* IP part */
	uint32_t srcip;
	uint32_t dstip;
	uint16_t size;
	uint8_t ttl;	// Time to Live
	uint8_t ecn;	// Explicit Congestion Control flag
	/* TCP part */
	uint32_t seq;
	uint32_t ack;
	uint16_t cwnd;	// Congestion Window size
	//uint16_t srcport;
	//uint16_t dstport;
	uint8_t flags;
};
