#include "utils.h"
#define SIZE_ARRAY 1024
#define MAX_BINARY_LENGTH 12
#define OK 1
#define SIZE 10
#define ADD_TO_MEMORY 100

/*****************************************************************************************************************************************************************/	
/*The function gets: symbols table and its size, symbols to check table and its size, name of the file, Error flag, instruction table. The function performs the second pass on the file */
void scan2(sym*, int, stc*, int, char*, int*, char[][MAX_BINARY_LENGTH+1]);

/*The function gets: the input line (after clearing tabs and spaces from the beginning), symbols table, its size and its capacity, symbols to check table, its size and its capacity, commands array, IC, DC, line number (LC), ERROR, error_memory_anomaly, instruction table and data table.
The function classifies the sentence and acts accordingly.*/
void c_sentence(char*, sym**, int*, int*, stc**, int*, int*, com*, int*, int*, int, int*, int*, char[][MAX_BINARY_LENGTH+1], char data[][MAX_BINARY_LENGTH+1]);

/*The function gets the instruction table, data table, IC, DC and the name of the file.
The function produces a file with the suffix '.am'*/
void createObjFile(char[][MAX_BINARY_LENGTH+1], char[][MAX_BINARY_LENGTH+1], int, int, char*);

/*The function creates an array of symbols.*/
void buildSymbolsTable (sym**);

/*The function creates an array of symbols to check.*/
void buildSymbolsToCheckTable (stc**);

/*The function receives a pointer to a file and checks whether the current line in the file exceeds the valid line length of a line, which is 80.*/
int check_line_length_exceeded (FILE*);
