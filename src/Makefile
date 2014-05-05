INCLUDE = -Iusr/local/include
LDFLAGS = -Lusr/local/lib
LDLIBS = -lcurl -lm

command1: command1.o
	$(CC) command1.o cjson/cJSON.o -o command1 $(INCLUDE) $(LDFLAGS) $(LDLIBS) 
command1.o: command1.c
	$(CC) $(CFLAGS) -c command1.c

clean:
	rm *.o command1
