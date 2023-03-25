#include "improve.h"
#include "utils.h"

FILE * add_space(FILE * ifp)
{
	long end;
	char c;
	FILE * ifp_new = fopen("with_space" , "w+");
	fseek(ifp, 0, SEEK_END);
	end = ftell(ifp);
	fseek(ifp,0,SEEK_SET);
	c = fgetc(ifp);
	while(ftell(ifp) != end)/* while not end of file */
	{	
		fputc((int)c, ifp_new);
		if(c == ':')/*if label declaration add space*/
		{
			fputc(' ', ifp_new);
		}
		c = fgetc(ifp);
	}
	return ifp_new;
}

FILE * erase_white_space(FILE * ifp)
{
	char c;
	long end;
	int leave = 1;
	int flag = 0;
	FILE * ifp_new = fopen("clearmid" , "w+");
	fseek(ifp, 0, SEEK_END);
	end = ftell(ifp);
	fseek(ifp,0,SEEK_SET);
	c = fgetc(ifp);
	while(ftell(ifp) != end)/* while not end of file */
	{
		long pos = ftell(ifp);
		if(isspace(c))/* if is white space character */
		{
			while(ftell(ifp) != end)/* while not end of file */
			{
				leave = 0;
				c = fgetc(ifp);
				if(!isspace(c))/* skip white space till other character */
				{
					leave = 1;
			 		break;
			 	}
			}
		}
		if(leave == 1)/*non white space character found */
		{
			fseek(ifp, pos-1, SEEK_SET);
			c = fgetc(ifp);
			if(flag == 1)
			{
				while(isspace(c))
				{
					if(c == '\n')
					{
						fputc(c, ifp_new);
					}
					c = fgetc(ifp);
				}
				flag = 0;
			}
			if(isspace(c))
			{
				flag = 1;
				if(c == '\t')
				{
					c = ' ';
				}
			}
			fputc(c, ifp_new);
			c = fgetc(ifp);
		}
	}
	return ifp_new;
}
FILE * remove_comments(FILE * ifp)
{
	char line[MAX_LINE];
	FILE * ifp_new = fopen("delete_comment" , "w+");
	fseek(ifp, 0, SEEK_SET);
	while (fgets(line, MAX_LINE, ifp)) /* while not end of file read a line*/
	{
		if(strcmp(line, "") && line[0] != ';')/* if is comment line */
		{
			fputs(line, ifp_new);
			fputc((int)(' '), ifp_new);
		}
	}
	return ifp_new;
}

FILE * delete_stray_labels(FILE * ifp)
{
	char c;
	char copy[MAX_LINE];
	const char seperate_words[6] = " \t";
	char * token = NULL;
	char * line;
	long end;
	long pos;
	FILE * ifp_new = fopen("del_lab" , "w+");
	fseek(ifp, 0, SEEK_END);
	end = ftell(ifp);
	fseek(ifp,0,SEEK_SET);
	while(ftell(ifp) != end)/* while not end of file */
	{
		char label_name[MAX_LABEL_NAME] = {0};
		line = read_line(ifp);
		strcpy(copy, line);
		token = strtok(copy, seperate_words);
		if(token[strlen(token) - 1] == ':')
		{
			int len = strlen(token);
			strncpy(label_name, token, strlen(token) - 1);			
			token = strtok(NULL, seperate_words);
			if(strcmp(token, ".entry") == 0)
			{
				c = line[len + 1];
				while(c != '\0')
				{
					fputc((int)c, ifp_new);
					len++;
					c = line[len + 1];
				}
				pos = ftell(ifp);
				check_if_label_exists(pos, label_name, ifp);
				fseek(ifp, pos, SEEK_SET);
			}
			else if(strcmp(token, ".extern") == 0)
			{
				c = line[len + 1];
				while(c != '\0')
				{
					fputc((int)c, ifp_new);
					len++;
					c = line[len + 1];
				}
				pos = ftell(ifp);
				check_if_label_exists(pos, label_name, ifp);
				fseek(ifp, pos, SEEK_SET);
			}
			else
			{
				fputs(line, ifp_new);
			}
		}
		else
		{
			fputs(line, ifp_new);
		}
		fputc('\n', ifp_new);
		
	}
	return ifp_new;
}
void check_if_label_exists(long pos, char * label_name, FILE * ifp)
{
	char copy[MAX_LINE];
	const char seperate_words[4] = " :\t";
	char * token = NULL;
	char * line;
	long end;
	fseek(ifp, 0, SEEK_END);
	end = ftell(ifp);
	fseek(ifp,0,SEEK_SET);
	while(ftell(ifp) != end)/* while not end of file */
	{
		line = read_line(ifp);
		strcpy(copy, line);
		token = strtok(copy, seperate_words);
		if(strcmp(token, label_name) == 0)
		{
			if(pos != ftell(ifp))
			{
				fprintf(stderr, "ERROR - label %s declared more than once\n", label_name);
				error = 1;
			}
		}
	}
	if(error != 1)
	{
		fprintf(stderr, "WARNING - label %s with no effect\n", label_name);
	}
}
void check_length(FILE * ifp)
{
	long end;
	char * line;
	int line_num = 0;
	fseek(ifp, 0, SEEK_END);
	end = ftell(ifp);
	fseek(ifp,0,SEEK_SET);
	while(ftell(ifp) != end)
	{
		line = read_line(ifp);
		line_num++;
		if(strlen(line) > MAX_LINE)
		{
			fprintf(stderr, "ERROR in line %d: Line longer than allowed\n", line_num);
		}
	}
}
