#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>

typedef struct Token {
	int ID;
	int value;
	int line;
	int characternum;
	char* characters;
} Token;

typedef struct Symbol {
	long long int address;
	char type;
	char symbol[10];
} Symbol;

typedef struct Node {
	int ID;
	int value;
	Symbol* symbol;
	struct Node* left;
	struct Node* right;
} Node;

/**************************************************************
Functions for the Symbol Table
**************************************************************/

void start_symbol();

Symbol* newSymbol(char*,char);

Symbol* findSymbol(char*);

void freeSymbol();

int* makeInt(int);

/**************************************************************
Here are the functions for Stack
**************************************************************/

int tree_To_Code(Node*);

int call_program();

void finish();

void littleEndian(int);

void littleEndian(long long int);

/**************************************************************
Here are the functions for the Node
**************************************************************/

void Print_tree(Node *);

Node* get_tree();

Node* get_arith(Node*);

Node* get_parenth(Node*);

Node* get_multy(Node*);

Node* get_power(Node*);

Node* get_unary(Node*);

Node* get_terminal(Node*);

Node* create_node(Token*);

/*********************************************************
Seperates the tree functions from the node token functions
*********************************************************/

int initialize (char *);

Token* get_token();

void token_clear(Token*);

void token_print(Token*);

int ifend();

/********************************************************
Functions that will be hard coded to be called
*********************************************************/

void print(long int);

int read();

#endif
