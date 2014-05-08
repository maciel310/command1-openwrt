INCLUDE = -Iusr/local/include
LDFLAGS = -Lusr/local/lib
LDLIBS = -lcurl -lm

command1: command1.o cJSON.o firebase.o serial.o
	$(CC) command1.o cJSON.o firebase.o serial.o -o command1 $(INCLUDE) $(LDFLAGS) $(LDLIBS) 
%.o: %.c
	$(CC) $(CFLAGS) -c $<
cJSON.o: cjson/cJSON.c
	$(CC) -c cjson/cJSON.c

clean:
	rm *.o command1
