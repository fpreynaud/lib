#include <stdio.h>
#include <stdlib.h>
#define nl() printf("\n");

int* get_indexes(char* source);
int is_whitespace(char c);
void mystrncpy(char* target, char* source, int limit);
int strlen_d(char* input, char delim);
int size=0;

int* get_indexes(char* source)
{
	int len = strlen_d(source,'\0');
	int* result = (int*)malloc(len * sizeof(int));
	int i = 0, j = 0;
	int n = 0;

	for(i = 0; i < len; i++)
		result[i] = 0;
	i = 0;

	while (i < len)
	{
		if(source[i] >= '0' && source[i] <= '9')
			n = 16 * (source[i] - '0');
		else if (source[i] >= 'A' && source[i] <= 'F')
			n = 16 * (source[i] - 'A' + 0x0A);
		else if (source[i] >= 'a' && source[i] <= 'f')
			n = 16 * (source[i] - 'a' + 0x0A);
		else
		{
			i++;
			n = 0;
			continue;
		}

		if(i + 1 < len)
		{
			if(source[i+1] >= '0' && source[i+1] <= '9')
				n += (source[i+1] - '0');
			else if (source[i+1] >= 'A' && source[i+1] <= 'F')
				n += (source[i+1] - 'A' + 0x0A);
			else if (source[i+1] >= 'a' && source[i+1] <= 'f')
				n += (source[i+1] - 'a' + 0x0A);
		}

		result[j++] = n % 128;
		n = 0;
		i += 2;
		size++;
	}

	return result;
}

int is_whitespace(char c)
{
	switch(c)
	{
		case ' ':
		case '\t':
		case '\n':
			return 1;
		default:
			return 0;
	}
}

void mystrncpy(char* target, char* source, int limit)
{
	int i;

	for(i = 0; i < limit && source[i] != '\0';i++)
		target[i] = source[i];

	target[i] = '\0';
}

int strlen_d(char* input, char delim)
{
	int i = 0;
	int len = 0;

	for(i = 0; input[i] != delim; i++)
		len++;

	return len;
}

int main(int argc, char** argv)
{
	char* s = NULL;
	char** ascii_table;
	int* indexes = NULL;
	int len = 0, i = 0;

	if(argc < 2)
	{
		fprintf(stderr,"Not enough arguments given.\nUsage: %s <hexa string to convert>\n", argv[0]);
		exit(1);
	}

	s = argv[1];

	len = strlen_d(s, '\0');
	ascii_table = (char**)malloc(128*sizeof(char*));

	for(i = 0; i < 8; i++)
		ascii_table[i] = (char*)malloc(4*sizeof(char));

	mystrncpy(ascii_table[0],"NUL",3);
	mystrncpy(ascii_table[1],"SOH",3);
	mystrncpy(ascii_table[2],"STX",3);
	mystrncpy(ascii_table[3],"ETX",3);
	mystrncpy(ascii_table[4],"EOT",3);
	mystrncpy(ascii_table[5],"ENQ",3);
	mystrncpy(ascii_table[6],"ACK",3);
	mystrncpy(ascii_table[7],"BEL",3);

	for(i = 8; i < 16; i++)
		ascii_table[i] = (char*)malloc(3*sizeof(char));

	mystrncpy(ascii_table[8],"BS",2);
	mystrncpy(ascii_table[9],"HT",2);
	mystrncpy(ascii_table[10],"LF",2);
	mystrncpy(ascii_table[11],"VT",2);
	mystrncpy(ascii_table[12],"FF",2);
	mystrncpy(ascii_table[13],"CR",2);
	mystrncpy(ascii_table[14],"SO",2);
	mystrncpy(ascii_table[15],"SI",2);


	for(i = 16; i < 25; i++)
		ascii_table[i] = (char*)malloc(4*sizeof(char));

	mystrncpy(ascii_table[16],"DLE",3);
	mystrncpy(ascii_table[17],"DC1",3);
	mystrncpy(ascii_table[18],"DC2",3);
	mystrncpy(ascii_table[19],"DC3",3);
	mystrncpy(ascii_table[20],"DC4",3);
	mystrncpy(ascii_table[21],"NAK",3);
	mystrncpy(ascii_table[22],"SYN",3);
	mystrncpy(ascii_table[23],"ETB",3);
	mystrncpy(ascii_table[24],"CAN",3);

	ascii_table[25] = (char*)malloc(3*sizeof(char));
	mystrncpy(ascii_table[25],"EM",2);

	for(i = 26; i < 28; i++)
		ascii_table[i] = (char*)malloc(4*sizeof(char));

	mystrncpy(ascii_table[26],"SUB",3);
	mystrncpy(ascii_table[27],"ESC",3);

	for(i = 28; i < 32; i++)
		ascii_table[i] = (char*)malloc(3*sizeof(char));

	mystrncpy(ascii_table[28],"FS",2);
	mystrncpy(ascii_table[29],"GS",2);
	mystrncpy(ascii_table[30],"RS",2);
	mystrncpy(ascii_table[31],"US",2);

	ascii_table[32] = (char*)malloc(6*sizeof(char));
	mystrncpy(ascii_table[32],"SPACE",5);

	for(i = 33; i < 127; i++)
	{
		ascii_table[i] = (char*)malloc(2*sizeof(char));
		ascii_table[i][0] = (char)i;
		ascii_table[i][1] = '\0';
	}

	ascii_table[127] = (char*)malloc(4*sizeof(char));
	mystrncpy(ascii_table[127], "DEL", 3);

	/*
	for(i = 0; i < 128; i++)
	{
		printf("%d\t%s\n",i, ascii_table[i]);
	}
	*/

	indexes = get_indexes(s);

	for(i = 0; i < size; i++)
		printf("%s ", ascii_table[indexes[i]]);
	nl()

	return 0;
}


