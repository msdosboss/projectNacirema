default: client.o networking.o
	gcc client.o networking.o -o client
	
networking.o: networking.c
	gcc -c networking.c -o networking.o

client.o: client.c
	gcc -c client.c -o client.o

clean:
	rm client