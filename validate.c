#include "validate.h"
#include "utils.h"

void check_label(FILE *ifp)
{
	char line[MAX_LINE];/*An array of size MAX_LINE that holds the current row*/
	char copy[MAX_LINE];/*A copy array of size MAX_LINE that holds the current row*/
	char* word = NULL;/*Holds every time a circumcision and the initiation of the word to Nall*/
	int count_line = 0;/*Holds the row number*/
	int i;/*index*/
	fseek(ifp,0,SEEK_SET);
	while (fgets(line, sizeof(line), ifp)) /* pass line on the file */ 
	{
		count_line++;
		strcpy(copy, line);
		word = strtok(copy, ")(,\t\n"); /* define word variable here */
		while (word != NULL) 
		{
			if ((strchr(word, ':') != NULL))/*if this is label*/ 
			{
				int j = 0;
				word = strtok(word, ":");/*The label without the colon*/
				if(strlen(word) > MAX_LABEL_NAME)/*If the size of the label is longer than MAX_LABEL_NAME we will return an error*/
				{
					fprintf(stderr, "ERROR - in line %d: Illegal label name, %s, the length big too much\n\n", count_line, word);
					error = 1;
					return;
				}
				for(j = 0; j < 16; j++)/*If the label is the name of an opcode we will return an error*/
				{
					if((strcmp(word, (opcode[j]).opName) == 0))
					{
						fprintf(stderr, "ERROR - in line %d: Illegal label name, %s, name of opcode\n\n", count_line, word);
						error = 1;
						return;
					}
				}
				if((strcmp(word, "extern") == 0) || (strcmp(word, "entry") == 0) || (strcmp(word, "data") == 0) || (strcmp(word, "string") == 0))
				/*If the label is the name of a reserved word we will return an error*/
				{
					fprintf(stderr, "ERROR - in line %d: Illegal label name, %s, name is data / string\n\n", count_line, word);
					error = 1;
					return;
				}
				if((strchr(word, '#') != NULL) || (strchr(word, '-') != NULL) || (strchr(word, '+') != NULL) || (strchr(word, '.') != NULL))
				/*If the label is an invalid character we will return an error*/
				{
					fprintf(stderr, "ERROR - in line %d: Illegal label name, %s containt illegal character\n\n", count_line, word);
					error = 1;
					return;
				}
				if((word[0] == 'r') && (word[1] >= '0') && (word[1] <= '7') && (strlen(word) == 2))/*If it is a register it is an invalid label and we will return an error*/
				{
					fprintf(stderr, "ERROR - in line %d: Illegal label name, %s is name of register\n\n", count_line, word);
					error = 1;
					return;
				}
				if (word[0] < 'A'  ||  (word[0] > 'Z' && word[0] < 'a')  ||  word[0] > 'z')
				/*Checking if the label starts with an uppercase or lowercase letter and if not we will return an error.*/ 
				{
					fprintf(stderr, "ERROR - in line %d: Illegal label name, %s has to begin with letter\n\n", count_line, word);
					error = 1;
					return;
				}
				
				for (i = 1; word[i] != '\0'; i++) 
				{
					if (!isalnum(word[i]))/*Checking if the label has a character that is not a number or letter will return an error*/
					{
						fprintf(stderr,"ERROR - in line %d: Illegal label name, %s included illegal character in index %d\n\n", count_line, word, i);
						error = 1;
						return;
					}
				}
			}
			word = strtok(NULL, " )(,\t\n");/*hold the next word*/
		}
	}
}

void check_multiple_colons_in_line(FILE *ifp)
{
	char line[MAX_LINE];/*An array of size MAX_LINE that holds the current row*/
	int line_num = 0;/*Holds the row number*/
	int i;/*index*/
	int colon_count;/*Holds the number of colons in the word*/
	fseek(ifp, 0, SEEK_SET);
	while (fgets(line, sizeof(line), ifp)) 
	{
		line_num++;
		colon_count = 0;
		for (i = 0; line[i]; i++) 
		{
			if (line[i] == ':') 
			{
				colon_count++;
			}
			if (colon_count > 1)/*If there is more than one colon in a row it means that there is more than one hatch label in a row and we will return an error*/ 
			{
				fprintf(stderr, "ERROR: more than one colon in line %d\n", line_num);
				error = 1;
				return;
			}
		}
	}
}

