# syslogsc

# DESCRIPTION

It starts as a holiday project, the main principles to be:
1. Simple (The size of source code of syslogs3.c is 2K byte and two tiny
   shell scripts run3.sh and wc.sh );

2. Robast; 
3. Fault tolerance and automatic restarts it case of core dump's (If it
   fails it'll be restarted by timer);

4. Upgrade versions of service on fly, different version of syslogs may
   share UDP port, so just start new version of this syslog and then do
   tests and stops old;

5. Soft adaptation and zero configuration for different types of incoming syslog messages
   It just parse syslog header "<N>" for N and write messages to related file
   /var/log/N-syslogs.log.

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

./syslogc 514 "Some Test Record"

Or

./syslogc 514 "Some First Record" "Second record"

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
   o) Do backup
   o) Start new workers by syslogs3 or syslogs
   o) Test it 
   o) Stop old versions.

7) Start syslogs-safe pool in user-space
   
   ./syslogs-safe &

# LIMITS AND DEFAULTS

  The size of incoming message is constant with 64K value;

  The number of parallel workers (pool size) is 8;

  The default port is 514 for simple version and 20514 for syslogs-safe 
  (it should start with non root permission). 

  The limit of message for handle in alone syslogs-safe is 1000.

  The supervisor watchdog period is 10 seconds in a single process,
  for the pool of N prosesses it will be about 10/N in the whole. 

# FAQ:

# Q1) Whatis the difference between syslogs and syslogs2
# A1) The syslogs2 is more complicated.
    It :
    - Reads and parse preambule of message, 
       open log's file , related with it;
    - Remove preambule, write message to this file.

    So, all syslog messages will be distributed inside a set of syslogs files.
    Each kind of preambule "<N>" will be inside it's own file.
  
    While more simple syslogs writes incoming messages as is, without any
    transformation.

# Q2) Can it be faster ?
# A2) Shure. 
    The syslogs shuld be faster than syslogs2, it do not any transforms or parsing 
    for message.
    The syslogs2 is reopen log file for each incoming message, and parse it for
    computation of log file name, but syslogs2 can works in parallel mode and
    it should compensate long log writing.

# Q3) Where it can be used ?
# A3) For education, for systems where needs robast and fault tolerance events logging and handling. 
    
# Q4) Can I use this syslog in the userspace ?
# A4) Yes you can, 
    but for it needs to change port number, for example to
    50514 and the file path in the defintion #define LOGMASK to "/home/user/var/log/%d-syslogs.log"
    inside file syslogs3.c

# Q5) That is syslogs-safe ?
# A5) It is user-space syslog daemon with supervisor and some restrictions 
for number of incoming messages, after reach it, daemon starts another process and stops.
It aslo have internal supervisor that keeps given number of listening
processes.
All workers is separate processes in user-space with non root permissions. 

# Q6) Is it safe ?
# A6) No, like many UDP services working at low ports,
 and like many C code with calls sprintf(...), but such challenges may me redused by using syslogs-safe version of this syslog daemon. 
  It handle ony limited count of incoming message then this worker stops.
  Others workers starts needed numbers of it copies in they supervisors.
  The number of limit for handled process is an issue of needed syslog's productivity and
  safety. Using Stack-Smashing Protector (old name is ProPolice) may reduce
  some risks. 
  SSP is a subsystem for checking stack safety.
  From gcc-4.1-stage2, SSP was intergated to GCC.
  Gentoo Linux and OpenBSD include SSP in the default GCC package.

# All comments are welcomed.
       