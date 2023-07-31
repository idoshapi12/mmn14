assembler: common.o preAsembler.o firstPass.o secondPass.o my_assembler.o label.o instructions.o directive.o
	gcc -g -Wall -pedantic -ansi common.o preAsembler.o firstPass.o secondPass.o my_assembler.o label.o instructions.o directive.o -o assembler

common.o : common.c common.h
	gcc -g -c -Wall -pedantic -ansi common.c -o common.o
directive.o : directive.c directive.h
	gcc -g -c -Wall -pedantic -ansi directive.c -o directive.o
instructions.o : instructions.c instructions.h label.h
	gcc -g -c -Wall -pedantic -ansi instructions.c -o instructions.o
label.o : label.c label.h instructions.h
	gcc -g -c -Wall -pedantic -ansi label.c -o label.o
preAsembler.o : preAsembler.c preAsembler.h 
	gcc -g -c -Wall -pedantic -ansi preAsembler.c -o preAsembler.o
firstPass.o : firstPass.c firstPass.h directive.h instructions.h label.h
	gcc -g -c -Wall -pedantic -ansi firstPass.c -o firstPass.o
secondPass.o : secondPass.c secondPass.h directive.h instructions.h label.h
	gcc -g -c -Wall -pedantic -ansi secondPass.c -o secondPass.o
assembler.o : my_assembler.c preAsembler.h firstPass.h secondPass.h instructions.h label.h
	gcc -g -c -Wall -pedantic -ansi my_assembler.c -o assembler.o





