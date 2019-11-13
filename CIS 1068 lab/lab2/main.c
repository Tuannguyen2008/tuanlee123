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
int command_background(char *input[]);
int command_pipe(char *input[]);
int run_command(char *argument[]);
int batchfile(char *input[]);
int command_external(char *input[]);
int redirect(char **args1 , char **args2 , int type);
int piping(char **args1 , char **args2);

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
                else {  // all enternal command that bash support
                    if(argument[0] != NULL && argument[1] == NULL){ 
                            argument[1] = NULL;
                            command_external(argument);
                    }
                    else if (argument[0] != NULL && argument[1][0] == '&'){ //background
                            printf("in here");
                            argument[1] = NULL;
                            command_background(argument);
                    }

                     else if(argument[0] != NULL && argument[1][0] == '>'  && argument[1][1] == '>' && argument[2] != NULL) { // append to file
                            printf("sucess append to file\n");
                            argument[1] = NULL;
                            redirect(&argument[0],&argument[2],3);
                    }
                     else if(argument[0] != NULL && argument[1][0] == '>' && argument[2] != NULL) { // write to file //who > users , ls > file , 
                            printf("success write to file\n");
                           argument[1] = NULL;
                           redirect(&argument[0],&argument[2],1);
                    }

                    else if(argument[0] != NULL && argument[1][0] == '<' && argument[2] != NULL && argument[3] == NULL){ // input redirect
                            // printf("redirect");
                            argument[1] = NULL;
                            redirect(&argument[0], &argument[2], 2);
                    }

                   
                    else if(argument[0] != NULL && argument[1][0] == '<' && argument[2] != NULL && argument[3][0] == '>' && argument[3][1] == '>' && argument[4] != NULL && argument[5] == NULL){ // input : wc < in.txt >> out.txt
                             printf("redirect2\n");
                            argument[1] = NULL;
                            argument[3]= NULL;
                            redirect(&argument[0], &argument[2], 2);
                            redirect(&argument[2],&argument[4], 3);
                    }

                    else if(argument[0] != NULL && argument[1][0] == '<' && argument[2] != NULL && argument[3][0] == '>' && argument[4] != NULL && argument[5] == NULL){ // input : wc < in.txt > out.txt
                             //printf("redirect2\n");
                            argument[1] = NULL;
                            argument[3]= NULL;
                            redirect(&argument[0], &argument[2], 2);
                            redirect(&argument[2],&argument[4], 1);
                    }

                    else if (argument[0] != NULL && argument [1] != NULL && argument[2][0] == '|' && argument[3] != NULL){ //piping
                            printf("pipe");
                            argument[2] = NULL;
                            piping(&argument[0],&argument[3]);

                    }

                    else{
                            printf("Invalid command\n");
                    }
                }   

                return 0;
}
int redirect(char **args1 , char **args2 , int type){

    int fd[3]; //file descriptor
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
        else if(type == 3){
            fd[2] =  open(file , O_WRONLY | O_CREAT | O_APPEND, 0600); //open file to append
            if(fd[2] == -1){
                printf("Error Number %d\n", errno);
                perror("Error opening file descriptor");
            }
            dup2(fd[2] , 1); //duplicate file descriptor with stdout
            close(fd[2]); //close fd
        }
        execvp(args1[0] , args1);
        exit(errno);
    }
    else{
        waitpid(pid , &status , WUNTRACED);
    }

    return 1;
}

int piping(char **args1 , char **args2){

    pid_t pid1 , pid2;
    int status1 , status2 = 0;
    int pipefd[2];

    if(pipe(pipefd) == -1){ //create pipe for file descriptor
        printf("Error Number %d\n", errno);
        perror("Error creating pipe");
    }

    pid1 = fork();
    if(pid1 < 0){
        printf("%s\n", "Forking child process failed"); //fail to create
        exit(1);
    }
    else if(pid1 == 0){

        close(pipefd[0]); //close unnecessary part (read)
        dup2(pipefd[1] , 1); // duplicate file descriptor to write from stdout
        close(pipefd[1]);//close file descriptor

        execvp(args1[0] , args1); //execute args1
        exit(errno);

    }
    else{
        pid2 = fork();
        if(pid2 < 0){
            printf("%s\n", "Forking child process failed"); //fail to create
            exit(1);
        }
        else if(pid2 == 0){
            close(pipefd[1]);//close unnecessary part(write)
            dup2(pipefd[0] , 0); //duplicate file descriptor to read from stdin
            close(pipefd[0]);//close file descriptor

            execvp(args2[0] , args2); //execute args2
            exit(errno);
        }
        else{
            close(pipefd[0]);
            close(pipefd[1]); //close file descriptors
            waitpid(pid1 , &status1 , WUNTRACED);
            waitpid(pid2 , &status2 , WUNTRACED); //wait
        }
    }
    return 1;
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
        
        int status = 0;
        waitpid(pid , &status , WUNTRACED); //wait
    }
    return 0;
}
// support external command
int command_external(char *input[]){

    pid_t pid;
    int status = 0;
    
        pid = fork();
        if(input[0] !=  NULL && input[1] == NULL){
        if (pid == 0){ // child process execute
             execvp(input[0],input);
                //exit(errno);
         }
        else{   // wait for child to complete
           waitpid(pid , &status , WUNTRACED); //wait

    }
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
