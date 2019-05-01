#include "Tree.h"

Token* current_token = NULL;

//The grand father function.  This is where it will go before anywhere else.
Node* get_tree() {
	
	if (current_token == NULL) {
		current_token = get_token();
	}
	
	Node* current_node = NULL;
	
	//currently the switch statement is useless but when we add in more stuff later it will change
	switch(current_token->ID) {
		case 24 : {
			current_node = create_node(current_token);
			
			current_node = get_arith(current_node);
			
			if (current_node->ID == -1) {
				printf("\nError processing token before token at line %d and character %d",current_token->line,current_token->characternum);
				return NULL;
			}
			return current_node;
		}
		case 11 :  {
			current_node = create_node(current_token);
			
			current_node = get_parenth(current_node);
			printf("Got here 1\n");
			printf("Current Token: %d\n",current_token->ID);
			printf("Got here 2\n");
			if (current_node->ID == -1) {
				printf("\nError processing token before token at line %d and character %d",current_token->line,current_token->characternum);
				return NULL;
			}
			return current_node;
		}
		case 3 : {
			current_node = create_node(current_token);
			
			current_node = get_arith(current_node);
			
			if (current_node->ID == -1) {
				printf("\nError processing token before token at line %d and character %d",current_token->line,current_token->characternum);
				return NULL;
			}
			return current_node;
		}
		case 25 : {
			Symbol* check = findSymbol(current_token->characters);
			if (check->type == 'F') {
				current_node = create_node(current_token);
				current_node->value = 1;
				current_node->symbol = check;
				current_token = get_token();
				current_node->left = get_tree();
				
				printf("Current Token: %d",current_token->ID);
			}
			break;
		}
		case 22 : {
			current_token = get_token();
			return current_node;
			break;
		}
	}
}


//First order, get +,-
// E -> T | T + F | T - F
//T is terminal F is Factor
Node* get_arith(Node* current_node) {
	current_node = get_multy(current_node);
	while (1) {
		if (current_token->ID == 2 || current_token->ID == 3) {
			
			Node* temp = create_node(current_token);
			
			current_token = get_token();
			
			temp->left = current_node;
			temp->right = get_multy(temp);
			
			current_node = temp;
			
			if (temp->right->ID == -1) {
				return temp->right;
			}
			
		} else {
			return current_node;
		}
	}
}


//second order, get *,/,mod
// F -> T | T * P | T / P \ T mod P
//P is power
Node* get_multy(Node* current_node) {
	current_node = get_power(current_node);
	while (1) {
		if (current_token->ID == 31 || current_token->ID == 4 || current_token->ID == 25) {
			
			if (current_token->ID == 25 && 0 != strcmp(current_token->characters,"mod")) {
				Token temptoken = {-1,-1,-1,-1,current_token->characters};
				Node* badnode = create_node(&temptoken);
				return badnode;
			}
			
			Node* temp = create_node(current_token);
			
			current_token = get_token();
			
			
			temp->left = current_node;
			temp->right = get_power(temp);
			
			current_node = temp;
			
			if (temp->right->ID == -1) {
				return temp->right;
			}
		} else {
			return current_node;
		}
	}
}


//Third order power
//P -> T | T^P
Node* get_power(Node* current_node) {
	current_node = get_unary(current_node);
	if (current_token->ID == 5) {
		Node* temp = create_node(current_token);
		
		current_token = get_token();
		
		temp->left = current_node;
		temp->right = get_power(temp);
		
		current_node = temp;
		
		if (temp->right->ID == -1) {
			return temp->right;
		}
		
		return current_node;
		
	} else {
		return current_node;
	}
}


//Fourth order unary
//P -> -T | +T | T
Node* get_unary(Node* current_node) {
	if (current_token->ID == 2 || current_token->ID == 3) {
		
		Node* temp = create_node(current_token);
		
		if (current_token->ID == 2)
			temp->ID = 32;
		if (current_token->ID == 3) {
			temp->ID = 33;
		}
		
		current_token = get_token();
		
		Node* another = get_parenth(current_node);
		
		temp->left = another;
		
		current_node = temp;
		
		return current_node;
		
	} else {
		current_node = get_parenth(current_node);
		return current_node;
	}
}


//Fifth order parentheses
//P -> T | (E)
Node* get_parenth(Node* current_node) {
	if (current_token->ID == 11) {
		current_token = get_token();		
		current_node = get_tree();
		
		if (current_token->ID == 12) {
			current_token = get_token();
			return current_node;
		} else {
			Token temptoken = {-1,-1,-1,-1,current_token->characters};
			Node* badnode = create_node(&temptoken);
			return badnode;
		}
	}
	return get_terminal(current_node);
}


//Last Order Terminal
// P -> T
Node* get_terminal(Node* current_node) {
	if (current_token->ID == 24) {
		current_node = create_node(current_token);
		current_token = get_token();
		return current_node;
	} else {
		Token temptoken = {-1,-1,-1,-1,current_token->characters};
		Node* badnode = create_node(&temptoken);
		return badnode;
	}
}


//Allocates the memory for the node
Node* create_node(Token* atoken) {
	void* aspace = malloc(sizeof(Node));
	Node* anode = (struct Node* )aspace;
	anode->ID = atoken->ID;
	if (atoken->ID == 24) {
		anode->value = atoken->value;
	}
	return anode;
}



void Print_tree(Node* anode) {
	Node* temp = anode;
	
	if (temp->left != NULL) {
		Print_tree(temp->left);
	}
	if (temp->right != NULL) {
		Print_tree(temp->right);
	}
	
	printf("%d(%d)\t",temp->ID,temp->value);
}
