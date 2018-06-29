#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
     
#define BUFLEN  65536
#define NPACK   10
#define SRV_IP "127.0.0.1" 
#define PORT    514

void err(char *s) { perror(s); exit(1); }
     
int main(int argc, const char * argv[])
{
    struct sockaddr_in si_other;
    int s, i, slen=sizeof(si_other),fs=165;
    char buf[BUFLEN];
    time_t sent_time;
    char date[BUFLEN];
    
    time(&sent_time);
    
    sprintf(date,"%s",ctime(&sent_time));    
    date[strlen(date)-1]=0x0;
    printf("time: %s\n",date);
     
    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) err("socket");
    
    memset((char *) &si_other, 0, sizeof(si_other));
    
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);
    
    if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
          fprintf(stderr, "inet_aton() failed\n");
          exit(1);
        }
    
for (i=1; i<argc; i++) {
          printf("Sending packet %d\n", i);
          sprintf(buf, "<%d>: %s UTC: syslogc %s\n", fs, date, argv[i]);
          if (sendto(s, buf, BUFLEN, 0, (void *) &si_other, slen)==-1) err("sendto()");
        }
    
 close(s);
 return 0;
}