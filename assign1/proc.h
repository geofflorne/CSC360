typedef struct proc{
    pid_t pid;
    char *cmd;
    int isStop;
    struct proc *next;
} proc;

proc *head = NULL;
proc *curr = NULL;
int count = 0;

proc* add(pid_t pid, char *cmd, int isStop){
    proc *ptr = (proc*)malloc(sizeof(proc));
    if(ptr == NULL){
        printf("malloc failed\n");
        exit(EXIT_FAILURE);
    }

    ptr->pid = pid;
    ptr->cmd = strdup(cmd);
    ptr->isStop = isStop;
    ptr->next = NULL;

    if(head == NULL){
      head = curr = ptr;
    }else{
      curr->next = ptr;
      curr = ptr;
    }
    count++;
    return ptr;
}


proc* find(pid_t pid, proc ** prev){
    proc *ptr = head;
    proc *tmp = NULL;
    int found = 0;
    while(ptr != NULL){
      if(ptr->pid == pid){
          found = 1;
          break;
      }else{
        tmp = ptr;
        ptr = ptr->next;
      }
    }
    if(found){
      if(prev){
        *prev = tmp;
      }
      return ptr;
    }else{
      return NULL;
    }
}

int delete(pid_t pid){
  proc *prev = NULL;
  proc *del = NULL;

  del = find(pid, &prev);

  if(del == NULL){
        return -1;
    }else{
        if(prev != NULL)
            prev->next = del->next;

        if(del == curr){
            curr = prev;
        }
        if(del == head){
            head = del->next;
        }
    }

    free(del);
    del = NULL;
    count--;
    return 0;
}

void set_isStop(pid_t pid, int isStop){
  proc *ptr = head;
  while(ptr!=NULL){
    if(ptr->pid == pid){
      ptr->isStop = isStop;
      break;
    }else{
      ptr = ptr->next;
    }
  }
}

void print_list(){
    proc *ptr = head;
    printf("\n");
    while(ptr != NULL)
    {
        printf("%d:    %s\n",ptr->pid,ptr->cmd);
        ptr = ptr->next;
    }
    printf("Total backgound jobs: %d\n",count);
    return;
}
