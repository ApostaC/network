
all: test nameserver test2

test: test.c mydns.o
	gcc test.c -o test -g

test2: test2.c mydns.c mydns.h
	gcc test2.c mydns.c mydns.h -o test2 -g

mydns: mydns.c mydns.h
	gcc mydns.c mydns.h -g

nameserver: dns_server.c mydns.h mydns.c parse_lsa.h parse_lsa.c
	gcc dns_server.c mydns.h mydns.c parse_lsa.h parse_lsa.c -o nameserver -g
