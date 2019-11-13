#include<stdio.h>
#include <sys/types.h>
int main(){

   /* printf("iam parent\n");
    int i = fork();
    if(i == 0){
        printf("iam child\n");
    }*/
    int result = 0;
    int i = 0;
    for (i =0 ; i < 2; i++){
        fork();
        result++;
        printf("result %d\n",result);
    }
    printf("result2 %d\n",result);

    return 0;

}

