CPPFLAGS = -std=c++11 -Werror=vla

all: pa4.exe

pa4.exe: doubleHashingHashTable.o linearProbingHashTable.o openAddressingHashTable.o quadraticProbingHashTable.o shop.o main.o
	g++ -o $@ $(CPPFLAGS) doubleHashingHashTable.o linearProbingHashTable.o openAddressingHashTable.o quadraticProbingHashTable.o shop.o main.o

%.o: %.cpp
	g++ $(CPPFLAGS) -c $< -o $@ 

clean:
	del *.o *.exe