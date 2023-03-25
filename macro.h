#ifndef MACRO_H
#define MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define EOS '\0'
/* Struct for macros. Includes macros text, name, beggining line and end line*/
typedef struct macro
{
	char name[80];
	char * txt;
	int beg;
	int end;
}macro;

/* This function finds macro declarations and sets macro array using dynamic allocation.
	The function then sends the array to another function to open macros
	@param FILE pointer to read from users file
	@param char * , name of user file
	@retrurn char * , returns pointer to file with opened macros*/
FILE * macros(FILE *, char *);

/*This function is used my macros and open_macro to find
	macro declerations.
	@param char * , receives line to check if is macro declaration
	@return int, returns 1 if is macro decleration, 2 if is end of decleration
		and 0 if is regular line*/
int is_macro(char *);

/*This function opens new file and opens macros and deletes
	macro declarations.
	@param FILE *, pointer to users file to copy the text to new file
	@param macro* , pointer to array of macro strutures to copy macro text
	@param char *,  name of original users file
	@return FILE *,  returns pointer to new file with opened macros*/
FILE * open_macro(FILE *, macro *, int, char *);

/*This function checks that macro is declared porperly.
	@param char *, line to be checked if is legal
	@param macro*, pointer to array of macros */
void checking_errors(char *, macro *);

/*This function counts the macros in function by finding "mcr"
	@param FILE *, pointer to original files to find macros
	@return int, returns number of macros in file*/
int count_mcr(FILE *);

#endif
