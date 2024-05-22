#include "utils.h"
#define MAX_BINARY_INT 10
#define MAX_BINARY_LENGTH 12

/******************************************************************************************************************************************************************/
/*The function gets name of file, symbols table and its size. The function produces a file with the suffix '.ent'*/
void createEntFile (char*, sym*, int);

/*The function gets name of file, symbols table and its size, symbols to check table and its size. The function produces a file with the suffix '.ext'*/
void createExtFile (char*, sym*, int, stc*, int);

/*The function gets an integer and a string. The function converts the integer to binary with 10 bits and puts it in the sting*/
void convert_int_to_10bit_binary (int, char*);

/*The function gets symbol name, symbols table, its size and type of the symbol. 
Depending on the label type, the function checks if it is in the symbol table and returns a value accordingly*/
int cheack_symbols_table (char*, sym*, int, int);

/*A function that receives a string and returns a pointer to the first non-white character in the string */
char* skipSpacesAndTabs(char*);

/*The function receives a pointer to a file and checks whether the current line in the file exceeds the valid line length of a line, which is 80.*/
int check_line_length_exceeded (FILE*);

/*The function gets a string, an array of commands, and a type. The function checks whether the received string is a valid string based on defined rules and the received type*/
int is_it_a_label (char*, com*, int);
