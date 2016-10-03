#include <unistd.h>     // fork(), execvp()
#include <stdio.h>      // printf(), scanf(), setbuf(), perror()
#include <stdlib.h>     // malloc()
#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid()
#include <signal.h>     // kill(), SIGTERM, SIGKILL, SIGSTOP, SIGCONT
#include <errno.h>      // errno
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAXPROC 1024

typedef struct {
    pid_t pid;
    char *cmd;
} proc;


int main(){

    proc *proc_list[MAXPROC] ;
    char *input = NULL ;
    char *prompt = "PMan: > ";

    while(1) {

        input = readline(prompt);

        // update_bg_process();

        if ( strcmp("bgkill", input) == 0){

        } else if ( strcmp("bgstart", input) == 0){
          printf("in bgstart\n");
        } else if ( strcmp("bgstop", input) == 0){
          printf("in bgstop\n");
        } else if ( strcmp("bg cmd", input) == 0){
          printf("in bg cmd\n");
        } else if ( strcmp("bglist", input) == 0){
          printf("in bglist\n");
        } else if ( strcmp("pstat", input) == 0){
          printf("in pstat\n");
        } else {
          printf("%s: command not found\n", input);
        }

        // if ( input = bgkill / bgstart / bgstop )
        //     handle signal using kill() ;
        //
        // else if (input  = bg cmd) {
        //
        //     1. pid = fork();
        //     2. in child proces :  exec*(cmd)
        //     3. create a new item in proc_list using malloc().
        //     4. store pid, cmd, istStop = 0 in new item.
        //     5. add new item in proc_list.
        //
        // } else if (input = pstat pid) {
        //
        //     FILE *fp = popen("/proc/[pid]/stat(status)", "r") ;
        //     read file content using fgets. (see pipe.c)
        //     output variables listed in assign.
        //
        // } else if( input = bglist ) {
        //
        //     list all items in proc_list where isStop = 0 ;
        //
        // } else {
        //     handle command exception.
        // }

    }
    return 1;
}
