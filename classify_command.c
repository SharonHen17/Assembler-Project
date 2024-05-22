/*On this page, we recognize the instruction and act accordingly*/

#include "classify_command.h"

void c_command (int i, char* s, com* commands, int* IC, int DC, int LC, int* ERROR, int* error_memory_anomaly, char instructions[][MAX_BINARY_LENGTH+1], stc** symbols_to_check, int* cur_stc, int* cap_stc){
/************************Setting Variables************************/
	char first_memory_word[MAX_BINARY_LENGTH + 1] = "";  /* +1 for the null terminator*//*First memory word of the command line*/	
	char another_memory_word_0[MAX_BINARY_LENGTH + 1] = "";  /* +1 for the null terminator*//*Used for the case where both operands are registers*/		
	char another_memory_word_1[MAX_BINARY_LENGTH + 1] = "";  /* +1 for the null terminator*//*First additional memory word*/	
	char another_memory_word_2[MAX_BINARY_LENGTH + 1] = "";  /* +1 for the null terminator*//*Second additional memory word*/
	char sym_name [MAX_LENGTH_SYMBOL] = "";	/*Symbol name*/
	char num [MAX_LENGTH_LINE]; /*The number received as a operand*/
	char* source = "000"; /*Initialize the source code*/
	char* destination = "000"; /*Initialize the destination code*/
	char int_10bit_binary[MAX_BINARY_INT+1]; /*The integer in 10 bits binary representing*/
	int j, L = 0, len, s_op_register = -1;/*If the first operand is a register, then it is stored in 's_op_register'*//*L-counts how many additional lines must be added in order to code the instruction; len-The length of the parameter (number or lable)*/
	int value; /*Holds the value of the integer after translating the string to a number*/
	
	struct {
	char* name;
	char* code;
	} registers[] = {
					{"r0", "00000"},
					{"r1", "00001"},
					{"r2", "00010"},
					{"r3", "00011"},
					{"r4", "00100"},
					{"r5", "00101"},
					{"r6", "00110"},
					{"r7", "00111"},
					};
/*****************************************************************/

	if (i==14 || i==15){ /*Operation name is rts/stop*/
		s = skipSpacesAndTabs(s);
		if (*s != '\n' && *s != '\0'){
			printf("Error in line %d: The instruction '%s' must have no operands\n", LC, commands[i].name);
			*ERROR = TRUE;
			return;
		}
		strcat(first_memory_word,"000");/*There is no source operand, so the addressing method of the source operand is '000'*/
		strcat(first_memory_word, commands[i].opcode); /*Opcode*/
		strcat(first_memory_word,"00000");/*There is no destination operand, so the addressing method of the source operand is '000'. Also, A,R,E: A.*/
		strcpy (instructions[*IC], first_memory_word);
		(*IC)++;
		anomaly_check_in_memory_image (*IC, DC, ERROR, error_memory_anomaly);
		return;
	}

	if (*s != ' ' && *s != '\t'){
		printf("Error in line %d: There must be at least one space/tab between the name of the operation and its operands\n", LC);
		*ERROR = TRUE;
		return;
	}
	s = s + 1;
	s = skipSpacesAndTabs(s);
	
	if (i==0 || i==1 || i==2 || i==3 || i==6){ /*mov/cmp/add/sub/lea; 2 operands*/
		if (*s == '\n' || *s == '\0'){
			printf("Error in line %d: The instruction '%s' must include two operands\n", LC, commands[i].name);
			*ERROR = TRUE;
			return;
		}

		/*source operand*/
		if ((len = isInteger(s)) != FALSE){/*Checking if the operand is an integer*/
			if(i==6){/*lea*/
				printf("Error in line %d: The source operand of '%s' instruction must be a lable\n", LC, commands[i].name);
				*ERROR = TRUE;
				return;
			}
			strncpy (num, s, len);
			value = atoi(num);

			/*Check if the integer can be represented in 10 bits, (-512) to 511*/
			if (value < -(MAX_BIN_NUM_10+1) || value > MAX_BIN_NUM_10) {
				printf("Error in line %d: Integer cannot be represented in 10 bits.\n", LC);
				*ERROR = TRUE;	
				return;
			}
			convert_int_to_10bit_binary(value, int_10bit_binary);
			s = s + len;
			L++;
			strcat(another_memory_word_1, int_10bit_binary);/*The integer in 10 bits binary representing*/
			strcat(another_memory_word_1, "00");/*A,R,E: A*/
			source = "001";
		}
		else{
			if ((len = is_it_a_label(s, commands, COM_PAR)) != FALSE){/*Checking if the operand is a lable*/
				L++;			
				source = "011";
				strncpy(sym_name, s, len);
				addSymbolToCheck(sym_name, (*IC)+L, LC, symbols_to_check, cur_stc);
				if (*cur_stc == *cap_stc)
					upSpaceSCT(symbols_to_check, cap_stc);
				s = s + len;
			}
			else{
				if (*s == '@'){ /*Checking if the operand is a register*/
					for (j=0; j<NUM_OF_REGISTERS; j++){
						if (!strncmp(s+1, registers[j].name, strlen(registers[j].name))){
							if (i==6){
								printf("Error in line %d: The source operand of '%s' instruction must be a lable\n", LC, commands[i].name);
								*ERROR = TRUE;
								return;
							}
							s_op_register = j; /*Store the register*/
							s = s + strlen(registers[j].name) + 1; /* +1 for '@'*/
							L++;
							strcat (another_memory_word_1, registers[j].code);
							strcat (another_memory_word_1, "0000000"); /*destination + 'A'*/
							source = "101";
							break;
						}
					}
					if (j==NUM_OF_REGISTERS){
						*ERROR = TRUE;
						printf("Error in line %d: The source operand of '%s' instruction is unidentified\n", LC, commands[i].name);
						return;
					}
				}
			
				else{
					*ERROR = TRUE;
					printf("Error in line %d: The source operand of '%s' instruction is unidentified\n", LC, commands[i].name);
					return;
				}
			}
		}	

		s = skipSpacesAndTabs(s);
		if (*s != ','){	
			printf("Error in line %d: The instruction '%s' accepts two operands that must be separated from each other by a comma\n", LC, commands[i].name);
			*ERROR = TRUE;
			return;
		}
		s=s+1;
		s = skipSpacesAndTabs(s);

		if (*s == ','){
			printf ("Error in line %d: Multiple consecutive commas\n", LC);
			*ERROR = TRUE;
			return;
		}

		if (*s == '\n' || *s == '\0'){
			printf("Error in line %d: The instruction '%s' must include two operands\n", LC, commands[i].name);
			*ERROR = TRUE;
			return;
		}
	}
	memset(sym_name, 0, sizeof(sym_name));


	/*destination operand*/

	/* The destination operand of all instructions can be 3, 5 */

	if (*s == '\n' || *s == '\0'){
		printf("Error in line %d: The instruction '%s' must include one operand\n", LC, commands[i].name);
		*ERROR = TRUE;
		return;
	}

	if ((len = isInteger(s)) != FALSE){/*Checking if the operand is an integer*/
		if (i!=1 && i!=12){
			printf("Error in line %d: The destination operand of '%s' instruction can't be an integer\n", LC, commands[i].name);
			*ERROR = TRUE;
			return;
		}
		strncpy (num, s, len);
		value = atoi(num);

		/*Check if the integer can be represented in 10 bits, (-512) to 511*/
		if (value < -(MAX_BIN_NUM_10 + 1) || value > MAX_BIN_NUM_10) {
			printf("Error in line %d: Integer cannot be represented in 10 bits.\n", LC);
			*ERROR = TRUE;	
			return;
		}
		convert_int_to_10bit_binary(value, int_10bit_binary);
		s = s + len;
		L++;
		strcat(another_memory_word_2, int_10bit_binary);/*The integer in 10 bits binary representing*/
		strcat(another_memory_word_2, "00");/*A,R,E: A*/
		destination = "001";
	}	

	else{
		if ((len = is_it_a_label(s, commands, COM_PAR)) != FALSE){/*Checking if the operand is a label*/
			L++;			
			destination = "011";
			strncpy(sym_name, s, len);
			addSymbolToCheck(sym_name, (*IC)+L, LC, symbols_to_check, cur_stc);
			if (*cur_stc == *cap_stc) 
				upSpaceSCT(symbols_to_check, cap_stc);
			s = s + len;
		}
		else{
			if (*s == '@'){/*Checking if the operand is a register*/
				for (j=0; j<NUM_OF_REGISTERS; j++){
					if (!strncmp(s+1, registers[j].name, strlen(registers[j].name))){
						if (s_op_register == -1)
							L++;
						s = s + strlen(registers[j].name) + 1; /* +1 for '@'*/
						strcat (another_memory_word_2, "00000");
						strcat (another_memory_word_2, registers[j].code); /*destination*/
						strcat (another_memory_word_2, "00"); /*'A'*/
						destination = "101";
						break;
					}
				}
				if (j==NUM_OF_REGISTERS){
					*ERROR = TRUE;
					printf("Error in line %d: The destination operand of '%s' instruction is unidentified\n", LC, commands[i].name);
					return;
				}
			}
			else{
				*ERROR = TRUE;
				printf("Error in line %d: The destination operand of '%s' instruction is unidentified\n", LC, commands[i].name);
				return;
			}
		}
	}

	s = skipSpacesAndTabs(s);
	if (*s != '\n' && *s != '\0'){
		if (i==0 || i==1 || i==2 || i==3 || i==6) /*mov/cmp/add/sub/lea; 2 operands*/
			printf("Error in line %d: The instruction '%s' must include only two operands\n", LC, commands[i].name);
		else{
			printf("Error in line %d: The instruction '%s' must include only one operand\n", LC, commands[i].name);
			}
		*ERROR = TRUE;
		return;
	}	

	strcat(first_memory_word, source); /*source*/
	strcat(first_memory_word, commands[i].opcode); /*Opcode*/
	strcat(first_memory_word, destination); /*destination*/
	strcat(first_memory_word, "00"); /*A,R,E: A*/
	strcpy (instructions[*IC], first_memory_word);

	if (i==0 || i==1 || i==2 || i==3 || i==6){ /*mov/cmp/add/sub/lea; 2 operands*/
		if (s_op_register != -1 && L == 1){/*source & destination are register*/
			strcat (another_memory_word_0, registers[s_op_register].code);
			strcat (another_memory_word_0, registers[j].code);
			strcat(another_memory_word_0, "00"); /*A,R,E: A*/
			strcpy (instructions[(*IC)+L], another_memory_word_0);
		}
		else{
			strcpy (instructions[(*IC)+L-1], another_memory_word_1);
			strcpy (instructions[(*IC)+L], another_memory_word_2);
		}
	}
	else{
		strcpy (instructions[(*IC)+L], another_memory_word_2);/*One operand*/
	}

	*IC = *IC + L + 1;
	anomaly_check_in_memory_image (*IC, DC, ERROR, error_memory_anomaly);
}
