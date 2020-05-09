#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <ctype.h>
#include "include/parser.h"

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("usage: ./interpreter xx.tbl \n");

		return -1;
	}

	FILE *fp = fopen(argv[1], "rb");
	if(!fp)
	{
		printf("read file :%s error\n", argv[1]);
		
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);

	char *src = (char*)malloc(lSize + 1);
	if(!src)
	{
		printf("alloc failed!\n");
		
		return -1;
	}
	
	memset(src, 0, lSize + 1);
	rewind(fp); 
	
	fread(src, sizeof(char), lSize, fp);
	
	parser(src);

	//parser("def abc(a, b){ c = a; d = b;  e = c * d^2; return e * 3;} p = abc(100, 2);");
	
	free(src);

	printf("Press any key to continue!\n");
	
	getchar();
	
	return 0;
}
