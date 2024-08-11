/*
Name  : Rehan Shakoor
Batch : ECEP (24004) (E)

C project 1 (LSB steganography) submission

Submission date : 07/07/2024
*/

//This header file contains declarations of all the 
//function which are used in decoding part of this project

#ifndef DECODE_FUNC_HEADER_H
#define DECODE_FUNC_HEADER_H

//function declaration to check CLA for decoding
void check_decode_CLA(uint8_t argc, uint8_t **argv);

//function declaration to do decode checks and then actual decoding
void decode_func(void);

//function declaration to extract encoded message from given data
 uint8_t* decode_data(uint8_t *file_data, uint32_t encoded_size);

//function declaration to create/open output file and 
//store decoded secret message to it
void store_msg_to_file(uint8_t *sec_msg, uint16_t encoded_size);

#endif
