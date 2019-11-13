#include <stdio.h>


int all_letters(char *s);
int num_in_range(char *s1, char b, char t);
int diff(char *s1, char *s2);
void shorten(char *s, int new_len);
int len_diff(char *s1, char *s2);


int main()
{


    char str[100]= "first";

    char str2[100]= "first second" ;
    int len = 5;

    printf("All Letters -> %d",all_letters(str));

    printf("\nIn Range -> %d",num_in_range(str2,'a','d'));

    printf("\nDiff = %d",diff(str,str2));

   shorten(str2, len);

   printf("\nAfter Shorten string length by %d, new string:'%s' ", len, str2);

   // printf("\nstr1 - str2 = %d", len_diff(str, str2));


    return 0;
}

//Returns 1 if all of the characters in the string are either upper- or lower-case letters of the alphabet. It returns 0 otherwise
int all_letters(char *s){

    while((*s) != '\0'){

        if( ((*s) >= 'A' & (*s) <= 'Z') | ((*s) >= 'a' & (*s) <= 'z' )){

            return 1;
        }
        else{
            return 0;
    }
        }
        /*
        if(isupper(*s) | islower(*s)){
            return 1;
        }
        else {
            return 0;
        }
    }
    */
}

//returns the number of characters c in s1 such that b<=c<=t
int num_in_range(char *s1, char b, char t){  // b<=c<=t
    int count=0;

    while ((*s1) != '\0'){

        if(b <= (*s1) & (*s1) <= t){

        count++;
                }
        s1++;

        }
     return count;
}

//returns the number of positions in which s1 and s2 differ, i.e., it returns the number of changes that would need to be made in order to transform s1 into s2, where a change could be a character substitution, an insertion, or a deletion.
int diff(char *s1, char *s2){

    int count = 0;
    while (*(s1++) != '\0' && *(s2++) != '\0' ){

            if(*s1 != *s2)
            count++;
        }

    while (*(s1++) != '\0') // if s1 is larger
            count++;

    while (*(s2++) != '\0')  // if s2 is larger
            count++;

    return count;
}

//Shortens the string s to new_len. If the original length of s is less than or equal to new_len, s is unchanged
void shorten(char *s, int new_len){

   char *a = s;
    int len;

    while ( (*a++) != '\0')
        len++;
        printf("\nOriginal string:'%s' and length:%d ",s, len );
        if( len > new_len)
            s[new_len] = '\0';

}
// Returns the length of s1 - the length of s2
int len_diff(char *s1, char *s2){
    int count = 0;
    int count1 = 0;
    for( int i = 0 ; i < strlen(s1) ; i++){
        count++;

    }
    printf("\nlenn: %d",count);

     for( int i = 0 ; i < strlen(s2) ; i++){
        count1++;

    }
    printf("\nlenn: %d",count1);


    return count1 - count;


}
