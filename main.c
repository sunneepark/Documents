#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h> //file

#define MAX_LINE 80

char* cmd_stack[100];
int stack_index;

char* first_;
char* second;

int fd_wr;
int fd_rd;

void what_command(char* input) { 
	//1 is basic, 2 is pipe, 3 is add function
	if (strchr(input, '>') != NULL)
		re_out_command(input);
	else if (strchr(input, '<') != NULL)
		re_in_command(input);

	else if (strchr(input, '|') != NULL)
		pipe_command(input);
	else
		basic_command(input);
}
void command(char* input){
	strcpy(cmd_stack+(++stack_index),input);
	what_command(input);
	
}
void basic_command(char* input) {
	char* args[MAX_LINE/2+1];
	int index=0; //command amount
	
	args[index] = strtok(input, " "); //tokenizer to space	
	while (args[index]) {
		index++;
		args[index] = strtok(NULL, " ");
	}
	char first[1024];
	strcpy(first,args[0]);

	if (strcmp(first, "cd") == 0){
		if((chdir(args[1]))==-1)
			printf("No Directory\n");
	}

	//history function
	else if(strcmp(first,"history")==0)
		history_command(first,1);
	else if(strchr(first,'!')!=NULL)
		history_command(first,2);
	//execvp function
	else if (fork() == 0) {
		execvp(args[0], args);
		printf("command not found\n");
		exit(0);
	}
}
void history_command(char* str, int type){
	if(type==1){//print up to the latest 10
		int check=0;
		int i=stack_index;
		while(check<=10){
			if(i<=0) break;
			printf("%d %s\n",i, cmd_stack+i);
			i--; check++;
		}
	}
	else if(type==2){
		if(strncmp(str,"!!",2)==0){
			if((--stack_index)>0){
				char ago[1024];
				strcpy(ago,cmd_stack+stack_index);
				printf("%s\n",ago);
				command(ago);
			}
			else
				printf("no command in history\n");
		}	
		else{
			char* ago_string; //nth command
			ago_string=strtok(str,"!");
			int ago_index=atoi(ago_string);
			int direc=0;
			if(stack_index >10) //when count command is over 10
				direc=(stack_index-10)+ago_index;
			else
				direc=ago_index;
			if(direc>0){
				stack_index--;
				strcpy(ago_string,cmd_stack+direc);
				printf("%s\n",ago_string);
				command(ago_string);
			}
			else //error handling
				printf("bash:%d, event not found\n",ago_index);
		}
	}
}
void pipe_command(char* str){
	int fd[2]; //handling pipe

	first_=strtok(str,"|");
	second=strtok(NULL,"|");

	if(pipe(fd)<0){
		perror("pipe error : ");
		exit(0);
	}

	if(fork()==0){
		close(1); //close child_process input
		if(dup(fd[1])!=0);
		close(fd[0]);
		close(fd[1]);
		command(first_);
		wait(NULL);//wait child process
		exit(0);
	}
	
	if(fork()==0){
		close(0); //close child_process input
		if(dup(fd[0])!=0);
		close(fd[0]);
		close(fd[1]);
		command(second);
		wait(NULL); //wait child process
		exit(0);
	}
	if (close(fd[0]) == -1 || close(fd[1]) == -1) perror("error");
	while(wait(NULL)!=-1);
}
void add_command(char* str){
	if(strchr(str, '>') != NULL)
		re_out_command(str);

	else if(strchr(str, '<') != NULL)
		re_in_command(str);

}
void re_out_command(char* str){
	char* j;
	int append_flag=0;
	if((j=strstr(str,">>"))!=NULL)
		append_flag=1;
	
	first_=strtok(str,">");
	second=strtok(NULL,">");
	
	char* arg[MAX_LINE/2+1];
	int index=0; //command amount
	arg[index] = strtok(first_, " "); //tokenizer to space	
	while (arg[index]) {
		index++;
		arg[index] = strtok(NULL, " ");
	}
	second=strtok(second," ");

	if(fork()==0){
		if(append_flag==0)
			fd_wr=open(second,O_WRONLY|O_CREAT|O_TRUNC,0644);
		else if(append_flag==1)
			fd_wr=open(second,O_WRONLY|O_CREAT|O_APPEND,0644);
		dup2(fd_wr,STDOUT_FILENO);//file descripter

		close(fd_wr);
		
		execvp(arg[0],arg);
		exit(0);
	}
	wait(NULL);
}
void re_in_command(char* str){
	first_=strtok(str,"<");
	second=strtok(NULL,"<");
	
	char* arg[MAX_LINE/2+1];
	int index=0; //command amount
	arg[index] = strtok(first_, " "); //tokenizer to space	
	while (arg[index]) {
		index++;
		arg[index] = strtok(NULL, " ");
	}
	second=strtok(second," ");
	if(fork()==0){
		fd_wr=open(second,O_RDONLY|O_CREAT,0644);
		dup2(fd_wr,STDIN_FILENO); //file descripter
		close(fd_wr);
		
		execvp(arg[0],arg);
		exit(0);
	}
	wait(NULL);
}


int main(void) {
	struct sigaction act;
	act.sa_handler=SIG_IGN;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGINT,&act,NULL);

	int should_run = 1; //flag to determine when to exit program
	while (should_run) {
		char buf[255];
		getcwd(buf,255); //pwd
		printf("~%s$",buf);
		fflush(stdout);
		
		char input[2048];		
		fgets(input,sizeof(input),stdin);
		input[strlen(input)-1]='\0';
		fflush(stdin);
	
		if (strcmp(input,"exit")==0) { //when it is exit, it terminated
			should_run = 0;
			exit(0);
		}
		if(strcmp(input,NULL)==0)
			exit(0);
		command(input);
		wait(NULL);
	}
	return 0;
}
