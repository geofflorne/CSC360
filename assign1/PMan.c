#include <unistd.h> // fork(), execvp()
#include <stdio.h> // printf(), scanf(), setbuf(), perror()
#include <stdlib.h> // malloc()
#include <sys/types.h> // pid_t
#include <sys/wait.h> // waitpid()
#include <signal.h> // kill(), SIGTERM, SIGKILL, SIGSTOP, SIGCONT
#include <errno.h> // errno
#include <string.h> //strtok()
#include <readline/readline.h> //readline()
#include <readline/history.h>
#include "proc.h"

#define ARGLIMIT 100
#define BUFFERSIZE 1024

void update_bg_process() {
  //sleep to allow processes to terminate before checking.
  usleep(20);
  int status;
  int id;
  int opts = WNOHANG | WUNTRACED | WCONTINUED;

  //use while loop to monitor all process with -1 option.
  while (1) {
    id = waitpid(-1, & status, opts);
    if (id == 0) {
      break;
    } else if (id > 0) {
      if (WIFEXITED(status)) {
        printf("%d terminated\n", id);
        delete(id);
      } else if (WIFSTOPPED(status)) {
        set_isStop(id, 1);
      } else if (WIFCONTINUED(status)) {
        set_isStop(id, 0);
      }
    }

    if (id < 0) {
      break;
    }

  }
  return;
}

void printStats(pid_t pid) {
  FILE * fp;
  char dir[BUFFERSIZE];
  int i = 0;
  char stat[BUFFERSIZE];
  char * stat_array[BUFFERSIZE];
  char status_array[64][BUFFERSIZE];
  int voluntary, nonvoluntary;
  char * tok;

  sprintf(dir, "/proc/%d/status", pid);
  //check if file exsits
  if (access(dir, F_OK) == -1) {
    printf("Error: Process %d does not exsit\n", pid);
    return;
  }
  fp = fopen(dir, "r");

  //use fgets to put each line of /proc/pid/status into array elemets.
  i = 0;
  while (fgets(status_array[i], BUFFERSIZE, fp) != NULL) {
    i++;
  }

  //the infromation we need is in the last two elements
  sscanf(status_array[i - 2], "voluntary_ctxt_switches: %d", & voluntary);
  sscanf(status_array[i - 1], "nonvoluntary_ctxt_switches: %d", & nonvoluntary);

  fclose(fp);

  sprintf(dir, "/proc/%d/stat", pid);
  fp = fopen(dir, "r");

  fgets(stat, sizeof(stat), fp);

  tok = strtok(stat, " ");

  //split stat into array on spaces
  i = 0;
  while (tok != NULL) {
    stat_array[i++] = tok;
    tok = strtok(NULL, " ");
  }

  fclose(fp);
  //print information
  printf("pid: %s\n", stat_array[0]);
  printf("comm: %s\n", stat_array[1]);
  printf("state: %s\n", stat_array[2]);
  printf("utime: %f\n", atof(stat_array[13]) / sysconf(_SC_CLK_TCK));
  printf("stime: %f\n", atof(stat_array[14]) / sysconf(_SC_CLK_TCK));
  printf("rss: %s\n", stat_array[23]);
  printf("voluntary_ctxt_switches: %d\n", voluntary);
  printf("nonvoluntary_ctxt_switches: %d\n", nonvoluntary);

}

int main() {

  proc * ptr = NULL;
  char * input = NULL;
  char * prompt = "PMan: > ";
  char * args[ARGLIMIT];
  char * tok;
  char * dir;
  char * buff;
  int pid;
  int i;

  while (1) {
    update_bg_process();
    input = readline(prompt);
    //go to begining of loop if no input
    if (strcmp(input, "") == 0){
      continue;
    }
    tok = strtok(input, " ");
    //read user imput and take appropriate action
    if (strcmp(tok, "pstat") == 0) {

      tok = strtok(NULL, " ");
      if (tok == NULL) {
        printf("usage: pstat <pid>\n");
        continue;
      }
      printStats(atoi(tok));

    } else if (strcmp(tok, "bg") == 0) {
      tok = strtok(NULL, " ");
      if (tok == NULL) {
        printf("usage: bg <command>\n");
        continue;
      }
      //tokanize args into array
      i = 0;
      while (tok != NULL) {
        args[i++] = tok;
        tok = strtok(NULL, " ");
      }
      args[i++] = NULL;

      pid = fork();
      if (pid > 0) {
        //add child process to list in parent
        add(pid, args[0], 0);
      } else if (pid == 0) {
        //start child process
        execvp(args[0], args);
        exit(1);
      } else {
        printf("fork failed\n");
      }

    } else if (strcmp(tok, "bgkill") == 0) {
      //     handle signal using kill() ;
      tok = strtok(NULL, " ");
      if (tok == NULL) {
        printf("error: no pid\n");
      } else {
        kill(atoi(tok), SIGQUIT);
        printf("%d terminated\n", atoi(tok));
        delete(atoi(tok));
      }

    } else if (strcmp(tok, "bgstart") == 0) {
      //     handle signal using kill() ;
      tok = strtok(NULL, " ");
      if (tok == NULL) {
        printf("error: no pid\n");
      } else {
        kill(atoi(tok), SIGCONT);
        set_isStop(atoi(tok), 0);
      }

    } else if (strcmp(tok, "bgstop") == 0) {
      //     handle signal using kill() ;
      tok = strtok(NULL, " ");
      if (tok == NULL) {
        printf("error: no pid\n");
      } else {
        kill(atoi(tok), SIGSTOP);
        set_isStop(atoi(tok), 1);
      }

    } else if (strcmp(tok, "bglist") == 0) {
      print_list();
    } else {
      printf("%s: command not found\n", input);
    }

    update_bg_process();
  }
  return 1;
}
