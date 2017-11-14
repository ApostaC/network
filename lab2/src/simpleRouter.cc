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
		packet=make_packet(_myip,SimpleRouter::BROADCAST_IP,sizeof(Pkthdr),2,0,++_seq,0,0,HELLO,NULL);
		broadCast(packet);
	}
	else if(t==&_timer_lsp)
	{
		//routingTable->packet
		int neigh[_neighbours.size()];
		memset(neigh,0,sizeof(neigh));
		int ind=0;
		for(auto entry : _neighbours)
		{
			neigh[ind++]=entry.first;
		}
		if(_state==STABLE)
		{
			packet=make_packet(_myip,0,sizeof(Pkthdr)+sizeof(neigh),2,0,_seq,0,0,LSP,(char*)(&neigh));
		}else
		{
			packet=make_packet(_myip,0,sizeof(Pkthdr)+sizeof(neigh),2,0,++_seq,0,0,LSP,(char*)(&neigh));
		}

		broadCast(packet);
	}
	else
	{
		assert(false);
	}
}

bool SimpleRouter::updateNeigh(int addr,bool operation)
{
	if(operation)
	{
		return _neighbours.set(addr,1);
	}
	else
	{
		_neighbours.erase(addr);
		return false;
	}
}

int SimpleRouter::push(int port, Packet *packet)
{
	assert(packet);
	struct Pkthdr *oldhdr = (struct Pkthdr *)(packet->data());
	WritablePacket *ack;
	//is ack?
	if(oldhdr->flags & ACK)	
	{
		//add existence?
	}
	else if (oldhdr->flags & HELLO)	// is hello?
	{
		click_chatter("%s router received an hello packet from %s",inet_ntoa(_myip),inet_ntoa(oldhdr->srcip));
		//send ack back...
		ack=make_packet(_myip,oldhdr->srcip,sizeof(Pkthdr),2,0,oldhdr->seq,ACK,0,ACK | HELLO,NULL);
		output(port).push(ack);
		//add existence
		updateNeigh(oldhdr->srcip,true);
		packet->kill();
	}
	else if (oldhdr->flags & LSP)	// is link state packet?
	{
		click_chatter("%s router received an LSP packet from %s",inet_ntoa(_myip),inet_ntoa(oldhdr->srcip));
		ack=make_packet(_myip,oldhdr->srcip,sizeof(Pkthdr),2,0,oldhdr->seq,ACK,0,ACK | LSP,NULL);
		output(port).push(ack);
		for(int i=0;i<noutputs();i++)
		{
			if(i==port) continue;
			if(port_active(true,i)){
				click_chatter("broadcasting the existence of the router whose ip address is %s\n",inet_ntoa(in_addr(_myip)));
				output(i).push(packet);
			}
		}
		updateNeigh(oldhdr->srcip,true);
		packet->kill();
	}
	else	//data packet
	{
		if(oldhdr->ttl<0)
		{
			click_chatter("get a packet which ttl is less than 0, discard!");
			packet->kill();
		}
		oldhdr->ttl--;
		if(oldhdr->dstip == SimpleRouter::BROADCAST_IP)
		{
			click_chatter("%s router get a broadCast packet!");
			broadCast(packet);
			return;
		}
		//click_chatter("wrong packet type");
		//packet->kill();
	}
}

CLICK_ENDDECLS
EXPORT_ELEMENT(SimpleRouter)
