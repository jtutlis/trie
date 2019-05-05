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

TrieNode *makeTrieNode(){
    return (TrieNode *) calloc(1, sizeof(TrieNode));
} 

PossibleWords *makePossibleWords(){
    return (PossibleWords *) calloc(1, sizeof(PossibleWords));
}

void addPossibleWord(PossibleWords *head, char *word){
    PossibleWords *newNode = makePossibleWords();
    newNode->word = word;
    printf("adding word: ");
    puts(word); 

    while(head->next != NULL){
        head = head->next;
    }
    head->next = newNode;
}

void printPossibleWords(PossibleWords *head, char *currPath){
    printf("Possible words for '%s':\n", currPath);
    while(head){
        if (head->word != NULL){
            puts(head->word);
        }
        head = head->next;
    }
}

void add(TrieNode *root, char *string){
    if (string != NULL || (*string) != '\0'){
        TrieNode *node = root;
        while((*string) != '\0' && node != NULL){
            // printf("%c\n", (*string));
            if (node->alphabet[CHAR_TO_INDEX(*string)] == NULL){
                node->alphabet[CHAR_TO_INDEX(*string)] = makeTrieNode();
                //printf("added layer for char: %c\n", (*string));
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
    TrieNode *prev = NULL;

    // naviagte to currnent location
    while(curr){
        prev = curr;
        curr = curr->alphabet[CHAR_TO_INDEX(*string)];
        string++;
    }

    PossibleWords *possibleWords = makePossibleWords();
    searchTreeForPossibleWords(prev, possibleWords, tempString);
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
                    printf("New string: %s\n", newPath);
                }
                printf("Checking string: %s\n", newPath);

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

int main(){
    char *string = {"test\0"};
    TrieNode *tree = makeTrieNode();
    add(tree, string);
    add(tree, "help");
    add(tree, "helpme");
    add(tree, "helpyou");

    assert(search(tree, "help"));
    assert(search(tree, "helpme"));
    assert(search(tree, "helpyou"));

    //  printf("Size of tree: %d\n", sizeOfTree(tree));
    printPossibleWords(findPossibleWords(tree, "h"), "h");



    return 0;
}

