/*
Name  : Rehan Shakoor
Batch : ECEP (24004) (E)

C project 1 (LSB steganography) submission

Submission date : 07/07/2024
*/

//This file contains definitions of all the function related to
//encoding part of the project

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//header to provide typedef of fixed size data types
//which are independent of compiler and hardware used on
#include <stdint.h>

//include header to use functions related to encoding and decoding
#include "encode_func_header.h"
#include "common_func_header.h"

//include below header file to us the struct to store relevent information
#include "struct_header.h"

//function definition to check CLA for encoding
void check_encode_CLA(uint8_t argc, uint8_t **argv)
{
	//print progress mesage
	printf("Info    : Checking and reading CLA for encoding\n");

	//for -e option, only 4 to 7 arguments can be passed
	if(argc >= 4 && argc <= 7)
	{
		//check extension of input file (it should be .bmp)
		if(check_extension(argv[2], ".bmp"))
		{
			//store .bmp input file name in info struct
			info.input_bmp_file_name = argv[2];

			//print progress message
			printf("Success : Extension of image file matches with .bmp\n");

			//secret message should be in .txt file
			if(check_extension(argv[3], ".txt"))
			{
				//print progress message
				printf("Success : Extension of text file matches with .txt\n");

				//store .txt input file name in info struct
				info.input_txt_file_name = argv[3];

				//if no o/p file name and magic string
				//is not provided (only four arguments)
				if(argc == 4)
				{
					//set default .bmp output file name in info struct
					info.output_bmp_file_name = "steged_img.bmp";

					//set default magic string in info struct
					info.magic_string = "#*";

					//print progress message
					printf("Info    : Using default output file name : %s\n",info.output_bmp_file_name);
					printf("Info    : Using default magic string\n");
				}
				//five or greater arguments are passed
				else if(argc >= 5)
				{
					//if next argument is "-p" then the following
					//argument will be magic string
					if(strcmp(argv[4], "-p") == 0)
					{
						//magic string is provided by user
						if(argc >= 6)
						{
							//set user provied magic string in info struct
							info.magic_string = argv[5];

							//print progress message
							printf("Info    : Using user-provided magic string\n");
						}
						//else print error msg and help menu and exit
						else
						{
							printf("Error   : Nothing provided after -p option\n");
							help_menu();
						}

						//output file name is provided after magic string
						if(argc == 7)
						{
							//set user provided .bmp output file name in info struct
							info.output_bmp_file_name = argv[6];

							//print progress message
							printf("Info    : Using user-provided output file name : %s\n",info.output_bmp_file_name);
						}
						//else set default output file name
						else
						{
							//set default .bmp output file name in info struct
							info.output_bmp_file_name = "steged_img.bmp";

							//print progress message
							printf("Info    : Using default output file name : %s\n",info.output_bmp_file_name);
						}
					}
					//else that argument is output file name
					else
					{
						//set user provided .bmp output file name in info struct
						info.output_bmp_file_name = argv[4];

						//print progress message
						printf("Info    : Using user-provied output file name : %s\n",info.output_bmp_file_name);

						//check next argument is there or not
						if(argc >= 6)
						{
							//if next argument is -p
							if(strcmp(argv[5], "-p") == 0)
							{
								//next argument will be magic string
								if(argc == 7)
								{
									//set user provied magic string in info struct
									info.magic_string = argv[6];

									//print progress message
									printf("Info    : Using user-provided magic string\n");
								}
								//else print error msg, display help menu and exit
								else
								{
									printf("Error   : Nothing provided after -p option\n");
									help_menu();
								}
							}
							//print help menu and exit
							else
							{
								printf("Error   : Provide -p then magic string\n");
								help_menu();
							}
						}
						//else set default magic string
						else
						{
							//set default magic string in info struct
							info.magic_string = "#*";

							//print progress message
							printf("Info    : Using default magic string\n");
						}
					}
				}
				//now we can call encode function to do encode check and actual encoding
				encode_func();
			}
			//else print error message and help menu
			else
			{
				printf("Error   : %s is not a .txt file\n",argv[3]);
				help_menu();
			}
		}
		//else print error message and help menu
		else
		{
			printf("Error   : %s file is not a .bmp file\n",argv[2]);
			help_menu();
		}

	}
	//else print error message and help menu
	else
	{
		printf("Error   : Invalid number of arguments passed\n");
		help_menu();
	}
}


