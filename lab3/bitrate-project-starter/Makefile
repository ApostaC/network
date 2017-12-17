
all: test nameserver

test: test.c mydns.o
	gcc test.c -o test -g

mydns: mydns.c mydns.h
	gcc mydns.c mydns.h -g

nameserver: dns_server.c mydns.h mydns.c parse_lsa.h parse_lsa.c
	gcc dns_server.c mydns.h mydns.c parse_lsa.h parse_lsa.c -o nameserver -g
