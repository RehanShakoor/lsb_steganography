/*
Name  : Rehan Shakoor
Batch : ECEP (24004) (E)

C project 1 (LSB steganography) submission

Submission date : 07/07/2024
*/

//This header file contains declarations of all the 
//function which are used in encoding part of this project

#ifndef ENCODE_FUNC_HEADER_H
#define ENCODE_FUNC_HEADER_H

//function declaration to check CLA for encoding
void check_encode_CLA(uint8_t argc, uint8_t **argv);

//function declaration to check encode conditions and do actual encoding
void encode_func(void);

//function declaration to do actual encoding
//this function will store each bits of sec_msg into
//lsb (0th bit) of each byte of copied .bmp file and return pointer
//to updated value, original .bmp file is not changed in this function
uint8_t* encode_msg(FILE *fp_bmp, uint8_t *sec_msg, uint16_t txt_size, uint32_t bitmap_size);

//function declaration to create output file and store all data into it
//data included header, header info and encoded bitmap values
void create_output_file(FILE *fp_bmp, uint8_t *encode_bitmap, uint32_t bitmap_size);

#endif
