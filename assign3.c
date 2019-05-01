#include "Tree.h"

int main (int arg,char *argV[]) {

	int catchit = initialize(argV[1]);
	
	if (catchit = 0) {
		printf("Could not open the file\n");
	}
	
	start_symbol();
		
	Node* current_tree = get_tree();
	
	if (current_tree->ID == -1) {
		printf("Something went wrong\n");
	}
			
	Print_tree(current_tree);
		
	catchit = tree_To_Code(current_tree);
		
	if (catchit == 2) {
		printf("Something went wrong with that last tre");
	}
		
	int result = call_program();
		
	printf("\nResult of operation: %d",result);
		
	printf("\n\n");
	
	finish();
	return 0;
}
