#include <stdio.h>

int main(int argc, char *argv[]){


	for(int i = 0; i < argc; i++){
		printf("argument: %s",argv[i]);
	}
	puts("");
	return 0;
}