/*
Name  : Rehan Shakoor
Batch : ECEP (24004) (E)

C project 1 (LSB steganography) submission

Submission date : 07/07/2024
*/

//this header file contains struct definition

#ifndef STRUCT_HEADER_H
#define STRUCT_HEADER_H

//below info struct will be used to store all relevent information
//magic string will tell, whether a particular .bmp file contains encoded data or not
extern struct info_struct
{
	uint8_t *output_bmp_file_name;  //stores output bmp file name
	uint8_t *magic_string;          //stores magic string
	uint8_t *input_bmp_file_name;   //stores input .bmp file name
	uint8_t *input_txt_file_name;   //stores input .txt file name (contains secret msg)
	uint8_t *output_txt_file_name;  //stores output .txt file name (stores decoded msg)
} info;

#endif