//Tuan Nguyen
// Lab2 : Simple shell
// CIS 3207

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>

int command_cd(char *input);
char *get_input();
char **get_parse(char *input);
int command_dir(char *input[]);
int command_pause(char *input);
int command_environ(char *input[]);
int command_help(char *input[]);
int command_echo(char *input[]);
int command_redirect(char *input[], char *input2[], int type);
int command_background(char *input[]);
int command_redirect2(char *input[]);
int command_pipe(char *input[], char *newinput[]);
int run_command(char *argument[]);
int batchfile(char *input[]);
int command_ls(char *input[]);

extern int errno; // return error in program
extern char **environ; // use to get environment string 

int main(){
    char *input;   
    char **argument; 


    while(1){  // run until user hit quit
         printf("%s", getenv("PWD")); // get the path of current directory
         char cwd[20]; 
         getcwd(cwd,sizeof(cwd)); // get the size of current directory
         printf("/myshell>%s ",cwd); //print current directory
         input = get_input(); // get input
         argument = get_parse(input); // parse input into token separate by space " "

         run_command(argument); // run each command in the shell
        }
}

int run_command(char *argument[]){

                if(strncmp(argument[0] , "cd", 2) == 0){        // command to change current directory
                    command_cd(argument[1]);
                
                }       
                else if(strncmp(argument[0], "dir",3) == 0){    // command to list content of current directory
                    command_dir(argument);
                }
                else if(strncmp(argument[0] , "clear", 5) == 0){    // command to clear screen
                    printf("\033[H\033[2J");
                }
                else if(strncmp(argument[0] , "pause", 5) == 0){ // command to pause user input until they hit enter 
                    command_pause(argument[1]);
                }
                else if(strncmp(argument[0] , "environ", 7) == 0){ // command to list all the enviroment variable in the shell
                    command_environ(argument);
                }
                else if(strncmp(argument[0] , "exit", 4) == 0){ // command to exit the program
                    printf("Shell is exit\n");
                    exit(0);
                }
                else if(strncmp(argument[0] , "help", 4) == 0){ // command to print readme file  contain information about using the shell
                    command_help(argument);
                }
                else if(strncmp(argument[0] , "echo", 4) == 0){ // command to echo // ex: echo hello , will print hello to screen 
                    command_echo(argument);
                }
                else if(strncmp(argument[0], "./myshell", 9) == 0){ // run batch file to execute command inside
                    batchfile(argument);
                }     
                else {
                    for(int i = 0 ; argument[i] != NULL; i++){
                        if(argument[i][0] == '&'){  // check for background
                            //printf("background");
                            argument[i] = NULL; 
                            command_background(argument);
                            break;
                        }
                        else if (argument[i] != NULL && argument[i][0] != '&'){  //check for external command
                              pid_t pid;
                            pid = fork();

                            if(pid < 0){
                                printf("fork fail");
                                }
                                 else if (pid == 0){
                                execvp(argument[0],argument);
                                    exit(1);
                                }
                                else{
                                int status = 0;
                                wait(&status);
                            }

                        }
                        else if(argument[i][0] == '<'){ // command for i/o redirect : <

                            command_redirect(argument, argument, 1);
                            }
                        }
                    }
                               


               /* else if(strncmp(argument[0], "ls", 2) == 0){ // run ls 
                    command_ls(argument);
                }
                else if(argument[0] != NULL && argument[1][0] == '&' && argument[2] == NULL){ // run background
                    command_background(argument);
                }
                else if (argument[0] != NULL && argument[1] == NULL){ // input invalid shell command
                    printf("Invalid command\n");
                }
                else if(argument[0] != NULL && argument[1][0] == '<'){ // command for i/o redirect : <
                    command_redirect(argument);
                }
                else if(argument[0] != NULL && argument[1] != NULL && argument[2][0] == '>'){  // command for i/o redirect : > , >>
                    command_redirect2(argument);
                }
                else if (argument[0] != NULL && argument [1] != NULL && argument[2][0] == '|' && argument[3] != NULL){ //command for pipe two commands : input1 | input2
                     command_pipe(argument);
                   
                }*/
               

                return 0;
}
// read batch file (batch.txt) and run command
int batchfile(char *input[]){
   // printf("in here 2\n");
    
    FILE *infile = fopen(input[1], "r+"); // open batch file to read
    char ch[200]; // size of character to read 

        while(fgets(ch,200,infile) !=  NULL){ // read each character
            char **input1 = get_parse(ch); // parse character inside batch file into token "space" 
            run_command(input1); // run each line in batch file and execute 
            }
        fclose(infile);
     
    return 0;
}

