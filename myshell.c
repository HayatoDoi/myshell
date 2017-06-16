#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>

void pwd_command(void);
void help_command(void);
void whoami_command(void);
void cd_command(void);
void error(void);
void addprompt(void);
void rmdir_command(void);
void ls_command(void);
void mkdir_command(void);
void rm_command(void);
void cp_command(void);
void mv_command(void);
void cat_command(void);
void touch_command(void);
void chmod_command(void);

void clear_buffer(void);

char recvbuf[1024];
char sendbuf[1024];
char workbuf[1024];
char *tp;
int i;
int readnum;

int main(){

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
    }else if(strcmp(tp,"pwd") == 0){
      pwd_command();
    }else if(strcmp(tp,"exit") == 0){
      _exit(0);
    }else if(strcmp(tp,"help") == 0){
      help_command();
    }else if(strcmp(tp,"whoami") == 0){
      whoami_command();
    }else if(strcmp(tp,"cd") == 0){
      cd_command();
    }else if(strcmp(tp,"rmdir") == 0){
      rmdir_command();
    }else if(strcmp(tp,"ls") == 0){
      ls_command();
    }else if(strcmp(tp,"mkdir") == 0){
      mkdir_command();
    }else if(strcmp(tp,"rm") == 0){
      rm_command();
    }else if(strcmp(tp,"cp") == 0){
      cp_command();
    }else if(strcmp(tp,"mv") == 0){
      mv_command();
    }else if(strcmp(tp,"cat") == 0){
      cat_command();
    }else if(strcmp(tp,"touch") == 0){
      touch_command();
    }else if(strcmp(tp,"chmod") == 0){
      chmod_command();
    }else{
      sprintf(sendbuf,"%s\n","unknown command");
    }
    write(1, sendbuf, 1024);
  }

  return(0);
}

void clear_buffer(void){
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

void pwd_command(void){
  getcwd(workbuf,1024);
  strcpy(sendbuf,workbuf);
  strcat(sendbuf,"\n");
}

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

void whoami_command(void){
  getlogin_r(sendbuf,1024);
  strcat(sendbuf,"\n");
}

void cd_command(void){
  tp = strtok(NULL," ");
  chdir(tp);
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
