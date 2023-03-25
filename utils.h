#ifndef UTILS_H
#define UTILS_H

#define EOS '\0'
#define MAX_LABEL_NAME 31
#define MAX_LINE 81
#define NUM_OF_OPCODES 17

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "globals.h"

/* This function imitates fgets.
	@param FILE *, pointer to file to be read from
	@retrurn char *, returns line */
char * read_line(FILE * ifp);

/*This function counts number of labels in file.
	@param FILE *, counts label in this file
	@return int, return amount of labels */
int count_labels(FILE *ifp);

/*This function checks if line has non white space characters.
	@param char *, line to be checked
	@return int, returns 1 if has non white space, else 0 */
int hasNonWhitespace(char *, int);

#endif
