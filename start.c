/*
* Student name: Sharon Hen.
* ID: 318607223.
* Date: 17/08/2023.
* The project is assembler software, for a predefined assembly language.
*/

/*Assumptions:
*'.extern' guideline can have more than one parameter (labels).
* '.entry' guideline can receive only one parameter (which is a label).
* In a macro definition there is at least one space/tab between 'mcro' and the name of the macro.
* In the macro definition, in the macro definition, 'mcro' can be preceded by only white characters and similarly also for 'endmcro'.
*/

/*On this page, we pick up the assembly files*/

#include "start.h" 

int main(int argc, char *argv[]){
/************************Setting Variables************************/
	FILE *ifp, *amfp; /*ifp- current file; amfp- after macro file*/
	char file_name [MAX_LENGTH_LINE]; /*file name, with the suffix '.as'*/
	char new_file_name [MAX_LENGTH_LINE]; /*file name, with the suffix '.am'*/
/*****************************************************************/

	if (argc == 1){   
		printf ("There is no assembly file\n");
 		return !OK;
	}

	while (--argc > 0)
	{
		strcpy (file_name, *++argv);     
		strcat (file_name,".as");

		if ((ifp = fopen(file_name, "r")) == NULL){
			printf ("Can't open file %s.as\n", *argv);
			continue;		
		}
		else{
			/*Build new file name, with the suffix '.am'*/		
			strcpy (new_file_name, *argv);
			strcat (new_file_name,".am");
			
			/*Open the file with the suffix '.am' for writing*/
			if ((amfp = fopen(new_file_name, "w")) == NULL){	
				printf ("Can't open file %s.am\n", *argv);
				continue;
			} 
			else{
				printf("*********************************%s*********************************\n", file_name);
				pre_as(ifp, amfp, *argv);
				fclose(ifp);
			}
		}
	}/*end while*/
	return OK;
}
