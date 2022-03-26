# CS356 Project 2

Name : HaotianXue 

Student ID : 518021910506 

Email : xavihart@sjtu.edu.cn

___

[toc]

## Introduction

This is the report for Project2 of course CS356, also for chapter 3 in COS book. This project is divided into two sections. 

In the first part, we designed a simple shell which can simulate some functions of linux shell, which is supported by utilization of process operations and pipe operations in linux. 

In the second part we designed a Linux kernel module that uses the /proc file system for displaying a task’s information based on its process identifier value, which is supported by what we have done in Project1.



## Simple Shell Implementation

 ### Key Functions of Simple Shell

  To simulate the Linux Shell, we should realize the following functions:

- Users can input basic command they want to execute, like `ls`, `ps` ,`cat ...`

- Users can input `!!` to reuse the last command, which will through out a warning is no history exists

- Users can use `&` to decide whether we should wait for the child command to be done

- Users can use `>` and `<` to redirect the file input and output 

- Users can use `|` to do pipe communication between two processes, e.g. `ls | less`, `ls -l | sort`

- Users can use `exit` to close the simple shell

  

### Implementation Details

 ####  Basic Command Execution

The program is run in a while loop, and we use `should_run` to decide if we can run, then we try to get the command from user input:

```c
while(scanf("%s", arg[n])){
        args[n] = arg[n];
	    n ++;
		c = getchar();
        scanf("%c", &c);
		if(c == '\n'){
		   break;
		}
}
```

Not considering special cases, we use `execvp(args[0], args)`to execute the command simulating the shell, after that we continue the loop.

Also, we fork a child process to run the execution.

#### History Recalling 

 If we get the command `!!`, then we need to replace the current command with the history command, if there is no history command, we through out warnings. If the command is not `!!` we need to save the current command for use:

```c
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
```



#### Termination Detection

We also need to check if the command is `exit` to determine whether or not we should close down the shell:

```c
if(strcmp(arg[0], "exit") == 0){
	      should_run = 0;
	      continue;
}
```



#### Wait-or-not Detection

In this part we check the `&` symbol to decide if we should wait for the child process to be done:

```c
if(strcmp(args[n- 1], "&") ==  0){
       n -- ;
       wait_ = 0;
       args[n] = NULL;
}
```



####  Redirection 

We need to detect if the second to last command is `>` or `<` to make file redirection, if needed, we use `dup2` function to do that, this is done in the `pid == 0` part:

```c
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
```



#### Pipe communication

If there is `|` is the command, we use the output of the first command as the input for the second half. For example if we have `ls -l | less`, then the output for `ls -l` is input into `less`. We use the `pipe(fd)` function to implement this:

___pipe detection___

```c
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
```
___pipe implementation___

```c
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
```



### Tests and Results

- Test basic command, redirections, history and wait-or-not 

  ![image-20200526011023113](C:\Users\HP\AppData\Roaming\Typora\typora-user-images\image-20200526011023113.png)

​                      <img src="C:\Users\HP\AppData\Roaming\Typora\typora-user-images\image-20200526011117797.png" alt="image-20200526011117797" style="zoom:80%;" />

- test for pipe

  ![image-20200526011301289](C:\Users\HP\AppData\Roaming\Typora\typora-user-images\image-20200526011301289.png)

​                                        ![image-20200526011226039](C:\Users\HP\AppData\Roaming\Typora\typora-user-images\image-20200526011226039.png)   

![image-20200526011411439](C:\Users\HP\AppData\Roaming\Typora\typora-user-images\image-20200526011411439.png)

##  Linux Kernel Module for Task Information 

### Key Features

​    We use /proc system to check information for certain process, and the key features are:

- Using linux /proc file system to implement

- Deal with user write to /proc file

- Return process information to users through /proc read

  

### Implementation Details

The main structure is from the [source code](https://github.com/greggagne/osc10e/blob/master/ch3/pid.c) for COS book.

We need to change the `proc_read` and `proc_write` , in read part we deal with user read, we get he target process id and use `pid_task` structure to get the information includes `command`, `pid` and ` state `. Then we check if the process exist in current state. 

```C    
static ssize_t proc_write(struct file *file, const char __user *usr_buf, size_t count, loff_t *pos)
{
        char *k_mem;

        // allocate kernel memory
        k_mem = kmalloc(count, GFP_KERNEL);

        /* copies user space usr_buf to kernel buffer */
        if (raw_copy_from_user(k_mem, usr_buf, count)) {
		printk( KERN_INFO "Error copying from user\n");
                return -1;
        }

	/**
 	 * kstrol() will not work because the strings are not guaranteed
	 * to be null-terminated.
	 * 
	 * sscanf() must be used instead.
	 */
	sscanf(k_mem, "%ld", &l_pid);

        kfree(k_mem);

        return count;
}

```

```c
static ssize_t proc_read(struct file *file, char __user *usr_buf, size_t count, loff_t *pos)
{
        int rv = 0;
        char buffer[BUFFER_SIZE];
        static int completed = 0;
        struct task_struct *tsk = NULL;

        if (completed) {
                completed = 0;
                return 0;
        }

        tsk = pid_task(find_vpid(l_pid), PIDTYPE_PID);

        completed = 1;
	
	if (tsk == NULL)  // pid not exist in current state
	{
		rv = sprintf(buffer, "Process not found!---\n");
		
	}
	else
	{
		int state;
                pid_t pid;
                char command[MAX_LINE];
                state = tsk -> state;
		pid = tsk -> pid;
		
		strcpy(command, tsk -> comm);
		rv = sprintf(buffer, "INFO: command = [%s], pid = [%d], state = [%d]\n", command, pid, state);
		
	}

        // copies the contents of kernel buffer to userspace usr_buf 
        if (raw_copy_to_user(usr_buf, buffer, rv)) {
                rv = -1;
        }

        return rv;
}
```

### Tests and Results

![image-20200526012422742](C:\Users\HP\AppData\Roaming\Typora\typora-user-images\image-20200526012422742.png)

​      (Since `ps` command is done when we execute the next command, so it does not exit in current state)

## Conclusion

Through this project, I implement a simple shell using knowledge of process and pipe communication, which we learn more about the programming for simple multi-process program under linux environment. Also I learn to write linux kernel modules for task information and have a better understanding of the /proc file system.

Thanks for the instructions and useful help offered by Prof. Wu and all the TAs!