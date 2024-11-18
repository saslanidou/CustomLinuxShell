#ifndef TEST6_H
#define TEST6_H

#define MAX_COMMANDS 10
#define MAX_CHAR 50

 //global metavlites gia na mporoume na tis xeiristoume se oles tis sunartiseis

// flags gia entopismo anakateuthinsis kai solinosewn
int input_red_overwrite_flag = 0;
int output_red_overwrite_flag = 0;
int output_red_append_flag = 0;
int pipe_flag = 0;
//arxeia anakateuthinsis eisodou kai eksodou
char* input_file = NULL;
char* output_file = NULL;
char* output_file_append = NULL;
//metrites anakateuthinsis kai solinosewn
int input_red_overwrite_count = 0; 
int output_red_overwrite_count = 0;
int output_red_append_count=0;
int pipe_count=0;

int total1=0;
int total2=0;

//diavasma entolis
void get_line(char line[])
{
	char path[1024];
	char* ret;
	int i=0;
	
	// emfanisi promt
	getcwd(path, sizeof(path));
	printf("D: %s my_shell$: ", path);
	
    ret = fgets(line,MAX_CHAR, stdin); // dexetai to line apo to plhktrologio (stdin)
	
	//diagrafi '\n' apo telos tis grammis
    while(line[i] != '\n')
    {
    	i++;
    }

    line[i] = '\0';
    	
    //exit 
	if(strcmp(line,"exit") == 0 || ret == NULL)
	{
		exit(0);
	}
}

// xwrismos line se commands
int parse_line(char* temp[], char line[])
{
    int i = 0;
    
    temp[i] = strtok(line, " "); // xwrizei to line se mikrotera kommatia kai ta apothikeuei sti temp[i]
    
    if(temp[i] == NULL)
    {
    	printf("no command\n");
    	return 1;
    } 
    
    while(temp[i] != NULL)
    {
        i++;
        temp[i] = strtok(NULL, "  "); // epistrefei ton deikti null ean den exei alles entoles gia anaktisi
    }
    return 1;
}

//euresi anakateuthinsis kai swlinosis
int find_redirection_and_pipe(char* temp[])
{
    int i = 0;
  
 
    while(temp[i] != NULL)  // oso to temp[i] exei entoles kai den einai NULL
    {
        if(strcmp(temp[i],">") == 0) // elegxei an h entolh pou vrisketai to temp[i] einai idia me to ">" an nai tote ektelei oti vrisketai mesa sthn if
        {
            output_red_overwrite_flag = 1; // oti vrike ena redirection
            output_file = temp[i+1]; // an to i einai to redirection tote to output file einai mia thesi meta ara temp[i+1]
            return i; // na epistrefei thn thesi i tou redirection
        }
        if(strcmp(temp[i], ">>") == 0)
        {
            
            output_red_append_flag = 1;
            output_file_append = temp[i+1];
            return i;
        }
        if(strcmp(temp[i],"<") == 0)
        {
            input_red_overwrite_flag = 1;
            input_file = temp[i+1];
            
            // se periptwsi 2 anakateuthinsewn
            output_file = temp[i+3]; // ean exoume kai > kai < h >> tote tou leme na paei an i thesi tou prwtou < tote to +1 einai to input to +2 to >> h to > kai to +3 to output_file
            output_file_append = temp[i+3];
            output_red_overwrite_flag =1; // edw oti vrike to > // an den vrethikan den maw epireazoun
            output_red_append_flag = 1; // edw oti vrike to >>
            
            return i; // epistefei thn thesi tou i pou vrisketai to <
        }
        if(strcmp(temp[i],"|") == 0)
        {
            pipe_flag = 1; // an vrei to pipe
            return i; // epistefei thn thesi tou i pou vrisketai to pipe
        }
        
        i++; // gia na kanei ton epomeno elegxo h while
    }
   
    return i; // edw epistrefei thn teliki timh tou i otan ginetai null otan den vrike kati apo ta parapanw
}

