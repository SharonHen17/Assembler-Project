/*On this page there is a collection of auxiliary functions for the memory encoding processes and input tests.*/

#include "classify_func.h"

int check_line_length_exceeded (FILE* f){
	int line_length = 0; /*The length of the line*/
	char ch;

	while ((fgetc(f)) != '\n'){
		line_length++;
		if (line_length > MAX_LENGTH_LINE){
			/* Read characters until a newline or end-of-file is encountered*/
    		while ((ch = fgetc(f)) != '\n' && ch != EOF) {
        		/* Skip characters */
    		}
			return !OK;
		}
	}
	fseek (f, ((-1)*(line_length+1)), SEEK_CUR);	
	return OK;
}

int isInteger(char* num) {
	int i=0;	
	/* Check for optional sign prefix */
    if (*num == '+' || *num == '-') {
		i++; /* Skip the sign character */
        if (!isdigit(num[i])) {
            return FALSE;  /* Only sign character without digits */
        }
    }

	/* Check for remaining digits */
    while (num[i] != '\0' && num[i] != ',' && num[i] != '\t' && num[i] != ' ' && num[i] != '\n') {
        if (!isdigit(num[i])) {
            return FALSE;  /* Non-digit character found */
        }
        i++;
    }
    return i;  /* All characters are digits */
}

void convert_int_to_10bit_binary (int value, char* binary) {
	int i, index = MAX_BINARY_INT; /*index-the size of the binary representation*/
	
    binary[index] = '\0';  /* Null terminator */
		
    for (i = 0; i < MAX_BINARY_INT; i++) {
       	binary[--index] = '0' + (value & 1);  /* Get the least significant bit and convert it to character */
       	value >>= 1;  /* Shift the value right by 1 bit */
    }
}

int is_it_a_label (char* s, com* commands, int type){
	int i=0, j;

	/*Starts with a letter of the alphabet (uppercase or lowercase)*/
	if(!isalpha(*s)){
		return FALSE;
	}
	i++;

	while (isalnum(s[i])){
		i++;
		if (i > MAX_LENGTH_SYMBOL){  
			return FALSE;
		}
	}
	
	if (type == START){
		if(s[i] != ':')
			return FALSE;
	}

	if (type == ENT_PAR){
		if(s[i] != ' ' && s[i] != '\t' && s[i] != '\0' && s[i] != '\n')
			return FALSE;
	}


	if (type == COM_PAR || type == EXT_PAR){
		if(s[i] != ' ' && s[i] != '\t' && s[i] != '\0' && s[i] != '\n' && s[i] != ',')
			return FALSE;
	}

	/*Checking if the label is the name of a command line*/
	for (j=0; j<NUM_OF_COMMANDS; j++){
		if ((i == strlen(commands[j].name)) && !strncmp(commands[j].name,s,i)){
			return FALSE;
		}
	}

	/*It's a lable*/
	if (type == START)
		return i+1;/*the length of the lable + ':'*/
	return i;
}

void addSymbolToCheck(char* s, int i, int line, stc** st, int * cur){
	stc temp; /*The symbol we will add to the table*/
	strcpy(temp.name, s);
	temp.index = i;
	temp.line = line;
	
	(*st)[*cur] = temp;
	(*cur) ++;
}

void upSpaceSCT(stc** st, int* max){
	stc* temp; /*A temporary pointer to the symbol to check table*/

	*max += SIZE;
	temp = (stc*) realloc (*st, (*max)*sizeof(stc));

	if (temp == NULL){
		printf("Not enough memory.\n");
		exit(0);
	}
	*st = temp;
}

void addSymbol(char* s, int address, int type, sym** st, int* cur){
	sym temp; /*The symbol we will add to the table*/
	strcpy(temp.name, s);
	temp.address = address;
	temp.type = type;
	
	(*st)[*cur] = temp;
	(*cur) ++;
}

void upSpaceST(sym** st, int* max){
	sym* temp; /*A temporary pointer to the symbol table*/

	*max += SIZE;
	temp = (sym*) realloc (*st, (*max)*sizeof(sym));
	if (temp == NULL){
		printf("Not enough memory.\n");
		exit(0);
	}
	*st = temp;
}

int convertToBinary12Bits (int value, char data[][MAX_BINARY_LENGTH+1], int* DC){
	char binary[MAX_BINARY_LENGTH + 1];  /*+1 for the null terminator*//*The 12 bits binary representation*/
	int i;

    if (value < -(MAX_BIN_NUM_12+1) || value > MAX_BIN_NUM_12){
        return !OK;
    }
	
    /*Handle negative numbers using 2's complement representation*/
    if (value < 0){
        value = (1 << MAX_BINARY_LENGTH) + value; /*Equivalent to num = 2^12 + num*/
    }

    /*Convert the integer to binary representation*/
    for (i = MAX_BINARY_LENGTH - 1; i >= 0; i--){
        binary[i] = '0' + (value & 1); /*Extract the least significant bit*/
        value >>= 1; /*Shift right to get the next bit*/
    }

    binary[MAX_BINARY_LENGTH] = '\0'; /*Null-terminate the string*/
	strcpy (data[*DC], binary);
	(*DC)++;

	return OK;
}

