#ifndef SIMPLE_HOST_HH
#define SIMPLE_HOST_HH
#include <click/element.hh>
#include <click/timer.hh>
#include <click/hashtable.hh>
#include <click/packet.hh>
enum State {STABLE, MOVING};
#define MAX_ROUTER_NUM 255
typedef int Graph[MAX_ROUTER_NUM][MAX_ROUTER_NUM];	

CLICK_DECLS

class SimpleRouter : public Element{
	public:
		SimpleRouter();
		~SimpleRouter();
		const char *class_name() const { return "SimpleRouter";}
	  	const char *port_count() const { return "1-/1-";}
		const char *processing() const { return PUSH; }

		int configure(Vector<String> &conf, ErrorHandler *errh);
		void push(int port, Packet *packet);
		int initialize(ErrorHandler*);
		void run_timer(Timer*);

	public:
		static const uint32_t BROADCAST_IP			= 0xffffffff;
		static const uint16_t DEFAULT_LSP_TIMEVAL	= 10;
		static const uint16_t DEFAULT_HELLO_TIMEVAL	= 3;
	private:
		void dijkstra();
		void broadCast(WritablePacket *);
	private:
		uint32_t _myip;						//local ip addr
		HashTable<int,int> _routing_table;	//address -> port
		HashTable<int,int> _latest_lsp;		//address -> seq_num
		HashTable<int,int> _neighbours;		//address -> 1...
		Timer _timer_hello;
		Timer _timer_lsp;
		uint16_t _lsp_tval;
		uint16_t _hel_tval;
		uint32_t _time_out;
		uint32_t _seq;
		State _state;
		Graph _g;
			

};


CLICK_ENDDECLS


#endif 
