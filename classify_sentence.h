#include "utils.h"
#define NUM_OF_GUIDELINES 4
#define NUM_OF_COMMANDS 16
#define MAX_BINARY_LENGTH 12
#define ADD_TO_MEMORY 100

/*****************************************************************************************************************************************************************/	
/*The function gets: an index which represents the command, a string that represents what the command accepts as parameter(s), commands array, IC, DC, LC, ERROR, error_memory_anomaly, instructions table, symbols to check table, its size and its capacity.
The function classifies the command sentence and acts accordingly.*/
void c_command (int, char*, com*, int*, int, int, int*, int*, char[][MAX_BINARY_LENGTH+1], stc**, int*, int*);

/*The function gets: an index which represents the guideline, a string that represents what the guideline accepts as parameter(s), symbols table, its size and its capacity, LC, ERROR, error_memory_anomaly, data table, DC, IC, commands array, symbols to check table, its size and its capacity.
The function classifies the command sentence and acts accordingly.*/
void c_guideline(int, char*, sym**, int*, int*, int, int*, int*, char[][MAX_BINARY_LENGTH+1], int*, int, com*, stc**, int*, int*);

/*The function gets symbol name, symbols table, its size and type of the symbol. 
Depending on the label type, the function checks if it is in the symbol table and returns a value accordingly*/
int cheack_symbols_table (char*, sym*, int, int);

/*The function gets a symbol name, its address, its type and the symbol table and its size. The function, as its name, adds a new symbol to the table.*/
void addSymbol(char*, int, int, sym**, int*);

/*The function receives a pointer to the symbol table and its current capacity. The function increases the size of the table.*/
void upSpaceST(sym**, int*);

/*The function gets a string, an array of commands, and a type. The function checks whether the received string is a valid string based on defined rules and the received type*/
int is_it_a_label (char*, com*, int);

/*A function that receives a string and returns a pointer to the first non-white character in the string */
char* skipSpacesAndTabs(char*);
