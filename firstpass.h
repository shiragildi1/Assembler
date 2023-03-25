#ifndef FIRSTPASS_H
#define  FIRSTPASS_H
#define EOS '\0'

#include<ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

/*This function sets the opcode array with the name
	and the number of paramaters that it receives. */
void firstpass();

/*sets the code array, reads file and codes 
	every line if necessary.
	@param FILE *, file to read from and code lines */
void set_array_code(FILE */*, int, int*/);

/*This function adds a number or ascii value to data table.
	@param short, number or letter to add to data table*/
void insert_data(short);

/*This function creates the data table using
	dynamic allocation*/
void set_array_data();

/*This function checks which funtion is used in the line.
	@param char *, word to check which opcode
	@return int, returns number of opcode according to 
	opcode table*/
int find_op_value(char *);

/*This function creates the label table using
	dynamic allocation*/
void set_array_labels(FILE *);

/*This function adds a label to label table.
	@param char *,name of label to add to data table
	@param int, address of label to add to data table */
void insert_label(char *, int);

/*This function how many numbers there in data line
	@param char *, line to be checked
	@param int, flag if line has label
	@return int, the amount of numbers in line */
int counter_data(char *, int);

/*This function codes one paramater opcode
	@param int, number of opcode
	@param char *, line to be checked
	@param int, IC
	@param int, DC
	@return int, returns the updated IC */
int one_par_code(int, char *, int, int);

/*This function checks that labels are not declared twice.
	@param FILE *, pointer to file(to count labels, uses "count_lables") */
void check_labels(FILE * ifp);

/*This function finds the labels that are declared in 
	data line and adds IC + 101 to their address.
	@param FILE *, pointer to file(to count labels, uses "count_lables") */
void fix_data_label_address(FILE *);

#endif

