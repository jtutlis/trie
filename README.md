# trie
Generates a trie of all words entered from a file and can search trie for all possible words that can be generated from a given string. 

Compile the project using make
>make

Run
>./trie input_file search_string

The input file must contain all lowercase letters and the words must be seperated by new line characters.

Example output:

./trie words_alpha.txt programa
Number of possible words: 2
Possible words for 'programa':
programable
programatic

