#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <crypt.h>
/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the code.This program uses 2 thread to crack an encrypted password.

  Compile with:
    cc -o 2thread 2thread.c -lcrypt -pthread

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./2thread > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/


int n_passwords = 4;

char *encrypted_passwords[] = {

"$6$KB$eOk5f/THD7xQr6vKniSca/DdqY.m.gCg6Z.4zNWqD7V4yjkDuwCm2v5raQsqMcDEg344dywrZiRLhY65j4.Wg0",

"$6$KB$8X.3iBYPJ2h9z6w4pyfSxAaDTzQ2d9EvBX3tFiumB/vRZe/dX3hCUZtKd8ODAC/rYJN6YR9VzCTSDouhizU5k0",

"$6$KB$4nK6AiN8cb9rFzgjpJ1AxhQaBm0KMwBrmqtALfde6qVJzCR1SqIfeJCCepWjStZHYnkXLf4KFuvpw/Psf2c/y0",

"$6$KB$ZqbtoRA1d3cqX9Jj5guigg/vIbZyfH5qPUQ6hMdPDKS0Wb81FcaLZl9x8NK9qKzKQHfu6fm1ZX8ht6nyfy9/u."
};

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}
//thread function 1  
void *kernel_function_1(void *salt_and_encrypted){

int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
	for(x='A'; x<='M'; x++){
	    for(y='A'; y<='Z'; y++){
	      for(z=0; z<=99; z++){
		printf("Thread1");
		sprintf(plain, "%c%c%02d",x, y, z);
		enc = (char *) crypt(plain, salt);
		count++;
		if(strcmp(salt_and_encrypted, enc) == 0){
		  printf("#%-8d%s %s\n", count, plain, enc);
		} else {
		  printf(" %-8d%s %s\n", count, plain, enc);
		}
	      }
	    }
	  }
  printf("%d solutions explored\n", count);

}
//thread function 2
void *kernel_function_2(void *salt_and_encrypted){

int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);
	for(x='N'; x<='Z'; x++){
	    for(y='A'; y<='Z'; y++){
	      for(z=0; z<=99; z++){
		printf("Thread2");
		sprintf(plain, "%c%c%02d", x, y, z);
		enc = (char *) crypt(plain, salt);
		count++;
		if(strcmp(salt_and_encrypted, enc) == 0){
		  printf("#%-8d%s %s\n", count, plain, enc);
		} else {
		  printf(" %-8d%s %s\n", count, plain, enc);
		}
	      }
	    }
	  }
  printf("%d solutions explored\n", count);

}
int time_difference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


int main(int argc, char *argv[]){
pthread_t t1, t2;
  int i;
struct timespec start, finish;
  long long int time_elapsed;   
  clock_gettime(CLOCK_MONOTONIC, &start);

  for(i=0;i<n_passwords;i<i++) {
  pthread_create(&t1, NULL, kernel_function_1, encrypted_passwords[i]);
  pthread_create(&t2, NULL, kernel_function_2, encrypted_passwords[i]);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);		

  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 


  return 0;
}
