#include "Tree.h"

void* int4_space = malloc(sizeof(int) * 100);
int* int4_table = (int*) int4_space;

void* symbol_space = malloc(sizeof(Symbol) * 100);
Symbol* symbol_table = (struct Symbol* )symbol_space;

int int4_offset = 0;
int symbol_offset = 0;

void print (long int i)
{
    printf("Value:  %d\n",i);
}

int read() {
	
	int a;
	
	scanf("%d",&a);
	
	return a;
}

void start_symbol() {
	char temp[6] = {'p','r','i','n','t','\0'};
	newSymbol(temp,'F');
	symbol_table[symbol_offset - 1].address = (long long int)print;
	char temp2[5] = {'r','e','a','d','\0'};
	newSymbol(temp2,'F');
	symbol_table[symbol_offset - 1].address = (long long int)read;
	char temp3[5] = {'i','n','t','4','\0'};
	newSymbol(temp3,'T');
	symbol_table[symbol_offset - 1].address = (long long int)int4_space;
}

Symbol* newSymbol(char* symbold,char typed) {
	struct Symbol temp;
	temp.type = typed;
	strcpy(temp.symbol,symbold);
	symbol_table[symbol_offset++] = temp;
	return &symbol_table[symbol_offset - 1];
}

void freeSymbol() {
	free(symbol_table);
	free(int4_space);
}

Symbol* findSymbol(char* findit) {
	int i = 0;
	while (i < symbol_offset) {
		if (strcmp(symbol_table[i].symbol,findit) == 0) {
			return &symbol_table[i];
		}
	}
	return NULL;
}

int* makeInt(int value) {
	int4_table[int4_offset++] = value;
	return &int4_table[int4_offset - 1];
}
