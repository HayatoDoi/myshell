#include <string.h>
#include <unistd.h>

extern char sendbuf[1024];
extern char workbuf[1024];

void pwd_command(void){
  getcwd(workbuf,1024);
  strcpy(sendbuf,workbuf);
  strcat(sendbuf,"\n");
}
