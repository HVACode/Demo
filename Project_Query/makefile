CC=g++
OBJS1=main.o Query.o TextResult.o
OBJS2=showFile.o
CFLAGS+=-c -W -g
all:query showFile


query:$(OBJS1)
	$(CC) $^ -o $@ 
main.o:main.cpp
	$(CC) $^ $(CFLAGS) -o $@
Query.o:Query.cpp
	$(CC) $^ $(CFLAGS) -o $@
TextResult.o:TextResult.cpp
	$(CC) $^ $(CFLAGS) -o $@

showFile:$(OBJS2)
	$(CC) $^ -o $@ 
$(OBJS2):showFile.cpp
	$(CC) $^ $(CFLAGS) -o $@

.PHONY:clean
clean:
	rm -f *.o
	rm -f showFile
	rm -f query