/*On this page there is a collection of auxiliary functions for the preassembler process*/

#include "func_pre_as.h"

char* skipSpacesAndTabs(char *s) {
    int i;
    for (i = 0; s[i] && (s[i] == ' ' || s[i] == '\t'); i++) {} /*Count how many spaces there are at the beginning*/
    return s + i;
}

int extractMacroName (char *s, char *t, mcro* mt, int cur, int LC, com* commands){
	int len, i; /*len- holds the length of the mcro name*/
	s = skipSpacesAndTabs(s);
	if(*s == '\n' || *s == '\0'){
		printf("Error in line %d: Defining a mcro without giving it a name\n", LC);
		return !OK;
	}

	if ((len=is_it_a_label (s, commands, MCRO)) == FALSE){
		printf("Error in line %d: Invalid mcro name\n", LC);
		return !OK; 
	}

	/*The same mcro name must not be defined more than once*/
	for (i=0; i<cur; i++){
		if (len == strlen(mt[i].name) && !strncmp(mt[i].name,s,len)){
			printf("Error in line %d: Mcro name must not be defined more than once\n", LC);
			return FALSE;
		}
	} 
	strncpy(t, s, len);
	s = s + len;
	s = skipSpacesAndTabs(s);
	if(*s != '\n' && *s != '\0'){
		printf("Error in line %d: There must be no additional characters in the macro definition line\n", LC);
		return !OK;
	}
	return OK;
}

int skipAfterEndMcro (FILE* f, int* LC){
	char st [MAX_LENGTH_LINE]; /*input line*/
	char* temp; /*temporary pointer on the input line*/

	/*The assumption is that the mcro definition is correct, that is, we will definitely "encounter" the endmcro line, 
	so the loop will definitely end at some point*/
	while (fgets(st, sizeof(st), f) != NULL) {
		(*LC) ++;
		if ((temp = strstr(st, "endmcro")) != NULL){
			temp = temp + strlen("endmcro");
			if (*temp == ' ' || *temp == '\t' || *temp == '\n' || *temp == '\0'){
				/*The line is the end of a mcro definition*/
				temp = skipSpacesAndTabs(temp);
				if(*temp != '\n' && *temp != '\0'){
					printf("Error in line %d: There must be no additional characters in the mcro end line\n", *LC);
					return !OK;
				}
				return OK;
			}
		}
	}
	return OK;
}

void buildMcroTable (mcro** st){
	*st = (mcro*) malloc (SIZE*sizeof(mcro));
	if (*st == NULL){
		printf("Not enough memory.\n");
		exit(0);
	}
}

void addMcro(char* s, fpos_t t, mcro** mt, int* cur){
	mcro m; /*The mcro we will add to the table*/
	strcpy(m.name, s);
	m.s = t;
	
	(*mt)[*cur] = m;
	(*cur) ++;
}

void upSpaceMT(mcro** mt, int* max){
	mcro* temp; /*A temporary pointer to the mcro table*/

	*max += SIZE;
	temp = (mcro*) realloc (*mt, (*max)*sizeof(mcro));

	if (temp == NULL){
		printf("Not enough memory.\n");
		exit(0);
	}
	*mt = temp;
}

int checkingIfLineIsCallingToMc(char* line, mcro* mt, int size){
    int i;
	char* s_mcro_name; /*A pointer to the start of the mcro calling (if it was found)*/
	char ch;

	/*We assume that each line of input in the original file has at most one call to a macro*/

    for (i = 0; i < size; i++){
		if ((s_mcro_name = strstr(line, mt[i].name)) != NULL){
			ch = *(s_mcro_name + strlen(mt[i].name));
			if ((ch == ' ' || ch == '\t' || ch == '\n' || ch == '\0') && (s_mcro_name == line || *(s_mcro_name-1) == '\t' || *(s_mcro_name-1) == ' '))
				return i;
		}
    }
    return -1; /*This line isn't a mcro's calling*/
}

void replaceMcroName (char* line, mcro* mt, int i, FILE* ifp, FILE* amfp){
	char st [MAX_LENGTH_LINE + 1]; /*+1 for the null terminator*//*input line*/
	char* s_mcro_name; /*A pointer to the start of the mcro calling*/
	char* temp; /*temporary pointer on the input line*/

	temp = line;
	s_mcro_name = strstr(line, mt[i].name);
	while (temp != s_mcro_name){
		fputc (*temp, amfp);
		temp++;
	}

	fsetpos (ifp, &(mt[i].s));/*Go back in the file to the line where the mcro was defined*/
	while (fgets(st, sizeof(st), ifp) != NULL) {	
		if (strstr(st, "endmcro") == NULL){
			fputs (st, amfp);
		}
		else break;
	}
	
	temp += strlen(mt[i].name);
	while (*temp != '\n' && *temp != '\0'){
		fputc (*temp, amfp);
		temp++;
	}
	fputc (*temp, amfp);
}