//change current directory
int command_cd(char *input){
        printf("In command Cd\n");
        char *currentpath= getenv("PWD");

        if(chdir(input) == 0){ // user enter cd <namedir>
                
                printf("Had sucessful change home directory to / %s\n", input); 
                return EXIT_SUCCESS;
		}
        else if(input == NULL){ // user enter cd with no argument 
                printf("current path: %s/myshell> \n", currentpath); // print the current path of directory
            
                return EXIT_SUCCESS;
        }
		else {    // print error 
           perror(input);
           return EXIT_FAILURE;  
		}
    return 0; //successful change current directory*/
}

//read user command input can store in a buffer
char *get_input(){

	int buffer_size = 1000;  //initiated buffer size to 1000
    int character; //store the character that user input

	int position = 0; //the position of character

	char *input = malloc(sizeof(char)*buffer_size); //allocate the buffer

	while(1){

		character = getchar(); //get character
		if(character == EOF || character == '\n'){  //get the character till end of file or end of line
			input[position] = '\0'; // get input position until end of string
			//putchar(input); // put character to outout
			return input;
		}
		else{
			input[position] = character; //if input position equal character
		}
		position++; //increment position

	}
        free(input); //free input after done using
    return input;
}

//parse user input into args which separated by " " space
char **get_parse(char *input){
    char **split_line;
    char *token;
    int sizeOfLine = 100;
    int position = 0;
    split_line = malloc(sizeof(char *) * sizeOfLine); // get the size of line

    token = strtok(input, " "); // use strtok that take input and delimiter to split string into token

    while(token != NULL){ // token is not end of line
        split_line[position++] = token; //increment the position of line
        token = strtok(NULL, " "); //split line into token use when user type cd > << &

    }

    split_line[position] = NULL; //split line until the end of string
    return split_line;
}

// check for input that contain background: ex: input &
int command_background(char *input[]){
    printf("in background\n");
    pid_t pid; // set up process id
    
    pid = fork();  // call fork to create new process
    if (pid == 0){
        execvp(input[0],input);  // execvp(name of external coommand, array of input pass in
        exit(1);
    }
    else{   //parent can run after child process has finished
       int state = 0;
       wait(&state);

    } 
    return 0;
}

