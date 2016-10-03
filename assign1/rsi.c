#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


int main(){

    char *input = NULL ;
    char *prompt = "your command:";
while(1){
    input = readline(prompt);

    printf("%s\n", input);
  }
    return 1;
}
