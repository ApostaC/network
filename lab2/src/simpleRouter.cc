#include <click/config.h>
#include <click/confparse.hh>
#include <click/timer.hh>
#include <click/error.hh>	
#include <click/packet.hh>
#include <arpa/inet.h>
#include "simpleRouter.hh"
#include "packets.hh"

extern WritablePacket *make_packet(uint32_t srcip, uint32_t dstip, uint16_t size, uint8_t ttl, uint8_t ecn,
							uint32_t seq, uint32_t ack, uint16_t cwnd, uint8_t flags, char *content);
CLICK_DECLS

SimpleRouter::SimpleRouter():_timer_lsp(this),_timer_hello(this)
{
	_lsp_tval=DEFAULT_LSP_TIMEVAL;
	_hel_tval=DEFAULT_HELLO_TIMEVAL;
	_state=MOVING;
}
int SimpleRouter::initialize(ErrorHandler *errh)
{
	_timer_hello.initialize(this);
	_timer_lsp.initialize(this);
	_timer_hello.schedule_after_sec(_hel_tval);
	_timer_lsp.schedule_after_sec(_lsp_tval);
}

int SimpleRouter::configure(Vector<String> &conf, ErrorHandler *errh) {
{
	if (cp_va_kparse(conf, this, errh,
				"MYIP", cpkP + cpkM, cpUnsigned, &_myip,
				"PERIOD_LSP", cpkP, cpUnsigned, &_lsp_tval,
				"PERIOD_HELLO", cpkP, cpUnsigned, &_hel_tval,
				"TIME_OUT", cpkP, cpUnsigned, &_time_out,
				cpEnd) < 0) {
		return -1;
	}
	return 0;
}

void SimpleRouter::broadCast(WritablePacket * packet)
{
	for(int i=0;i<noutputs();i++)
	{
		if(port_active(true,i)){
			click_chatter("broadcasting the existence of the router whose ip address is %s\n",inet_ntoa(in_addr(_myip)));
			output(i).push(packet);
		}
	}
}
void SimpleRouter::run_timer(Timer * t)
{
	WritablePacket *packet;
	if(t==&_timer_hello)	//broadcast
	{
		packet=make_packet(_myip,SimpleRouter::BROADCAST_IP,sizeof(Pkthdr),2,0,_seq,0,0,HELLO,NULL);
		broadCast(packet);
	}
	else if(t==&_timer_lsp)
	{
		if(_state==STABLE)
		{
			click_chatter("no new graph state, skip...!\n");
			return;	
		}
		//routingTable->packet
		int neigh[_neighbours.size()];
		memset(neigh,0,sizeof(neigh));
		int ind=0;
		for(auto entry : _neighbours)
		{
			neigh[ind++]=entry.first;
		}
		packet=make_packet(_myip,0,sizeof(Pkthdr)+sizeof(neigh),2,0,++_seq,0,0,LSP,(char*)(&neigh));
		broadCast(packet);
	}
	else
	{
		assert(false);
	}
}



int SimpleRouter::push(int port, Packet *packet)
{
	assert(packet);
	struct Pkthdr *oldhdr = (struct Pkthdr *)(packet->data());
	WritablePacket *ack;
	//in port 0?
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SimpleRouter)
