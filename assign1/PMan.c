#include <unistd.h>             // fork(), execvp()
#include <stdio.h>              // printf(), scanf(), setbuf(), perror()
#include <stdlib.h>             // malloc()
#include <sys/types.h>          // pid_t
#include <sys/wait.h>           // waitpid()
#include <signal.h>             // kill(), SIGTERM, SIGKILL, SIGSTOP, SIGCONT
#include <errno.h>              // errno
#include <string.h>             //strtok()
#include <readline/readline.h>  //readline()
#include <readline/history.h>
#include "proc.h"

#define ARGLIMIT 100

void update_bg_process() {
    usleep(20);
    int status;
    int id;
    int opts = WNOHANG | WUNTRACED | WCONTINUED;

    //use while loop to monitor all process with -1 option.
    while(1) {

      id = waitpid(-1, &status, opts);
      if (id == 0){
          break;
      }else if (id > 0){
        if(WIFEXITED(status)){
          printf("%d terminated\n", id);
          delete(id);
        }else if(WIFSTOPPED(status)){
          set_isStop(id,1);
        }else if(WIFCONTINUED(status)){
          set_isStop(id,0);
        }
      }
      if (id < 0){
        break;
      }
    }

    return;
}

int main(){

    proc *ptr = NULL;
    char *input = NULL;
    char *prompt = "PMan: > ";
    char *tok;
    int   pid;
    char *args[ARGLIMIT];
    int   i;

    while(1) {
        update_bg_process();
        input = readline(prompt);
        tok = strtok(input, " ");
        if (strcmp(tok, "pstat") == 0){
            //     FILE *fp = popen("/proc/[pid]/stat(status)", "r") ;
            //     read file content using fgets. (see pipe.c)
            //     output variables listed in assign.
        }else if (strcmp(tok, "bg") == 0){
            tok = strtok(NULL, " ");
            if(tok == NULL){
              printf("usage: bg <command>\n");
              continue;
            }
            i = 0;
            while(tok != NULL){
              args[i++] = tok;
              tok = strtok(NULL, " ");
            }
            args[i++] = NULL;

            pid = fork();
            if (pid > 0){
              add(pid, args[0], 0);
            }else if (pid == 0){
              execvp(args[0], args);

            }else{
                printf("fork failed\n");
            }


        }else if (strcmp(tok, "bgkill") == 0){
            //     handle signal using kill() ;
            tok = strtok(NULL, " ");
            if(tok == NULL){
              printf("error: no pid\n");
            }else{
              kill(atoi(tok), SIGQUIT);
              printf("%d terminated\n", atoi(tok));
              delete(atoi(tok));
            }

        }else if (strcmp(tok, "bgstart") == 0){
            //     handle signal using kill() ;
            tok = strtok(NULL, " ");
            if(tok == NULL){
              printf("error: no pid\n");
            }else{
              kill(atoi(tok), SIGCONT);
              set_isStop(atoi(tok),0);
            }

        }else if (strcmp(tok, "bgstop") == 0){
            //     handle signal using kill() ;
            tok = strtok(NULL, " ");
            if(tok == NULL){
              printf("error: no pid\n");
            }else{
              kill(atoi(tok), SIGSTOP);
              set_isStop(atoi(tok),1);
            }

        }else if (strcmp(tok, "bglist") == 0){
            print_list();
        }else {
          printf("%s: command not found\n", input);
        }


    }
    return 1;
}
