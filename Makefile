pre-commit: keywords.txt codestyle

keywords.txt: $(wildcard src/*.cpp) $(wildcard src/*.h)
	arduino-keywords --output .. src

.PHONY: codestyle
codestyle: $(wildcard src/*.cpp) $(wildcard src/*.h) $(wildcard examples/*/*.ino)
	find \
		src \
		examples \
		-type f \
		\( \
			-name '*.ino' \
			-o -name '*.cpp' \
			-o -name '*.h' \
		\) \
	-print0 \
	| xargs -0 -r astyle --options=codestyle.conf

.git/hooks/pre-commit:
	echo "#!/bin/sh" > $@
	echo "make pre-commit" >> $@
	chmod +x $@
