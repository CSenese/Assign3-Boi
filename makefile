#
# PROGRAM:    assign1
# PROGRAMMER: Chris Senese
# LOGON ID:   z1773470
# DATE DUE:   3/21/2019
#/

# Compiler variables

# Rule to link object code files to create executable file
assign3: assign3.o Tree.o Stack.o Get_Token.o Symbol.o
	g++ $(CCFLAGS) -o assign3 assign3.o Tree.o Stack.o Get_Token.o Symbol.o
   
# Rules to compile source code files to object code
assign3.o: assign3.c Tree.h
	g++ $(CCFLAGS) -c assign3.c
	
Get_Token.o: Get_Token.c Tree.h
	g++ $(CCFLAGS) -c Get_Token.c

Tree.o: Tree.c Tree.h
	g++ $(CCFLAGS) -c Tree.c
	
Stack.o: Stack.c Tree.h
	g++ $(CCFLAGS) -c Stack.c
	
Symbol.o: Symbol.c Tree.h
	g++ $(CCFLAGS) -c Symbol.c
# Pseudo-target to remove object code and executable files
clean:
	-rm *.o assign3