void createEntFile (char* filename, sym* symbols, int cur){
	int i;
	FILE *ofp; /*The file with the suffix ".ent"*/
	char new_file_name [MAX_LENGTH_LINE]; /*The name of the file + .ent*/

	strcpy (new_file_name, filename);
	strcat (new_file_name,".ent");
	/*Open the file with the suffix ".ent" for writing*/
	if ((ofp = fopen(new_file_name, "w")) == NULL){	
		printf ("Can't open file %s.ent\n", new_file_name);
	} 
	else{
		for (i=0; i<cur; i++)
			if (symbols[i].type == ENTRY)
				fprintf(ofp,"%s %d\n",symbols[i].name ,symbols[i].address);
		fclose(ofp);
	}
}

void buildSymbolsTable (sym ** st){
	*st = (sym*) malloc (SIZE*sizeof(sym));
	if (*st == NULL){
		printf("Not enough memory.\n");
		exit(0);
	}
}

void buildSymbolsToCheckTable (stc** st){
	*st = (stc*) malloc (SIZE*sizeof(stc));
	if (*st == NULL){
		printf("Not enough memory.\n");
		exit(0);
	}
}

void createExtFile (char* filename, sym* symbols, int cur, stc* symbols_to_check, int cur_stc){
	int i,j;
	FILE *ofp; /*The file with the suffix ".ext"*/
	char new_file_name [MAX_LENGTH_LINE]; /*The name of the file + .ext*/

	strcpy (new_file_name, filename);
	strcat (new_file_name,".ext");
	/*Open the file with the suffix ".ext" for writing*/
	if ((ofp = fopen(new_file_name, "w")) == NULL){	
		printf ("Can't open file %s.ext\n", new_file_name);
	} 
	else{
		for (i=0; i<cur_stc; i++){
			for (j=0; j<cur; j++){
				if (!strcmp(symbols_to_check[i].name, symbols[j].name))
					if (symbols[j].type == EXTERNAL)
						fprintf(ofp,"%s %d\n",symbols_to_check[i].name ,symbols_to_check[i].index + ADD_TO_MEMORY);
			}	
		}			
		fclose(ofp);
	}
}

/*The function gets a binary code and convert it to decimal*/
int binaryToDecimal(char* binary){
    int decimal = 0; /*The decimal number*/ 
	int i;

    for (i = HALF_BIN_MACHINE_CODE - 1; i >= 0; i--){
        int bit = binary[i] - '0';  /*Convert the character '0' or '1' to the corresponding integer value*/
        decimal += bit * pow(POW2, HALF_BIN_MACHINE_CODE - 1 - i);
    }

    return decimal;
}

void createObjFile(char instructions[][MAX_BINARY_LENGTH+1], char data[][MAX_BINARY_LENGTH+1], int IC, int DC, char* filename){
	char base64 [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; /*Base 64 array*/
	char new_file_name [MAX_LENGTH_LINE]; /*The file name with the suffix ".ob"*/
	char half [HALF_BIN_MACHINE_CODE+1]=""; /*Hald machine code*/
	char machineCode [MAX_BINARY_LENGTH+1]=""; /*Machine code*/
	int i;
	FILE *ofp; /*The file with the suffix ".ob"*/

	strcpy (new_file_name, filename);
	strcat (new_file_name,".ob");
	/*Open the file with the suffix ".ob" for writing*/
	if ((ofp = fopen(new_file_name, "w")) == NULL){	
		printf ("Can't open file %s.as\n\n", new_file_name);
	} 
	
	else {
		fprintf(ofp,"%d %d\n",IC,DC);	

		for (i=0; i<IC; i++){
			strcpy(machineCode,instructions[i]); 
			strncpy(half, machineCode, HALF_BIN_MACHINE_CODE); 
			fputc(base64[binaryToDecimal(half)], ofp); 
			strncpy(half, machineCode + HALF_BIN_MACHINE_CODE, HALF_BIN_MACHINE_CODE); 
			fputc(base64[binaryToDecimal(half)],ofp); 
			fputc('\n',ofp); 
		}

		for (i=0; i<DC; i++){
			strcpy(machineCode,data[i]); 
			strncpy(half, machineCode, HALF_BIN_MACHINE_CODE); 
			fputc(base64[binaryToDecimal(half)], ofp); 
			strncpy(half, machineCode + HALF_BIN_MACHINE_CODE, HALF_BIN_MACHINE_CODE); 
			fputc(base64[binaryToDecimal(half)],ofp); 
			fputc('\n',ofp); 
		}
		fclose(ofp);
	}
}

int cheack_symbols_table (char* s, sym* symbols, int cur, int type){
	int i;
	for (i=0; i<cur; i++){
		if ((strlen(s) == strlen(symbols[i].name)) && !strncmp(symbols[i].name,s,strlen(s))){
			if (type == EXTERNAL){
				if(symbols[i].type == EXTERNAL)
					return CONTINUE;
				return FALSE;
			}
			return FALSE;
		}
	}
	return TRUE;
}

void anomaly_check_in_memory_image (int IC, int DC, int* ERROR, int* error_memory_anomaly){
	if (*error_memory_anomaly == FALSE){
		if (IC + DC > MAX_NUM_MEMORY_CELLS){/*After all, 924 is the number of cells in the memory that we can insert the coding into (1023-100+1=924)*/
			*ERROR = TRUE;
			printf("A memory anomaly was detected\n");
		}
		*error_memory_anomaly = TRUE;
	}
}
