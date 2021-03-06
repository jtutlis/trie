/*
 *
 * =====================================================================================
 *
 *       Filename:  trie.c
 *
 *    Description:  Trie data structure
 *
 *        Version:  1.0
 *        Created:  05/04/2019 06:17:31 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jacob Tutlis 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "defs.h"

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')
#define INDEX_TO_CHAR(i) (char)'a' + i
#define VALID_CHAR(c) (c >= (int) 'a' && c <= (int) 'z')

TrieNode *makeTrieNode(){
    //return (TrieNode *) calloc(1, sizeof(TrieNode));
    TrieNode *node = (TrieNode *) malloc(sizeof(TrieNode));
    memset(node, 0, sizeof(TrieNode));
    return node;
}

PossibleWords *makePossibleWords(){
    //return (PossibleWords *) calloc(1, sizeof(PossibleWords));
    PossibleWords *possibleWords = (PossibleWords *) malloc(sizeof(PossibleWords));
    memset(possibleWords, 0, sizeof(PossibleWords));
    return possibleWords; 
}

void addPossibleWord(PossibleWords *head, char *word){
    size_t len = strlen(word);
    if (len){
        PossibleWords *newNode = makePossibleWords();
        char *newWord = (char *) malloc(len+1);
        strncpy(newWord, word, len);
        newWord[len] = '\0';
        newNode->word = newWord;

        while(head->next != NULL){
            head = head->next;
        }
        head->next = newNode;
    }
}

void freePossibleWords(PossibleWords *head){
    if (head){
        free(head->word);
        freePossibleWords(head->next);
    }
    free(head);
}

void freeTrieTree(TrieNode *root){
    if(root){
        for (int i = 0; i < ALPHABET_SIZE; i++){
            if (root->alphabet[i]){
                freeTrieTree(root->alphabet[i]);
            }
        }
    }

    free(root);
}

void printPossibleWords(PossibleWords *head, char *currPath){
    printf("Possible words for '%s':\n", currPath);
    while(head){
        if (head->word != NULL){
            printf("%s\n", head->word);
        }
        head = head->next;
    }
}

void add(TrieNode *root, char *string){
    //printf("adding word: %s\n", string);
    if (string != NULL && (*string) != '\0' && strlen(string) > 0){
        TrieNode *node = root;
        while((*string) != '\0' && node != NULL){
            // printf("%c\n", (*string));
            if (node->alphabet[CHAR_TO_INDEX(*string)] == NULL){
                node->alphabet[CHAR_TO_INDEX(*string)] = makeTrieNode();
                // printf("added layer for char: %c\n", (*string));
            }
            node = node->alphabet[CHAR_TO_INDEX(*string)];
            string++;
        } 
        if (node != NULL){
            node->isLeaf = 1;
        }
    }
}

// returns 1 if in tree, 0 otherwise
int search(TrieNode *root, char *string){
    TrieNode *node = root;
    while(node){
        if ((*string) != '\0'){
            node = node->alphabet[CHAR_TO_INDEX(*string)];
        } else {
            if (node->isLeaf){
                return 1;
            } else {
                return 0;
            }
        }
        string++;
    }
    return 0;
}

// calculates the number of words in tree
int sizeOfTree(TrieNode *root){
    int sum = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++){
        if (root->alphabet[i]){
            sum += sizeOfTree(root->alphabet[i]);
        }
    }

    if (root->isLeaf){
        sum++;
    }

    return sum;
}

// returns a linked list of possible words the given string can form
PossibleWords *findPossibleWords(TrieNode *root, char *string){
    if (root == NULL || string == NULL){
        return NULL;
    }
    char *tempString = string;
    TrieNode *curr = root;

    // naviagte to currnent location
    while(curr && VALID_CHAR(*string)){
        curr = curr->alphabet[CHAR_TO_INDEX(*string)];
        string++;
    }

    PossibleWords *possibleWords = makePossibleWords();
    searchTreeForPossibleWords(curr, possibleWords, tempString);
    return possibleWords;
}
// helper function
void searchTreeForPossibleWords(TrieNode *root, PossibleWords *head, char *currPath){
    if (root != NULL){
        for (int i = 0; i < ALPHABET_SIZE; i++){
            if (root->alphabet[i]){
                char newChar = INDEX_TO_CHAR(i);
                char *newPath = generateString(currPath, newChar);
                if (root->alphabet[i]->isLeaf){
                    addPossibleWord(head, newPath);
                    //printf("New string: %s\n", newPath);
                }
                //printf("Checking string: %s\n", newPath);

                searchTreeForPossibleWords(root->alphabet[i], head, newPath);  
                free(newPath); 
            }
        }
    }
}

char *generateString(char *currPath, char newChar){


    size_t len = strlen(currPath);
    char *newString = (char *) malloc(len + 2); // 1 for new char + \0
    strncpy(newString, currPath, len);
    newString[len] = newChar;
    newString[len+1] = '\0';

    return newString;
}

void readFile(FILE *input, TrieNode *root){
    int wordCount = 0;
    char buffer[1024];
    int c;
    int count = 0;
    while((c = fgetc(input)) != EOF){
        char l = (char) c;
        if (l != '\n' && l != '\0'){
            if (c >= (int) 'a' && c <= (int) 'z'){
                buffer[count] = l;
                count++;
            }
        } else {
            wordCount++;
            buffer[count] = '\0';
            if (count > 0){
                add(root, buffer);
            }
            count = 0;
            memset(buffer, 0, 1024);
        }
    }
}

int howManyPossibleWords(PossibleWords *possibleWords){
    int count = -1;
    while(possibleWords){
        possibleWords = possibleWords->next;
        count++;
    }

    return count;
}

int main(int argc, char **argv){

    if (argc != 3){
        printf("Invalid arguemets: ./trie input_file search_string\n");
        return 1;
    }
    char * string = argv[2];    
    FILE *input = fopen(argv[1], "r");
    
    TrieNode *tree = makeTrieNode();
    readFile(input, tree);

    //printf("Size of tree: %d\n", sizeOfTree(tree));
    PossibleWords *possibleWords = findPossibleWords(tree, string);
    printf("Number of possible words: %d\n", howManyPossibleWords(possibleWords));
    printPossibleWords(possibleWords, string);

    freePossibleWords(possibleWords);
    freeTrieTree(tree);
    fclose(input);
    return 0;
}

