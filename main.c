#include<stdio.h>
#include<stdlib.h>
#include"encryption.c"
#include"decryption.c"

int main(int argc, char *argv[]) //char *argv[] is a 2D array or an Array of char pointers.
{
	switch(argv[1][1]) // ./a.out -e, we are accessing "e" here.i.e 1st row 1st col.
	{
		case 'e':encrypt(argv); // calls encrypt() fumction which would be INCLUDED from line 3.
			 break;

		case 'd':decrypt(argv);
			 break;
		default : printf("Inavlid Input\n");

	}
	return 0;
}

/*
./a.out 	-e 	-s 	text_file.c 	-i 	beautiful.bmp 	-o 	beauty.bmp
R0		R1 	R2 	R3	   	R4 	R5	    	R6 	R7
		"-" is C0 of R1
		"e" is C1 of R1 
*/
