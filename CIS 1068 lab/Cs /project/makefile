objects = main.o fifoqueue.o priorityqueue.o otherFunction.o
CFLAGS=-std=c99


lltest : $(objects)
	cc -o lltest $(objects)


main.o : fifoqueue.h priorityqueue.h otherFunction.h

.PHONY : clean
clean :
	rm $(objects)
