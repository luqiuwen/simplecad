#################################################

# Makefile : Simple CAD
# Author   : Lu Qiuwen, 2013200725
# Course   : COMP533

#################################################

INC=
LIB=-lm -lopencv_core -lopencv_highgui -lpthread

CC=clang
CFLAG =-g -static

PRG=cad
OBJ=arc.o circle.o command.o eclipse.o line.o point.o window.o calc.o cmdcontrol.o drawarea.o graph.o main.o rect.o window_img.o


$(PRG):$(OBJ)
	$(CC) $(INC) $(LIB) $(OBJ) -o $@

.SUFFIXES:.c .o .cpp
.cpp.o:
	$(CC) $(CFLAG) $(INC) -c $*.cpp -o $*.o
.c.o:
	$(CC) $(CFLAG) $(INC) -c $*.c -o $*.o

.PRONY:clean
clean:
	@echo "Remove linked files"
	rm -f $(OBJ)

# End of Makefile
###################################################