void process_data(FILE *ifp) 
{
	char line[MAX_LINE];/*An array of size MAX_LINE that holds the current row*/
	int is_data_section = 0;/*will hold one if found in a data file*/
	int count_line = 0;/*Holds the row number*/
	char *data_start;/*where number start*/
	char data_line[MAX_LINE];/*Holds the continuation of the line after the data*/
	char *data_ptr = NULL;/*Pointer to the location of the data*/ 
	fseek(ifp, 0, SEEK_SET);
	 
	while (fgets(line, sizeof(line), ifp)) 
	{
		count_line++;
		data_ptr = strstr(line, ".data"); 
		
		if (data_ptr) 
		{
			is_data_section = 1;
			data_start = data_ptr + strlen(".data");
			
			strcpy(data_line, data_start);
			
			if (is_data_section) 
			{ 
				is_valid_commas(data_line, count_line);/*Checking whether the line is valid*/
				is_valid_number(data_line, count_line);/*Checking whether the numbers are valid*/
			}
		}
	}
}

void is_valid_number(char *str, int line_number) 
{
	int i;/*index*/
	char *number;/*hold number*/
	int j;/*index*/
	int has_decimal_point = 0;/*if the number ud decimal*/
	if(str[0] != ' ')/*if missing a space btween data end the first number return error*/
	{
		fprintf(stderr, "ERROR: there is no space between .data end number on line %d\n", line_number);
		error = 1;
		return;
	}
	number = strtok(str," ,\n");/*seperated the number by comma*/
	if (number == NULL)/*if after the data there is no anithing*/ 
	{ 
		fprintf(stderr, "ERROR: invalid number on line %d, empty string\n", line_number);
		error = 1;
		return;
	}

	while(number != NULL)/*The loop checks each number after the data*/ 
	{
		if((atoi(number) < -8192)  ||  (atoi(number) > 8191))/*check if the numer is legal*/
		{
				fprintf(stderr, "ERROR: invalid number on line %d,number is out of limits\n" , line_number);
				error = 1;
				return;
		} 
		if (number[0] == '-'  ||  number[0] == '+'  ||  isdigit(number[0]))/*if there is - or + start the check from next index*/
		{
			i = 1;
		}
		else/*if ther is no - or + in begining the number is illegal number*/ 
		{
			fprintf(stderr, "ERROR: invalid number on line %d, character 0 of number %s\n", line_number, number);
			error = 1;
			return;
		}

		for (j = 0; j < strlen(number); j++)/*the loop check if the number is decimal number or not*/ 
		{
			if (number[j] == '.') 
			{
				has_decimal_point = 1;
				break;
			}
		}

		for (; i < strlen(number); i++)/*check the number end return error if there is a point*/ 
		{
			if (!isdigit(number[i])) 
			{	
				if (has_decimal_point) 
				{
					fprintf(stderr, "ERROR: invalid number on line %d, duplicate decimal point in number %s\n", line_number, number);
					error = 1;
					return;
				}
				else 
				{
					fprintf(stderr, "ERROR: invalid number on line %d, character %d of number %s\n", line_number, i, number);
					error = 1;
					return;
				}
				break;
				
			}
		}
		number = strtok(NULL," ,\n");/*hold the next number*/
	}
}

