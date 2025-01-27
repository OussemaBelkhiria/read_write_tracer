#include <sys/ptrace.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/user.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/ptrace.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <stdbool.h>
 
int main (int argc, char * argv[]){
    
    pid_t child_process = fork();
    if (child_process < 0) {
        perror("failed to create child process\n");
        return -1;
    }
     if (argc < 2)  {perror("not enough arguments\n"); return -1;}
     if(child_process == 0) {
          /* we forward the execution of the program to the child process using execvp
             the exec family functions do not return when executed successfully
             execp doesn´t need the full path because it looks into directories */
      if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0)
            {perror("failed to trace child process\n");
            return -1;} 
        if (execvp(argv[1], &argv[1]) < 0){
            perror("failed to execute the program\n");
            return -1;
        }
                  
    }
   int status;
   bool case_rw = false;
   struct ptrace_syscall_info pt;
   if (waitpid(child_process, &status, 0) < 0) {perror("failure during tracing  : in waiting \n"); return -1;}
        
    if (WIFEXITED(status))
            exit(WEXITSTATUS(status));
        
    // info from the manual : in case of entry or exit the data returned from the struct ptrace_syscall_info pt is limited to 0 (None) unless the option TRACESYSGOOD is set
    if(ptrace(PTRACE_SETOPTIONS, child_process, 0, PTRACE_O_TRACESYSGOOD) < 0) 
      
      {perror("failure during tracing  : PTRACE_SETOPTIONS\n");  return -1;}   
    while(1) {
        // contintue to the next syscall
       if (ptrace(PTRACE_SYSCALL, child_process, NULL, NULL) < 0) 
         {perror("failure during tracing  : PTRACE_SYSCALL\n"); return -1;}
        
          
          
        if (waitpid(child_process, &status, 0) < 0) {perror("failure during tracing  : in waiting \n");
                                                    return -1;}
          
     
        if (WIFEXITED(status))
                exit(WEXITSTATUS(status));
                
        
        size_t size = sizeof(pt);
       
        if(ptrace(PTRACE_GET_SYSCALL_INFO, child_process, &size, &pt) < 0) 
         {perror("failure during tracing  : PTRACE_GET_SYSCALL_INFO\n");
          return -1;}
        
        if (pt.op == PTRACE_SYSCALL_INFO_ENTRY) {    
            //check if the syscall is read or write :
            if (pt.entry.nr == SYS_write ) 
            {  
            case_rw = true;    
            fprintf(stderr,"write(%llu, 0x%llx, %llu) = ",pt.entry.args[0], pt.entry.args[1], pt.entry.args[2]);
            }
            
            else if (pt.entry.nr == SYS_read) 
            {
            case_rw = true; 
            fprintf(stderr,"read(%llu, 0x%llx, %llu) = ",pt.entry.args[0], pt.entry.args[1], pt.entry.args[2]);
            }
        }

        // after the syscall is executed retrieve the return value
        else if (pt.op  == PTRACE_SYSCALL_INFO_EXIT && case_rw == true ) {
            case_rw = false; 
            fprintf(stderr,"%lld\n",pt.exit.rval);
        }
    }
    return 0;

 }