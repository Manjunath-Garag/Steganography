#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void decrypt(char *argv[])
{
	FILE *fp1,*fp2;
	int i,j;
	unsigned int file_size=0,temp1=0;

	if(strcmp(argv[2],"-o")==0)
	{
		fp1=fopen(argv[3],"r"); //beauty.bmp
		fseek(fp1,54L,SEEK_SET);
		//print("%ld",ftell(fp1));  //54
		unsigned char ch;

		for(i=54;i<86;i++)
		{
			fread(&ch,sizeof(char),1,fp1);
			//	printf("%d",ch);
		}
		//		printf("\nfp position:%ld\n",ftell(fp1));

		//De-cryption of File size. To get the size of the file.
		fseek(fp1,54L,SEEK_SET);
		printf("\nfp1 Image Pointer position:%ld\n",ftell(fp1));

		for(i=0;i<=31;i++)
		{
			fread(&ch,sizeof(char),1,fp1); //Accepted as a Char. !st accepts the 54th Byte.
			/* unsigned int*/
			temp1=((int)ch & 1) ? 1: 0;
			// type-casted as an Integer. LSB is extracted and ANDed with 1 will give you the Required LSB

			/* Unsigned int file_size=0; */
			file_size =(temp1 | file_size);
			//	printf("%d",temp1);

			if(i!=31)// Last bit need not be Shifted. for the last time it Does'nt Shift
				file_size <<= 1;

		}
		printf("\nThe file size is:%d\n",file_size);
		//		printf("fp position after file size:%ld\n",ftell(fp1));
	}//-o
	else
	{
		printf("-o Args Not matched\n");
		exit(1);
	}

	unsigned char str1[5],str2[5],ch3,ch4,temp2=0;
	printf("Enter a the Password\n");
	scanf("%s",str2);  //str2 is storing the Password.

	for(i=0;i<3;i++)
	{
		for(j=0;j<=7;j++) // this loop is for calculating bit by bit.
		{
			fread(&ch3,sizeof(char),1,fp1);
			ch4=(ch3 & 1)?1:0;//LSb will be stroed in ch4
			/*Unsigned char*/	temp2 |= ch4; 
			if(j!=7)
				temp2 <<= 1;

		}
		str1[i]=(char)temp2;//each bit is stored in str1[i]. DECRYPTED value is stored here.
		temp2=0;	
	}
	//	printf("%s\n",str1);
	//	printf("outside i loop,after Magic string fp position:%ld\n",ftell(fp1));


	if(strcmp(str1,str2)==0)  // To check the Magic string is Matched.
	{
		printf("Magic Strings are Matched\n\n");
		if(strcmp(argv[4],"-t")==0)	// To check the it the Cmd line agr matches. 
		{
			fp2=fopen(argv[5],"w");//secret.txt
			for(i=0;i<file_size;i++)// here we are trying to Extract the TEXT from beauty.bmp Byte-By-Byte.Ex:hello 1st::'h'
			{
				for(j=0;j<=7;j++) // Byte-by-Byte and Extracting the LSB from that Byte. h's 8 BITS are Extracted.
				{
					fread(&ch3,sizeof(char),1,fp1);
					ch4=(ch3 & 1)?1:0;//fp1's LSB.
					temp2 |= ch4;//temp2 will be 0.
					if(j!=7)	//NOt Left shifted For the last time, since it is att he End Position.
						temp2 <<=1;
				}
				fwrite(&temp2,sizeof(char),1,fp2);
				putc((char)temp2,stdout);
				temp2=0;
			}
		}//-t
		else
		{
			printf("Is'nt -t, Some mis-match\n");
			exit(1);
		}
	}
	else
	{
		printf("Magic String NOt Matched\n");
	}
	//printf("fp position after copy:%ld\n",ftell(fp1));

}//decrypt.
