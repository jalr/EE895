pre-commit: keywords.txt

keywords.txt: src/EE895.h
	arduino-keywords -f $<

.git/hooks/pre-commit: .git_hooks/pre-commit.sh
	cp $< $@
