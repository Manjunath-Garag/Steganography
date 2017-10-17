#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void encrypt();

void encrypt(char *argv[])	//pass by reference. accepting the Base address.
{
	FILE *fp, *fp1,*fp2;
	char ch1;
	int file_size=0,img_size, width,height,i,j;


	//File size.
	if(strcmp(argv[2],"-s")==0)
	{	
		printf("%s\n",argv[2]);	//De-refrenced.
		fp=fopen(argv[3],"r"); //text_file.txt open in read mode.
		if(fp == NULL)
		{
			printf("Text FIle is'nt Avail\n");
		}

		fseek(fp,0,SEEK_END); // to check the Size of the file. Cursor moves to the end of the text_file.txt File. 
		file_size=ftell(fp);
		unsigned int size_of_file = file_size;
	}//-s
	else
		printf("It's not -s, Some Mis-match\n");

	unsigned int size_of_file = file_size;
	printf("Size of the file is:%d Bytes\n\n",size_of_file);


	//Image Size.
	if(strcmp(argv[4],"-i")==0)
	{
		fp1=fopen(argv[5],"r"); //beautiful.bmp
		if(fp1 == NULL)
		{
			printf("Image FIle is'nt Avail\n");
		}

		//Width of the Image.	
		fseek(fp1,18L,SEEK_CUR);// The pointer is at 18'th position of the Header file which stores width Info. 
		fread(&width,sizeof(int),1,fp1); //Sizeof(int) has to be accessed i.e 4 bytes is accessed and stored onto width. 
		//	printf("The Width is %d\n",width);

		rewind(fp1);

		//Height of the Image.
		fseek(fp1,22L,SEEK_CUR);
		fread(&height,sizeof(int),1,fp1);
		//	printf("The Height is %d\n",height);

		img_size= width * height*3;
		printf("Size of the img file:%d Bytes\n\n",img_size);
	}//-i
	else
		printf("Its not -i, some mismatch\n");


	rewind(fp1);// we have to rewind because we are trying to read that file. and copy the 1st 54-Bytes.
	if((img_size * 8) > file_size)	
	{
		if(strcmp(argv[6],"-o")==0)
		{
			fp2=fopen(argv[7],"w+");//Copying the 54 Bytes to a new file. beauty.bmp
			char temp;
			for(i=0;i<=53;i++)// we are copying the Hearder file here.
			{
				fread(&temp,sizeof(char),1,fp1);// Copies 8 Bits at a time Beacuse it is of Char type, from fp1 to Temp.
				fwrite(&temp,sizeof(char),1,fp2);// copies the data from Temp to fp2 i.e New bmp image.
			}

			for(i=31;i>=0;i--) // 32 Bytes for the File Size. 32 bytes.
			{
				ch1=fgetc(fp1); //Extracting the RGB value.
				//fputs(&ch1,stdout);
				//This Pointing To 54th Byte. of the Original Image, Extracts the 54th Bit, and Stores the value of 54th bit in ch.  
				//below we are tyring to put MSB of text file to LSB of Image file.
				ch1= ( (size_of_file >> i) & 1 ) ? (ch1 | 1 ) : (ch1 & 0xfe);

				fputc(ch1, fp2);
			}
			printf("Text file size is Encrypted Successfully\n\n");
		}//-o
		else
			printf("It's not -o, some Mis-match\n");

		//Magic String.
		char magic_str[]= "#*";
		for(i=0;i<3;i++) // this has to run for As many chars we give. 3 Chars.
		{	
			ch1=fgetc(fp1);		//now pointer is at 86th Byte.
			for(j=7;j>=0;j--)	// 8 bits or 1 Byte each.
			{
				//ch1=fgetc(fp1); this has to written Before the J loop other-wise each time it fetches the same value.
				ch1=( (magic_str[i] >> j )&1 ) ?(ch1 | 1) : (ch1 & 0xfe);
				fputc(ch1,fp2);
				//printf("%c",ch1);
			}
		}
		printf("Magic String is Encrypted Successfully\n\n");
		//	printf("The position of the fp2 Copy File is after copying the Magic String Bytes.(24) %ld\n",ftell(fp2));//fp2 is 110
		//	printf("The position of the fp Text File is %ld\n",ftell(fp)); //Pos is 15.


		rewind(fp);
		//	printf("The position of the fp Text File is %ld\n",ftell(fp)); // Rewinded Pos is 0.

		// TO read the string from text.txt file
		for(i=0;i<file_size;i++)	// To add hello world.
		{
			unsigned char ch=fgetc(fp); // text.txt file.
			for(j=7;j>=0;j--)
			{
				//unsigned char ch=fgetc(fp); // text.txt file.This should be written Before J loop, otherwies aech time it will be Incremented or takes the same value each time.
				ch1=fgetc(fp1);// then the postion of Image. 111 RGB value

				unsigned char ch2=( (ch >> j) & 1) ?(ch1 | 1) : (ch1 & 0xfe);
				//ch1=( (ch >> j) & 1) ?(ch1 | 1) : (ch1 & 0xfe);
				fputc(ch2,fp2);
			}
		}
		printf("Text file Message is Encrypted to Copoied Image Pointer\n\n");
		//	printf("The position of the fp2 Image Copy File is %ld\n",ftell(fp1)); // Pos is 209
		//	printf("The position of the fp2 Copy File is %ld\n",ftell(fp2)); //Pos is 230

		//copying thr remaining Bits or area.
		char ch3;
		for(i=ftell(fp1);i<(img_size);i++) //i=file pointer of Image_file. 
		{
			ch3=fgetc(fp1);
			fputc(ch3,fp2);
		}
		//	printf("The position of the fp2 Image Copy File is %ld\n",ftell(fp1));
		//	printf("The position of the fp2 Copy File is %ld\n",ftell(fp2));

	}//checking if IMG size is Greater than File SIze. 

	fcloseall();

}//encrypt

