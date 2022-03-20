all : main

main : main.o
	g++ -o $@ $<

%.o : %.cpp
	g++ -c $<

clean :
	rm *.o *.jpg *.txt main