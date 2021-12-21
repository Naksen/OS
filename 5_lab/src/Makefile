.PHONY: all clean

all:
	gcc -fPIC -c lib1.c -o d1.o 
	gcc -fPIC -c lib2.c -o d2.o 
	gcc -shared -o libd1.so d1.o -lm
	gcc -shared -o libd2.so d2.o -lm
	gcc prog1.c -L. -ld1 -o main_1 -Wl,-rpath -Wl,. -lm 
	gcc prog2.c -L. -ldl -o main_2 -Wl,-rpath -Wl,. -lm 

clean:
	rm d1.o d2.o libd1.so libd2.so main_1 main_2