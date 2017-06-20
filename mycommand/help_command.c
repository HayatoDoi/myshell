#include <string.h>

extern char sendbuf[1024];

void help_command(void){
  strcat(sendbuf,"Command List\n");
  strcat(sendbuf,"pwd : Display Process Working Directory\n");
  strcat(sendbuf,"help : Diplay Help\n");
  strcat(sendbuf,"whoami : Display Login Name\n");
  strcat(sendbuf,"cd : Change Process Working Directory\n");
  strcat(sendbuf,"rmdir : Delete Empty Directory\n");
  strcat(sendbuf,"ls : List Directory\n");
  strcat(sendbuf,"mkdir : Create Empty Directory\n");
  strcat(sendbuf,"rm : Delete File\n");
  strcat(sendbuf,"cp : Copy File\n");
  strcat(sendbuf,"mv : Move or Rename File\n");
}