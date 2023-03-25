#ifndef IMPROVE_H
#define IMPROVE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define EOS '\0'

/* This function creates a temporary files and erases
	extra white spaces from file.
	@param FILE *, delete extra white spaces from there
	@return FILE *, returns pointer to new cleared file */
FILE * erase_white_space(FILE *);

/* This function creates a temporary files and erases
	labels with no effect (before .entry \ .extern).
	@param FILE *, delete extra labels from there
	@return FILE *, returns pointer to new cleared file */
FILE * delete_stray_labels(FILE *);

/*This function is used by "delete_stray_labels" to check that
	label in line of entry / extern is not declared more than once.
	@param pos, pos to know that is not the same line as entry / extern
	@param label_name, label to be checked
	@param ifp, file to check label declarations */
void check_if_label_exists(long pos, char * label_name, FILE * ifp);

/* This function adds space after label declaration if missing.
	@param FILE *, add space if necessary to this file
	@return FILE *, returns pointer to new file with added spaces */
FILE * add_space(FILE *);

/* This function creates a temporary files and erases
	comment lines from given file.
	@param FILE *, delete comment lines from there
	@return FILE *, returns pointer to new cleared file */
FILE * remove_comments(FILE *);

/*This function checks that no line is longer than allowed(81)
	@param FILE *, pointer to file to read lines */
void check_length(FILE * ifp);

#endif
