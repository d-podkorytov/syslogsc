# syslogsc

# DESCRIPTION

It starts as a holiday project, the main principles to be:
*) Simple (The size of source code of syslogs3.c is 2K byte and two tiny
   shell scripts run3.sh and wc.sh )

*) Robast 
*) Fault tolerance and automatic restarts it case of core dump's (If it
   fails it'll be restarted by timer)

*) Upgrade versions of service on fly, different version of syslogs may
   share UDP port, so just start new version of this syslog and then do
   tests and stops old

*) Soft adaptation and zero configuration for different types of incoming syslog messages
   It just parse syslog header "<N>" for N and write messages to related file /var/log/N-syslogs.log

Syslogsc is the simpleist syslog daemons set and command line client with supervisors inside.

This server runs as N workers, each is separated OS process with supervisor inside. 
Internal supervisors is timers, based on ALARM system call.

Supervisor starts with some period and checks a count of workers and start they if needed. 

Also it has a external supervisor module. It give second alternative option for supervising.

It looks like some local crone task for such applications.

Sylogsc is listening with REUSEADDR socket option and share one 514 UDP port between set of workers. 
 
So, this syslog service is set of separated processes.
Each syslog process is listening in his own 
address space and writing to common log file /var/log/syslogs.log.

The format of date and time in message of client syslogc is not RFC compatible, 
it is very simple for writing.
It can be changed for needs of compatibility in the future.
 
# COMPILE

make

# USING

1) Start single daemon:

sudo make start

or

./syslogs > /var/log/syslogs.log

2) test client

./syslogc "Some Test Record"

Or

./syslogc "Some First Record" "Second record"

3) Start multyple instances of syslog:

./run.sh

4) Run it under foring supervisor:

./supervisor

5) Run syslog with internal supervising:

./syslogs3 &

This script ensures what working 8 syslogs and start if needed.

syslogs is more trivial version of service without internal supervision,
it can be managed by strating ./supervisor 

6) Upgrading syslogs on the fly:
   *) Do backup
   *) Start new workers by syslogs3 or syslogs
   *) Test it 
   *) Stop old versions

# FAQ:

Q1) Whatis the difference between syslogs and syslogs2
A1) The syslogs2 is more complicated.
    It :
    *) Reads and parse preambule of message, 
       open log's file , related with it;
    *) Remove preambule, write message to this file

    So all syslog messages will be distributed inside a set of syslogs files.
    Each kind of preambule "<N>" will be inside it's own file.
  
    While more simple syslogs writes incoming messages as is, without any
    transformation.

Q2) Can it be faster ?
A2) Shure. The syslogs shuld be faster than syslogs2, it do not any transforms or parsing 
    for message.
    The syslogs2 is reopen log file for each incoming message, and parse it for
    computation of log file name, but syslogs2 can works in parallel mode and
    it should compensate long log writing.

Q3) Where it can be used ?
A3) For education, for systems where needs robast and fault tolerance events
    logging and handling. 
    
Q4) Can I use this syslog in the userspace ?
A4) Yes you can, but for it needs to change port number, for example to
    50514 and the file path in the defintion #define LOGMASK to "/home/user/var/log/%d-syslogs.log"
    inside file syslogs3.c
     