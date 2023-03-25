#include "firstpass.h"
#include "utils.h"
#include "globals.h"

void firstpass()
{
	(opcode[0]).opName = "mov";
	(opcode[1]).opName = "cmp";
	(opcode[2]).opName = "add";
	(opcode[3]).opName = "sub";
	(opcode[4]).opName = "not";
	(opcode[5]).opName = "clr";
	(opcode[6]).opName = "lea";
	(opcode[7]).opName = "inc";
	(opcode[8]).opName = "dec";
	(opcode[9]).opName = "jmp";
	(opcode[10]).opName = "bne";
	(opcode[11]).opName = "red";
	(opcode[12]).opName = "prn";
	(opcode[13]).opName = "jsr";
	(opcode[14]).opName = "rts";
	(opcode[15]).opName = "stop";
	(opcode[16]).opName = NULL;
	(opcode[0]).count_parameter = 2;
	(opcode[1]).count_parameter = 2;
	(opcode[2]).count_parameter = 2;
	(opcode[3]).count_parameter = 2;
	(opcode[4]).count_parameter = 1;
	(opcode[5]).count_parameter = 1;
	(opcode[6]).count_parameter = 2;
	(opcode[7]).count_parameter = 1;
	(opcode[8]).count_parameter = 1;
	(opcode[9]).count_parameter = 1;
	(opcode[10]).count_parameter = 1;
	(opcode[11]).count_parameter = 1;
	(opcode[12]).count_parameter = 1;
	(opcode[13]).count_parameter = 1;
	(opcode[14]).count_parameter = 0;
	(opcode[15]).count_parameter = 0;
	(opcode[16]).count_parameter = '\0';
}

