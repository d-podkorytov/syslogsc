#include <syslog.h>
#include <stdio.h>
#include <stdarg.h>

#include <fcntl.h>

 myopenlog(const char *ident, int option, int facility){ openlog(ident,  option, facility);}

 mysyslog(int priority, const char *format, ... )       
  {//syslog(priority, format, Arg);
     va_list arg;
     int done;
     char s[1024*16];
     
     va_start (arg, format);
     done = vsprintf (s, format, arg);
     // udp_syslog(s);
     va_end (arg);
  
  return done;

  }

 mycloselog()                                          {closelog();}

// vsyslog(int priority, const char *format, va_list ap);

main()
{};