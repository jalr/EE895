all: keywords.txt

keywords.txt: src/EE895.h
	arduino-keywords -f $<