void set_array_code(FILE * ifp)
{
	int count_line = 0;
	int line_user = 0; /*for printing errors*/
	char * line;
	long end;
	const char seperate_words[6] = " ,)(\t";
	IC = 0;
	DC = 0;
	codes = (code *) calloc(2000, sizeof(code));
	if(codes == NULL)
	{
		printf("no allocation");
		exit(1);
	}
	set_array_data();
	set_array_labels(ifp);
	fseek(ifp, 0, SEEK_END);
	end = ftell(ifp);
	fseek(ifp, 0, SEEK_SET);
	while(ftell(ifp) != end)/*while not end of file, A*/
	{
		int flag = 0;	
		char copy[MAX_LINE];
		char * token = NULL;
		char label_name[MAX_LABEL_NAME];
		int i;
		line = read_line(ifp);/*reads the first line from file*/
		line_user++;
		strcpy(copy, line);
		codes[count_line].address = IC /*+ DC */+ 100;
		token = strtok(copy, seperate_words);/*holds the first word*/
		for(i = 0; i < strlen(token); i++)/*checking if the first word is label*/
		{
			if(token[i] == ':')
			{
				flag = 1;/*if the first word is label flag=1*/
				strcpy(label_name, token);
				label_name[strlen(label_name)-1] = '\0';
				token = strtok(NULL, seperate_words);/*token hold the second word*/
			}	
		}
		if(strcmp(token, ".data") == 0)/*if data line, B*/
		{
			int num_of_data = counter_data(line, flag);/*count number after .data */
			if(flag == 1)
			{
				insert_label(label_name, DC*10000); /* will be recognized as label in data line */
			}
			
			DC += num_of_data;
		}/*end if B */
		else if(strcmp(token, ".string") == 0)/*if string line, C*/
		{
			int k = 0, m = 0;/*indexes*/
			int cd = 0;/*number of letters in string*/
			if (flag == 1)
			{
				insert_label(label_name, DC * 10000);/* will be recognized as label in data line */
			}
			while(line[k] != '"')/*find first quotation */
			{
				k++;
			}
			k++;
			m = k;
			while(line[m] != '"' )/*find second quotation and number of letters in string */
			{
				m++;
				cd++;
			}
			
			while(line[k] != '"' )
			{
				insert_data((short)(line[k]));/*add letter to data table */
				k++;
				/*m++;*/
			}
			insert_data(0);
			/*count_line++;*/
			DC += (cd + 1);
		}/*end if C*/
		else if(strcmp(token, ".entry") == 0)/*if entry */
		{
			if(flag == 1)
			{
				fprintf(stderr, "WARNING in line %d - label with no effect\n", count_line);
				/*warning*/
			}
		}
		else if(strcmp(token, ".extern") == 0)/*if extern */
		{
			if(flag == 1)
			{
				fprintf(stderr, "WARNING in line %d - label with no effect\n", count_line);
				/*warning*/
			}
			token =  strtok(NULL, seperate_words);
			insert_label(token, 50);/*insert extern label to label table */
		}
		else/*if line has function D*/
		{
			int opVal;
			if(flag == 1)
			{
				insert_label(label_name, IC + 100);
			}
			opVal =  find_op_value(token);/*find function number */
			if(opVal == 17 && token[0] != '.')/*if illegal function */
			{
				fprintf(stderr, "ERROR in line %d - no such function found\n", line_user);
				error = 1;
				break;
			}
			if((opcode[opVal]).count_parameter == 0)/* if function does not have operands */
			{
				token =  strtok(NULL, " \t");
				codes[count_line].code_line = (short *)calloc(1, sizeof(short));
				if(codes[count_line].code_line == NULL)
				{
					printf("no allocation");
					exit(1);
				}
				codes[count_line].num_of_words = 1;
				IC += 1;
				codes[count_line].code_line[0] = (0 | (opVal << 6));/* set code for line */
			}
			else if((opcode[opVal]).count_parameter == 1)/* if function has 1 operands */
			{
				token =  strtok(NULL, " \t");
				IC = one_par_code(opVal, token, count_line, IC);
			}
			else if((opcode[opVal]).count_parameter == 2)/* if funtion has 2 operands */
			{
				int flag_reg = 0;/*flag is on if first operand is register */
				token =  strtok(NULL, " \t,");
				codes[count_line].code_line = (short *)calloc(3, sizeof(short));
				if(codes[count_line]. code_line == NULL)
				{
					printf("no allocation");
					exit(1);
				}
				codes[count_line].num_of_words = 3;
				IC += 3;
				codes[count_line].code_line[0] = (0 | (opVal << 6));
				if(token[0] == '#')/* if operand is number */
				{
					char number[10];
					int conv_number;
					int num_len = 0;
					int b = 1;
					if(token[1] == '-' || token[1] == '+')
					{
						b = 2;
					}
					while(isdigit(token[b]))
					{
						num_len++;
						b++;
					}
					strncpy(number, (&(token[1])), num_len + 1);
					conv_number = atoi(number);
					codes[count_line].code_line[1] = (conv_number << 2);
				}
				else if((token[0] == 'r') && (token[1] >= '0') && (token [1] <= '7') && ((token[2] == EOS) || (isspace(token[2])) || (token[2] == ',') || (token[2] == ')')))
				/* if is register between 0 and 7*/
				{
					int reg_num = token[1] - 48;
					flag_reg = 1;
					codes[count_line].code_line[1] = (reg_num << 8);
					codes[count_line].code_line[0] |= (3 << 4);
				}
				else/* is label */
				{
					codes[count_line].code_line[0] |= (1 << 4);
					codes[count_line].code_line[1] = (short)('?');
				}
				
				token = strtok(NULL, " \t,");/* find next operand*/
				if(token[0] == '#')/* if is number */
				{
					char number[10];
					int conv_number;
					int num_len = 0;
					int b = 1;
					while(isdigit(token[b]))
					{
						num_len++;
						b++;
					}
					strncpy(number, (&(token[1])), num_len + 1);
					conv_number = atoi(number);
					codes[count_line].code_line[2] = (conv_number << 2);
				}
				else if(token[0] == 'r' && token[1] >= '0' && token [1] <= '7' && ((token[2] == EOS) || (isspace(token[2])) || (token[2] == ',') || (token[2] == ')')))
				/* if is register between 0 and 7*/
				{
					int reg_num = token[1] - 48;
					if(flag_reg == 1)/* if previous operand is register */
					{
						IC --;
						codes[count_line].code_line[1] |= (reg_num << 2);
						codes[count_line].num_of_words = 2;
					}
					else/*previous operand not register */
					{
						codes[count_line].code_line[2] = (reg_num << 2);
					}
					codes[count_line].code_line[0] |= (3 << 2);
				}
				else/* is label */
				{
					codes[count_line].code_line[0] |= (1 << 2);
					codes[count_line].code_line[2] = (short)('?');
				}
			}
			count_line++;
		}/*end else D */
	}/*end while A*/
	check_labels(ifp);/* are there double declarations */
	fix_data_label_address(ifp);
}
void fix_data_label_address(FILE * ifp)
{
	int i;
	int num_labels = count_labels(ifp);
	for(i = 0; i < num_labels; i++)
	{
		if(labels[i].label_address >= 10000)/*if recognized as data line */
		{
			labels[i].label_address = ((labels[i].label_address / 10000) + IC + 101); /*divide address by 10000 and add IC + 101*/
		}
	}
}
int one_par_code(int opVal, char * token, int count_line, int IC)
{
	codes[count_line].code_line = (short *)calloc(2, sizeof(short));
	if(codes[count_line]. code_line == NULL)
	{
		printf("no allocation");
		exit(1);
	}
	codes[count_line].num_of_words = 2;
	codes[count_line].code_line[0] |= (opVal << 6);
	if(token[0] == 'r' && token[1] >= '0' && token [1] <= '7' && ((token[2] == EOS) || (isspace(token[2])) || (token[2] == ',') || (token[2] == ')')))
	/* if is register between 0 and 7*/
	{
		int a = token[1] - 48;
		codes[count_line].code_line[1] = (a << 2);
		codes[count_line].code_line[0] |= 12;
		token =  strtok(NULL, " \t");
		IC += 2;
	}
	else if(opVal == 12)/* function prn*/
	{
		if(token[0] == '#')/* if is number */
		{
			char number[10] = {0};
			int conv_number;
			strcpy(number, (&(token[1])));
			conv_number = atoi(number);
			codes[count_line].code_line[1] = (conv_number << 2);
			IC += 2;
		}
		/*check if 0*/
	}
	else if(opVal == 13 || opVal == 9 || opVal == 10)/*if is jump funcion: jsr, jmp, bne, E*/
	{
		int b = 0;
		int indOfBrac = 0;
		int flag2 = 0;
		int num_len = 0;
		for(b = 0; b < strlen(token); b++)
		{
			if(token[b] == '(')
			{
				flag2 = 1;
				indOfBrac = b + 1;
			}
		}
		if(flag2 == 1)/* if is miun 2, G */
		{
			int flag_reg = 0;
			codes[count_line].code_line = (short *)calloc(4, sizeof(short));
			if(codes[count_line]. code_line == NULL)
			{
				printf("no allocation");
				exit(1);
			}
			codes[count_line].num_of_words = 4;
			IC += 4;
			codes[count_line].code_line[0] |= (opVal << 6);
			codes[count_line].code_line[0] |= (2 << 2);
			codes[count_line].code_line[1] = (short)('?');
			if(token[indOfBrac] == '#')/* if is number, F*/
			{
				char number[10] = {0};
				int conv_number;
				b = 1;
				if((token[indOfBrac + 1]) == '-' || (token[indOfBrac + 1]) == '+')
				{
					b = 2;
				}
				while(isdigit(token[indOfBrac + b]))
				{
					num_len++;
					b++;
				}
				strncpy(number, (&(token[indOfBrac + 1])), num_len + 1);
				conv_number = atoi(number);
				codes[count_line].code_line[2] = (conv_number << 2);
				if(token[indOfBrac + num_len + 3] == '#')
				{
					char number1[10] = {0};
					int conv_number1;
					int num_len1 = 0;
					b = num_len + 3;/*make sure that actually strarts in the right place*/
					while(isdigit(token[indOfBrac + b]))
					{
						num_len1++;
						b++;
					}
					strncpy(number, (&(token[indOfBrac + 4])), num_len1 + 1);
					conv_number1 = atoi(number1);
					codes[count_line].code_line[3] = (conv_number1 << 2);
				}
				 else if(token[indOfBrac + num_len + 3] == 'r' && token[indOfBrac + num_len + 4] >= '0' && token[indOfBrac + num_len + 4] <= '7' 
				 	&& ((token[indOfBrac + num_len + 5] == EOS) || (isspace(token[indOfBrac + num_len + 5])) ||
				  	(token[indOfBrac + num_len + 5] == ',') || (token[indOfBrac + num_len + 5] == ')')))
				 /* if is register between 0 and 7*/
				{
					int reg_num = token[indOfBrac + num_len + 4] - 48;
					codes[count_line].code_line[3] = (reg_num << 2);
					codes[count_line].code_line[0] |= (3 << 10);
				}
				else/* is label */
				{
					codes[count_line].code_line[3] = (short)('?');
					codes[count_line].code_line[0] |= (1 << 10);
				}
			}/*end if F */
			else if(token[indOfBrac] == 'r'  && token[indOfBrac + 1] >= '0' && token [indOfBrac + 1] <= '7' && ((token[indOfBrac + 2] == EOS)
				 || (isspace(token[indOfBrac + 2])) || (token[indOfBrac + 2] == ',') || (token[indOfBrac + 2] == ')')))
			/* if is register between 0 and 7*/
			{ 
				int reg_num = token[indOfBrac + 1] - 48;
				flag_reg = 1;
				codes[count_line].code_line[2] = (reg_num << 8);
				codes[count_line].code_line[0] |= (3 << 12);
				if(token[indOfBrac + 3] == 'r' && token[indOfBrac + 4] >= '0' && token [indOfBrac + 4] <= '7'  && ((token[indOfBrac + 5] == EOS) || 
					(isspace(token[indOfBrac + 5])) || (token[indOfBrac + 5] == ',') || (token[indOfBrac + 5] == ')')))
				/* if is register between 0 and 7*/
				{	
					if(flag_reg == 1)
					{
						codes[count_line].num_of_words = 3;
					}
					reg_num = token[indOfBrac + 4] - 48;
					codes[count_line].code_line[2] |= (reg_num << 2);
					codes[count_line].code_line[0] |= (3 << 10);
					IC--;
				}
				else if(token[indOfBrac + 3] == '#')/* if is number */
				{
					char number[10] = {0};
					int conv_number;
					b = 5;
					num_len = 0;
					while(isdigit(token[indOfBrac + b]))
					{
						num_len++;
						b++;
					}
					strncpy(number, (&(token[indOfBrac + 4])), num_len + 1);
					conv_number = atoi(number);
					codes[count_line].code_line[3] = (conv_number << 2);
				}
				else/*is label*/
				{
					codes[count_line].code_line[3] = (short)('?');
					codes[count_line].code_line[0] |= (1 << 10);
				}
			}
			else/*is label (first label) */
			{
				int b = 0;
				int indOfComma = 0;
				codes[count_line].code_line[2] = (short)('?');
				codes[count_line].code_line[0] |= (1 << 12);
				for(b = 0; b < strlen(token); b++)
				{
					if(token[b] == ',')
					{
						indOfComma = b + 1;
					}
				}
				if(token[indOfComma + 0] == 'r' && token[indOfComma + 1] >= '0' && token [indOfComma + 1] <= '7'  
					&& ((token[indOfComma + 2] == EOS) || (isspace(token[indOfComma+ 2])) || 
					(token[indOfComma + 2] == ',') || (token[indOfComma + 2] == ')')))
				/* if is register between 0 and 7*/
				{
					int reg_num = token[indOfComma + 1] - 48;
					codes[count_line].code_line[3] = (reg_num << 2);
					codes[count_line].code_line[0] |= (3 << 10);
				}
				else if(token[b + 2] == '#')/* if is number */
				{
					char number[10] = {0};
					int conv_number;
					b = 3;
					num_len = 0;
					while(isdigit(token[indOfComma + b]))
					{
						num_len++;
						b++;
					}
					strncpy(number, (&(token[indOfComma + 4])), num_len + 1);
					conv_number = atoi(number);
					codes[count_line].code_line[3] = (conv_number << 2);
				}
				else/* is label */
				{
					codes[count_line].code_line[3] = (short)('?');
					codes[count_line].code_line[0] |= (1<< 10);
				}
			}
		}/* end if G*/
		else/* not miun 2 , H */
		{
			IC += 2;
			if(token[0] == '#')/*if is number */
			{
				char number[10];
				int conv_number;
				int num_len = 0;
				int b = 1;
				while(isdigit(token[b]))
				{
					num_len++;
					b++;
				}
				strncpy(number, (&(token[1])), num_len + 1);
				conv_number = atoi(number);
				codes[count_line].code_line[1] = (conv_number << 2);
			}
			else if(token[0] == 'r' && token[1] >= '0' && token [1] <= '7' && ((token[2] == EOS) || (isspace(token[2])) || (token[2] == ',') || (token[2] == ')')))
			/* if is register between 0 and 7*/
			{
				int reg_num = token[1] - 48;
				codes[count_line].code_line[1] = (reg_num << 8);
				codes[count_line].code_line[0] |= (3 << 2);
			}
			else/* is label */
			{
				codes[count_line].code_line[0] |= (1 << 2);
				codes[count_line].code_line[1] = (short)('?');
			}
		}/*end else H */
		
	}/*end if E*/
	else/* is label*/
	{
		IC += 2;
		codes[count_line].code_line[0] |= (1 << 2);
		codes[count_line].code_line[1] = (short)('?');
	}
	return IC;
}

