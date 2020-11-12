all: keywords.txt

keywords.txt: $(wildcard src/*.cpp) $(wildcard src/*.h)
	arduino-keywords --output .. src
