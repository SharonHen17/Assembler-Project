#include "utils.h"
#define NUM_OF_REGISTERS 8
#define MAX_BINARY_INT 10
#define MAX_BINARY_LENGTH 12
#define MAX_BIN_NUM_10 511

/*****************************************************************************************************************************************************************/	
/*The function gets a symbol name, line number in the file which it appears and the symbol table and its size. The function, as its name, adds a new symbol to the table.*/
void addSymbolToCheck(char*, int, int, stc**, int*);

/*The function receives a pointer to the symbol to check table and its current capacity. The function increases the size of the table.*/
void upSpaceSCT(stc**, int*);

/*The function gets an integer and a string. The function converts the integer to binary with 10 bits and puts it in the sting*/
void convert_int_to_10bit_binary (int, char*);

/*The function gets: IC, DC, ERRO and error_memory_anomaly.
If there is an exception in the number of memory cells, then the functiom updates the flag 'error_memory_anomaly'*/
void anomaly_check_in_memory_image (int, int, int*, int*);

/*A function that receives a string and returns a pointer to the first non-white character in the string */
char* skipSpacesAndTabs(char*);

/*The function gets a string and checks whether it is a valid number. If so, returns the length of the number, otherwise returns FALSE*/
int isInteger(char*);

/*The function gets a string, an array of commands, and a type. The function checks whether the received string is a valid string based on defined rules and the received type*/
int is_it_a_label (char*, com*, int);
