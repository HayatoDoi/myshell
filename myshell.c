#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "myshell.h"

#include "mycommand/pwd_command.h"
#include "mycommand/help_command.h"
#include "mycommand/whoami_command.h"
#include "mycommand/cd_command.h"

void rmdir_command(void);
void ls_command(void);
void mkdir_command(void);
void rm_command(void);
void cp_command(void);
void mv_command(void);
void cat_command(void);
void touch_command(void);
void chmod_command(void);

void error(void);
void addprompt(void);
void clear_buffer(void);

extern char recvbuf[1024];
extern char sendbuf[1024];
extern char workbuf[1024];
extern char *tp;

typedef struct _cname_func {
  char* name;
  void (*func)(void);
} cname_func;

cname_func cf[] = {
  {"pwd", pwd_command},
  {"help",help_command},
  {"whoami", whoami_command},
  {"cd", cd_command},
  {"rmdir", rmdir_command},
  {"ls", ls_command},
  {"mkdir", mkdir_command},
  {"rm", rm_command},
  {"cp", cp_command},
  {"mv", mv_command},
  {"cat", cat_command},
  {"touch", touch_command},
  {"chmod", chmod_command},
};

int main(){
  int i;
  int readnum;

  while(1){
    readnum = 0;
    lseek(0,0,SEEK_SET);
    clear_buffer();

    addprompt();
    write(1,sendbuf,1024);
    clear_buffer();

    readnum = read(0,recvbuf,1024);
    recvbuf[readnum-1] = '\0';

    tp = strtok(recvbuf," ");
    if(tp == NULL){
      error();
    }
    for(i = 0;i < sizeof(cf) / sizeof(cf[0]);i++ ){
      if(strcmp(tp,cf[i].name) == 0){
        cf[i].func();
        break;
      }
    }
    if(i == sizeof(cf) / sizeof(cf[0])){
      sprintf(sendbuf,"%s\n","unknown command");
    }
    write(1, sendbuf, 1024);
  }

  return(0);
}

void clear_buffer(void){
  int i = 0;
  for(i = 0; i < 1024; i++){
    sendbuf[i] = '\0';
    recvbuf[i] = '\0';
    workbuf[i] = '\0';
  }
}

void chmod_command(void){
  char *arg1 = strtok(NULL," ");
  char *arg2 = strtok(NULL," ");
  if(arg1 == NULL || arg2 == NULL){
    strcpy(sendbuf,"Please input argument:(\n");
    return;
  }
  int permission = strtol(arg2,NULL,8);
  chmod(arg1,permission);
}

void touch_command(void){
  char *arg1 = strtok(NULL," ");
  if(arg1 == NULL){
    strcpy(sendbuf,"Please input argument:(\n");
    return;
  }
  creat(arg1,0644);
}

void cat_command(void){
  char *arg1 = strtok(NULL," ");
  if(arg1 == NULL){
    strcpy(sendbuf,"Please input argument:(\n");
    return;
  }
  int fd = open(arg1,O_RDONLY);
  int readcount = read(fd,workbuf,1024);
  if(readcount == 0){
    strcpy(sendbuf,"");
    close(fd);
    return;
  }
  close(fd);
  strcpy(sendbuf,workbuf);
}

void cp_command(void){
  char *arg1, *arg2;
  arg1 = strtok(NULL," ");
  arg2 = strtok(NULL," ");
  int errorcode = link(arg1,arg2);
  if(errorcode == -1){
     strcat(sendbuf,strerror(errno));
  }
}

void mv_command(void){
  char *arg1, *arg2;
  arg1 = strtok(NULL," ");
  arg2 = strtok(NULL," ");
  int errorcode = rename(arg1,arg2);
  if(errorcode == -1){
    if(errno == EISDIR){
      strcpy(workbuf,arg2);
      strcat(workbuf,"/");
      strcat(workbuf,arg1);
      errorcode = rename(arg1,workbuf);
      if(errorcode == -1){
        strcat(sendbuf,strerror(errno));
        strcat(sendbuf,"\n");
      }
    }else{
      strcat(sendbuf,strerror(errno));
      strcat(sendbuf,"\n");
    }
  }
}

void rm_command(void){
  tp = strtok(NULL," ");
  int errorcode = unlink(tp);
  if(errorcode == -1){
    strcat(sendbuf,strerror(errno));
    strcat(sendbuf,"\n");
  }
}

void mkdir_command(void){
  tp = strtok(NULL," ");
  int errorcode = mkdir(tp,S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH | S_IXOTH);
  if(errorcode == -1){
    strcat(sendbuf,strerror(errno));
    strcat(sendbuf,"\n");
  }
}

void rmdir_command(void){
  tp = strtok(NULL," ");
  rmdir(tp);
}

void ls_command(void){
  char dirname[4096];
  DIR *dir;
  struct dirent *de;

  strcpy(sendbuf,"");
  tp = strtok(NULL," ");
  if(tp == NULL){
    strcpy(dirname,".");
  }else{
    strcpy(dirname,tp);
  }
  dir = opendir(dirname);
  de = readdir(dir);
  while(de != NULL){
    strcat(sendbuf,de->d_name);
    strcat(sendbuf,"\n");
    de = readdir(dir);
  }
  closedir(dir);
}

void addprompt(void){
  getlogin_r(workbuf,1024);
  strcpy(sendbuf,workbuf);
  strcat(sendbuf,"@");
  getcwd(workbuf,1024);
  strcat(sendbuf,workbuf);
  strcat(sendbuf,"$ ");
}

void error(void){
  strcpy(sendbuf,"Please Input Command\n");
}
