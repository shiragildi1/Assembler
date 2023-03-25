#include "secondpass.h"
#include "utils.h"

void process_file(FILE *ifp) 
{
	char line[MAX_LINE] = {0};
	char copy[MAX_LINE] = {0};
	int which_line = 0; /* line number in array_code table */
	int which_line_user = 0;/* line number for printing errors to user */
	int which_word = 0;/* position of word in codes -> code_line */
	fseek(ifp, 0, SEEK_SET);
	
	while (fgets(line, sizeof(line), ifp)) /*while not end of file, A*/
	{
		char * word;/* word to be checked */
		which_line_user++;
		strcpy(copy, line);
		word = strtok(copy, " )(,\t\n");/* isolate 1st word */
		while (word != NULL) /* checks word type (label, number, .data,....), B*/
		{
			int i;
			int flag = 0;/*flag if word is opcode*/
		   	for(i = 0; i < 16; i++)/*If the label is the name of an opcode we will return an error*/
			{
				if((strcmp(word, (opcode[i]).opName) == 0))
				{
					flag = 1;
				}
			}
			if(flag == 1)/*if is opcode*/
			{
				word = strtok(NULL, " )(,\t\n");/*take next word*/
			}
			else if ((strchr(word, ':') != NULL))/* if is label declaration*/
			{
				word = strtok(NULL, " )(,\t\n");
				which_word--;/* don't count line (in array_code), line doesn't get coded*/
				
			}
			else if((strcmp(word, ".extern") == 0))/*if extern*/
			{
				word = strtok(NULL, " )(,\t\n");
				which_line--;
				break;
			}
			else if ( strcmp(word, ".entry") == 0) /*if entry*/
			{
				word = strtok(NULL, " )(,\t\n");
				which_line--;
				break;
			} 
			else if (strcmp(word, ".data") == 0)/*if data*/
			{
				word = strtok(NULL, " )(,\t\n");
				which_line--;
				break;
			} 
			else if (strcmp(word, ".string") == 0)/*if string*/
			{
				word = strtok(NULL, " )(,\t\n");
				which_line--;
				break;
			}
			else if((word[0] == 'r') && (word[1] <= '7') && (word[1] >= '0') && (strlen(word) == 2))/*if is register*/
			{
				word = strtok(NULL, " )(,\t\n");
			}
			else if(is_number(word))
			{
				word = strtok(NULL, " )(,\t\n");
			}
			else if(strchr(word, '#') != NULL)/* if word contains hashtag*/
			{
				word = strtok(NULL, " )(,\t\n");
			}
			else if(is_string(word))
			{
				word = strtok(NULL, " )(,\t\n");
			}
			else /*if is label (use of label and not declaration)*/
			{
				int address;/*will hold address of label from symbol table*/
				address = found_the_label_in_symbol_tabel(word ,ifp);/*the function finds the label whithout definition */
				if(address == 9999)/* if label was not declared and not defined */
				{
					fprintf(stderr, "ERROR in line %d - undeclared label '%s' used\n", which_line_user, word);
					error = 1;
					break;
				}
				if(error == 0)
				{
					find_label_without_defined(which_line , which_word , address,ifp);/*updates array codes */
				}
				word = strtok(NULL, " )(,\t\n");/*take the next word*/
			}
		   which_word++;/* update word number*/
		}/*end while B*/
		which_line++;/*update line number*/
		which_word = 0;/*reset word number*/
	}/*end while A*/
}/*end function*/

int is_number(const char* word) 
{
	int i = 0;
	if (word[0] == '-' ) 
	{
		i = 1;
	}

	for (; word[i] != '\0'; i++) 
	{
		if (!isdigit(word[i])) 
		{
			return 0;
		}
	}
	return 1;
}

int is_string(char *word) 
{
	int len = strlen(word);
	if (len >= 2 && word[0] == '"' && word[len-1] == '"') /*check if is string(has quotation marks*/
	{
		return 1;
	}
	return 0;
}

int found_the_label_in_symbol_tabel(char * word, FILE * ifp)/*the function returns the address of label*/
{
	int i;
	int address = 9999;/*defualt value*/
	int count_label = count_labels(ifp);
	
	for(i = 0; i < count_label; i++)
	{
		if(strcmp(labels[i].label_name, word) == 0)
		{
			address = (labels[i].label_address);
			break;	
		} 
	}
	return address;
}

void find_label_without_defined(int which_line, int which_word, int address, FILE * ifp)
{
	int i;
	int count_label = count_labels(ifp);
	for(i = 0; i < count_label; i++)/*scan symbol table*/
	{
		 if((labels[i].label_address == 50) && (which_line >= 0) && (address == 50))/*if is external label*/
		{
			if (codes[which_line].code_line[which_word] == (short)('?'))/*if word in array code has to be coded*/
			{
				codes[which_line].code_line[which_word] = 1;
			}
		}
	}

	if ((which_line >= 0) && (codes[which_line].code_line[which_word] == (short)('?')))/*if label is not extern*/
	{
		codes[which_line].code_line[which_word] = ((address << 2)+2);/*code word to addres and add ARE bits*/
	}
}