void is_valid_commas(char* line, int line_num) 
{
	int commas = 0;/*hold the number of comma*/
	int len = strlen(line);/*hold the length of line*/
	int i;/*index*/
	int flag = 0;/*check if missing comma*/
	
	
	if ((line[0] == ',') || (line[0] == ' ' && line[1] == ',')) /*if thre is comma befor the first number return error*/
	{
		fprintf(stderr,"ERROR- in line %d: comma before first number\n", line_num);
		error = 1;
		return;
	}
	if ((line[len-2] == ',')||((line[len-3] == ',')&& (line[len-2]== ' ')))/*if thre is comma after the first number return error*/
	{
		fprintf(stderr, "ERROR- in line %d: comma after last number\n", line_num);
		error = 1;
		return;
	}

	for (i = 0; i < len-1; i++)/*the loop passes on the line end checking a commas*/ 
	{
		if(isdigit(line[i]) && (line[i+1] == ' '))
		{
			flag = 1;
		}
		if(line[i] == ',')
		{
			flag = 0;
		}
		if(isdigit(line[i+1]) && (flag == 1))
		{
			fprintf(stderr,"ERROR- in line %d: missing comma between numbers\n",line_num);
			error = 1;
			return;
		}
		if (line[i] == ',' && line[i+1] == ',') 
		{
			commas++;
		}
		if(i< (strlen(line)-2))
		{
			if ((line[i] == ',' && line[i+1] == ' ' && (line[i+2] == ',')))
			{
				commas++;
			}
		}
	}
	
	 if(commas > 0)/*if there ara more commas there allwed return error*/
	{
		fprintf(stderr,"ERROR- in line %d: more than one comma between numbers\n", line_num);
		error = 1;
		return;
	}
}

void process_string(FILE *ifp) 
{
	char line[MAX_LINE];
	int is_string_section = 0;
	int count_line = 0;
	char * string_start;
	char string_line[MAX_LINE];
	char * string_ptr = NULL;
	
	fseek(ifp, 0, SEEK_SET);
	 
	while (fgets(line, sizeof(line), ifp)) 
	{
		count_line++;
		string_ptr = strstr(line, ".string"); 
		
		if (string_ptr) 
		{
			is_string_section = 1;
			string_start = string_ptr + strlen(".string");
			
			strcpy(string_line, string_start);
			
			if (is_string_section) 
			{ 
				is_valid_string(string_line, count_line);
			}
		}
	}
}

void is_valid_string(char * str, int line_num) 
{
	int len = strlen(str);/*holds the length of line*/
	int i;/*index*/
	int num_quotes = 0;/*hold number of quotation mark*/
	
	if(str[0] != ' ')/*if thre is no space btween .string to the string*/
	{
		fprintf(stderr, "ERROR: there is no space between .string end number on line %d\n", line_num);
		error = 1;
		return;
	}
	for(i = 0; i < len; i++)
	{
		if(str[i] == '"')/*if there is a quotation mark*/
		{
			num_quotes++;
			if (num_quotes == 2 && i + 1 < len && str[i + 1] != ' ' && str[i + 1] != '\n' && str[i + 1] != '\r')/*if after the quote there is a tavs*/ 
			{
				fprintf(stderr,"ERROR- Invalid character after quotation mark on line %d\n", line_num);
				error = 1;
				return;
			}
			if(num_quotes == 2 && i + 1 < len && str[i + 1] == ' ' && str[i + 2] != ' '   && str[i + 2] != '\n' && str[i + 2] != '\r') 
			{
				fprintf(stderr,"ERROR- Invalid character after quotation mark on line %d\n", line_num);/*if thre is tavs after the quotation mark*/
				error = 1;
				return;
			}
			if(num_quotes == 2 && i + 1 < len && str[i + 1] == ' ' && str[i + 2] == ','   && str[i + 2] != '\n' && str[i + 2] != '\r')/*if thre is too much operands*/ 
			{
				fprintf(stderr,"ERROR- there is too much operands %d\n", line_num);
				error = 1;
				return;
			}
			if(num_quotes == 2 && i + 1 < len && str[i + 1] != ' ' && str[i + 1] == ','   && str[i + 1] != '\n' && str[i + 1] != '\r') 
			{
				fprintf(stderr,"ERROR- there is too much operands %d\n", line_num);/*if thre is too much operands*/
				error = 1;
				return;
			}
			
		}
	}
		
		if(num_quotes > 2)/*if there are more then quotation mark*/
		{
			fprintf(stderr,"ERROR- string has more than two quotation marks on line %d\n", line_num);
			error = 1;
			return;
		}
		if(num_quotes < 2)/*if there are less then quotation mark*/
		{
			fprintf(stderr,"ERROR- string has less than two quotation marks on line %d\n", line_num);
			error = 1;
			return;
		}
}
void process_entry(FILE *ifp) 
{
	char line[MAX_LINE];
	long pos;
	int is_entry_section = 0;
	int count_line = 0;
	char * entry_start;
	char entry_line[MAX_LINE];
	char * entry_ptr = NULL;
	fseek(ifp, 0, SEEK_SET);
	 
	while (fgets(line, MAX_LINE, ifp) != NULL) 
	{
		count_line++;
		entry_ptr = strstr(line, ".entry "); 
		
		if (entry_ptr) 
		{
		is_entry_section = 1;
		entry_start = entry_ptr + strlen(".entry ");
		
		strcpy(entry_line, entry_start);
		
		if (is_entry_section) 
		{ 
			pos = ftell(ifp);
			is_valid_entry(entry_line, count_line,ifp);
			fseek(ifp,pos,SEEK_SET);
		}
		}
		if(feof(ifp)) 
		{
			break;
		}
	}
}

