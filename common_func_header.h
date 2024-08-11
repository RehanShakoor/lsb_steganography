/*
Name  : Rehan Shakoor
Batch : ECEP (24004) (E)

C project 1 (LSB steganography) submission

Submission date : 07/07/2024
*/

//This header file contains declarations of all the functions
//which are common in both encoding and decoding part of the project

#ifndef COMMON_FUNC_HEADER_H
#define COMMON_FUNC_HEADER_H

//function declaration to print help menu
void help_menu(void);

//function definition to check extension
//this function checks if the file_name extension matches with the
//extension provides as ext_name (passed as .bmp, .txt)
bool check_extension(char *file_name, char *ext_name);

//function declaration to calculate size of given file in bytes
//here we are iterating over the whole file to calculate the size
//therefore it would be better to read the content and store it in
//heap for its use in future this will save memory and time
uint8_t * file_size_and_data(char *file_name);

//function declaration to read from a file
//this function will read given bytes from given offset from the given file
//this function will store read value in dynamic memory and
//return pointer to the start address of that memory
uint8_t* read_file(FILE *fp, uint16_t offset, uint32_t bytes);

//function declaration to copy two buffer, source and destination address
//of two buffer are given, buffer_size and offset for destination at 
//which data form source will be copied is required.
uint8_t* buffer_copy(uint8_t *des, uint8_t *src,  uint16_t offset, uint32_t buffer_size);

//function declaration to write to a file, this function will
//write given message to file pointer by fp at the given offset
void write_file(uint8_t *msg, uint32_t size, FILE *fp, uint32_t offset);

//function declaration to get nth bit of variable ch
uint8_t get_bits(uint8_t ch, uint8_t nth);

//function declaration to update lsb (0th bit) of variable ch
//with the value of bit variable and return update temp value
uint8_t update_lsb(uint8_t ch, uint8_t bit);

//Test function definition to print content of each bytes of buffer in hex form
//function takes argument as pointer to buffer and byte counts to be printed
void print_buffer(uint8_t *buff, uint32_t size);

#endif
