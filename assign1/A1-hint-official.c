// copyright @ Yongjun Xu
// do not distribute without author's permission

/******** Note *************/
This file is to list both stopped & running bg process.

1. No need to handle foreground process.
    e.g. user input "cat foo.txt", just output no commnad found.
2. Handle exceptions. e.g. stop a process doesnt exist, etc.
3. Pseudo code below, implement functions with proper paramters.
/******** Note *************/


/*
Object: struct proc
Use: Information about a single process
Attributes:
    pit_t pid - process id
    char *cmd - command and arguments  
    bool isStop - whether process is stopped.
*/
typedef struct {
    pid_t pid;
    char *cmd;
} proc;


void update_bg_procss() {

    //use while loop to monitor all process with -1 option.
    while(1) {
    id = waitpid(-1, status, WNOHANG) to monitor the state of all child process
    if id = 0 ; none of children changed state, no proc_list update needed. break while loop.
    if id > 0 ; 
         Inform user that process [id] terminated. remove the item where pid = id in proc_list.
    if id < 0 :
        no more updates, break while loop.
    }

    return ;
}

main(){

    proc *proc_list[MAXPROC] ;

    while( read user input ) {

        update_bg_process();

        if ( input = bgkill / bgstart / bgstop )
            handle signal using kill() ;

        else if (input  = bg cmd) {

            1. pid = fork();
            2. in child proces :  exec*(cmd)
            3. create a new item in proc_list using malloc().
            4. store pid, cmd, istStop = 0 in new item.
            5. add new item in proc_list.

        } else if (input = pstat pid) {

            FILE *fp = popen("/proc/[pid]/stat(status)", "r") ;
            read file content using fgets. (see pipe.c)
            output variables listed in assign.

        } else if( input = bglist ) {
            
            list all items in proc_list ;

        } else {
            handle command exception.
        }

    }

}