FLAGS=-O3

engine_objects=chess.o

chess: $(engine_objects)
	g++ $(FLAGS) $(engine_objects) -o chess

chess.o: chess.cpp
	g++ -c $(FLAGS) chess.cpp

.PHONY: clean
clean:
	rm -v *.o
