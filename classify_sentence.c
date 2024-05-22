/*On this page, we identify the sentence type (derived from the assembly language file): guideline, instruction, blank, comment.*/

#include "classify_sentence.h"

void c_sentence(char* s, sym** symbols, int* cur, int* cap, stc** symbols_to_check, int* cur_stc, int* cap_stc, com* commands, int* IC, int* DC, int LC, int* ERROR, int* error_memory_anomaly, char instructions[][MAX_BINARY_LENGTH+1], char data[][MAX_BINARY_LENGTH+1]){
/************************Setting Variables************************/
	int i, len; /*len- holds the length of the symbol name*/
	char sym_name [MAX_LENGTH_SYMBOL] = ""; /*Symbol name*/
	char* guidelines [] = {".data", ".string", ".extern", ".entry"}; /*The names of the guidelines*/
/*****************************************************************/

	/*If the line is a comment line, then it is skipped*/
	if (*s == ';')
		return;

	s = skipSpacesAndTabs(s);
	/*If the line is an empty line, then it is skipped*/
	if (*s == '\n')
		return;

	/*Checking if the sentence is a command line*/
	for (i=0; i<NUM_OF_COMMANDS; i++){
		if (!strncmp(commands[i].name, s, strlen(commands[i].name))){
			c_command(i, s + strlen(commands[i].name), commands, IC, *DC, LC, ERROR, error_memory_anomaly, instructions, symbols_to_check, cur_stc, cap_stc);
			return;
		}
	}

	/*Checking if the sentence is a guideline*/
	for (i=0; i<NUM_OF_GUIDELINES; i++){
		if (!strncmp(guidelines[i], s, strlen(guidelines[i]))){
			c_guideline(i, s + strlen(guidelines[i]), symbols, cur, cap, LC, ERROR, error_memory_anomaly, data, DC, *IC, commands, symbols_to_check, cur_stc, cap_stc);
			return;
		}
	}

	/*So the first word of the sentence isn't a command line or a guideline. Therefore, it has to be a label*/	
	/*Let's check if it is a label*/

	if ((len = is_it_a_label(s, commands, START)) != FALSE){
		strncpy(sym_name, s, len-1);
		s = s + len;
		s = skipSpacesAndTabs(s);

		/*Checking if the sentence is a guideline*/
		for (i=0; i<NUM_OF_GUIDELINES; i++){
			if (!strncmp(guidelines[i], s, strlen(guidelines[i]))){
				if (i == 0 || i == 1){/*There is a label before .data OR .string guideline*/
					if (cheack_symbols_table (sym_name, *symbols, *cur, START) == FALSE){
						*ERROR = TRUE;
						printf("Error in line %d: The same label must not be defined more than once\n", LC);
						return;
					}
					addSymbol(sym_name, (*DC), DATA, symbols, cur);
					if (*cur == *cap) 
						upSpaceST(symbols, cap);
				}
				else /*There is a label before .extern or .entry guideline*/
					printf ("Note in line %d: Label defined at the beginning of '%s' guideline is meaningless\n", LC, guidelines[i]);
				c_guideline(i, s + strlen(guidelines[i]), symbols, cur, cap, LC, ERROR, error_memory_anomaly, data, DC, *IC, commands, symbols_to_check, cur_stc, cap_stc);
				return;
			}
		}

		if (cheack_symbols_table (sym_name, *symbols, *cur, START) == FALSE){
			*ERROR = TRUE;
			printf("Error in line %d: The same label must not be defined more than once\n", LC);
			return;
		}
		/*Checking if the sentence is a command line*/
		for (i=0; i<NUM_OF_COMMANDS; i++){
			if (!strncmp(commands[i].name, s, strlen(commands[i].name))){
				addSymbol(sym_name, (*IC)+ADD_TO_MEMORY, RELOCATABLE, symbols, cur);
				if (*cur == *cap) {
					upSpaceST(symbols, cap);
				}
				c_command(i, s + strlen(commands[i].name), commands, IC, *DC, LC, ERROR, error_memory_anomaly, instructions, symbols_to_check, cur_stc, cap_stc);
				return;
			}
		}
		printf ("Error in line %d: After the label, could not identify the sentence type\n", LC);
		*ERROR = TRUE;
	}
	else{
		*ERROR = TRUE;
		printf ("Error in line %d: Invalid label\n", LC);
	}
}	