int counter_data(char * line, int flag)
{
	char copy[MAX_LINE];
	const char seperate_words[6] = " ,)(\t";
	char * token = NULL;
	int i;
	int counter = 0;
	strcpy(copy, line);
	token = strtok(copy, seperate_words);/*hold the first word*/
	 if(flag ==1)
	{
		 token = strtok(NULL, seperate_words);  
	}
	   
	token = strtok(NULL, seperate_words);
	for(i = 0; i<strlen(line);i++)
	{
		if(token != NULL)
		{
			short n;
			char copy2[MAX_LINE];
			strcpy(copy2, token);
			n = atoi(copy2);
			counter++;
			insert_data(n);
			token = strtok(NULL, seperate_words);	
		}
	}
	return counter;
}

void insert_data(short character)
{
	static int size = 0;/* holds currnet size of data array */
	short * temp = (short*)realloc(data_table, (size + 2) * sizeof(short));
	if (temp == NULL)
	{
		printf("no allocation");
		return;
	}
	data_table = temp;
	/*data_table[size + 1]= EOS;*/
	data_table[size]= character;
	size += 1;
}

void set_array_data()
{
	data_table = (short*)calloc(1, sizeof(short));
	if (data_table == NULL) 
	{
		printf("no allocation!!!!!!!");
	}
}

