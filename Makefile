pre-commit: keywords.txt

keywords.txt: $(wildcard src/*.cpp) $(wildcard src/*.h)
	arduino-keywords --output .. src

.git/hooks/pre-commit:
	echo "#!/bin/sh" > $@
	echo "make pre-commit" >> $@
	chmod +x $@
