CXX=g++
CXXFLAGS=-g
EXEC=quadris
OBJECTS = main.o interpreter.o board.o block.o cell.o trie.o window.o

${EXEC} : ${OBJECTS}
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC} -lX11

clean:
	rm *.o
	rm quadris
