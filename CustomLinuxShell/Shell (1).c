#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include "Shell_functions.h"

int main(int argc , char *argv[])
{
    char* args[MAX_COMMANDS];
    char* pipe_args[MAX_COMMANDS];
    char line[MAX_CHAR];
    int fp1, fp2, fp3;
    int pipefd[2];
 
    pipe(pipefd);
  
    	
    while(read_parse_line(args,line,pipe_args)) // epistrefetai apo thn sinartisi 1 apote ginetai atermonos
    {
    	// dhmiourgia diergasias
        pid_t child_pid = fork(); // dhmiourgia thigatrikis diergasias

         if(child_pid == 0)
      	{ 
      	     // se periptwsi pou isxuei px cat < in.txt > out.txt
             if(total1==2)
            {
                //ean exoume < kai > kai ta input kai out put file den einai kena
          	    if(input_red_overwrite_flag == 1 && input_file != NULL && output_red_overwrite_flag == 1 && output_file != NULL)
            	{ 
            		
            		fp1=open(input_file, O_RDWR | O_CREAT, 0777); // anoigoume ayto to arxeio me ayth thn epithimiti prosvasi kai adeies prosvasis
            		fp2=open(output_file ,O_RDWR | O_CREAT, 0777);
                	dup2(fp1, 0); // dhmiourgoume ena antigrafo tou fp1 gia read
                	if(fp1<0)
                	{
                		perror("fp1: error can't open file\n");
                	}  
            		
                	dup2(fp2, 1); // dhmiourgoume ena antigrafo to fp2 gia write
                	if(fp2<0)
                	{
                		perror("fp2: error can't open file\n");
                	
                	}
                	close(fp1); // kleinoume file descriptors
                	close(fp2);
            	}
            	
          }

          // se periptwsi pou isxuei px cat < in.txt  >> out.txt
          else if(total2==2)
          {     
              // ean exei vrei < kai >> kai to input kai output file den einai kena
            	if(input_red_overwrite_flag == 1 && input_file != NULL && output_red_overwrite_flag == 1 && output_file_append != NULL)
          	    {
          		    fp1=open(input_file, O_RDWR|O_CREAT, 0777);
          		    fp3=open(output_file_append, O_RDWR|O_APPEND|O_CREAT, 0777);
                	dup2(fp1, 0);
                	if(fp1<0)
                	{
                		perror("fp1: error can't open file\n");
                	} 
                	
                	dup2(fp3,1);
                	if(fp3<0)
                	{
                		perror("fp3: error can't open file\n");
                	}
                	close(fp1);
                	close(fp3); 
            	}
          }
          // se periptwsi aplwn anakateuthinsewn		
          else
          {
                 // gia < 
                if(input_red_overwrite_flag == 1 && input_file != NULL)
                {
            	    fp1=open(input_file, O_RDWR|O_CREAT, 0777); // diavasma, grapsimo kai dhmiourgia
                    dup2(fp1, 0);
                    if(fp1<0)
                    {
                	    perror("fp1: error can't open file\n");
                    }  
                }
                // gia >
                if(output_red_overwrite_flag == 1 && output_file != NULL)
                {
                    fp2=open(output_file, O_RDWR|O_CREAT, 0777);
                    dup2(fp2, 1);
                    if(fp2<0)
                    {
                	    perror("fp2: error can't open file\n");
                    }  
                }
                // gia >>
                if(output_red_append_flag == 1 && output_file_append != NULL)
                {
                    fp3=open(output_file_append, O_RDWR|O_APPEND|O_CREAT, 0777);
                    dup2(fp3,1);
                    if(fp3<0)
                    {
                	    perror("fp3: error can't open file\n");
                    }   	
                }
          }
          // se periptwsi mias solinosis
            if(pipe_flag == 1)
            {
                execute_pipe(args, pipe_args, pipefd);
                exit(0);
            }
             execvp(args[0], args); // analoga se poia if mpainei ektelei kai thn katallili entoli
      	}
            else if(child_pid<0)
            {
      		    perror("Fork failed\n");
      		    exit(1);
            }
            // wait gia thn apofugh zombie		
            else
            {
                waitpid(child_pid,NULL, 0); // mpainei kai ta 2 epomenos tou leme na perimenei na teleiwsei to paidi etsi wste na sunexisei me to mhdenismo
                input_red_overwrite_flag = 0; // mhdenismos gia to epomeno promt (pateras)
	            output_red_overwrite_flag = 0;
                output_red_append_flag = 0;
                pipe_flag = 0;
                input_file = NULL;
                output_file = NULL;
                output_file_append=NULL;
                input_red_overwrite_count = 0; 
                output_red_overwrite_count = 0;
                output_red_append_count=0;
                pipe_count=0;
                total1=0;
                total2=0;
    	    }
    	
    }
    return 0;
}
