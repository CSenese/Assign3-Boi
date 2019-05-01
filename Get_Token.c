#include "Tree.h"

FILE* fp;

char word[100];
struct Token atoken = {0,0,0,0,word};
/****************************************************************
****************************************************************/
int initialize(char *filename) {
	if (fp = fopen(filename,"rb")) {
		return 0;
	} else {
		printf("The file could not be read");
		return 1;
	}
	return 2;
}

/*
cc values
0	strings				DONE
1	escape sequence		WIP
2	Real				DONE
3	Standard			DONE
4	two characters		DONE
5	number				DONE
6	identifier			DONE
7	Comment				DONE
*/


/****************************************************************\
This is my state machine.  It has 7 states, one for each listed above.
If a token is found to be an error token, the ID is changed to -1
and the machine retrieves the rest of the tokens.  State condition
code = 3 is the starting state and all 7 states are accepting.

Value of the token is both used to increment the character counter
and as the value of the integer tokens.

I used a 100 character buffer to hold the token string.
****************************************************************/

int CHARCOUNTER = 1;
int LINECOUNTER = 1;
	
Token* get_token() {
	token_clear(&atoken);

	char character;
	int cc = 3;
	while(1) {
		fread(&character,sizeof(char),1,fp);
		if (feof(fp)) {
			atoken.characters[atoken.value] = character;
			atoken.characters[atoken.value + 1] = '\0';
			return &atoken;
		}
		CHARCOUNTER++;
		if (character == '\n') {
			CHARCOUNTER = 1;
			LINECOUNTER++;
		}
		atoken.line = LINECOUNTER;
		atoken.characternum = CHARCOUNTER;
		
		/****************************************************************
		This is for strings.  It looks for another " and until then it
		just adds to the string.  Also when an escape \ is found, it changes
		the state to the escape character escape
		****************************************************************/
		
		if (cc == 0) {
			if (atoken.value > 98) {
				printf("Error:  String token is too long\r\n");
				atoken.ID = -1;
				atoken.characters[atoken.value] = '\0';
				return &atoken;
				cc = 3;
				continue;
			}
			switch (character) {
				case '\\' : {
					cc = 1;
					break;  }
				case '\"' : {
					atoken.characters[atoken.value] = character;
					atoken.characters[atoken.value + 1] = '\0';
					cc = 3;					
					return &atoken;
					}
				default: {
					atoken.characters[atoken.value] = character;
					atoken.value++;
				}
			}
		continue;
		
		/****************************************************************
		This is for the escape characters.  This is went to immediately after
		a \ is found.  If it reads anything
		****************************************************************/
		
		} else if (cc == 1) {
			char buffer [6];
			long temp = 0;
			char one,two,three;
			switch(character) {
				case 'n' : {
					atoken.characters[atoken.value] = (char) 10;
					atoken.value++;
					cc = 0;
					break;  }
				case 't' : {
					atoken.characters[atoken.value] = (char) 9;
					atoken.value++;
					cc = 0;
					break;  }
				case 'r' : {
					atoken.characters[atoken.value] = (char) 13;
					atoken.value++;
					cc = 0;
					break;  }
				case '\"' : {
					atoken.characters[atoken.value] = (char) 34;
					atoken.value++;
					cc = 0;
					break;  }
				case '\\': {
					atoken.characters[atoken.value] = (char) 92;
					atoken.value++;
					cc = 0;
					break;  }
				case 'a' : {
					atoken.characters[atoken.value] = (char) 7;
					atoken.value++;
					cc = 0;
					break;  }
				case 'b' : {
					atoken.characters[atoken.value] = (char) 8;
					atoken.value++;
					cc = 0;
					break;  }
				case 'u' : {
					fread(buffer,6,1,fp);
					int i;
					for (i = 0;i <6;i++) {
						switch(buffer[i]) {
							case 'a' ... 'f' : {
								temp += (buffer[i] - 87);
								if (i != 5)
									temp = temp << 4;
								break;
							}
							case '0' ... '9' : {
								temp += (buffer[i] - 48);
								if (i != 5)
									temp = temp << 4;
								break;
							}
							case 'A' ... 'F' : {
								temp += (buffer[i] - 55);
								if (i != 5)							
									temp = temp << 4;					
								break;
							}
							default : {
								atoken.ID = -1;
								break;
							}
						}
					}
					one = temp >> 14;
					two = (temp >> 7) & 127; 
					three = temp & 127;
					
					if (one != 0) {
						atoken.characters[atoken.value] = one;
						atoken.value++;
					}
					if (two != 0) {
						atoken.characters[atoken.value] = one;
						atoken.value++;
					}
					if (three != 0) {
						atoken.characters[atoken.value] = one;
						atoken.value++;
					}
					
					cc = 0;
					break;  }
				default : {
					atoken.ID = -1;
					atoken.characters[atoken.value] = '\\';
					atoken.value++;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					cc = 0;
				}
			}
		continue;
		} else if (cc == 3) {
			/****************************************************************
			This is the starting state.  It looks primarily for the one character
			tokens and the beginning of anything else then changes the state
			accordingly.
			****************************************************************/
			switch(character) {
				case '"' : {
					atoken.characters[atoken.value] = character;
					atoken.value++;
					cc = 0;
					atoken.ID = 1;
					break;
					}
				case '+' : {
					atoken.ID = 2;
					atoken.characters[atoken.value + 1] = '\0'; 
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '-' : {
					atoken.ID = 3;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '/' : {
					atoken.ID = 4;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '^' : {
					atoken.ID = 5;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '<' : {
					cc = 4;
					atoken.ID = 6;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;  }
				case '>' : {
					cc = 4;
					atoken.ID = 7;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;  }
				case '=' : {
					cc = 4;
					atoken.ID = 8;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;  }
				case '~' : {
					cc = 4;
					atoken.ID = 9;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;  }
				case '!' : {
					atoken.ID = 10;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '(' : {
					atoken.ID = 11;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case ')' : {
					atoken.ID = 12;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '{' : {
					atoken.ID = 13;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '}' : {
					atoken.ID = 14;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '[' : {
					atoken.ID = 15;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case ']' : {
					atoken.ID = 16;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '&' : {
					atoken.ID = 17;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '|' : {
					atoken.ID = 18;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '.' : {
					atoken.ID = 19;
					cc = 2;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;  }
				case '@' : {
					atoken.ID = 20;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case ':' : {
					atoken.ID = 21;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case ';' : {
					atoken.ID = 22;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					break;  }
				case ',' : {
					atoken.ID = 23;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
					break;  }
				case '0' ... '9' : {
					cc = 5;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					atoken.ID = 24;
					break;  }
				case 'a' ... 'z' : {
					cc = 6;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					atoken.ID = 25;
					break;  }
				case 'A' ... 'Z' : {
					cc = 6;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					atoken.ID = 25;
					break;  }
				case '#' : {
					cc = 7;
					atoken.characters[0] = '#';
					break;  }
				case '*' : {
					atoken.ID = 31;
					atoken.characters[atoken.value + 1] = '\0';
					atoken.characters[atoken.value] = character;
					return &atoken;
				}
				default: {
					break;  }
			}
		continue;
		} else if (cc == 5) {
			if (atoken.value > 98) {
				printf("Number too long");
				atoken.ID = -1;
				atoken.characters[atoken.value] = '\0';
				cc = 3;
				return &atoken;
			}
			/****************************************************************
				This is for integers, it looks for 0-9 and makes sure that 0 is not
				the first digit.  A zero by itself is handled by anything else
				coming along into this state.
				It also changes the state if it gets anything that makes REAL numbers
				come into play.
			****************************************************************/
			switch(character) {
				case '0' ... '9' : {
					if (atoken.characters[0] == '0') {
						atoken.characters[atoken.value] = character;
						atoken.value++;
						atoken.characters[atoken.value] = '\0';
						atoken.ID = -1;
						return &atoken;
						break;
					} else {
						atoken.characters[atoken.value] = character;
						atoken.value++;
						break;
					}
				}
				case '.' : {
					atoken.ID = 19;
					cc = 2;
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				}
				case 'e' : {
					if (atoken.characters[0] == '0') {
						atoken.characters[atoken.value] = character;
						atoken.value++;
						atoken.ID = -1;
						break;
					} else {
						atoken.ID = 19;
						cc = 2;
						atoken.characters[atoken.value] = character;
						atoken.value++;
						break;
					}
				}
				case 'E' : {
					if (atoken.characters[0] == '0') {
						atoken.characters[atoken.value] = character;
						atoken.value++;
						atoken.ID = -1;
						break;
					} else {
						atoken.ID = 19;
						cc = 2;
						atoken.characters[atoken.value] = character;
						atoken.value++;
						break;
					}
				}
				default : {
					ungetc(character,fp);
					CHARCOUNTER--;
					atoken.characters[atoken.value] = '\0'; 
					atoken.value = atoi(atoken.characters);
					cc = 3;					
					return &atoken;
				}
			}
		continue;
		} else if (cc == 7) {
			/****************************************************************
			This is for the comments. It looks for the end of the line if the first
			character in the token is a # or it looks for the ->>
			****************************************************************/
			switch (character) {
				case '\n' : {
					if (atoken.characters[0] == '#') {
						cc = 3;
						token_clear(&atoken);
						break;
					} else {
						break;
					}
				}
				case '-' : {
					fread(&character,sizeof(char),1,fp);
					if (feof(fp)) {
						token_clear(&atoken);
						return &atoken;
					}
					CHARCOUNTER++;
					if (character == '>') {
						fread(&character,sizeof(char),1,fp);
						if (feof(fp)) {
							token_clear(&atoken);
							return &atoken;
						}
						CHARCOUNTER++;
						if (character == '>') {
							token_clear(&atoken);
							cc = 3;
							break;
						} else {ungetc(character,fp);CHARCOUNTER--;}
					} else {ungetc(character,fp);CHARCOUNTER--;}
				break;
				}
				default : {
					break;
				}
			}
		continue;
		} else if (cc == 4) {
			/****************************************************************
				This is for two character tokens. it switches around the first
				character in the token so far.  This is because there were 4
				options for 2nd characters and 2 options for first.
				It also looks for the beginning of comment block by reading
				at lease one extra letter.
			****************************************************************/
			switch (atoken.characters[0]) {
				case '<' :  {
					if (character == '=') {
						atoken.ID = 26;
						atoken.characters[1] = '=';
						atoken.characters[2] = '\0';
						cc = 3;						
						return &atoken;
					} else if (character == '-'){
						atoken.ID = 27;
						atoken.characters[1] = '-';
						atoken.characters[2] = '\0';
						cc = 3;						
						return &atoken;
					} else if (character == '<') {
						atoken.characters[1] = '<';
						fread(&character,sizeof(char),1,fp);
						if (feof(fp)) {
							atoken.characters[atoken.value] = '\0';
							return &atoken;
						}
						CHARCOUNTER++;
						if (character == '-') {
							cc = 7;
							break;
						} else {
							ungetc(character,fp);
							CHARCOUNTER--;
							atoken.ID = -1;
							atoken.characters[2] = '\0';
							cc = 3;							
							return &atoken;
						}
					} else {ungetc(character,fp);
							CHARCOUNTER--;
							atoken.ID = -1;
							atoken.characters[1] = '\0';
							cc = 3;							
							return &atoken;
						}
					break;
				}
				case '>' : {
					if (character == '=') {
						atoken.ID = 28;
						atoken.characters[1] = '=';
						atoken.characters[2] = '\0';
						cc = 3;
						return &atoken;
					} else {
						atoken.ID = -1;
						atoken.characters[atoken.value] = '\0';
						ungetc(character,fp);
						CHARCOUNTER--;
						cc = 3;
						return &atoken;
					}
					break;
				}
				case '~' : {
					if (character == '=') {
						atoken.ID = 29;
						atoken.characters[1] = '=';
						atoken.characters[2] = '\0';
						cc = 3;						
						return &atoken;
					} else {
						atoken.ID = -1;
						atoken.characters[atoken.value] = '\0';
						ungetc(character,fp);
						CHARCOUNTER--;
						cc = 3;						
						return &atoken;
					}
					break;
				}
				default : {
					ungetc(character,fp);
					cc = 3;
					return &atoken;
				}
			}
		continue;
		} else if (cc == 2) {
			/****************************************************************
				This is the REAL number section.  It has to look through the 
				token so far to distinguish if there already was one of 
				whatever needs to come in.  It reads until it finds something
				not part of a REAL number and sends that whole token as error
				or not.
			****************************************************************/
			switch (character) {
				case '0' ... '9' : {
						atoken.characters[atoken.value] = character;
						atoken.value++;
					break;
				}
				case 'e' : {
					int i;
					for (i = 0;i < atoken.value;i++) {
						if (atoken.characters[i] == 'e' || atoken.characters[i] == 'E') {
							i = -1;
							break;
						}
					}
					if (atoken.characters[atoken.value - 1] == '.') {
						i = -1;
					}
					if (i == -1) {
						atoken.ID = -1;
					}
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				}
				case 'E' : {
					int i;
					for (i = 0;i < atoken.value;i++) {
						if (atoken.characters[i] == 'e' || atoken.characters[i] == 'E') {
							i = -1;
							break;
						}
					}
					if (atoken.characters[atoken.value - 1] == '.') {
						i = -1;
					}
					if (i == -1) {
						atoken.ID = -1;
					}
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				}
				case '.' : {
					int i;
					for (i = 0;i < atoken.value;i++) {
						if (atoken.characters[i] == '.') {
							i = -1;
							break;
						}
					}
					if (i == -1) {
						atoken.ID = -1;
					}
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				}
				case '+' : {
					if (atoken.characters[atoken.value - 1] == 'e' || atoken.characters[atoken.value - 1] == 'E') {
						
					} else {
						atoken.ID = -1;
					}
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				}
				case '-' : {
					if (atoken.characters[atoken.value - 1] == 'e' || atoken.characters[atoken.value - 1] == 'E') {
						
					} else {
						atoken.ID = -1;
					}
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				}
				default : {
					if (atoken.characters[atoken.value - 1] == 'e' || atoken.characters[atoken.value - 1] == 'E' ) {
						atoken.ID = -1;
					}
					ungetc(character,fp);
					CHARCOUNTER--;
					atoken.characters[atoken.value] = '\0';
					cc = 3;
					return &atoken;
				}
			}
			continue;
		} else if (cc == 6) {
			/****************************************************************
				Identifiers were found here. Most straightforward state.  
				It looks for anything not alphanumeric or _ and cuts it off 
				when it finds otherwise.
			****************************************************************/
			switch (character) {
				case 'a' ... 'z' : {
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				} 
				case 'A' ... 'Z' : {
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				}
				case '_' : {
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				}
				case '0' ... '9' : {
					atoken.characters[atoken.value] = character;
					atoken.value++;
					break;
				}
				default : {
					ungetc(character,fp);
					CHARCOUNTER--;
					cc = 3;					
					atoken.characters[atoken.value] = '\0';
					atoken.value = atoi(atoken.characters);
					return &atoken;
				}
			}
			continue;
		}
	}
	return &atoken;
}


/****************************************************************
This is what I use to clear the token so that I can reuse it
****************************************************************/

void token_clear(Token *atoken) {
	atoken->value = 0;
	atoken->ID = 0;
	atoken->line = 0;
	atoken->characternum = 0;
	memset(atoken->characters, 0, strlen(atoken->characters));
}


/****************************************************************
This prints the token and based on if it has an error, prints an
error message
****************************************************************/
void token_print(Token* atoken) {
	if (atoken->ID == 24) {
	if (atoken->ID < 0) {
			printf("Invalid Token:\n");
			printf("Token:  %s\tID:  %d\nLine:  %d\tCharacter:   "
			"%d\tValue:  %d\n\n",atoken->characters,atoken->ID,
			atoken->line,(atoken->characternum - strlen(atoken->characters)),atoken->value);
			token_clear(atoken);
		} else {
			printf("Token:  %s\tID:  %d\nLine:  %d\tCharacter:   "
			"%d\tValue:  %d\n\n",atoken->characters,atoken->ID,
			atoken->line,(atoken->characternum - strlen(atoken->characters)),atoken->value);
			token_clear(atoken);
		}	
	} else {
		if (atoken->ID < 0) {
			printf("Invalid Token:\n");
			printf("Token:  %s\tID:  %d\nLine:  %d\tCharacter:   %d\n\n",atoken->characters,atoken->ID,
			atoken->line,(atoken->characternum - strlen(atoken->characters)));
			token_clear(atoken);
		} else {
			printf("Token:  %s\tID:  %d\r\nLine:  %d\tCharacter:   %d\n\n",atoken->characters,atoken->ID,
			atoken->line,(atoken->characternum - strlen(atoken->characters)));
			token_clear(atoken);
		}
	}
}

int ifend() {
	if (feof(fp)) {
		return 1;
	}
	return 0;
}
