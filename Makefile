all : main.o common_func_def.o decode_func_def.o encode_func_def.o
	gcc main.o common_func_def.o decode_func_def.o encode_func_def.o -o lsb_steg

main.o : main.c
	gcc -c main.c

common_func_def.o : common_func_def.c 
	gcc -c common_func_def.c  

decode_func_def.o : decode_func_def.c 
	gcc -c decode_func_def.c 

encode_func_def.o : encode_func_def.c 
	gcc -c encode_func_def.c 

clean : 
	rm *.o lsb_steg