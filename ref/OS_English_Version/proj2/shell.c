#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#define MAX_LINE 80
#define READ_END  0
#define WRITE_END 1


int main(void){
	int has_hist=0;
	int hist = 0;
	char arg[10][MAX_LINE / 2 + 1];
	char *args[MAX_LINE / 2 + 1];
	char last_args[10][MAX_LINE / 2 + 1];
	int should_run = 1;
        int num_arg = 0; 
        int n = 0;
	int last_n = 0;
        char c;
	int wait_=1;
        
        int use_pipe=0;
        char *pipes[MAX_LINE / 2 + 1];
        int pipe_len=0;
        int pipe_fd[2];
       


      while(should_run){


        printf("osh>"); 
        fflush(stdout);
        if(wait_ == 0) wait(NULL);	      
        
        pid_t pid;
        wait_ = 1;
        n = 0;
	use_pipe = 0;
	pipe_len = 0;

       for(int i = 0;i < 10;++i) args[i] = NULL;
	
	while(scanf("%s", arg[n])){
		
                args[n] = arg[n];
	        n ++;
		//c = getchar();
                scanf("%c", &c);

		if(c == '\n'){
		   break;
		}

        }
      //printf("n:%d\n", n);
      if(strcmp(arg[0], "exit") == 0){
	      should_run = 0;
	      continue;
      }

      if(strcmp(arg[0], "!!") == 0){
           hist = 1;
	   if(!has_hist){
               printf("There is no history command!\n");
	       n = 0;
               hist = 0;
	       continue;
	   }
	   else{
	     //  printf("last_n:%d\n",last_n);
	       for(int i = 0;i < last_n;++i){
	           args[i] = last_args[i];
	           printf("%s ", args[i]);
	       } 
               printf("\n");
               n = last_n; 
	   }
      }else{
         // printf("copy to last:%d\n", n);
          for(int i = 0;i < n;++i){
                 strcpy(last_args[i], args[i]); //without the last "NULL"
          }
              last_n = n;
      }   
            
          
         // printf("lastn:%d, n:%d\n", last_n, n);
          
	  
                     
	  hist = 0;	  
	  has_hist = 1;
// exe part:
// check < and > and |
          // check &
	  if(strcmp(args[n- 1], "&") ==  0){
                 n -- ;
                 wait_ = 0;
                 args[n] = NULL;
          }
              
          // check pipe
	  for(int i = 0;i < n;++i){
	      if(strcmp(args[i], "|") == 0){
	          use_pipe = 1;
		  args[i] = NULL;
		  for(int k = i + 1;k < n;++k){
		      pipes[pipe_len ++] = args[k];
		  }
                  n -= pipe_len;
                 // printf("pipe output : %s, pipe len:%d\n", pipes[0], pipe_len);
	          break;
	      }
	  }

          pid = fork();
    
          if(pid < 0){
	      perror("Error:");
	      return -1;
	  }	  
	  else if(pid == 0){
              
                                        
              if(n >= 3 && strcmp(args[n - 2], ">") == 0){
	          int f;
	          f = open(args[n - 1],  O_CREAT|O_RDWR|O_TRUNC, S_IRUSR|S_IWUSR);
	          dup2(f, STDOUT_FILENO);
	          args[n - 1] = NULL;
	          args[n - 2] = NULL;
	          n -= 2;	  
	      }
	      if(n >= 3 && strcmp(args[n - 2], "<") == 0){
	          int f2 = open(args[n - 1], O_RDONLY);
		  dup2(f2, STDIN_FILENO);
		  args[n - 2] = NULL;
                  args[n - 1] = NULL;
                  n -= 2;
	      }
              
        
              if(use_pipe){   
                 // printf("use pipe\n");    
                 if(pipe(pipe_fd) == -1){
                     fprintf(stderr, "Pipe created failed!------\n");
                     return 1;
                 }
                      
	         pid_t pipe_pid;
                 pipe_pid = fork();
                 if(pipe_pid < 0){
		      fprintf(stderr, "Fork failed(pipe)\n");
		      return 1;
		 }
                 else if(pipe_pid == 0){
		      close(pipe_fd[READ_END]);
		      dup2(pipe_fd[WRITE_END], STDOUT_FILENO);
		      execvp(args[0], args);
		      close(pipe_fd[WRITE_END]);
		      exit(0);
		 }else{
		     close(pipe_fd[WRITE_END]);
		     dup2(pipe_fd[READ_END], STDIN_FILENO);
		     pipes[pipe_len] = NULL;
		     execvp(pipes[0], pipes);
		     close(pipe_fd[READ_END]);
		     wait(NULL);
		 }		 
                  
              }
              else{
                //  printf("not use pipe\n");     
	          execvp(args[0], args);
	      }
              exit(0);
	  }
	  else{
             if(wait_)
	         wait(NULL);   
	  }
}

  


	return 0;
}

