#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>

#define SECONDS 10
     
void Check(int a)
{time_t start;

 time(&start);
 
 printf("Check %s",ctime(&start));

 system("./run.sh");

 signal(SIGALRM,(void *)Check);
 alarm(SECONDS);

}
     
int main(int argc, const char * argv[])
{
  signal(SIGALRM,(void *)Check);
  alarm(SECONDS);
  
 for(;;) {sleep(1000);}
 return 0;
}