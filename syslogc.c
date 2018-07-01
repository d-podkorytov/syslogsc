#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
     
#define BUFLEN  4096
#define NPACK   10
#define SRV_IP "127.0.0.1" 
#define PORT    514

void err(char *s) { perror(s); exit(1); }

int parse_addr_port(const char * from,char * to_addr, short * port)
{char ch;
 short i;
 char * p;
  
  p=malloc(255);
//  printf("rindex: %s\n",rindex(from,':')+1);
  //printf("port: %s\n",strrchr(from,':')+1);
  //sprintf(to_addr,"%s",strtok(from,":"));
  strcpy(to_addr,strtok(from,":"));

  //printf("port: %s\n",from-strlen(to_addr));

   //sscanf((char *)(strrchr(from,':')+1),"%d",port);
  strcpy(p,from+strlen(to_addr)+1);
  *port=atoi(p);
 free(p);  
 return 2;
 //return sscanf(from,"%s%c%d",to_addr,&ch,port);
};
     
int main(int argc, const char * argv[])
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other),fs=165;
    char buf[BUFLEN];
    time_t sent_time;
    char date[BUFLEN];
    char * to_addr;
    int port=514;
    
    time(&sent_time);
    
    to_addr=malloc(255);
    
    sprintf(date,"%s",ctime(&sent_time));    
    date[strlen(date)-1]=0x0;
    printf("time: %s\n",date);
    
    //if ( sscanf(argv[1],"%d",&port)!=1 ) {port=PORT;}
     if ( parse_addr_port(argv[1], to_addr, &port ) !=2)
      {
        printf("dest:  %s:%d\n",to_addr,port);
 
        sprintf(to_addr,"%s",SRV_IP);
        port=PORT;
      }

    printf("time: %s\n",date);
    printf("destination:  %s:%d\n",to_addr,port);
     
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) err("socket");
    
    memset((char *) &si_other, 0, sizeof(si_other));
    
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);
    
    if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
          fprintf(stderr, "inet_aton() failed\n");
          exit(1);
        }
    
for (i=2; i<argc; i++) {
          printf("Sending packet %d\n", i);
          sprintf(buf, "<%d>: %s UTC: syslogc %s\n", fs, date, argv[i]);
          if (sendto(s, buf, BUFLEN, 0, (void *) &si_other, slen)==-1) err("sendto()");
        }
    
 close(s);
 return 0;
}