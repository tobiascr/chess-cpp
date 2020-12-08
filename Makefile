FLAGS=-O3

engine_objects=chess.o game_state.o

chess: $(engine_objects)
	g++ $(FLAGS) $(engine_objects) -o chess

chess.o: chess.cpp
	g++ -c $(FLAGS) chess.cpp

game_state.o: game_state.cpp
	g++ -c $(FLAGS) game_state.cpp

.PHONY: clean
clean:
	rm -v *.o
