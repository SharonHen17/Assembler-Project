/*On this page, we perform the pre-assembler process in which the macros are deployed and a file with the 'am' extension is generated*/

#include "pre_assembler.h"

void pre_as (FILE *ifp, FILE *amfp, char* file_name){
/************************Setting Variables************************/
	char line [MAX_LENGTH_LINE + 1]; /* +1 for the null terminator*//*input line*/
	char macro_name [MAX_LENGTH_LINE]=""; /*Holds the mcro name, if it was found*/
	char* temp; /*A temporary pointer that helps scan the input line*/

	mcro* mt_s; /*Mcro table*/
	int cur_mt = 0; /*Current position in the mcro table*/
	int capacity_mt = SIZE; /*The capacity of the mcro table*/
	int pos_mc_table; /*Index of the mcro in the table*/
	int LC = 0; /*Line counter*/
	int ERROR = FALSE; /*A flag which tell us if there were errors in the macro defintion*/
	int is_there_a_macro = FALSE; /*A flag which tell us if mcro was found*/
	fpos_t s_macro, cur_pos_file;/*To record position*/	
	com commands [] = {{"mov", "0000"},
						{"cmp", "0001"},
						{"add", "0010"},
						{"sub", "0011"},
						{"not", "0100"},
						{"clr", "0101"},
						{"lea", "0110"},
						{"inc", "0111"},
						{"dec", "1000"},
						{"jmp", "1001"},
						{"bne", "1010"},
						{"red", "1011"},
						{"prn", "1100"},
						{"jsr", "1101"},
						{"rts", "1110"},
						{"stop", "1111"}}; /*Array of commands, which includes their name and opcode*/
/*****************************************************************/

	buildMcroTable(&mt_s); /*Build an empty mcro table*/
	while (fgets(line, sizeof(line), ifp) != NULL){	
		LC++;   
		if ((temp = strstr(line, "mcro")) != NULL){
			if ((*(temp + SIZE_OF_WORD_MCRO) == ' ' || *(temp + SIZE_OF_WORD_MCRO) == '\t') && ((temp == line) || (*(temp-1) == ' ') || (*(temp-1) == '\t'))){
				/*The line is a beginning of a mcro definition*/
				if (extractMacroName(temp+SIZE_OF_WORD_MCRO, macro_name, mt_s, cur_mt, LC, commands) == OK){
					fgetpos(ifp, &s_macro); /*Recording the position of the macro*/						
					if (skipAfterEndMcro(ifp, &LC) == OK){
						addMcro(macro_name, s_macro, &mt_s, &cur_mt);
						if (cur_mt == capacity_mt) /*Increasing the capacity of the table*/
							upSpaceMT(&mt_s, &capacity_mt);
						is_there_a_macro = TRUE; /*Found mcro*/
					}
					else ERROR = TRUE;
				}
			else ERROR = TRUE;
			continue;
			}
		}
		pos_mc_table = checkingIfLineIsCallingToMc(line, mt_s, cur_mt);
		if (is_there_a_macro == TRUE && pos_mc_table!=-1){
			/*There is a mcro (at least one) in the mcro table && this line is a mcro's calling*/
			fgetpos(ifp, &cur_pos_file); /*Recording the position of the file*/
			replaceMcroName (line, mt_s, pos_mc_table, ifp, amfp);
			fsetpos(ifp, &cur_pos_file);
		}
		else
			fputs(line, amfp);
	} /*End while*/
				
	fclose(amfp);
	if (ERROR == TRUE){
		strcat (file_name,".am");
		if (remove(file_name))
			printf ("Error deleting file %s\n", file_name);
	}
	else{
		scan1 (file_name, commands);
	}
	free(mt_s);	
}
