/*
 * =====================================================================================
 *
 *
 *       Filename:  defs.h
 *
 *    Description:  defs 
 *
 *        Version:  1.0
 *        Created:  05/04/2019 06:18:24 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jacob Tutlis 
 *   Organization:  
 *
 * =====================================================================================
 */

#pragma once

#define ALPHABET_SIZE 26


typedef struct TrieNode{
    struct TrieNode *alphabet[ALPHABET_SIZE];
    int isLeaf;
} TrieNode;

typedef struct PossibleWords {
    struct PossibleWords *next;
    char *word;
} PossibleWords;

PossibleWords *findPossibleWords(TrieNode *root, char *string);
void searchTreeForPossibleWords(TrieNode *root, PossibleWords *head, char *currPath);
char *generateString(char *currPath, char newChar);
