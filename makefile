CFLAGS:=-c -Wall -Weffc++ -g -std=c++11  -Iinclude -pthread
LDFLAGS:=-lboost_system -pthread


all: StompBookClubClient
	g++ -o bin/StompBookClubClient bin/connectionHandler.o bin/Client.o bin/Book.o bin/User.o $(LDFLAGS)

StompBookClubClient: bin/connectionHandler.o bin/Client.o bin/Book.o bin/User.o

bin/connectionHandler.o: src/connectionHandler.cpp
	g++ $(CFLAGS) -o bin/connectionHandler.o src/connectionHandler.cpp

bin/Client.o: src/Client.cpp
	g++ $(CFLAGS) -o bin/Client.o src/Client.cpp

bin/Book.o: src/Book.cpp
	g++ $(CFLAGS) -o bin/Book.o src/Book.cpp

bin/User.o: src/User.cpp
	g++ $(CFLAGS) -o bin/User.o src/User.cpp

.PHONY: clean
clean:
	rm -f bin/*
