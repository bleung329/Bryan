all:
	gcc -o control control.c
	gcc -o client client.c

clean:
	rm client
	rm control
	rm story.txt
