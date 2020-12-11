FLAGS=-O3

engine_objects=chess.o game_state.o move.o move_generator.o

chess: $(engine_objects)
	g++ $(FLAGS) $(engine_objects) -o chess

chess.o: chess.cpp
	g++ -c $(FLAGS) chess.cpp

game_state.o: game_state.cpp
	g++ -c $(FLAGS) game_state.cpp

move.o: move.cpp
	g++ -c $(FLAGS) move.cpp

move_generator.o: move_generator.cpp
	g++ -c $(FLAGS) move_generator.cpp

.PHONY: clean
clean:
	rm -v *.o
