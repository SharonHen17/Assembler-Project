/*On this page, we recognize the guidline type:'.data', '.string', '.extern', '.entry', and act accordingly*/

#include "classify_guideline.h"

void c_guideline(int i, char* s, sym** symbols, int* cur, int* cap, int LC, int* ERROR, int* error_memory_anomaly, char data[][MAX_BINARY_LENGTH+1], int* DC, int IC, com* commands, stc** symbols_to_check, int* cur_stc, int* cap_stc){
/************************Setting Variables************************/
	char num [MAX_LENGTH_LINE]; /*The number received as a parameter*/
	char sym_name [MAX_LENGTH_SYMBOL]=""; /*Symbol name*/
	int len, ans; /*len- the length of the parameter (number or lable); ans- holds the returning value of the function 'cheack_symbols_table'*/
	char* quotation_mark; /*The pointer of the quotation mark which ends the valid string - it's for .string guideline*/
/*****************************************************************/
	s = skipSpacesAndTabs(s);
	
	if (i == 0){ /*.data guideline*/
		if (s[0] == '\n') {
			printf("Error in line %d: The guideline '.data' must have at least one parameter\n", LC);
			*ERROR = TRUE;
			return;
		}
		
		if (s[0] == ','){
			printf("Error in line %d: The parameters of the guideline '.data' can't start with ','\n", LC);
			*ERROR = TRUE;
			return;
		}   

		while (s[0] != '\n' && s[0] != '\0' && (len = isInteger(s)) != FALSE){
			/*printf("\n|%s|\n", s);*/
			strncpy (num, s, len);
			if (convertToBinary12Bits(atoi(num), data, DC) == !OK){
       			printf("Error in line %d: Integer out of range for 12-bit binary representation\n", LC);
				*ERROR = TRUE;
 				return;
			}
			anomaly_check_in_memory_image (IC, *DC, ERROR, error_memory_anomaly);
			s = s + len;
			s = skipSpacesAndTabs(s);
			if (s[0] != '\n'){/*If the string of the parameters doesn't end*/
				if (s[0] == ','){/*The parameters must be separated by ','*/
					s = s + 1;
					s = skipSpacesAndTabs(s);
					if (s[0] == '\n'){
						printf("Error in line %d: The parameters of the guideline '.data' can't end with ','\n", LC);
						*ERROR = TRUE;
						return;
					}
				
					if (s[0] == ','){
						printf ("Error in line %d: Multiple consecutive commas\n", LC);
						*ERROR = TRUE;
						return;
					}
				}
				else{
						printf("Error in line %d: The parameters of the guideline '.data' must be separated by ','\n", LC);
						*ERROR = TRUE;
						return;
					}
			}
			memset(num, 0, sizeof(num));
		}/*End while*/

		if (len == FALSE){
			printf ("Error in line %d: The parameters of the guideline '.data' must be integers\n", LC);
			*ERROR = TRUE;
		}	

	}/*End if .data guideline*/

/********************************************************************************************************************/

	if (i==1){/*.string guideline*/
		if (s[0] == '\n') {
			printf("Error in line %d: The guideline '.string' must have exactly one parameter - a valid string\n", LC);
			*ERROR = TRUE;
			return;
		}
		
		if (s[0] != '"'){
			printf("Error in line %d: The parameter of the guideline '.string' must start with '\"'\n", LC);
			*ERROR = TRUE;
			return;
		} 

		s = s + 1;
		
		if ((quotation_mark = strrchr (s, '"')) == NULL){
			printf("Error in line %d: The parameter of the guideline '.string' must end with '\"'\n", LC);
			*ERROR = TRUE;
			return;
		}

		while (s != quotation_mark){
			convertToBinary12Bits (*s, data, DC);
			anomaly_check_in_memory_image (IC, *DC, ERROR, error_memory_anomaly);
			s = s + 1;
		}	
		
		/*Ascii code '\0' (end of string)*/
		strcpy (data[*DC], "000000000000");
		(*DC)++;
		anomaly_check_in_memory_image (IC, *DC, ERROR, error_memory_anomaly);

		s = s + 1;
		s = skipSpacesAndTabs(s);
		if (*s != '\n'){
			printf("Error in line %d: A valid string is the only parameter of '.sting' guideline\n", LC);
			*ERROR = TRUE;
			return;
		}
		
	}/*End if .string guideline*/

/********************************************************************************************************************/

	if (i==2){/*'.extern' guideline*/
		if (s[0] == '\n') {
				printf("Error in line %d: Guideline '.extern' must have one parameter - a valid lable\n", LC);
				*ERROR = TRUE;
				return;
		}
		
		if (s[0] == ','){
			printf("Error in line %d: The parameters of the guideline '.extern' can't start with ','\n", LC);
			*ERROR = TRUE;
			return;
		}   

		while (s[0] != '\n' && (len = is_it_a_label(s, commands, EXT_PAR)) != FALSE){
			strncpy(sym_name, s, len);
			s = s + len;
			s = skipSpacesAndTabs(s);
			ans = cheack_symbols_table (sym_name, *symbols, *cur, EXTERNAL);
			if (ans == TRUE){
				addSymbol(sym_name, -1, EXTERNAL, symbols, cur);
				if (*cur == *cap) 
					upSpaceST(symbols, cap);
			}
			if (ans == FALSE){
				*ERROR = TRUE;
				printf("Error in line %d: The same label must not be defined more than once\n", LC);
				return;
			}
			/*if (ans == CONTINUE): The label is already defined as '.extern' - The situation is normal but there is no need to add it to the symbol table again*/

			if (s[0] != '\n'){/*If the string of the parameters doesn't end*/
				if (s[0] == ','){/*The parameters must be separated by ','*/
					s = s + 1;
					s = skipSpacesAndTabs(s);
					if (s[0] == '\n'){
						printf("Error in line %d: The parameters of the guideline 'extern' can't end with ','\n", LC);
						*ERROR = TRUE;
						return;
					}
				
					if (s[0] == ','){
						printf ("Error in line %d: Multiple consecutive commas\n", LC);
						*ERROR = TRUE;
						return;
					}
				}
				else{
						printf("Error in line %d: The parameters of the guideline '.data' must be separated by ','\n", LC);
						*ERROR = TRUE;
						return;
					}
			}
			memset(sym_name, 0, sizeof(sym_name));
		}/*End while*/

		if (len == FALSE){
			*ERROR = TRUE;
			printf("Error in line %d: The label which used as a parameter in '.extern' guideline isn't valid\n", LC);
			return;
		}	
	}/*End if '.extern' guideline*/

/********************************************************************************************************************/

	if (i==3){ /*'.entry' guideline*/
		if (s[0] == '\n'){
				printf("Error in line %d: Guideline '.entry' must have one parameter - a valid lable\n", LC);
				*ERROR = TRUE;
				return;
		}

		if ((len = is_it_a_label(s, commands, ENT_PAR)) != FALSE){ /*Checking if the operand is a lable*/
			strncpy(sym_name, s, len);
			s = s + len;
			s = skipSpacesAndTabs(s);
			addSymbolToCheck(sym_name, -1, LC, symbols_to_check, cur_stc); /*We send '-1' to the field 'index' to mark that this is a lable in '.entry'*/
			if (*cur_stc == *cap_stc)
				upSpaceSCT(symbols_to_check, cap_stc);
		}
		else{
			*ERROR = TRUE;
			printf("Error in line %d: The label used as a parameter in '.entry' guideline isn't valid\n", LC);
			return;
		}

		if (*s != '\n'){
			printf("Error in line %d: One valid lable is the only parameter of '.entry' guideline\n", LC);
			*ERROR = TRUE;
			return;
		}
	}/*End if '.entry' guideline*/
}
