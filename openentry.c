#include "openentry.h"
#include "utils.h"

void write_to_entry_file(FILE *ifp, char * file_name) 
{
	FILE * ofp;
	char line[MAX_LINE];
	long beg;/*will hold beggining position of ofp*/
	strcat(file_name, ".ent");
	ofp = fopen(file_name, "w");/*create entry file*/
	fseek(ifp, 0, SEEK_SET);
	if (ofp == NULL) 
	{
		printf("Cannot open file px.ent for writing.\n\n");
		return;
	}
	while (fgets(line, sizeof(line), ifp)) 
	{
		char * entry_ptr = strstr(line, ".entry");/*checks if line is '.entry'*/
		char label_name[MAX_LABEL_NAME] = {0}; 
		if (entry_ptr != NULL) 
		{
			char * label_ptr = entry_ptr + strlen(".entry");/*label_ptr is string from after '.entry' */
			while (isspace(* label_ptr)) 
			{
				label_ptr++; /*skip character*/
			}
			sscanf(label_ptr, "%s", label_name);

			fprintf(ofp, "%s\t%d\n", label_name, get_label_address(label_name,ifp));/*prints label name and address to entry file */
		}
	}
	fseek(ofp, 0, SEEK_SET);
	beg = ftell(ofp);
	fseek(ofp, 0, SEEK_END);
	if(beg == ftell(ofp))
	{
		remove(file_name);
	}

	fclose(ofp);
}

int get_label_address(char * label_name, FILE *ifp) 
{
	int i;
	int numlabels = count_labels(ifp);
	for (i = 0; i < numlabels; i++) 
	{
		if (strcmp(labels[i].label_name, label_name) == 0) 
		{
			return labels[i].label_address;
		}
	}
	return -1;
}
