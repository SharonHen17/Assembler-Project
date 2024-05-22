#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LENGTH_LINE 80
#define MAX_LENGTH_SYMBOL 31

struct Mcro {
	char name [MAX_LENGTH_LINE];/*The name of the mcro*/
	fpos_t s;/*The position of the mcro*/
};

typedef struct Mcro mcro;

struct command {
	char* name;
	char* opcode;
};

typedef struct command com;

struct symbol {
	char name[MAX_LENGTH_SYMBOL]; /*The name of the symbol*/
	int address; /*The address of the symbol*/
	int type; /*The symbol is relocatable/external/entry*/	
};

typedef struct symbol sym;

struct symbol_to_check {
	char name [MAX_LENGTH_SYMBOL]; /*The name of the symbol*/
	int index; /*The index in the instructions table*/
	int line; /*Line number in the file*/
};

typedef struct symbol_to_check stc;


enum {FALSE, TRUE, CONTINUE, DO};

enum {RELOCATABLE, EXTERNAL, DATA, ENTRY};

enum {START, ENT_PAR, EXT_PAR, COM_PAR, MCRO};
