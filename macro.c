#include "macro.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

FILE * macros (FILE * ifp, char * file_name)
{
	char c;	
	int count = 0;
	long fend;
	int line_num = 0;
	int a;
	int b;
	FILE * ifp_am;
	int num_of_mcr = count_mcr(ifp);
	macro * mcr_arr = (macro*) calloc (num_of_mcr , sizeof(macro));/*dynamic allocation for macro array*/
	macro * mcr_p = mcr_arr;
	if(mcr_arr == NULL)
	{
		printf("no allocation");
		exit(1);
	}
	fseek(ifp, 0, SEEK_END);
	fend = ftell(ifp);
	fseek(ifp, 0, SEEK_SET);
	while(ftell(ifp) != fend)/*while not end of file - A*/
	{
		int mcr;
		char * line = read_line(ifp);
		line_num++;
		if(strlen(line) == 0 ||(hasNonWhitespace(line, strlen(line)) == 0))/*check that line is not empty and has non white space*/
		{
			while(strlen(line) == 0 || (hasNonWhitespace(line, strlen(line)) == 0))
			{
				line = read_line(ifp);
				line_num++;
				if(fgetc(ifp) == EOF)
				{
					break;
				}
				fseek(ifp, -1, SEEK_CUR);
			}
			if(fgetc(ifp) == EOF)
			{
				break;
			}
			fseek(ifp, -1, SEEK_CUR);
		}
		mcr = is_macro(line);/*checks if start of macro and gives name*/
		if(mcr == 1)/*if is macro decleration, B*/
		{
			char copy[MAX_LINE];
			char * space = " \t";
			char * cp;
			strcpy(copy, line);
			cp = strtok(copy, space);
			cp =  strtok(NULL, space);
			strcpy(mcr_arr[count].name, cp);
			mcr_arr[count].beg = line_num;
			mcr_arr[count].txt = (char*) calloc (1 , sizeof(char));/*dynamic allocation for macro text*/
			if(mcr_arr[count].txt == NULL)
			{
				printf("no allocation");
				exit(1);
			}
			while(mcr != 2)/*while is not end of macro declaration, C*/
			{
				line = read_line(ifp);
				line_num++;
				if(strlen(line) == 0 ||(hasNonWhitespace(line, strlen(line)) == 0))/*check that line is not empty and has non white space*/
				{
					while(strlen(line) == 0 || (hasNonWhitespace(line, strlen(line)) == 0))
					{
						line = read_line(ifp);
						line_num++;
						if(fgetc(ifp) == EOF)
						{
							break;
						}
						fseek(ifp, -1, SEEK_CUR);
					}
					if(fgetc(ifp) == EOF)
					{
						break;
					}
					fseek(ifp, -1, SEEK_CUR);
				}
				if(is_macro(line) == 2)/*if end of macro dont copy text*/
				{
					break;
				}
				fseek(ifp, -strlen(line)-1, SEEK_CUR);
				if(strlen(mcr_arr[count].txt) != 0)/*add \n to macro text if necessary*/
				{
					char * temp = (char *)realloc(mcr_arr[count].txt, (strlen(mcr_arr[count].txt) + 2) * sizeof(char));
					if(temp == NULL)
					{
						printf("no allocation");
						exit(1);
					}
					mcr_arr[count].txt = temp;
					*(mcr_arr[count].txt + strlen(mcr_arr[count].txt) + 1) = EOS;
					*(mcr_arr[count].txt + strlen(mcr_arr[count].txt)) = '\n';
				}
				while((c=fgetc(ifp)) != '\n')/*copy macro text, D*/
				{
					char * temp = (char *)realloc(mcr_arr[count].txt, (strlen(mcr_arr[count].txt) + 2) * sizeof(char));
					if(temp==NULL)
						{
						printf("no allocation");
						exit(1);
					}
					mcr_arr[count].txt = temp;
					*(mcr_arr[count].txt + strlen(mcr_arr[count].txt) + 1) = EOS;
					*(mcr_arr[count].txt + strlen(mcr_arr[count].txt)) = c;
				}/*end of while D*/
			}/*end of while C*/
			mcr_arr[count].end = line_num;
			count++;
		}/*end of if B*/
	}/*end of while A*/
	ifp_am = open_macro(ifp, mcr_p, num_of_mcr, file_name);
	for(a = 0; a < num_of_mcr; a++)
	{
		for(b = a + 1; b < num_of_mcr; b++)
		{
			if(strcmp(mcr_arr[a].name, mcr_arr[b].name) == 0 && a != b)
			{
				fprintf(stderr, "ERROR - double declaration of macro %s\n", mcr_arr[a].name);
				error = 1;
			}
		}
		for(b = 0; b < 16; b++)/*If the label is the name of an opcode we will return an error*/
				{
					if((strcmp(mcr_arr[a].name, (opcode[b]).opName) == 0))
					{
						fprintf(stderr, "ERROR - illegal name '%s' - for macro\n",mcr_arr[a].name);
						error = 1;
					}
				}
				if((strcmp(mcr_arr[a].name, "extern") == 0) || (strcmp(mcr_arr[a].name, "entry") == 0) || (strcmp(mcr_arr[a].name, "data") == 0) || 					(strcmp(mcr_arr[a].name, "string") == 0) || (strcmp(mcr_arr[a].name, "mcr") == 0) || (strcmp(mcr_arr[a].name, "endmcr") == 0))
				/*If the label is the name of a reserved word we will return an error*/
				{
					fprintf(stderr, "ERROR - illegal name '%s' - for macro\n",mcr_arr[a].name);
					error = 1;
				}
	}
	
	free(mcr_arr);
	return ifp_am;
}/* end of function*/
	
