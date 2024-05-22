/*On this page we perform the second pass where we build the machine code in its entirety using the symbol values.*/

#include "second_scan.h"

void scan2(sym* symbols, int cur, stc* symbols_to_check, int cur_stc, char* file_name, int* ERROR, char instructions[][MAX_BINARY_LENGTH+1]){	
/************************Setting Variables************************/
	char memory_word[MAX_BINARY_LENGTH + 1] = ""; /* +1 for the null terminator*//*A memory word to be added to the instruction table*/
	char int_10bit_binary[MAX_BINARY_INT+1]; /*The address of the label in binary*/

	int i, j/*, len, ans*/; /*len- the length of the parameter (number or lable); ans- holds the returning value of the function 'cheack_symbols_table'*/
	int is_there_entry = FALSE; /*A flag that says if there is a label of type ENTRY*/
	int is_there_extern = FALSE;/*A flag that says if there is a label of type EXTERN*/
/*****************************************************************/

	for (i=0; i<cur_stc; i++){
		for (j=0; j<cur; j++){
			if (!strcmp(symbols_to_check[i].name, symbols[j].name)){
				if (symbols_to_check[i].index == -1){/*Label received at '.entry' guideline*/
					if(symbols[j].type == ENTRY)
						break;
					if (symbols[j].type == EXTERNAL){
						*ERROR = TRUE;
						printf("Error in line %d: The same label can't be defined as both '.entry' and '.extern'\n", symbols_to_check[i].line);
						break;
					}							
					/*The label used in '.entry' guideline is defined in the file*/
					is_there_entry = TRUE;
					symbols[j].type = ENTRY;
					break;
				}
				else{ /*A label received as an operand in a command*/
					if (symbols[j].type == EXTERNAL){
						strcat(memory_word, "0000000000");/*The address of the label*/	
						strcat(memory_word, "01"); /*A,R,E: E*/	
						is_there_extern = TRUE;	
					}
					else{
						convert_int_to_10bit_binary(symbols[j].address, int_10bit_binary);	 
						strcat(memory_word, int_10bit_binary);/*The address of the label*/
						strcat(memory_word, "10"); /*A,R,E: R*/
					}
					strcpy(instructions[symbols_to_check[i].index], memory_word);
					memset(memory_word, 0, sizeof(memory_word));
					break;
				}
			}
		}	
		if (j==cur){
			printf("Error in line %d: The label '%s' which received as a parameter is not defined in this file\n",symbols_to_check[i].line, symbols_to_check[i].name);
			*ERROR = TRUE;
		}	
	} 

	if (*ERROR == FALSE){
		if (is_there_entry == TRUE)
			createEntFile (file_name, symbols, cur);
		if (is_there_extern == TRUE)
			createExtFile (file_name, symbols, cur, symbols_to_check, cur_stc);
	}
}
