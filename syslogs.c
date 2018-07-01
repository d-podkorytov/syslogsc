#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
     
#define BUFLEN  65536
#define PORT    514

void err(char *s)
{
        perror(s);
        exit(1);
}
    
int main(void)
{
    struct sockaddr_in si_me, si_other;
    int s, i, slen=sizeof(si_other);
    char buf[BUFLEN];

    fclose(stdin);

    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1) err("socket");

    memset((char *) &si_me, 0, sizeof(si_me));
    si_me.sin_family = AF_INET;
    si_me.sin_port = htons(PORT);
    si_me.sin_addr.s_addr = htonl(INADDR_ANY);
    
    if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) err("setsockopt(SO_REUSEADDR) failed");
    
    if (bind(s, &si_me, sizeof(si_me))==-1) err("bind");

    for (;;) {
      if (recvfrom(s, buf, BUFLEN, 0, (void *)&si_other, &slen)==-1) err("recvfrom()");
      fprintf(stderr,"%d %d Received packet from %s:%d\n",getppid(),getpid(),inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
      printf("%s", buf);
      fflush(stdout);
    }

    close(s);
    return 0;
}
    