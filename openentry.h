#ifndef OPENENTRY_H
#define OPENENTRY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define EOS '\0'

/*created .ent file and writes all declarations of entry labels from user file.
	The function uses 'get_label_address'.
	@param FILE *, file to find entry labels
	@param char *, user file name (for name of ent file) */
void write_to_entry_file(FILE * ifp, char *);

/*This function receives label name(from entry line) from previous 
	function and finds declaration of label
	@param char *, label name to find declaration
	@param FILE *, file to find entry label declaration */	
int get_label_address(char *, FILE *);

#endif
