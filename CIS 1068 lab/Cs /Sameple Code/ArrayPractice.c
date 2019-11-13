#include <stdio.h>

#define LEN_A 10

/* function declarations */
void print_array(int[], int);
void reverse(int[], int);
double average(int[], int);
void insert(int[], int, int, int);
int less_than_average(int[], int);
void rotate_right(int[], int, int);
void array_copy(int[], int[], int);
void initialize(int[], int, int);
void evens_on_left(int[], int);
int find(int A[], int len, int x);
int find_last(int A[], int len, int x);
int contains_dup(int A[], int len);
int secondLargest(int A[], int length);

int main(int argc, char **argv)
{
   int A[] = {9, 505, 321, 117, 27, 64, 13, 9, 12, 208};

   int B[LEN_A+1];
   int newValue=505;

   printf("A[] is:");
   print_array(A, LEN_A);

   initialize(B, LEN_A+1, 0);
   printf("B[] is:");
   print_array(B, LEN_A+1);

   array_copy(B, A, LEN_A);
   printf("but now it's\n");
   print_array(B, LEN_A+1);

   insert(B, newValue, 0, LEN_A);
   printf("and after inserting %d at the beginning it's\n", newValue);
   print_array(B, LEN_A+1);

   printf("The average of A[] is %f\n", average(A,LEN_A));
   printf("There are %d items in A[] less than this.\n", less_than_average(A,LEN_A));

  printf("Second Largest: %d\n",secondLargest(A, LEN_A));

   printf("\nReverse: ");
   reverse(A, LEN_A);
   print_array(A, LEN_A);

   printf("\nRotate Right: ");
   rotate_right(A, LEN_A, 3);
   print_array(A, LEN_A);


   printf("\nEven on left : ");
   evens_on_left(A, LEN_A);
   print_array(A, LEN_A);

   printf("The index of the first occurrence of %d is %d.\n", 9, find(A, LEN_A, 9));
   printf("The index of the last occurrence of %d is %d.\n", 9, find_last(A, LEN_A, 9));

   if (contains_dup(A, LEN_A))
       printf("A[] contains a dup\n");
   else
       printf("A[] doesn't contain a dup\n");

   if (contains_dup(B, LEN_A+1))
       printf("B[] contains a dup\n");
   else
       printf("B[] doesn't contain a dup\n");

   return 0;
}

/* functions implementation */

/* prints A[] inside parentheses with each value separated
   by commas to stdout (which will be the screen). */
void print_array(int A[], int length) {
   int i;
   printf("(");
   for(i = 0; i < length; i++){
       if(i != (length -1)){
           printf("%d, ", A[i]);
       } else{
           printf("%d)\n", A[i]);
       }
   }
}

/* returns the average of the items in A[] or 0.0 if A is empty */
double average(int A[], int length) {
/* for now return 1.0 as a placeholder. We do this just so that the program
     compiles without warning until we've finished */
   int i;
   double total;
   double average;
   if(length == 0){
       return 0.0;
   }
   for(i = 0; i < length; i++){
       total += A[i];
   }
   average = (total / length);
   return average;
}

/* returns the number of items in A that are less than the
   average of the items in A */
int less_than_average(int A[], int length) {
/* for now return 0 as a placeholder. We do this just so that the program
     compiles without warning until we've finished */
   double averageOfArray = average(A, length);
   int i;
   int belowAverage = 0;


   for(i = 0; i < length; i++){
       if(A[i] < averageOfArray){
           belowAverage++;
       }
   }
   return belowAverage;
}

/* Reverses the values in A[]. For example, if before the function,
   A[] contained [1,2,3,4,5], after the function, A[] contains
   [5,4,3,2,1] */
void reverse(int A[], int length) {
  for(int i = 0; i < length / 2; i++){
        int temp = A[i];
         A[i] = A[length - i - 1];
        A[length - i - 1] = temp;
    }
}

/* returns the second largest item in A[], or -1 if len < 2 */
int secondLargest(int A[], int length) {
/* for now return 0 as a placeholder. We do this just so that the program
     compiles without warning until we've finished */
   int i;
   double max = A[0];
   int j;
   int secondBiggest = A[0] - 1;

   if(length < 2){
                return -1;
        }

   for(i = 1; i < length; i++){
       if(A[i] > max){
           max = A[i];
           j = i;
       }
   }

   for(i = 0; i < length; i++){
       if(A[i] != max){
           if(A[i] > secondBiggest){
               secondBiggest = A[i];
           }
       }
   }
   return secondBiggest;
}

/* rotates the values in the array numPlaces to the right */
/* for example, if A[] is initially 10,20,30,40,50, and numPlaces=2 */
/* after the function finishes, A[] would be 40,50,10,20,30 */
void rotate_right(int A[], int length, int numPlaces) {
   int newArray[length];
   int i;

   array_copy(newArray, A, length);

   for (i = numPlaces; i < (length + numPlaces); i++) {
           A[i % length] = newArray[i - numPlaces];
   }
}

/* inserts the value n in A[] at the given index and shifts
   all other values up one position. A[] contains length items.

   It is up to the caller to make sure that there is enough space
   in A[] to accomodate the new item, and that the index is valid. */
void insert(int A[], int n, int index, int length) {
   int i;
   int newArray[length];

   array_copy(newArray, A, length);

   for(i = index; i < length; i++){
       A[i + 1] = newArray[i];
   }
   A[index] = n;
}

/* copies the first numItems from S to D. */
void array_copy(int D[], int S[], int numItems) {
   int i;

   for(i = 0; i < numItems; i++){
       D[i] = S[i];
   }
}


/* sets the first len elements of A to initialValue */
void initialize(int A[], int len, int initialValue) {
   int i;

   for(i = 0; i < len; i++){
       A[i] = initialValue;
   }
}

/* shuffles the values in A so that the even values
   are all to the left of the odd values */
void evens_on_left(int A[], int len) {
     int left = 0;
    int right = len - 1;

    while(left < right)
    {
        // Increment left index till the number is even
        // num%2 == 0, condition to check even number
        while(A[left]%2 == 0 && left < right)
        {
            left++;
        }

        // Decrement right index till the number is odd
        while(A[right]%2 == 1 && left < right)
        {
            right--;
        }

        // Time to swap
        if(left < right)
        {
            int temp = A[left];
            A[left] = A[right];
            A[right] = temp;
            left++;
            right--;
        }
    }
}


/* returns the index of the first occurrence of
* x in A[] or -1 if x doesn't exist in A[] */
int find(int A[], int len, int x) {
   int i;

   for(i = 0; i < len; i++){
       if(A[i] == x){
           return i;
       }
   }
   return -1;
}

/* returns the index of the last occurrence of
* x in A[] or -1 if x doesn't exist in A[] */
int find_last(int A[], int len, int x) {
   int i;
   int j;
   int none = 0;

   for(i = 0; i < len; i++){
       if(A[i] == x){
           j = i;
       }else{
           none++;
       }
   }
   if(none == len){
       return -1;
   }
   return j;
}

/* returns 1 if there is at least one element in A[]
   that is contained elsewhere in A[], or 0 if there isn't */
int contains_dup(int A[], int len) {
   int i;
   int j;
   int m;

   for(i = 0; i < len; i++){
       for(j = 0; j < len; j++){
           if(j != i){
               if(A[i] == A[j]){
                   return 1;
               }
           }
       }
   }
   return 0;
}
