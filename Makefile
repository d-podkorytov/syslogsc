all: syslogc syslogs syslogs2 supervisor syslogs3 

syslogc: syslogc.c
	gcc syslogc.c -o syslogc
	strip syslogc

syslogs: syslogs.c
	gcc syslogs.c -o syslogs
	strip syslogs

supervisor: supervisor.c
	gcc supervisor.c -o supervisor
	strip supervisor

syslogs2: syslogs2.c
	gcc syslogs2.c -o syslogs2
	strip syslogs2

syslogs3: syslogs3.c
	gcc syslogs3.c -o syslogs3
	strip syslogs3

start:
	./syslogs > /var/log/syslogs.log

clean:
	rm -f 1 2 *~ *core* err r s syslogc syslogs syslogs2 syslog3
	
test:
	./syslogc "test1" "test2"	

tar:    
	(cd .. && tar cvfz syslogsc.tgz syslogsc)