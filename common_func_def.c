/*
Name  : Rehan Shakoor
Batch : ECEP (24004) (E)

C project 1 (LSB steganography) submission

Submission date : 07/07/2024
*/

//This file will contain functions defintions which 
//is used in both encoding and decoding part of project

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//header to provide typedef of fixed size data types
//which are independent of compiler and hardware used on
#include <stdint.h>

//function definition to print help menu and exit from program
void help_menu(void)
{
	printf("\nHelp menu for LSB steganography\n");
	printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [output file] [-p magic string]\n");
	printf("./lsb_steg: Encoding: ./lsb_steg -e <.bmp_file> <.text_file> [-p magic string] [output file]\n");
	printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp_file> [output file] [-p magic string]\n");
	printf("./lsb_steg: Decoding: ./lsb_steg -d <.bmp_file> [-p magic string] [output file]\n");

	//exit 0, to terminate program and free all the resources
	exit(0);
}

//function definition to check extension
//this function checks if the file_name extension matches with the
//extension provides as ext_name (passed as .bmp, .txt)
bool check_extension(char *file_name, char *ext_name)
{
	//calculate size of file_name string
	char file_size = strlen(file_name);
	//calculate size of ext_name string
	char ext_size = strlen(ext_name);

	//extract only extension part from file_name
	char *file_ext = file_name + file_size - ext_size;

	//return 1 if file extension matches with the given extension
	return strcmp(file_ext, ext_name) == 0;
}

//function definition to calculate size of given file in bytes
//here we are iterating over the whole file to calculate the size
//therefore it would be better to read the content and store it in
//heap for its use in future this will save memory and time
uint8_t * file_size_and_data(char *file_name)
{
	//open file in r mode, as we only have to read it
	FILE *fp = fopen(file_name, "r");

	//print error if program can't open the file, and exit 0
	if(fp == NULL)
	{
		printf("Error : Can't open %s file\n",file_name);

		//exit 0, to exit from program and free all the resources
		exit(0);
	}
	//else proceed to calculate its size
	else
	{
		//size is used to store file size
		uint16_t size = 0;
		//ch reads one byte from file
		int8_t ch = 0;

		//initially allocate memory in heap
		//first 2 bytes will we used to store size of file
		//calloc is used to initialize it to zero
		uint8_t *temp = calloc(1, sizeof(size));

		//iterate over file till EOF is reached
		while((ch = fgetc(fp)) != EOF)
		{
			//increment size by 1
			size++;

			//increase dynamic memory as size increases
			temp = realloc(temp, size + sizeof(size));
			//store ch read from file to heap memory
			*(temp + size + sizeof(size) - 1) = ch;
		}

		//add NULL at last to treat msg as a string
		//add one byte at last to store NULL character
		temp = realloc(temp, size + sizeof(size) + 1);
		*(temp + size + sizeof(size) + 1 - 1) = '\0';

		//store size in first 2 bytes of memory
		*((uint16_t *) temp) = size;

		//return pointer to heap memory
		return temp;
	}
}

//function definition to read from a file
//this function will read given bytes from given offset from the given file
//this function will store read value in dynamic memory and
//return pointer to the start address of that memory
uint8_t* read_file(FILE *fp, uint16_t offset, uint32_t bytes)
{
	//move function pointer to required offset
	fseek(fp, offset, SEEK_SET);

	//allocate given bytes of memory in heap
	//+1 is used to add NULL character to it
	char *temp = malloc(bytes + 1);

	//print error if temp is NULL and exit from program
	if(temp == NULL)
	{
		printf("Error : Can't allocate dynamic memory\n ");

		//exit 0, to free all the resources
		exit(0);
	}
	//else proceed with the operations
	else
	{
		//read given bytes from fp and store it in heap
		fread(temp, bytes, 1, fp);

		//add NULL at last position
		*(temp + bytes) = '\0'; 

		//return point to heap
		return temp;
	}
}

//function definition to copy two buffer, source and destination address
//of two buffer are given, buffer_size and offset for destination at 
//which data form source will be copied is required.
uint8_t* buffer_copy(uint8_t *des, uint8_t *src,  uint16_t offset, uint32_t buffer_size)
{
	//iterate over buffer_size
	for(uint32_t i = 0; i <= (buffer_size - 1); i++)
	{
		//copy characters from source and store it at destination
		//offset is considered while copying
		*(des + offset + i) = *(src + i);
	}

	//return address of destination buffer
	return des;
}

//function definition to write to a file, this function will 
//write given message to file pointer by fp at the given offset
void write_file(uint8_t *msg, uint32_t size, FILE *fp, uint32_t offset)
{
	//move function pointer to required offset
	fseek(fp, offset, SEEK_SET);

	//write given bytes to fp
	fwrite(msg, 1, size, fp);
}


//function definition to get nth bit of variable ch
uint8_t get_bits(uint8_t ch, uint8_t nth)
{
	//create mask and retrive bit value using bitwise AND and
	//shift operations
	return ((1 << nth) & ch) >> nth;
}

//function definition to update lsb (0th bit) of variable ch
//with the value of bit variable and return update temp value
uint8_t update_lsb(uint8_t ch, uint8_t bit)
{
	//first clear 0th bit of ch
	ch &= ~(1 << 0);

	//update 0th bit of ch with bit value
	return ch |= (bit << 0);
}

//Test function definition to print content of each bytes of buffer in hex form
//function takes argument as pointer to buffer and byte counts to be printed
void print_buffer(uint8_t *buff, uint32_t size)
{
	for(uint32_t i = 0; i <= (size - 1); i++)
	{
		printf("%#x",*(buff + i));
	}
	printf("\n");
}