//function declaration to check encode conditions and do actual encoding
void encode_func(void)
{
	//print progress message
	printf("\nInfo    : Checking conditions required for encoding\n");

	//we open text file in r mode, because we only have to read it
	FILE *fp_txt = fopen(info.input_txt_file_name, "r");

	//print progress message
	printf("Info    : Opening secret message file for encoding\n");

	//print error if program is not able to open .txt file and return from function
	if(fp_txt == NULL)
	{
		printf("Error   : Can't open %s\n",info.input_txt_file_name);
		return;
	}
	//else proceed with the operations and checks

	//read .txt file content and size and store it in heap
	uint8_t *size_data_txt = file_size_and_data(info.input_txt_file_name);

	//extract size of .txt file from above pointer
	//size is in first 2 bytes, remaining is content
	uint16_t txt_size = *((uint16_t *) size_data_txt);

	//if txt_size if zero, then print error message
	if(txt_size == 0)
	{
		printf("Error   : Given file %s is empty\n",info.input_txt_file_name);
	}
	//else continue with operations
	else
	{
		//print progress message
		printf("Success : %s is not empty\n",info.input_txt_file_name);

		//we open bmp file in r+ mode, because have to read and write to it
		FILE *fp_bmp = fopen(info.input_bmp_file_name, "r+");

		//print progress message
		printf("Info    : Opening input bmp file to store encoded data\n");

		//print error if program is not able to open .bmp file
		if(fp_bmp == NULL)
		{
			printf("Error   : Can't open %s\n",info.input_bmp_file_name);
		}
		//proceed with .bmp file operations
		else
		{
			//check correctness of signature of .bmp file
			//read first 2 bytes from the start of .bmp header
			uint8_t *file_data = read_file(fp_bmp, 0, 2);

			//if signature matches then proceed
			if(strcmp(file_data, "BM") == 0)
			{
				//print progress message
				printf("Success : Signature of given input bmp file is correct\n");

				//now we can free heap memory used by last use of file_data
				free(file_data);

				//read bitmap size from .bmp info header
				//read 4 bytes from the offset of 34
				file_data = read_file(fp_bmp, 34, 4);

				//store bitmap size (bytes) in below variable
				uint32_t bitmap_size = *((uint32_t *) file_data);

				//now we can free heap memory used by last use of file_data
				free(file_data);

				//calculate size of magic string
				uint8_t magic_string_len = strlen(info.magic_string);

				//calculate total size required (in bits)
				//first we have to store magic string, then 2 bytes for text size
				//then we have to encode secret message. Each bits of data is
				//store in bitmap lsb (0th bit) therefore x8 is there
				uint32_t total_size_required = (magic_string_len + 2 + txt_size) * 8;

				//if bitmap size is larger then proceed
				if(bitmap_size >= total_size_required)
				{
					//print progress message
					printf("Success : Input bmp file have enough size to store encoded message\n");

					//size_data_txt contains size and content of secret msg file as a string
					//we have to add magic string to it in the front, data_buffer will contain
					//new updated data frame.

					//+1 is used to add NULL at last, this will help us to do string operations
					//like strcpy.
					uint8_t *data_buffer = malloc(total_size_required + 1);

					//first copy magic string into data buffer start
					strcpy(data_buffer, info.magic_string);

					//then concatenate remaining data next to it
					//we cant use strcat here, because initial byte of size stored in size_data_txt 
					//can be zero, which would be treated as NULL by strcat, therefore we will use
					//own buffer_copy function to copy the data.
					data_buffer = buffer_copy(data_buffer, size_data_txt, magic_string_len, txt_size + 2);

					//now we can free heap memory used by size_data_txt
					free(size_data_txt);

					//print progress message
					printf("Success : All message (magic string + size and data of sec msg) are combined\n");

					//now we can call function encode_msg to do actual encoding
					//this will return pointer to encoded bitmap data in heap
					uint8_t *encode_bitmap = encode_msg(fp_bmp, data_buffer, total_size_required, bitmap_size);

					//now we have to store header, header info and encoded bitmap
					//data into newly created output file
					create_output_file(fp_bmp, encode_bitmap, bitmap_size);

					//now we can free dynamic memory used by encode_bitmap
					free(encode_bitmap);

				}	
				//else print error and exit from program
				else
				{
					printf("Error   : Insufficient memory to store msg\n");
					printf("Error   : File size info is given below\n");
					printf("Error   : Bitmap size (in bytes) of %s is %u\n",info.input_bmp_file_name, bitmap_size);
					printf("Error   : Text file %s size (in bits) is %u\n",info.input_txt_file_name, txt_size * 8);
					printf("Error   : Magic string %s size (in bits) if %u\n", info.magic_string, magic_string_len * 8);
					printf("Error   : 2 bytes (16 bits) are required to encode size of secret msg\n");
					printf("Error   : Total size (in bits) = %u\n",total_size_required);

					//exit 0, to free all the resources
					exit(0);
				}

			}
			//else print error and exit from program
			else
			{
				printf("Error   : Signature of %s is INVALID\n",info.input_bmp_file_name);

				//exit 0, to free all the resources
				exit(0);
			}

		}
	}
}

