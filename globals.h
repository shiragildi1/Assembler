#ifndef GLOBALS_H
#define GLOBALS_H

/* Struct for labels, includes label name and address */
typedef struct symbol
{
	char label_name[MAX_LABEL_NAME];
	int label_address;
}symbol;

symbol * labels;

short * data_table;/* array of data */

/* Struct for code, includes address of first line and array
	for all code lines for current line in file */
typedef struct code
{
	short * code_line;
	int address;
	int num_of_words;
}code;
code * codes;

/* Struct for opcode, holds name and number of operands for each function */
typedef struct Opcode
{
	char *opName;
	int count_parameter;
	
}Opcode;
Opcode opcode[NUM_OF_OPCODES];

int IC;/*instruction counter*/
int DC;/*data counter*/
int error; /*flag, if equals 1 dont create output files*/

#endif
