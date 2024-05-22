/*On this page, we perform the first pass of the assembler where, among other things, we identify the symbols (labels) that appear in the program, and give each symbol a numerical value which is the address in memory that the symbol represents. Also, build the memory image (the instruction and information tables).*/

#include "first_scan.h"

void scan1 (char* file_name, com* commands){
/************************Setting Variables************************/
	FILE *amfp; /*After mcro file*/
	char new_file_name [MAX_LENGTH_LINE]; /*file name, with the suffix '.am'*/

	char line [MAX_LENGTH_LINE + 1]; /* +1 for the null terminator*//*input line*/
	long sizeOfFile; /*The size of the file*/
	int LC = 1 ; /*Line counter*/	
	int IC=0, DC=0;	 /*IC-Instruction counter; DC-Data counter*/
	int ERROR = FALSE; /*A flag which tell us if there are errors in the first pass and the second pass on the file*/
	int error_memory_anomaly = FALSE; /*A flag which tell us if there is a memory anomaly*/
	int i;

	sym* symbols; /*A pointer to the symbols table. All the symbols which are defined in the file will be here.*/
	int cur_sym = 0; /*Current position in the symbols table*/
	int capacity_sym = SIZE; /*The capacity of the symbols table*/

	stc* symbols_to_check; /*A pointer to the symbols to check table. In this table will be all the symbols which are received as a parameter in the instruction 	sentences*/
	int cur_stc = 0; /*Current position in the symbols to check table*/
	int cap_stc = SIZE; /*The capacity of the symbols to check table*/

	char instructions[SIZE_ARRAY][MAX_BINARY_LENGTH+1]; /*A table containing the coding of instruction sentences*/
	char data [SIZE_ARRAY][MAX_BINARY_LENGTH+1]; /*A table containing the coding of data sentences*/
/*****************************************************************/

	strcpy (new_file_name, file_name);
	strcat (new_file_name,".am");

	if ((amfp = fopen(new_file_name, "r")) == NULL){
		printf ("Can't open file %s.am\n", new_file_name);
		return;	
	}

	buildSymbolsTable(&symbols); /*Build an empty symbols table*/
	buildSymbolsToCheckTable(&symbols_to_check); /*Build an empty symbols to check table*/
	fseek(amfp, 0L, SEEK_END);
	sizeOfFile = ftell(amfp);
	rewind(amfp);
	
   	while (ftell(amfp)!= sizeOfFile) {
		if (check_line_length_exceeded(amfp) == !OK){
			printf("Error in line %d: You exceeded the acceptable line length in the file\n", LC);
			ERROR = TRUE;
			LC++;	
			continue;
		}
		fgets(line, sizeof(line), amfp);
		c_sentence(line, &symbols, &cur_sym, &capacity_sym, &symbols_to_check, &cur_stc, &cap_stc, commands, &IC, &DC, LC, &ERROR, &error_memory_anomaly, instructions, data);
		LC++;
	}/*End while*/

	/*+IC+100 DATA symbols*/
	for (i=0; i<cur_sym; i++)
		if (symbols[i].type == DATA)
			symbols[i].address = symbols[i].address + IC + ADD_TO_MEMORY;

	scan2 (symbols, cur_sym, symbols_to_check, cur_stc, file_name, &ERROR, instructions);
	if (ERROR == FALSE)
		createObjFile(instructions, data, IC, DC, file_name);
	fclose(amfp);
}