// metrites kai elegxos anakateuthinsewn kai solinosewn
void count_redirection_and_pipe(char* temp[])
{
    int i = 0;
    
    if(temp[i] == NULL)
    {
        printf("no command\n");
        return;
    }

    while(temp[i] != NULL)
    {
        if(strcmp(temp[i], ">") == 0)
        {
            output_red_overwrite_count++; // metritis gia ta posa > exoun vrei sto line
            
        }
        if(strcmp(temp[i], ">>") == 0)
        {
            output_red_append_count++; 
            
        }
        if(strcmp(temp[i], "<") == 0)
        {
            input_red_overwrite_count++;
        }
        if(strcmp(temp[i], "|") == 0)
        {
            pipe_count++; // metritis gia ta posa pipe exei vrei sto line 
        }
       
        i++; // gia na sunexistei o elegxos tis while
    }
   
    // se periptwsi pou isxuei px cat < in.txt > out.txt
    if((output_red_overwrite_count+input_red_overwrite_count)> 1)
    {
    	total1=2;  // global metavliti gia na thn xrisimopoihsoume kai se allh sunartisi
    }
    // se periptwsi pou isxuei px cat < in.txt  >> out.txt
    if((input_red_overwrite_count+output_red_append_count)>1)
    {
    	total2=2;
    }
    // se periptwsi pollaplwn solinwsewn kai anakateuthinsewn
    if((output_red_overwrite_count+input_red_overwrite_count+ output_red_append_count+ pipe_count)>2)
    {
    	perror("Can't support more than two processes\n");
    }
     
}
//anathesi twn entolwn apo to temp[] stous telikous pinakes twn arguments
int read_parse_line(char* args[],char line[], char* pipe_args[])
{
    char* temp[MAX_COMMANDS];
    int i=0;
    int thesi;
    
    get_line(line); // dexomaste apo to plhktrologio thn line
    parse_line(temp,line); // xwrizoume thn line me vasei ta kena (entoles) se pinaka temp
   
    count_redirection_and_pipe(temp); // metrame posa pipe kai redirection yparxoun se ena line
    thesi = find_redirection_and_pipe(temp); // me thn klhsh autis tis sunarthshs exoume epistrofi ths thesis i pou vriksetai to pipe h redirection
    while(i < thesi) // oso ti i einai mikrotero ths thesis
    {
        args[i] = temp[i]; // vazei tis entoles pou uparxoun sto temp ston pinaka args (diergasia paidi) aristera
        i++;
    }
    args[i] = NULL; // molis ginei thesi to i to kanei iso me null
    i++; // paei sthn epomeni thesi

    if(pipe_flag == 1) // an vrei pipe 
    {
        int j = 0;
        while(temp[i] != NULL) // oso to temp[i] exei entoles kai den einai null
        {
            pipe_args[j] = temp[i]; //  gemizei ton pinaka pipe_args me arguments apo to temp (deksia)
            i++; // allazoun thesi tautoxrona
            j++;
           
        }
    }
    return 1; // epistefei ena gia thn while sthn main
}


// ektelesis solinosis
void execute_pipe(char* args[], char* pipe_args[], int pipefd[])
{
    int child_pid;
    
    child_pid = fork(); // dimiourgei mia nea diergasia paidi

    if(child_pid == 0)
    {
    	close(pipefd[0]); //kleinoume to pipefd[0] giati den tha xrisimopoihthei
        dup2(pipefd[1],1); // dhmiourgoume antigrafo tou pipefd me 1 gia write
        close(pipefd[1]);  // to kleinoume
        execvp(args[0], args); // ektelesi tis entolis
        perror(args[0]); // an den eketelestei h exec tote perror
    }
    else
    {	
    	close(pipefd[1]); //kleinoume to pipefd[1] giati den tha xrisimopoihthei
        dup2(pipefd[0], 0); // dhmioyrgoume antigrafo tou pipefd gia read
        close(pipefd[0]); // to kleinoume
        execvp(pipe_args[0], pipe_args); // ekteloume thn entoli meta to pipe
        perror(pipe_args[0]); // ean paei kati lathos me thn ektelesi tote perror
               
    }

}

#endif

