#include "utils.h"
#include "globals.h"

char * read_line(FILE * ifp)
{
	char * line =  (char*) calloc (1 , sizeof(char));
	char c;
	if(line==NULL)
	{
		return "no allocation";
	}
	*line = EOS;
	while(((c=fgetc(ifp)) != '\n') && (c != EOF))
	{
		char *temp=(char*)realloc(line, (strlen(line) + 2) * sizeof(char));
		if(temp==NULL)
		{
			return "no allocation";
		}
		line = temp;
		*(line + strlen(line) + 1) = EOS;
		*(line + strlen(line)) = c;		
	}
	return line;
}

int count_labels(FILE *ifp)
{
	int count = 0;
	long end;
	char * line;
	long pos = ftell(ifp);
	fseek(ifp, 0, SEEK_END);
	end = ftell(ifp);
	fseek(ifp,0,SEEK_SET);
	while(ftell(ifp) != end)
	{
		if(fgetc(ifp) == ':')
		{
			count++;
		}
	}
	fseek(ifp,0,SEEK_SET);
	while(ftell(ifp) != end)/* while not end of file */
	{
		line = read_line(ifp);
		if(strncmp(".extern", line, 7) == 0)/* if extern label declared */
		{
			count++;
		}
	}
	fseek(ifp, pos, SEEK_SET);
	return count;
}

int hasNonWhitespace(char * line, int size)
{
	int i;
	for (i = 0; i < size; i++) 
	{
		if (!isspace(line[i]))
		{
			return 1;
		}
	}
	return 0;
}