int find_op_value(char *word)/*the function found the opcode number of operation*/
{
	int op;
	int i;
	op = 17;
	for(i = 0; i < 16 ; i++)
	{
		if(strcmp(opcode[i].opName, word)==0)
		{	
			op = i;	
		}
	} 
	return op;
}

void set_array_labels(FILE * ifp)
{
	int num_labels = count_labels(ifp);
	labels = (symbol *)calloc(num_labels, sizeof(struct symbol));
	if(labels == NULL)
	{
		printf("no allocation!!!!!!!\n");
		return;
	}	  
}

void insert_label(char * word, int address)/*the fanction insert label to symbole table*/
{
	int i = 0;
	while(labels[i].label_name[0] != '\0')/* find empty space in label array */
	{
		i++;
	}
	strcpy(labels[i].label_name, word);/*set label name */
	labels[i].label_address = address;/*set label address */
}

void check_labels(FILE * ifp)
{
	int i = 0;
	int j = 0;
	int num_of_labels = count_labels(ifp);
	for(i = 0; i < num_of_labels; i++)
	{
		for(j = i + 1; j < num_of_labels; j++)
		{
			if(strcmp(labels[i].label_name, labels[j].label_name) == 0)
			{
				fprintf(stderr, "ERROR - label %s declared more than once\n", labels[j].label_name);
				error = 1;
			}
		}
	}
}