// list the content of directory 
int command_dir(char *input[]){
    //printf("In dir command\n");
    DIR *dir;
    struct dirent *s;
    char *dirname;

    FILE *outfile;
    
     if (input[1] == NULL || input[1][0] == '>' || input[1][0] == '&'){ // didn't specify a file 
        dirname = ".";
            if(input[1] == NULL){       // command : dir  // print content of current directory to screen
                if(input[1] == NULL){
                printf("Success list content of current directory\n");
                
                dir = opendir(dirname);     
                while((s = readdir(dir)) != NULL){
                
                printf("%s\n",s ->d_name); 
                
                }
                closedir(dir);
                }
            }
           else if(input[1][0] == '&'){ // check for background
                command_background(input);
                printf("Success list content of current directory\n");
                
                dir = opendir(dirname);     
                while((s = readdir(dir)) != NULL){
                
                printf("%s\n",s ->d_name); 
                
                }
                closedir(dir);

                getchar();

            }
            else if(input[1][0] == '>' && input[1][1] == '>' && input[2] != NULL){ //command : dir >> filename
                printf("Success append list content of current directory to %s \n",input[2]);
                outfile = fopen(input[2],"a+");
               
                dir = opendir(dirname);
                while((s = readdir(dir)) != NULL){
                fprintf(outfile,  "%s\n", s->d_name);
                
                    }
                fclose(outfile);
                closedir(dir);
            }
            else if(input[1][0] == '>' && input[2] != NULL){    //command : dir > filename
                printf("Success write list content of current directory to %s \n",input[2]);
                outfile = fopen(input[2],"w+");
               
                dir = opendir(dirname);
                while((s = readdir(dir)) != NULL){
                fprintf(outfile,  "%s\n", s->d_name);
                    }
                fclose(outfile);
                closedir(dir);
            } 
    }

    else {      // specify a file 
        dirname = input[1];  
             
            if(input[1] != NULL && input[2] == NULL){
                printf("Success list content of directory: %s\n",input[1]); //command: dir foldername
                    dir = opendir(dirname);
                
                while((s = readdir(dir)) != NULL){
                     printf("%s\n",s ->d_name); 
                    } 
                     closedir(dir);
        } 

            else if(input[1] != NULL && input[2][0] == '>' && input[2][1] == '>' && input[3] != NULL){ //command: dir foldername >> filename
                printf("Success append content of directory %s to %s\n",input[1], input[3]);
                
                outfile = fopen(input[3],"a+");
               
                dir = opendir(dirname);
                while((s = readdir(dir)) != NULL){
                fprintf(outfile,  "%s\n", s->d_name);
                    }
                fclose(outfile);
                closedir(dir);
            }     
            else if(input[2][0] == '>' && input[3] != NULL){ //command: dir foldername > filename 
        
                printf("Success write content of directory %s to %s\n",input[1], input[3]);
                outfile = fopen(input[3],"w+");
               
                dir = opendir(dirname);
                while((s = readdir(dir)) != NULL){
                fprintf(outfile,  "%s\n", s->d_name);
                    }
                fclose(outfile);
                closedir(dir);
            }
            else {  // command : dir invalid folder name 
                 perror("error"); // print error
                return EXIT_FAILURE;
            }
         }   
            
 return 0;
}
// to pause input 
int command_pause(char *input){
    printf("Hit enter to continues"); 
    getchar();  // hit enter to continues

    return 0;
}
int command_redirect(char **args1 , char **args2 , int type){

    int fd[2]; //file descriptor
    pid_t pid;
    char *file = args2[0];
    int status = 0;

    pid = fork();
    if(pid < 0){
        printf("%s\n", "Forking child process failed");
        exit(1);
    }
    else if(pid == 0){

        if(type == 2){ //input redirection
            fd[0] = open(file, O_RDONLY , 0600); //open file to read
            if(fd[0] == -1){
                printf("Error Number % d\n", errno);
                perror("Error opening file descriptor");
            }
            dup2(fd[0] , 0); //duplicate file descriptoe with stdin
            close(fd[0]); //close fd
        }
        else if(type == 1){ //output redirection
            fd[1] = open(file , O_WRONLY | O_CREAT | O_TRUNC, 0600); //open file to write
            if(fd[1] == -1){
                printf("Error Number %d\n", errno);
                perror("Error opening file descriptor");
            }
            dup2(fd[1] , 1); //duplicate file descriptor with stdout
            close(fd[1]); //close fd
        }
        execvp(args1[0] , args1);
        exit(errno);
    }
    else{
        waitpid(pid , &status , WUNTRACED);
    }

    return 1;
}
// list evironment variable in the shell
int command_environ(char *input[]){
    int i = 0;
    FILE *inputfile;

    if(input[1] == NULL){
        printf("in here\n");
            while(1){
                if(environ[i] != NULL ){        //get environment string if not NULL
                     printf("%s\n",environ[i]); // print envionment string 
                    i++;}  
                else{   //done print all evironment string 
                     break; // exit the evironment command to run other command 
                }       
            }
    }
    else if(input[1][0] == '>' && input[1][1] == '>' && input[2] != NULL){ // support output redirect append to a file : environ >> filename.txt
        printf("Success append environ to : %s\n", input[2]);
            inputfile = fopen(input[2], "a+"); // append to file
           while(1){

                if(environ[i] != NULL ){        //get environment string if not NULL
                    fprintf(inputfile, "%s\n", environ[i]);
                    i++;
                }  
                else{   //done print all evironment string 
                     break; // exit the evironment command to run other command 
                }}
            fclose(inputfile);    


    }
    else if(input[1][0] == '>' && input[2] != NULL){    // support output redirect write  to file: environ > filename.txt
        
         printf("Success write environ to : %s\n", input[2]);
            inputfile = fopen(input[2], "w+"); // write to file
           while(1){

                if(environ[i] != NULL ){        //get environment string if not NULL
                    fprintf(inputfile, "%s\n", environ[i]);
                    i++;
                }  
                else{   //done print all evironment string 
                     break; // exit the evironment command to run other command 
                }}
            fclose(inputfile);      
    }
    else {
        printf("Error environ command\n");
    }
    return 0;
}
// print readme file to screen
int command_help(char *input[]){
    FILE *namefile;
    FILE *writefile;
    FILE *appendfile;

    int ch= 2000; // number of character to read in file
    
    if(input[1] == NULL){
        namefile = fopen("readme.txt","r+"); // open file to read

        while ((ch = getc(namefile)) != EOF) // get each character in file until end of file
            putchar(ch); // print each character to screen
            printf("\n"); 
    } 

    else if(input[1][0] == '>' && input[1][1] == '>' && input[2] != NULL){ // help >> filename.txt
        printf("Success append readme file to : %s\n",input[2]);
        namefile = fopen("readme.txt","r+"); // open file to read
        appendfile = fopen(input[2], "a+"); // append to file
         while ((ch = getc(namefile)) != EOF){ // get each character in file until end of file
            putc(ch,appendfile); 
        }
        fclose(appendfile);
    }
    else if(input[1][0] == '>' && input[2] != NULL){ // help > filename.txt
        printf("Success write readme file to : %s\n",input[2]);
        namefile = fopen("readme.txt","r+"); // open file to read
        writefile = fopen(input[2], "w+"); // append to file
         while ((ch = getc(namefile)) != EOF){ // get each character in file until end of file
            putc(ch,writefile); 
        }
        fclose(writefile);
    }
    else{
        printf("Error help command\n");
    }

    return 0;
}

