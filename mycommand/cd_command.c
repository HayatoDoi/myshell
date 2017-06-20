#include <string.h>
#include <unistd.h>

extern char *tp;

void cd_command(void){
  tp = strtok(NULL," ");
  chdir(tp);
}
