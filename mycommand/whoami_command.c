#include <string.h>
#include <unistd.h>

extern char sendbuf[1024];

void whoami_command(void){
  getlogin_r(sendbuf,1024);
  strcat(sendbuf,"\n");
}