void is_valid_entry(char * str, int line_num,FILE *ifp)
{
	char *word;/*hold the word after .entry*/
	
	word = strtok(str,",\n ");/*hold the word until ,*/
	
	if(word != NULL)/*if word there is no NULL*/
	{
		
		if(is_valid_label(word)  == 0)/*if tht word is illegal*/
		{
			fprintf(stderr,"ERROR- the word %s illegal for the label, in line %d\n",word,line_num);
			error = 1;
			return;
		}
		if((word[strlen(word)]-1 == ' ') && (word[strlen(word)] == ','))/*if there is too much operands*/
		{
			fprintf(stderr,"ERROR- there is too much operands, in line %d\n",line_num);
			error = 1;
			return;
		}
		if(check(word,ifp) == 0)/*if the check not fount the defination of label*/
		{
			fprintf(stderr,"ERROR- the label %s there after .entry is not defined, in line %d\n" ,word,line_num);
			error = 1;
			return;
		}
	}
	
}

int check(char *word, FILE *ifp) 
{
	char line[MAX_LINE];
	char *token;
	fseek(ifp, 0, SEEK_SET);
	while (fgets(line, MAX_LINE, ifp) != NULL) 
	{
		token = strtok(line, ":"); 
		if(token !=NULL)
		{
			if (strcmp(token, word) == 0)/*if label defination found in file*/ 
			{ 
				return 1;
			}
		}
		
	}
	return 0;/*if label defination found in file*/
}

void process_extern(FILE *ifp) 
{
	char line[MAX_LINE];
	long pos;
	int is_extern_section = 0;
	int count_line = 0;
	char * extern_start;
	char extern_line[MAX_LINE];
	char * extern_ptr = NULL;
	fseek(ifp, 0, SEEK_SET);
	 
	while (fgets(line, MAX_LINE, ifp) != NULL) 
	{
		count_line++;
		extern_ptr = strstr(line, ".extern "); 
		
		if (extern_ptr) 
		{
		is_extern_section = 1;
		extern_start = extern_ptr + strlen(".extern ");
		
		strcpy(extern_line, extern_start);
		
		if (is_extern_section) 
		{ 
			pos = ftell(ifp);
			is_valid_extern(extern_line, count_line,ifp);
			fseek(ifp,pos,SEEK_SET);
		}
		}
		if(feof(ifp)) 
		{
		break;
		}
	}
}

