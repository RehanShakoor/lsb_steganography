/*
Name  : Rehan Shakoor
Batch : ECEP (24004) (E)

C project 1 (LSB steganography) submission

Submission date : 07/07/2024
*/

//This file contains definitions of all the function related to
//decoding part of the project

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

//header to provide typedef of fixed size data types
//which are independent of compiler and hardware used on
#include <stdint.h>

//include header to use functions related to encoding and decoding
#include "decode_func_header.h"
#include "common_func_header.h"

//include below header file to use the struct to store relevent information
#include "struct_header.h"

//function definition to check CLA for decoding
void check_decode_CLA(uint8_t argc, uint8_t **argv)
{
    //print progress message
    printf("Info    : Checking and reading CLA for decoding\n");

	//for -d option, only 3 to 6 arguments can be passed
	if(argc >= 3 && argc <= 6)
	{
		//check extension of input file (it should be .bmp)
		if(check_extension(argv[2], ".bmp"))
		{
            //print progress message
            printf("Success : Extension of image file matched with .bmp\n");

			//store .bmp input file name in info struct
			info.input_bmp_file_name = argv[2];

			//if more arguments are provided then read them in proper sequence
			if(argc >= 4)
			{
				//if next argument is -p
				if(strcmp(argv[3], "-p") == 0)
				{
					//if more arguments are provided then read them in proper sequence
					if(argc >= 5)
					{
						//set user provied magic string in info struct
						info.magic_string = argv[4];

                        //print progress message
                        printf("Info    : Using user-provided magic string\n");

						//if more arguments are provided then read them in proper sequence
						if(argc == 6)
						{
							//set user provided output file name in info struct
							info.output_txt_file_name = argv[5];

                            //print progress message
                            printf("Info    : Using user-provided output file name : %s\n",info.output_txt_file_name);
						}
						//print error msg and help menu
						else
						{
							//set default output file name in info struct
							info.output_txt_file_name = "decoded_output.txt";

                            //print progress message
                            printf("Info    : Using default output file name : %s\n",info.output_txt_file_name);
						}
					}
					//else print error msg and help menu and exit
					else
					{
						printf("Error   : Nothing provided after -p option\n");
						help_menu();
					}
				}
				//else next argument is output file name
				else
				{
					//set user provided output file name in info struct
					info.output_txt_file_name = argv[3];

                    //print progress message
                    printf("Info    : Using user-provided output file name : %s\n",info.output_txt_file_name);
						
					//if more arguments are provided then read them in proper sequence
					if(argc >= 5)
					{
						//if next argument is -p
						if(strcmp(argv[4], "-p") == 0)
						{
							//if more arguments are provided then read them in proper sequence
							if(argc == 6)
							{
								//set user provided magic string in info struct
								info.magic_string = argv[5];

                                //print progress message
                                printf("Info    : Using user-provided magic string\n");
							}
							//print error msg and help menu
							else
							{
								printf("Error   : Nothing provided after -p option\n");
								help_menu();
							}
						}
						//else print error and help menu and exit
						else
						{
							printf("Error   : Provide -p then magic string\n");
							help_menu();
						}
					}
					//else set defult magic string
					else
					{
						//set default magic string in info struct
						info.magic_string = "#*";

                        //print progress message
                        printf("Info    : Using default magic string\n");
					}
				}
			}
			//else set default output file name and magic string
			else
			{
				info.magic_string = "#*";
				info.output_txt_file_name = "decoded_output.txt";

                //print progress message
                printf("Info    : Using default magic string\n");
                printf("Info    : Using user-provided output file name : %s\n",info.output_txt_file_name);	
			}

			//now we can call decode function
			decode_func();
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

//function definition to do decode checks and then actual decoding
void decode_func(void)
{
    //print progress message
    printf("\nInfo    : Checking conditions required for decoding\n");

    //we open bmp file in r mode, because we only have to read
    FILE *fp_bmp = fopen(info.input_bmp_file_name, "r");

    //print progress message
    printf("Info    : Opening input bmp file for decoding\n");

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
            //now we can free heap memory used by last use of file_data
			free(file_data);

            //print progress message
            printf("Success : Signature of given bmp file is correct\n");
            
            //print progress message
            printf("\nInfo    : Decoding process starts\n");

            //match magic string, first find length of magic string
            uint16_t magic_string_len = strlen(info.magic_string);

            //now read first n * 8 bytes from bitmap area of .bmp file
            //bitmap area is at the offset of 54 (to decode magic string)
            file_data = read_file(fp_bmp, 54, magic_string_len * 8);

            //below function will extract secret message encode in above data
            uint8_t* magic_str_decoded = decode_data(file_data, magic_string_len * 8);
            
            //now we can free heap memory used by last use of file_data
			free(file_data);

            //print progress message
            printf("Success : Magic string is decoded\n");

            //if magic string matches then proceed
            if(strcmp(magic_str_decoded, info.magic_string) == 0)
            {
                //now we can free heap memory used by magic_str_decoded
			    free(magic_str_decoded);

                //print progress message
                printf("Success : Decoded magic string matches\n");
            
                //now read size of encoded data in bitmap
                //this size is stored in 2 * 8 bytes just after magic string
                file_data = read_file(fp_bmp, 54 + magic_string_len * 8, 2 * 8);

                //below function will extract secret message encode in above data
                uint8_t* encoded_size_ptr = decode_data(file_data, 2 * 8);

                //fetch and store encoded size from its ptr
                uint16_t encoded_size = *((uint16_t *) encoded_size_ptr);

                //now we can free heap memory used by encoded_size_ptr
			    free(encoded_size_ptr);

                //read encoded data
                //this data is stored just after size data
                file_data = read_file(fp_bmp, 54 + (magic_string_len + 2) * 8, encoded_size * 8);

                //below function will extract secret message encode in above data
                uint8_t* sec_msg = decode_data(file_data, encoded_size * 8);

                //now we can free heap memory used by last call of file_data
			    free(file_data);

                //print progress message
                printf("Success : Secret message is decoded from given bmp file\n");
                
                //now we will create/open output file and store decoded message in it
                store_msg_to_file(sec_msg, encoded_size);

                //now we can free heap memory used by sec_msg
			    free(sec_msg);
            }
            //else print error and exit from program
            else
            {
                printf("Error   : Magic string %s is INVALID\n",info.magic_string);

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

//function definition to extract encoded message from given data
//encoded_size is the size (in bytes) of bitmap area from where we have to de decoding
 uint8_t* decode_data(uint8_t *file_data, uint32_t encoded_size)
 {
    //allocate dynamic memory to store secret message
    //in each byte of bitmap data, one bit of secret message is stored
    //therefore we divide encoded_size / 8
    uint8_t *sec_msg = malloc(encoded_size / 8);

    //check NULL for sec_mag
    if(sec_msg == NULL)
    {
        //print error message
        printf("Error   : Can't allocate dynamic memory for decoding\n");

        //exit 0, to terminate program and free all the resources
        exit(0);
    }

    //iterate over file data in the multiple of 8 bytes
	//because reading 8 bytes will give us 1 byte of secret msg
	for(uint16_t i = 0; i <= (encoded_size / 8 - 1); i++)
	{
		//encode_char will store decimal value of encoded character
		//multiplier is used to convert binary to decimal
		uint8_t encode_char = 0, multiplier = 1;

		//iterate over next 8 bytes of file_data
		for(uint8_t j = 0; j <= 7; j++)
		{
			//read one byte store in file_data
			uint8_t temp = *(file_data + i * 8 + j);

			//get 0th bit of temp
			uint8_t msg_bit = get_bits(temp, 0);
		
			//convert binary to decimal	
			encode_char += msg_bit * multiplier;
			//update mutiplier
			multiplier *= 2;
		}
		*(sec_msg + i) = encode_char;
	}

    //return sec_msg from function
    return sec_msg;
 }

//function definition to create/open output file and 
//store decoded secret message to it
void store_msg_to_file(uint8_t *sec_msg, uint16_t encoded_size)
{
    //print progress message
    printf("\nInfo    : Storing decoded data into output file : %s\n",info.output_txt_file_name);

    //open output file in write mode
    FILE *fp_output = fopen(info.output_txt_file_name, "w");

    //if file pointer is NULL
    if(fp_output == NULL)
    {
        //print error and exit
        printf("Error   : Can't open %s file\n",info.output_txt_file_name);

        //exit 0, is used to terminate the program and free all resources
        exit(0);
    }
    //else continue with the operations
    else
    {
        //write decoded secret message to the output file at the start
        write_file(sec_msg, encoded_size, fp_output, 0);

        //print progress message
        printf("Success : Decoded data stored output : %s\n",info.output_txt_file_name);
    }
}