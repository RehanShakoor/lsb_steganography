/*
Name  : Rehan Shakoor
Batch : ECEP (24004) (E)

C project 1 (LSB steganography) submission

Submission date : 07/07/2024
*/

//This is the file from which all functions are called
//Here execution starts from main()

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//header to provide typedef of fixed size data types
//which are independent of compiler and hardware used on
#include <stdint.h>

//below info struct will be used to store all relevent information
//magic string will tell, whether a particular .bmp file contains encoded data or not
struct info_struct
{
	uint8_t *output_bmp_file_name;  //stores output bmp file name
	uint8_t *magic_string;          //stores magic string
	uint8_t *input_bmp_file_name;   //stores input .bmp file name
	uint8_t *input_txt_file_name;   //stores input .txt file name (contains secret msg)
	uint8_t *output_txt_file_name;  //stores output .txt file name (stores decoded msg)
} info;

//include header to use functions related to encoding and decoding
#include "encode_func_header.h"
#include "decode_func_header.h"
#include "common_func_header.h"

//main function execution starts from here
int main(uint8_t argc, uint8_t **argv)
{
	//if no CLA is passed then print error and help menu
	if(argc == 1)
	{
		printf("Error : No argument is passed\n");
		help_menu();
	}
	//else proceed with checks and operations
	else
	{
		//if -e option is provided as CLA (encoding)
		if(strcmp(argv[1], "-e") == 0)
		{
			//below function will check CLA for encode functions
			check_encode_CLA(argc, argv);	
		}
		//if -d option is provided as CLA (decoding)
		else if(strcmp(argv[1], "-d") == 0)
		{
			//below function will check CLA for decode functions
			check_decode_CLA(argc, argv);	
		}
		//else print error and help menu
		else
		{
			printf("Error : %s argument is INVALID\n",argv[1]);
			help_menu();
		}
	}
	return 0;
}
