#include "utils.h"
#define MAX_BINARY_LENGTH 12
#define OK 1

/*****************************************************************************************************************************************************************/	
/*The function gets an integer, data table and DC. The function converts the number to binary with 12 bits and add it to the data table.
It returns 1 if it succeed, else 0*/
int convertToBinary12Bits (int, char[][MAX_BINARY_LENGTH+1], int*);

/*The function gets: IC, DC, ERRO and error_memory_anomaly.
If there is an exception in the number of memory cells, then the functiom updates the flag 'error_memory_anomaly'*/
void anomaly_check_in_memory_image (int, int, int*, int*);

/*The function gets symbol name, symbols table, its size and type of the symbol. 
Depending on the label type, the function checks if it is in the symbol table and returns a value accordingly*/
int cheack_symbols_table (char*, sym*, int, int);

/*The function gets a string, an array of commands, and a type. The function checks whether the received string is a valid string based on defined rules and the received type*/
char* skipSpacesAndTabs(char*);

/*The function gets a string and checks whether it is a valid number. If so, returns the length of the number, otherwise returns FALSE*/
int isInteger(char*);

/*The function gets a symbol name, its address, its type and the symbol table and its size. The function, as its name, adds a new symbol to the table.*/
void addSymbol(char*, int, int, sym**, int*);

/*The function receives a pointer to the symbol table and its current capacity. The function increases the size of the table.*/
void upSpaceST(sym**, int*);

/*The function gets a string, an array of commands, and a type. The function checks whether the received string is a valid string based on defined rules and the received type*/
int is_it_a_label (char*, com*, int);

/*The function gets a symbol name, line number in the file which it appears and the symbol table and its size. The function, as its name, adds a new symbol to the table.*/
void addSymbolToCheck(char*, int, int, stc**, int*);

/*The function receives a pointer to the symbol to check table and its current capacity. The function increases the size of the table.*/
void upSpaceSCT(stc**, int*);
