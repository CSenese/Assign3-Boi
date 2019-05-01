#include "Tree.h"

int p_offset = 0;
char* program = (char *)  mmap(0, 50000, PROT_EXEC | PROT_READ | PROT_WRITE,
			 MAP_PRIVATE | MAP_ANONYMOUS,
			   -1, 0);
int catchit;

int tree_To_Code(Node* current_node) {
	
	if(errno)
    {
      perror("mmap");
      return 2;
    }
	
	Node* temp = current_node;
	//This is how it is recursive. Does not change the original current node.
	if (temp->left != NULL) {
		catchit = tree_To_Code(temp->left);
	}
	if (temp->right != NULL) {
		catchit = tree_To_Code(temp->right);
	}
	
	switch (temp->ID) {
		case 24 : {
			/*push move into register then push onto stack*/
			program[p_offset++] = '\xB8';
			littleEndian(temp->value);
			program[p_offset++] = '\x50';
			break;
		}
		case 2 : {
			/*pop from stack */
			program[p_offset++] = '\x59';
			program[p_offset++] = '\x58';
			/* add EAX and ECX */
			program[p_offset++] = '\x03';
			program[p_offset++] = '\xC1';
			
			/* push from EAX */
			program[p_offset++] = '\x50';
			break;
		}
		case 3 : {
			/*pop from stack */
			program[p_offset++] = '\x59';
			program[p_offset++] = '\x58';
			
			/* subract EAX and ECX */
			program[p_offset++] = '\x2B';
			program[p_offset++] = '\xC1';
			
			/* push from EAX */
			program[p_offset++] = '\x50';
			break;
		}
		case 31 : {
			/*pop from stack */
			program[p_offset++] = '\x59';
			program[p_offset++] = '\x58';
			
			/* multiply EAX and ECX */
			program[p_offset++] = '\xF7';
			program[p_offset++] = '\xE1';
			
			/* push from EDX */
			program[p_offset++] = '\x50';
			break;
		} 
		case 4 : {
			program[p_offset++] = '\x33';
			program[p_offset++] = '\xD2';
			program[p_offset++] = '\x33';
			program[p_offset++] = '\xC0';
			
			/*pop from stack */
			program[p_offset++] = '\x59';
			program[p_offset++] = '\x58';			
			
			/* divides EDX:EAX and ECX */
			program[p_offset++] = '\xF7';
			program[p_offset++] = '\xF9';
			
			/* push from EAX */
			program[p_offset++] = '\x50';
			break;
		}
		case 33: {
			//pop from stack
			program[p_offset++] = '\x58';
			
			//negate eax
			program[p_offset++] = '\xF7';
			program[p_offset++] = '\xD8';
			
			//push eax
			program[p_offset++] = '\x50';
			break;
		}
		case 5 : {
			program[p_offset++] = '\x45';  // XOR r8, r8  // e = 0
 			program[p_offset++] = '\x31';  
  			program[p_offset++] = '\xc0';
  			program[p_offset++] = '\x41';  // POP r9
  			program[p_offset++] = '\x8f';
  			program[p_offset++] = '\xc1';
  			program[p_offset++] = '\x41';  // POP r10
  			program[p_offset++] = '\x8f';
 			program[p_offset++] = '\xc2';
  			program[p_offset++] = '\x45';  // TEST r10, r10 // if b < 0 goto end
  			program[p_offset++] = '\x85';  
  			program[p_offset++] = '\xd2';
  			program[p_offset++] = '\x7c';  // JL  end
  			program[p_offset++] = '\x1e';
  			program[p_offset++] = '\x41';  // INC r8
  			program[p_offset++] = '\xff';
  			program[p_offset++] = '\xc0';
  			program[p_offset++] = '\x45';  // TEST r10, r10 // if b == 0  done // loop
  			program[p_offset++] = '\x85';  // must be retested since INC r8 
  			program[p_offset++] = '\xd2';  // messes with flags
  			program[p_offset++] = '\x74';  // JE  end
  			program[p_offset++] = '\x16';  
  			program[p_offset++] = '\x41';  // TEST r10, 0x01 // if b & 0x01
  			program[p_offset++] = '\xf7';
  			program[p_offset++] = '\xc2';
  			program[p_offset++] = '\x01';
  			program[p_offset++] = '\x00';
  			program[p_offset++] = '\x00';
  			program[p_offset++] = '\x00';
  			program[p_offset++] = '\x74';  // JE square
  			program[p_offset++] = '\x04';
  			program[p_offset++] = '\x45';  // IMUL r8, r9 // e *= a
  			program[p_offset++] = '\x0f';
  			program[p_offset++] = '\xaf';
  			program[p_offset++] = '\xc1';
 			program[p_offset++] = '\x45';  // IMUL r9, r9 // a *= a // square;
  			program[p_offset++] = '\x0f';
  			program[p_offset++] = '\xaf';
  			program[p_offset++] = '\xc9';
  			program[p_offset++] = '\x41';  // SAR r10, 1 // b >>= 1
  			program[p_offset++] = '\xd1';
  			program[p_offset++] = '\xfa';
  			program[p_offset++] = '\xeb';  // JMP loop
  			program[p_offset++] = '\xe5';
  			program[p_offset++] = '\x41';  // MOV EAX, r8
  			program[p_offset++] = '\x8b';
  			program[p_offset++] = '\xc0';
  			program[p_offset++] = '\xff';
  			program[p_offset++] = '\xf0'; //PUSH EAX
			break;
		}
		case 25 : {
			int i = 0;
			if (temp->symbol->type == 'F') {
				while (i < temp->value) {
					program[p_offset++] = '\x8f';  //POP RDI
					program[p_offset++] = '\xc7';
					program[p_offset++] = '\x48';  // MOV RSI, &func
  					program[p_offset++] = '\xbe';
					long long int addr = temp->symbol->address;
					littleEndian(addr);
					program[p_offset++] = '\xff';  // CALL RSI
  					program[p_offset++] = '\xd6';
  					i++;
				}
			}
			break;
		}
	}
	return 0;
}
//used this as a stand alone function as opposed to at the end of generate the code tree
//this is because I couldn't find a good way to find the top of the recursion.
int call_program() {
	program[p_offset++] = '\x58';
	program[p_offset++] = '\xC3';
	
	int result = ((int(*)())(program))();
	
	p_offset = 0;
	
	return result;
}

void finish() {
	munmap(program, 50000);
	freeSymbol();
}

void littleEndian(int i) {
	program[p_offset++] = (char)(i & 0xFF);
	program[p_offset++] = (char)((i >> 8) & 0xFF);
	program[p_offset++] = (char)((i >> 16) & 0xFF);
	program[p_offset++] = (char)((i >> 24) & 0xFF);
}

void littleEndian(long long int i) {
	program[p_offset++] = (char)(i & 0xFF);
	program[p_offset++] = (char)((i >> 8) & 0xFF);
	program[p_offset++] = (char)((i >> 16) & 0xFF);
	program[p_offset++] = (char)((i >> 24) & 0xFF);
	program[p_offset++] = (char)((i >> 32) & 0xFF);
	program[p_offset++] = (char)((i >> 40) & 0xFF);
	program[p_offset++] = (char)((i >> 48) & 0xFF);
	program[p_offset++] = (char)((i >> 56) & 0xFF);
	
}
