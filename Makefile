build:
	gcc -o main main.c -lpthread
run:
	./main 6000
clean:
	rm -rf main
