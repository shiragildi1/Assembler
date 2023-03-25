#ifndef VALIDATE_H
#define VALIDATE_H
#define EOS '\0'

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*This function finds labels in file and checks that properly declared.
	@param FILE * ifp, pointer to file to check labels */
void check_label(FILE * ifp);

/*This function checks that there are'nt multiple colons after label.
	@param FILE * ifp, pointer to file to check colons */
void check_multiple_colons_in_line(FILE * ifp);

/*This function finds data lines and prints errors if there are any
	this function uses "is_valid_number" 
	@param FILE * ifp, pointer to file */
void process_data(FILE * ifp );

/*This function receives number (in char) and checks that it
	is legal. If not prints error.
	@param str, number to be checked
	@param count_line, number of data line */
void is_valid_number(char * str, int count_line);

/*This function receives data line and checks that all commas
	are legal. If not prints error.
	@param line, line to be checked
	@param line_num, number of data line */
void is_valid_commas(char * line, int line_num);

/*This function finds string lines and prints errors if there are any
	this function uses "is_valid_string" 
	@param FILE * ifp, pointer to file */
void process_string(FILE * ifp );

/*This function receives string line and checks that string is
	legal. If not prints error.
	@param str, string to be checked
	@param line_num, number of data line */
void is_valid_string(char * str, int line_num);

/*This function receives word and checks if label is
	valid. If not prints error.
	@param word, label to be checked 
	@return int, 1 if label is legal, else 0*/
int is_valid_label(char * word);

/*This function receives word and checks if is valid
	number. If not prints error.
	@param word, label to be checked 
	@return int, 1 if label is legal, else 0*/
int is_valid_number_sulamit(char * word);

/*This function receives word and checks if is valid
	register. If not prints error.
	@param word, label to be checked 
	@return int, 1 if label is legal, else 0*/
int is_register(char* word);

/*This function finds .entry lines and uses "check" to
	find definition of label in file.
	@param FILE * ifp, pointer to file */
void process_entry(FILE * ifp );

/*This function finds .entry lines and uses "check" to check
	that labels are not defined in file.
	@param FILE * ifp, pointer to file */
void process_extern(FILE * ifp );

/*This function receives label (from .entry/.extern line) and checks
	if label is defined in file.
	@param word, label to be checked
	@param FILE * ifp, pointer to file 
	@return int, 1 if definition found, else 0*/
int check(char * word, FILE * ifp);

/*These functions recieve line from after .entry/.extern 
	and checks that line is legal.
	@param str, line to be checked
	@param line_num, number of line
	@param ifp, "check" used ifp */
void is_valid_entry(char * str, int line_num, FILE * ifp );
void is_valid_extern(char * str, int line_num, FILE * ifp );

/* This function reads opcode lines from file 
	and sends to the right function for process
	@param ifp, pointer to file to find opcode lines*/
void process_opcode(FILE * ifp);

/*These function recieve a line to process. (opcode with same legal
	operands will go to the same function (ex: mov, sub, add)
	@param str, line to process
	@param line_num, line number */
void is_valid_mov(char * str, int line_num);
void is_valid_cmp(char * str, int line_num);
void is_valid_not(char * str, int line_num);
void is_valid_lea(char * str, int line_num);
void is_valid_jmp(char * str, int line_num);
void is_valid_prn(char * str, int line_num);
void is_valid_rts(char * str, int line_num);
/*Till here functions to process opcode */

/*These functions recieve word to check whether operand is 
	legal or not , if there is error, will be printed.
	"check013" for number, label and register.
	"check13" for label and register.
	@param word, operand to check
	@param line_num, operand line number
	@return int, if operand is legal returns 1 else 0 */
int check013(char * word, int line_num);
int check13(char * word, int line_num);

/*These functions checks that 2nd addressing method is legal, errors will
	be printed if there are any.
	"check_jmp_bne_jsr" checks that operands in paranthases are legal.
	"check_space_jmp_bne_jsr" checks that there are'nt any illegal spaces.
	@param word, operand to check
	@param line_num, operand line number */
void check_jmp_bne_jsr(char * str, int line_num);
void check_space_jmp_bne_jsr(char * str, int line_number);

#endif

