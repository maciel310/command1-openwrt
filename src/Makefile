INCLUDE = -Iusr/local/include
LDFLAGS = -Lusr/local/lib
LDLIBS = -lcurl -lm

command1: command1.o cJSON.o
	$(CC) command1.o cJSON.o -o command1 $(INCLUDE) $(LDFLAGS) $(LDLIBS) 
command1.o: command1.c
	$(CC) $(CFLAGS) -c command1.c
cJSON.o: cjson/cJSON.c
	$(CC) -c cjson/cJSON.c

clean:
	rm *.o command1
