#include "output.h"
#include "utils.h"

void write_to_output_file(char * file_name)
{
	FILE * ofp;
	long beg;/*will hold beggining position of ofp*/
	strcat(file_name, ".ob");
	ofp = fopen(file_name, "w"); 	
	if(ofp == NULL) 
	{ 
		printf("Failed to open file.\n");
		return;
	}
	else
	{
		int i = 0;
		int j = 0;
		if(IC + DC > 156)
		{
			fprintf(stderr, "ERROR - no allocation\n");
		}
		fprintf(ofp, "\t%d\t%d\n", IC, DC);
		while(codes[i].code_line != NULL)/* while still code to print */
		{
			j = 0;
			for(j = 0; j < codes[i].num_of_words; j++)
			{
					fprintf(ofp, "0%d\t", (codes[i].address) + j);
					printBinary(codes[i].code_line[j], ofp);
			}
			i++;
		}
		for(i = 0; i < DC; i++)
		{
			fprintf(ofp, "0%d\t", (IC + 100 + i));
			printBinary(data_table[i], ofp);
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

void printBinary(short num, FILE * ofp)
{
	int i;
	unsigned short mask = 1 << (BITS_TO_PRINT - 1);
	for(i = 0; i < BITS_TO_PRINT; i++)
	{
		if(mask & num)
		{
			fprintf(ofp, "/");
		}
		else
		{
			fprintf(ofp, ".");
		}
		mask >>= 1;
	}
	fprintf(ofp, "\n");
}