//function definition to do actual encoding
//this function will store each bits of sec_msg into
//lsb (0th bit) of each byte of copied .bmp file and return pointer
//to updated value, original .bmp file is not changed in this function
uint8_t* encode_msg(FILE *fp_bmp, uint8_t *sec_msg, uint16_t txt_size, uint32_t bitmap_size)
{
	//print porgress message
	printf("\nInfo    : Encoding process starts");

	//read bitmap size, amount of byte from bitmap part of .bmp file
	//bitmap area starts from 54 offset
	uint8_t *bitmap_data = read_file(fp_bmp, 54, bitmap_size);

	//print porgress message
	printf("\nInfo    : Writing encoded data in a buffer\n");

	//iterate over each characters of secret message and bitmap area
	for(uint16_t i = 0; i <= (txt_size - 1); i++)
	{
		//convert each character into 8-bits binary form
		//and store each bit intp lsb(0th bit) of each byte of .bmp file
		for(uint8_t j = 0; j <= (8 - 1); j++)
		{
			//read one byte at (i*8 + j)th offset from bitmap and store it in temp
			uint8_t temp = *(bitmap_data + i * 8 + j);

			//get jth bit of secret message's ith character
			uint8_t msg_bit = get_bits(*(sec_msg + i), j);

			//update lsb (bit) of temp with secret message bit
			temp = update_lsb(temp, msg_bit);

			//store updated temp value back to heap
			*(bitmap_data + i * 8 + j) = temp;
		}
	}

	//now we can free heap memory used by sec_msg
	free(sec_msg);

	//print porgress message
	printf("Success : Encoding process completed\n");

	//return pointer to bitmap data
	return bitmap_data;
}

//function definition to create output file and store all data into it
//data included header, header info and encoded bitmap values
void create_output_file(FILE *fp_bmp, uint8_t *encode_bitmap, uint32_t bitmap_size)
{
	//print progress message
	printf("\nInfo    : Creating/opening output image file\n");

	//create output file, we open that file in write mode, 
	//as we only have to write to it
	FILE *fp_output = fopen(info.output_bmp_file_name, "w");

	//print error if program cant create output file and exit
	if(fp_output == NULL)
	{
		printf("Error   : Can't create %s output file\n",info.output_bmp_file_name);

		//exit 0, to terminate program and free all the resources
		exit(0);
	}
	//else proceed with the operation
	else
	{
		//print progress message
		printf("Info    : Copying encoded data and header info\n");

		//first read header and header info from original .bmp file
		//header starts at offset 0, and it is of 54 bytes
		uint8_t *org_bmp_header = read_file(fp_bmp, 0, 54);

		//write header info (54 bytes) into output file at offset 0
		write_file(org_bmp_header, 54, fp_output, 0);

		//write encoded bitmap data to the output file at offset 54
		write_file(encode_bitmap, bitmap_size, fp_output, 54);

		//print progress message
		printf("Success : Output encoded image file created\n");
	}
}
