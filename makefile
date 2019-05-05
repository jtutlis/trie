all: trie

trie: trie.c defs.h
	gcc trie.c -o trie -Wall -g

clean:
	rm -f trie *.o
