#ifndef SECONDPASS_H
#define  SECONDPASS_H
#define EOS '\0'

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*This function codes all question marks in 'array_code' table
	by using other functions in this file. The function isolates 
	each word and checks its type (label, number...).
	@param FILE *, pointer to file to find labels to code */
void process_file(FILE *);

/*This function checks if recieved number (in char) is legal.
	@param const char * , number to be checked
	@return int, returns 1 if legal number, else 0 */
int is_number(const char *);

/*This function checks if recieved string is legal.
	@param char * , string to be checked 
	@return int, returns 1 if legal string, else 0 */
int is_string(char *);

/*This function recieves word and checks if is in symbol table.
	@param char *, name of label to be checked
	@param FILE *, to use in function 'count_labels'
	@return int, returns address of label if lf found else 9999 */
int found_the_label_in_symbol_tabel(char *, FILE *);

/*This function receives label with its address, finds it in the symbol
	table and updates its address in array_code table (from question
	mark to label_address).
	@param int, line number in code_array
	@param int, word(label) number in line in code_array(code_line)
	@param int, address of word (label) 
	@param FILE *, to use in function 'count_labels' */
void find_label_without_defined(int, int, int, FILE * );

#endif	
