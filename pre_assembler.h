#include "utils.h"
#define SIZE_OF_WORD_MCRO 4
#define OK 1
#define SIZE 10

/*****************************************************************************************************************************************************************/	
/*The function receives an input line ('s'), a string that will contain the name of the macro ('t'), mcro table and its size, the number of the input line and a table of instruction names. The operation extracts the macro name from the input line and, if it is valid, inserts it into a string 't'.*/
int extractMacroName (char* s, char* t, mcro*, int, int, com*);

/*The function receives a pointer to the source file and the number of the input line. The action skips the mcro definition in the source file. If executed successfully returns 1 otherwise returns 0.*/
int skipAfterEndMcro (FILE*, int*);

/*The function creates an array of this mcros.*/
void buildMcroTable (mcro**);

/*The function gets a mcro name, its starting position in the file and the mcro table and its size. The function, as its name, adds a new mcro to the table.*/
void addMcro (char*, fpos_t, mcro**, int*);

/*The function receives a pointer to the mcro table and its current capacity. The function increases the size of the table.*/
void upSpaceMT (mcro**, int*);

/*The function receives an input line, mcro table and its size. The function checks whether the received line is a line that has a call to the mcro, 
if so returns the index of the mcro in the table and otherwise returns -1*/
int checkingIfLineIsCallingToMc (char*, mcro*, int);

/*The function receives an input line, a macro table, an index in the table representing the macro we located, a pointer to the source file (ifp) and a pointer to the file used as the file after the macro is deployed (amfp). The function performs the deployment of the macro in amfp.*/
void replaceMcroName (char*, mcro*, int, FILE* ifp, FILE* amfp);

/*The function gets a file name and an array of commands. The function performs the first pass over the file (when it is after the macros have been deployed).*/
void scan1 (char*, com*);