/*// I/0 redirect : < 
int command_redirect(char *input[]){


    //printf("in redirect command\n");
    int ch = 1000;

        if(input[0] != NULL && input [1][0] == '<' && input[2] != NULL && input[3] == NULL){ // command : cat < filename
            //printf("here1\n");
            FILE *infile = fopen(input[2], "r");
            while((ch = getc(infile)) != EOF){
                putchar(ch);
                }
            printf("\n");
            fclose(infile);       

        }    
        else if(input[2] != NULL && input[3][0] == '>' && input[3][1] == '>' && input[4] != NULL){ // command: hello < in.txt >> out.txt
                printf("Success append to %s\n", input[4]);
                FILE *infile = fopen(input[2], "w+");
                if(infile == NULL){
                    printf("cannot open file");
                }
                else{
                    fprintf(infile, "%s", input[0]);
                }
                fclose(infile);
              
                infile = fopen(input[2], "r+");
                FILE *outfile = fopen(input[4],"a+");

                if(infile == NULL){
                    printf("cannot open file");
                }
                else{
                    while ((ch = getc(infile)) != EOF){
                         putc(ch, outfile);
                 }
                 fclose(outfile);}
        }

        else if(input[2] != NULL && input[3][0] == '>' && input[4] != NULL){ // command: hello < in.txt > out.txt
                printf("Success write to %s\n", input[4]);
                FILE *infile = fopen(input[2], "w+");
                if(infile == NULL){
                    printf("cannot open file");
                }
                else{
                    fprintf(infile, "%s", input[0]);
                }
                fclose(infile);
              
                infile = fopen(input[2], "r+");
                FILE *outfile = fopen(input[4],"w+");

                if(infile == NULL){
                    printf("cannot open file");
                }
                else{
                    while ((ch = getc(infile)) != EOF){
                         putc(ch, outfile);
                 }
                 fclose(outfile);
            } 
        }
    
   else { // user type random input
        printf("error\n"); // print error
   } 
    return 0;
}
// i/O redirect : >
int command_redirect2(char *input[]){
    int ch = 1000; // number of character to read
    //printf("in here");
    if(input[0] != NULL || input[1] != NULL || input[2][0] == '>'){  // command: cat in.txt >> out.txt 
        if(input[2][1] == '>'&& input[3] != NULL){      
            printf("Success append the text file to %s\n",input[3]);
           FILE *infile2 = fopen(input[1], "r+");       //open file to read
           FILE *outfile2 = fopen(input[3], "a+");      //append the content file to new file 
    

                while((ch = getc(infile2)) != EOF){
                    putc(ch, outfile2);
           }
           fclose(infile2);
           fclose(outfile2);
        }
        else if (input[2][0] == '>' && input[3] != NULL){ // command: cat in.txt > out.tx
             printf("Success write the text file to %s\n",input[3]);
            
                FILE *infile1 = fopen(input[1], "r+");  // open file to read
                FILE *outfile1 = fopen(input[3], "w+"); // write the content of file to new file 
           
                while((ch = getc(infile1)) != EOF){
                    putc(ch, outfile1);
           }
           fclose(infile1);
           fclose(outfile1);

        }    
    }

    return 0;
}
*/
// command for piping 
int command_pipe(char *input[], char *newinput[]){
    printf("in pipe command: \n");
    printf("the count: \n");
    int thePipe[2];  // use two file descriptor: one for read , one for write
    pid_t pid;

    pipe(thePipe); 

    pid = fork();
    if(pid < 0){        
        fprintf(stderr, "fork failed to create");
    }
    else if (pid == 0){ // child process
    
        dup2(thePipe[1],1); //duplicate first file 
        close(thePipe[0]);  // close  end of first pipe 
        close(thePipe[1]); // close file 
        //execlp("cat", input[0], input[1] , NULL); //ls -l
        execvp(input[0], input); // command: cat in.tx
        exit(1);
    }
    else {  // parent
        
        dup2(thePipe[0],0); // duplicate second file 
        close(thePipe[0]); // close end of second pipe
        close(thePipe[1]); // close file

        //execlp("wc", input[3], input[4], NULL); // more
        execvp(newinput[0], newinput); // command: wc -l , wc -c
       exit(1);

    }  


    int status;
     if(status == 0){   // wait for child to finish
        wait(&status);}
    return 0;
}
// command for echo 
int command_echo(char *input[]){  
    
   FILE *writefile; // create file to write 

   if(input[1] != NULL && input[2] == NULL){ // echo hello 
         printf("%s",input[1]); 
          printf("\n");
    }
    else if(input[1] != NULL && input[2] != NULL && input[3] == NULL){ //echo hello world
        printf("%s %s",input[1],input[2]); //print each input 
         printf("\n");
    }    
    
    else if(input[1] != NULL && input[2] != NULL && input[3][0] == '>' && input[3][1] == '>' && input[4] != NULL){ //echo hello world >> filename.txt
        printf("Success append the word to %s\n",input[4]);
        writefile = fopen(input[4], "a+");
                if(input[1] != NULL && input[2] != NULL){
                    fprintf(writefile, "%s %s", input[1], input[2]);
                    
            }
            fclose(writefile);  
    }
    else if(input[2][0] == '>' && input[2][1] == '>' && input[3] != NULL){ //echo hello >> filename.txt
        printf("Success append the word to %s\n",input[3]);
        writefile = fopen(input[3], "a+");
                if(input[1] != NULL){
                    fprintf(writefile, "%s", input[1]);
                    
            }
            fclose(writefile);   
    }
    else if(input[2][0] == '>' && input[3] != NULL){ //echo hello > filename.txt
        printf("Success write the word to %s\n",input[3]);
        writefile = fopen(input[3], "w+");
                if(input[1] != NULL){
                    fprintf(writefile, "%s", input[1]);
                    
            }
            fclose(writefile);   

    }
    else if (input[3][0] == '>' && input[4] != NULL){ // echo hello world > filename.txt
        printf("Success write the word to %s\n",input[4]);
        writefile = fopen(input[4], "w+");
                if(input[1] != NULL && input[2] != NULL){
                    fprintf(writefile, "%s %s", input[1], input[2]);
                    
            }
            fclose(writefile);   
    }

    return 0;
}