void is_valid_extern(char * str, int line_num,FILE *ifp)
{
	char *word;/*holds the word after .extern*/
	
	word = strtok(str,",\n ");
	
	if(word!=NULL)
	{
		
		if(is_valid_label(word)  == 0)
		{
			fprintf(stderr,"ERROR- the word %s illegal for the label, in line %d\n",word,line_num);
			error = 1;
			return;
		}
		if((word[strlen(word)]-1 == ' ') && (word[strlen(word)] == ','))
		{
			fprintf(stderr,"ERROR- there is too much operands, in line %d\n",line_num);
			error = 1;
			return;
		}
		if(check(word,ifp) == 1)/*if the check fount the defination of label*/
		{
			fprintf(stderr,"ERROR- the label %s illegal defenition of extern label declared in line %d\n" ,word,line_num);
			error = 1;
			return;
		}
	}
}

int is_valid_number_sulamit(char* word)
{
	int num;/*holds number after atoi*/
	const char* num_str;/*holds the number in char*/
	int i;/*index*/
	num_str = word + strlen("#");/*holds the number in char whithout hashtag*/
	
	for(i = 0; num_str[i] != '\0'; i++)/*pass on the  num_str*/ 
	{
		if(!isdigit(num_str[i])) 
		{
			if(i == 0) 
			{
				if((num_str[0] == '-') || (num_str[0] == '+'))
				{
					continue;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				if((i == strlen(num_str)-1) && (num_str[i] == ' '))
				{
					continue;
				}
				else
				{
					return 0;	
				}
			} 
		}
	}
	num = atoi(num_str);/*change the number(character) to number*/

	if(num > 2047  ||  num < -2048)/*check if the number in rang*/ 
	{
		return 0; 
	}
	return 1;
}

int is_register(char* word)/*chaking the register*/ 
{
	int num = 0;/*holds the number*/
	int len = strlen(word);/*holds the length of len*/
	int i;
	
	if((word[0] != ' ') && (word[0] != 'r'))/*if there is no 'r' in first place*/
	{
	
		return 0;	
		
	}
	else if((word[0] == ' ') && (word[1] != 'r'))/*if there is no 'r' in first place*/
	{
		return 0;
	}
	for (i = 1; i < len; i++) 
	{
		if (isdigit(word[i])) 
		{
			num = num * 10 + (word[i] - '0');/*add the next character to number*/
		} 
		else 
		{
			if((i == strlen(word)-1) && (word[i] == ' '))
			{
				return 1;
			}
			return 0; 
		}
	}
	if (num < 0  ||  num > 7)/*if the register is not btween 0 and 7*/
	{
		return 0;
	}
	return 1; 
}

int is_valid_label(char* word) 
{
	int i;/*index*/
	if(isdigit(word[0]))/*if isn't letter in first palace*/
	{
		return 0;
	}
	for (i = 0; i < strlen(word); i++) 
	{
		if (!isalpha(word[i]) && !isdigit(word[i])) 
		{
			if(((i == 0)||(i == strlen(word)-1)) && (word[i] == ' '))
			{
				return 1;
			}
			return 0;/*error if is not digit or letter*/
		}
	}
	return 1; 
}

void process_opcode(FILE *ifp) 
{
	char line[MAX_LINE];/*holds line*/
	int count_line = 0;/* holds line number*/
	fseek(ifp, 0, SEEK_SET);
	 
	while (fgets(line, sizeof(line), ifp)) 
	{
		int i;/*index*/
		char opcode_line[MAX_LINE] = {0};/*holds line from opcode*/
		char *opcode_ptr = NULL; /*holds line from after opcode*/
		count_line++;
		for(i = 0; i < 16; i++)
		{
			if(strstr(line, opcode[i].opName))
			{
				opcode_ptr = strstr(line, opcode[i].opName);
				strncpy(opcode_line, &(opcode_ptr[4]), strlen(opcode_ptr) - 4);/*copies to 'opcode line' from after opcode name*/
				if(i == 0 || i == 2 || i == 3)/*mov, add, sub*/
				{
					is_valid_mov(opcode_line, count_line);
				}
				else if(i == 1)/*cmp*/
				{
					is_valid_cmp(opcode_line, count_line);
				}
				else if(i == 4 || i == 5 || i == 7 || i == 8 || i == 11)/*not, red, dec, clr, inc*/
				{
					is_valid_not(opcode_line, count_line);
				}
				else if(i == 12)/*prn*/
				{
					is_valid_prn(opcode_line, count_line);
				}
				else if(i == 6)/*lea*/
				{
					is_valid_lea(opcode_line, count_line);
				}
				else if(i == 14 || i == 15)/*rts, stop*/
				{
					if(strlen(opcode_ptr) > 4)
					{
						strncpy(opcode_line, &(opcode_ptr[5]), strlen(opcode_ptr) - 5);
						is_valid_rts(opcode_line, count_line);
					}
				}
				else if(i == 9 || i == 10 || i == 13)/*jmp, bne, jsr*/
				{
					is_valid_jmp(opcode_line, count_line);
				}	
			}
		}
	}
}

void is_valid_mov(char * str, int line_num) 
{
	char copy[MAX_LINE];
	char *word = NULL;
	int number_operands = 0;
	
	strcpy(copy, str);
	word = strtok(copy, ",\n");
	if(check013(word, line_num) == 0)/*check source operand*/
	{
		return;
	}
	
	while (word != NULL) 
	{
		number_operands++;
		word = strtok(NULL, ",\n");
		if (word != NULL) 
		{
			if(word[0] == ' ')/*if space take word from index 1*/
			{
				char word1[MAX_LINE] ={0};
				strncpy(word1,&(word[1]),strlen(word)-1);
				strcpy(word,word1);
			}
			if(check13(word, line_num) == 0)/*check second operand*/
			{
				return;
			}
			
		}
	}
	if (number_operands != 2)/*if too many operands*/ 
	{
		fprintf(stderr, "ERROR: The number of operands does not match the number of operands that should be, in line %d\n", line_num);
		error = 1;
	}
}

void is_valid_cmp(char* str, int line_num) 
{
	char copy[MAX_LINE];
	char *word = NULL;
	int number_operands = 0;
	
	strcpy(copy, str);
	word = strtok(copy, " ,\n");
	
	if(check013(word, line_num) == 0)
	{
		return;
	}
	 
	while (word != NULL) 
	{
		number_operands++;
		word = strtok(NULL, " ,\n");
		
		if (word != NULL) 
		{
			if(word[0] == ' ')
			{
				char word1[MAX_LINE] ={0};
				strncpy(word1,&(word[1]),strlen(word)-1);
				strcpy(word,word1);
			}
			if(check013(word, line_num) == 0)
			{
				return;
			}
		}
	}
	if (number_operands != 2) 
	{
		fprintf(stderr, "ERROR: The number of operands does not match the number of operands that should be, in line %d\n", line_num);
		error = 1;
		return;
	}
}

void is_valid_not(char* str, int line_num) 
{
	char copy[MAX_LINE];
	char *word = NULL;
	int number_operands = 0;
	
	strcpy(copy, str);
	word = strtok(copy, " ,\n");
	if(check13(word, line_num) == 0)
	{
		return;
	}
	 
	while (word != NULL) 
	{
		number_operands++;
		word = strtok(NULL, " ,\n");
		
		if (word != NULL) 
		{
			fprintf(stderr, "ERROR: illegal character after function no operand allowed, in line %d\n",line_num);
			error = 1;
			return;	
		}
	}
}

void is_valid_lea(char* str, int line_num) 
{
	char copy[MAX_LINE];
	char *word = NULL;
	int number_operands = 0;
	
	strcpy(copy, str);
	word = strtok(copy, " ,\n");
	if((is_register(word) == 1))
	{
		fprintf(stderr, "ERROR: the label-Invalid operand for source operand, in line %d\n", line_num);
		error = 1;
		return;
	}
	if(is_valid_label(word) == 0)
	{
		fprintf(stderr, "ERROR: the label-Invalid operand for source operand, in line %d\n", line_num);
		error = 1;
		return;
	}

	while (word != NULL) 
	{
		number_operands++;
		word = strtok(NULL, " ,\n");
		
		if (word != NULL) 
		{
			if(word[0] == ' ')
			{
				char word1[MAX_LINE] ={0};
				strncpy(word1,&(word[1]),strlen(word)-1);
				strcpy(word,word1);
			}
			if(check13(word, line_num) == 0)
			{
				return;
			}
		}
	}
	if (number_operands != 2) 
	{
		fprintf(stderr, "ERROR: The number of operands does not match the number of operands that should be, in line %d\n", line_num);
		error = 1;
		return;
	}
}

void is_valid_jmp(char * str, int line_num) 
{
	char copy[MAX_LINE];
	char * word = NULL;
	int index = 0;
	strcpy(copy, str);
	word = strtok(copy, " ,\n");
	if(strchr(str,'(') != NULL)/*if second addressing method*/
	{
		while(str[index]!= ')')
		{
			index++;
		}
		index++;
		if((str[index] != EOS) && (str[index] != '\n') && ((str[index+1] != '\n')))
		{
			if(str[index] == ' ')
			{
				if(str[index+1] != EOS)/*if illegal character after parentheses*/
				{
					fprintf(stderr,"ERROR- illegal character after parentheses,in line %d\n", line_num);
					error = 1;
					return;
				}
			}
			else
			{
				fprintf(stderr,"ERROR- illegal character after parentheses,in line %d\n", line_num);
				error = 1;
				return;
			}
		}
		else
		{
			check_space_jmp_bne_jsr(str,line_num);
			check_jmp_bne_jsr(str,line_num);
		}
	}
	else
	{
		while (word != NULL) 
		{
			if(check13(word, line_num) == 0)/*check operand*/
			{
				return;
			}
			word = strtok(NULL, " ,\n");
			if (word != NULL) 
			{
				fprintf(stderr, "ERROR: The number of operands does not match the number of operands that should be , in line %d\n",line_num);
				error = 1;
				return;
			}
		}
	}
}

void is_valid_prn(char* str, int line_num) 
{
	char copy[MAX_LINE];
	char *word = NULL;
	int number_operands = 0;
	
	strcpy(copy, str);
	word = strtok(copy, ",\n");
	if(check013(word, line_num) == 0)
			{
				return;
			}
	 
	while (word != NULL) 
	{
		number_operands++;
		word = strtok(NULL, " ,\n");
		
		if (word != NULL) 
		{
			
			fprintf(stderr, "ERROR: illegal character after function no operand allowed, in line %d\n",line_num);
			error = 1;
			return;
		}
	}
}

void is_valid_rts(char* str, int line_num) 
{
	char copy[MAX_LINE];
	char *word = NULL;
	int number_operands = 0;
	
	strcpy(copy, str);
	
	word = strtok(copy, " ,\n");
	
	while (word != NULL) 
	{
	
		number_operands++;
		word = strtok(NULL, ",\n");
		if (word != NULL) 
		{
			fprintf(stderr, "ERROR: illegal character after function no operand allowed, in line %d\n",line_num);
			error = 1;
			return;
		}
	}
	if (number_operands != 0) 
	{
		fprintf(stderr, "ERROR: The number of operands does not match the number of operands that should be , in line %d\n",line_num);
		error = 1;
		return;
	}
}
int check013(char * word, int line_num)
 {
 	if(word[0] == '#')/*check if operand is number*/
	{
		if((is_valid_number_sulamit(word) == 0))
		{
			fprintf(stderr, "ERROR: the number - illegal number, in line %d\n", line_num);
			error = 1;
			return 0;
		}
	}
	
	else if(word[0] == 'r')/*check if operand is register*/
	{
		if((is_register(word) == 0))
	 	{
		 	if(is_valid_label(word) == 0)
		 	{
				fprintf(stderr, "ERROR: the label-Invalid operand for source/destination operand, in line %d\n", line_num);
				error = 1;
				return 0;
			}
	 	}
	}
	else
	{
		if(is_valid_label(word) == 0)/*if oprerand is illegal label*/
		{
		
				fprintf(stderr, "ERROR: the label-Invalid operand for source/destination operand, in line %d\n", line_num);
				error = 1;
				return 0;
		}
	}
	return 1;
 }
 
 int check13(char * word, int line_num)
 {
 	if(word[0] == '#')/*if in the first place there us a number*/
	{
		fprintf(stderr, "ERROR: the number-Invalid operand for destination operand, invalid number, in line %d\n", line_num);
		error = 1;
		return 0;
	}
	else if(word[0] == 'r')/*if in the first word is register*/
	{
		if((is_register(word) == 0))/*if the register is illegal*/
		{
		 	if(is_valid_label(word) == 0)
		 	{
				fprintf(stderr, "ERROR: illegal character in label, in line %d\n", line_num);
				error = 1;
				return 0;
			}
		}
	}
	else
	{
		if(is_valid_label(word) == 0)/*if the label is illegal*/
		{
				fprintf(stderr, "ERROR: the label-Invalid operand for source/destination operand, in line %d\n", line_num);
				error = 1;
				return 0;
		}
	}
	return 1;
 }
 
void check_jmp_bne_jsr(char * str, int line_num)
{
	char copy[MAX_LINE];
	char *cp;
	strcpy(copy,str);
	cp = strtok(copy,"(\n");
	if(cp != NULL)
	{
		
		if(is_valid_label(cp) == 1)
		{
			if(is_register(cp) == 1)/*register is illegal for operand*/
			{
			
			
				fprintf(stderr, "ERROR: the label-Invalid operand for destanation operand, in line %d\n", line_num);
				error = 1;
				return;
			}
		
			
			else/*if the label is legal*/
			{
				cp = strtok(NULL,",\n");
			
				if(cp != NULL)
				{
				
					if(check013(cp, line_num) == 0)/*check first parameter*/
					{
						return;
					}
					cp = strtok(NULL,")\n");
					if(cp != NULL)
					{
						
						if(check013(cp, line_num) == 0)/*check second parameter*/
						{
							return;
						}
					}
				}
			}
		}
		else/*missing label befor parenpheses*/
		{
			fprintf(stderr, "ERROR: missing label befor parenpheses, in line %d\n", line_num);
			error = 1;
			return;
		}
	}
}

void check_space_jmp_bne_jsr(char* str, int line_num) 
{
	int len = strlen(str);
	int i;
	char c;
	int seen_open_paren = 0;
	int seen_comma = 0;
	str = strtok(str,"\n");
	
	for (i = 0; i < len-1; i++) 
	{
		c = str[i];
		if ((c == ' ') && (i != strlen(str)-1))/*if illegal space*/
		{
			fprintf(stderr, "ERROR - unnecessary space found in string on line %d\n", line_num);
			error = 1;
			return;
		}

		if (c == '(')/*if illegal opening parentheses*/
		{
			if (seen_open_paren) 
			{
				fprintf(stderr, "ERROR - multiple opening parentheses found in string on line %d\n", line_num);
				error = 1;
				return;
			} 
			else 
			{
				seen_open_paren = 1;
			}
		}

		if (c == ')')/*if illegal closing parentheses*/ 
		{
			if (!seen_open_paren) 
			{
				fprintf(stderr, "ERROR - closing parenthesis found before opening parenthesis in string on line %d\n", line_num);
				error = 1;
				return;
			}
			 else 
			 {
				seen_open_paren = 0;
				seen_comma = 0;
			}
		}

		if (c == ',')/*illegal character after parentheses*/ 
		{
			if (!seen_open_paren) 
			{
				fprintf(stderr, "ERROR - comma found outside of parentheses in string on line %d\n", line_num);
				error = 1;
				return;
			} 
			else 
			{
				if (seen_comma)/*illegal comma*/ 
				{
					fprintf(stderr, "ERROR - multiple commas found between operands in string on line %d\n", line_num);
					error = 1;
					return;
				} 
				else 
				{
					seen_comma = 1;
				}
			}
		}
	}
   
	if (str[len-1] == ' ') 
	{
		return;
	}
}
