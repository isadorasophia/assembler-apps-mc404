#************************************************#
#          MC404, Assignment 1 Makefile          #
#           written by Isadora Sophia            #
#              September 23, 2015                #
#                                                #
#   An assembler that translates IAS language    #
#            into machine languague              #
#************************************************#

# list of object files that must be linked together to 
# build the ias-as executable
OBJ_FILES=main.o error.o data_access.o data_structure.o linkedlist.o

# flags used when compilling
FLAGS=-c -g -std=c99 -D_POSIX_C_SOURCE=200809L

# default rule
all: ias-as

# -- Rules for making each one of the object  -- #
# -- files! 								  -- #

# -- main.o as main program file
main.o: main.c error.h data_access.h data_structure.h linkedlist.h
	gcc $(FLAGS) main.c -o main.o

# -- error.o to handle errors
error.o: error.c error.h
	gcc $(FLAGS) error.c -o error.o

# -- data_access.o to handle data access in files
data_access.o: data_access.c data_access.h data_structure.h error.h
	gcc $(FLAGS) data_access.c -o data_access.o

# -- data_structure.o to handle basic data structure of the program
data_structure.o: data_structure.c data_structure.h linkedlist.h
	gcc $(FLAGS) data_structure.c -o data_structure.o

# -- linkedlist.o implements linked list
linkedlist.o: linkedlist.c linkedlist.h data_structure.h
	gcc $(FLAGS) linkedlist.c -o linkedlist.o

# -- rule to make the executable ias-as
ias-as: $(OBJ_FILES)
	gcc -g $(OBJ_FILES) -o ias-as -std=c99

# -- clean rule. it removes the ias-as file   -- #
# -- and any .o file on the directory 		  -- #
clean:
	rm -f ias-as *.o