int is_macro(char * str)
{
	char copy[MAX_LINE];
	char * space = " \t";
	char * cp;
	strcpy(copy, str);
	cp = strtok(copy, space);/*find first word in line*/
	if(strcmp(cp, "mcr") == 0)/*if macro declaration*/
	{
		return 1;
	}
	if(strcmp(cp, "endmcr") == 0)/*if end of macro decleration*/
	{
		return 2;
	}
	return 0;
}

FILE * open_macro(FILE * ifp, macro * mcr_p, int mcr_arr_len, char * file_name)
{
	FILE * ifp2;
	long p;
	int j = 0;
	int lnum = 0;
	long beg;/*will hold beggining position of ifp2*/
	strcat(file_name, ".am");
	ifp2 = fopen(file_name , "w+");/*open after macro file*/
	fseek(ifp, 0, SEEK_END);
	p = ftell(ifp);
	
	if(ifp2 == NULL)	
	{
		printf("Cannot open file. continue with next file.\n\n");
		exit(1);
	}
        else
	{
		int i;
		char * line;
		int mcr;
		fseek(ifp,0,SEEK_SET);
		while(ftell(ifp) != p)/*while not end of file, D*/
		{
			int flag = 0;        
			line = read_line(ifp);
			lnum++;
			if(strlen(line) == 0 || (hasNonWhitespace(line, strlen(line)) == 0))/*check that line is not empty and has non white space*/
			{
				while(strlen(line) == 0 || (hasNonWhitespace(line, strlen(line)) == 0))
				{
					line = read_line(ifp);
					lnum++;
					if(fgetc(ifp) == EOF)
					{
						break;
					}
					fseek(ifp, -1, SEEK_CUR);
				}
				if(fgetc(ifp) == EOF)
				{
					break;
				}
				fseek(ifp, -1, SEEK_CUR);
			}
			mcr = is_macro(line);
			if(mcr == 1)/*if is macro decleration , A*/
			{
				checking_errors(line, mcr_p);
				while(mcr != 2)/*skip macro, dont copy to new file, B*/
				{
					line = read_line(ifp);
					lnum++;
					if(strlen(line) == 0 || (hasNonWhitespace(line, strlen(line)) == 0))/*check that line is not empty and has non white space*/
					{
						while(strlen(line) == 0 || (hasNonWhitespace(line, strlen(line)) == 0))
						{
							line = read_line(ifp);
							lnum++;
							if(fgetc(ifp) == EOF)
							{
								break;
							}
							fseek(ifp, -1, SEEK_CUR);
						}
						if(fgetc(ifp) == EOF)
						{
							break;
						}
						fseek(ifp, -1, SEEK_CUR);
					}
					mcr = is_macro(line);
				}/*end while B*/
				checking_errors(line, mcr_p);
			}/*end of if A*/
			else /*check if call to macro or not*/
			{
				char copy[MAX_LINE];
				char * space = " \t";
				char * cp;
				strcpy(copy, line);
				cp = strtok(copy, space);
				while(cp != NULL)/*while not end of line C*/
				{
					flag = 0;
					for(j = 0; j < mcr_arr_len; j++)
					{
						if(strcmp(cp, mcr_p[j].name) == 0)/*if is call to macro copy macro text, B*/
						{
							flag = 1;
							if(lnum < mcr_p[j].beg)/*if macro declaration is later*/
							{
								fprintf(stderr, "ERROR - in Line %d:\nIllegal use of macro (macro declared later)\n",lnum);
								error = 1;
								break;
							}
							for(i=0; i < strlen(mcr_p[j].txt); i++)
							{
								fputc((int)(mcr_p[j]).txt[i], ifp2);
							}
							fputc(' ', ifp2);
						}/*end if B*/
					}
					if(flag == 0)/*if is regular work(not macro or decleration)*/
					{
						char ch;
						for(i = 0; i < strlen(cp); i++)/*copy text*/
						{
							ch = cp[i];
							fputc((int)ch, ifp2);

							/*c = fgetc(ifp);*/
						}
						fputc(' ', ifp2);
					}
					cp = strtok(NULL, space);
				}/*end while C*/
				fputc('\n', ifp2);
			}
		}/*end if A*/
	}/*end while D*/
	fseek(ifp2, 0, SEEK_SET);
	beg = ftell(ifp2);
	fseek(ifp2, 0, SEEK_END);
	if(beg == ftell(ifp2))
	{
		remove(file_name);
	}
	return ifp2;
}/*end of funtion*/
void checking_errors(char * line, macro * arr)
{
	char name_of_macro[MAX_LINE] = {0};
	char * token = NULL;
	const char seperate_words[4] = " \t\n";
	char first_word[MAX_LINE];
	int i;
	int beginning;
	static int m;
	char copy[MAX_LINE];
	strcpy(copy, line);
	
	token = strtok(copy, seperate_words);
	if(token != NULL) /*if there is a word in line*/ 
	{
		strcpy(first_word, token); /*copy the token to the first word*/
		token = strtok(NULL, seperate_words); /*entered to token the second word*/

		if(strcmp(first_word, "mcr") == 0) /*if findes a macro*/
		{
		    	strcpy(name_of_macro, token); /*entered to the name of macro the second word*/
			for(i = 0; i < 1000; i++) /*cheking what length of every macro*/
			{ 
				if(strcmp(arr[i].name, name_of_macro) == 0)
				{
					beginning = arr[i].beg;
					m = arr[i].end;
				}
			}
			token = strtok(NULL, seperate_words);/*entered to token the third word*/
			if(token != NULL)
			{
				fprintf(stderr, "ERROR - in line %d:\nThere are too many arguments after the definition of macro %s\n",beginning,name_of_macro);
				error = 1;
			}
		}
	
		if(strcmp(first_word, "endmcr") == 0)
		{
			if(token != NULL)
			{	
				fprintf(stderr, "ERROR- in line %d:\nThere are too many arguments after the definition of the end of the macro\n", m);
				error = 1;
			}
		}
	}
}

int count_mcr(FILE * ifp)
{
	int i = 0;
	long end;
	char * line;
	fseek(ifp, 0, SEEK_END);
	end = ftell(ifp);
	fseek(ifp, 0, SEEK_SET);
	while (ftell(ifp) != end)
	{
		line = read_line(ifp);
		if(strlen(line) == 0 || (hasNonWhitespace(line, strlen(line)) == 0))/*check that line is not empty and has non white space*/
		{
			while(strlen(line) == 0 || (hasNonWhitespace(line, strlen(line)) == 0))
			{
				line = read_line(ifp);
				if(fgetc(ifp) == EOF)
				{
					break;
				}
				fseek(ifp, -1, SEEK_CUR);
			}
			if(fgetc(ifp) == EOF)
			{
				break;
			}
			fseek(ifp, -1, SEEK_CUR);
		}
		if(is_macro(line) == 1)
		{
			i++;
		}
	}
	return i;
}
