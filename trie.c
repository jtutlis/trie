/*
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
#include <strings.h>
#include <assert.h>
#include "defs.h"

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

TrieNode *makeTrieNode(){
    return (TrieNode *) calloc(1, sizeof(TrieNode));
} 

void add(TrieNode *root, char *string){
    if (string != NULL || (*string) != '\0'){
        TrieNode *node = root;
        while((*string) != '\0' && node != NULL){
            printf("%c\n", (*string));
            if (node->alphabet[CHAR_TO_INDEX(*string)] == NULL){
                node->alphabet[CHAR_TO_INDEX(*string)] = makeTrieNode();
                printf("added layer for char: %c\n", (*string));
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


    printf("Size of tree: %d\n", sizeOfTree(tree));

    return 0;
}

