#include <stdio.h>

unsigned int add(unsigned int, unsigned int);
unsigned int sub(unsigned int, unsigned int);
unsigned int mul(unsigned int, unsigned int);
unsigned int lt(unsigned int, unsigned int);
unsigned int gt(unsigned int, unsigned int);
unsigned int getByte(unsigned int, unsigned int);

int main(int argc, char **argv)
{

 unsigned int i, j;

 printf("Enter an integer ");

 scanf("%u", &i);
 printf("\nEnter another integer ");
 scanf("%u", &j);
 printf("\ni + j = %u\n", add(i,j));
 printf("\ni - j = %u\n", sub(i,j));
 printf("\ni * j = %u\n", mul(i,j));
 printf("i > j = %s\n", (gt(i,j) ? "TRUE" : "FALSE"));
 printf("i < j = %s\n", (lt(i,j) ? "TRUE" : "FALSE"));
 printf("jth byte of i = 0x%02x\n", getByte(i,j));

 return 0;
}

unsigned int add(unsigned int i, unsigned int j)
{
/* can be done in a total of 7 lines, including one to declare an unsigned int, */
/* two for a while loop, and one for the return */
   while( j != 0){
        unsigned int carry = i & j;
        i = i ^ j;
        j = carry << 1;

    }
 return i ;

}

unsigned int sub(unsigned int i, unsigned int j)
{
/* Similar 7 lines, although there is a shorter way */
    return add(i, (~j +1));
}

unsigned int mul(unsigned int i, unsigned int j)
{
/* can be done in a total of 8 lines including one to declare unsigned ints */
/* two for a for loop, and one for the return */

    unsigned int multi = 0;
    while (j != 0){
        if (j & 1){
                multi = add(multi, i);
        }
                i <<= 1;
                j >>= 1;
        }
        return multi;
}

/* returns non-zero if i is less than j or zero otherwise */
unsigned int lt(unsigned int i, unsigned int j){
    if ( i < j){
        return 1;
    }
    else {
        return 0;
    }
}

/* returns non-zero if i is greater than j or zero otherwise */
unsigned int gt(unsigned int i, unsigned int j)
{
    if(i > j ){
        return 1;
    }
    else{
        return 0;
    }


}

/* returns the jth byte of i */
unsigned int getByte(unsigned int i, unsigned int j)
{
   unsigned int shift = 255;
	for (j =0 ; j != 1 && j != 0; j = sub(j, 1)) {
		shift = shift << 8;
	}
	return i & shift;

}
