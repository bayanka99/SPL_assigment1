
all: readStream
# clean compile link

#link: bin/main.o
#	@echo 'Linking'
#	g++ -o bin/main bin/main.o

#compile: src/main.cpp
#	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp


readStream: readStream.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/readStream.o readStream.cpp
	g++ -o bin/readStream bin/readStream.o



clean:
	@echo 'Cleaning'
	rm -f bin/*