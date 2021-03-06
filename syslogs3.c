#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
     
#define BUFLEN  65536
#define PORT    514
#define SECONDS 10
#define LOGMASK "/var/log/%d-syslogs.log" 

void err(char *s)
{
 perror(s);
 exit(1);
}

void Check(int a)
{time_t start;

// time(&start);
// fprintf(stderr,"Check %s",ctime(&start));

 fprintf(stderr,"Check \n");
 fflush (stderr);

 signal(SIGALRM,(void *)Check);
 alarm(SECONDS);

 system("./run3.sh &");

}


route(int FS, const char * buf)
{FILE * F;
 char name[255];

 sprintf(name,LOGMASK,FS);

 F=fopen(name,"a");

 if (F==0) fprintf(stderr," error \n");
  
 fprintf(F,"%s",buf);

 fclose(F);
}

void log(const char * buf)
{ char *b;
  int FS;
  
  b=(void *)malloc(65535);
  
  if (sscanf(buf,"<%d>%s",&FS,b)==2) 
   {route(FS,index(buf,':')+1);}
   else 
   {
    fprintf(stderr,"not routed msg '%s'\n",buf);
   }
 
 free(b);  
}
    
int main(void)
{
    struct sockaddr_in si_me, si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];

    fclose(stdin);

    signal(SIGALRM,(void *)Check);
    alarm(SECONDS);

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) err("socket");

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) err("setsockopt(SO_REUSEADDR) failed");
    
    if (bind(s, (void *)&si_me, sizeof(si_me))==-1) err("bind");


    for (;;) {
      if (recvfrom(s, buf, BUFLEN, 0, (void *)&si_other, &slen)==-1) err("recvfrom()");
      fprintf(stderr,"%d %d Received packet from %s:%d\n",getppid(),getpid(),inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
      log(buf);
      
    }

    close(s);
    return 0;
}
    