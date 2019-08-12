files=$(shell ls *.cpp)
names=$(files:.cpp=.name)


all: $(names)

%.name: %.cpp
	g++ -std=c++17 -Wall -Wextra -pg -o $* $^