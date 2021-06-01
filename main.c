#include <stdio.h>
#include <stdlib.h>
#include "xml.h"

int main()
{
	char* dataBegin = malloc(10000), *iterator;
	while(fgets(dataBegin, 10000, stdin) != 0)
	{
		iterator=dataBegin;
		while(*iterator != '\n')
			iterator++;
		*iterator=0;
		fprintf(stdout, "input: %s\n", dataBegin);
		xmlIn(dataBegin);
		fputs("\n\n", stdout);
	}
	free(dataBegin);
	return 0;
}
