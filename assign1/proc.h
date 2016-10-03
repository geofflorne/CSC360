typedef struct proc{
    pid_t pid;
    char *cmd;
    int isStop;
    struct proc *next;
} proc;

proc *head = NULL;
proc *curr = NULL;

proc* add(pid_t pid, char *cmd, int isStop){
    proc *ptr = (proc*)malloc(sizeof(proc));
    if(ptr == NULL){
        printf("malloc failed\n");
        exit(EXIT_FAILURE);
    }

    ptr->pid = pid;
    ptr->cmd = cmd;
    ptr->isStop = isStop;
    ptr->next = NULL;

    if(head == NULL){
      head = curr = ptr;
    }else{
      curr->next = ptr;
      curr = ptr;
    }
    return ptr;
}

void print(){
    proc *ptr = head;

    printf("\n -------Printing list Start------- \n");
    while(ptr != NULL)
    {
        printf("\n %d,%s,%d \n",ptr->pid,ptr->cmd,ptr->isStop);
        ptr = ptr->next;
    }
    printf("\n -------Printing list End------- \n");
    return;
}
