pre-commit: keywords.txt codestyle

keywords.txt: src/EE895.h
	arduino-keywords -f $<

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

.git/hooks/pre-commit: .git_hooks/pre-commit.sh
	cp $< $@
