# syslogsc
The simple syslog server with supervisor inside. This server run as N workers, each is separated process with supervisor inside. Supervisor checks a count of workers and start they if needed. Also supervisor can run as separated external process.
