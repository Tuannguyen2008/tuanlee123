/* Help Page */
The shell support 8 commands, i\o redirect, background, piping 
1) cd <dirname>: to change current directory to new directory
ex: cd folder , cd file 

2) clear: Clear the screen with white space
ex : clear 

3) dir <dirname>: list the content of directory or write the content into a .txt file 
ex: dir folder : list the content of folder 
ex: dir file : list the content of file
ex: dir > in.txt: write the content of current directory into in.txt
ex: dir >> in.txt: append the content of current directory into in.txt
ex: dir folder > in.txt : write list the content of folder into in.txt
ex: dir folder >> in.txt : append list the content of folder into in.txt
ex: dir <not dir name> : print "invalid dirname" and ask user to input another exist dirname

4) environ - list all the environment into screen, and support output redirect
ex: environ, environ > in.txt , environ >> in.txt
 
5) echo <argument> - display the <argument> follow by new line, and support output redirect
ex: echo hello, echo hello > in.txt , echo hello >> in.txt
  print:  hello

6) help : display help page into screen,  and support output redirect
ex: help , help > in.txt , help >> in.txt

7) pause: pause user input until they hit enter to continue
ex: pause

8) exit: quit the shell
ex: exit

Background command:
dir & : run dir in the background and return to command line prompt immediately after run dir

I/O redirect: redirection on either stdin and stout
ex: hello < input.txt > out.txt: input.txt take input "hello" and write to out.txt
ex: hello < input.txt >> out.txt: input.txt take input "hello" and append to out.txt
ex: input.txt > out.txt : write everything in input.txt to out.txt
ex: input.txt >> out.txt : append everything in input.txt to out.txt

Piping: take command1 | command2 and strung them together
ex: cat out.txt | wc -c 
	-read the word inside in.txt and return the count.
	ex: word inside out.txt is "hello", and the count is 5.
ex: cat out.txt | wc -l
	-count how many lines inside in.txt , and return the count