//
//  huffman.h
//  Huffman Coding Project
//
//  Created by pierre dewatre on 25/10/2020.
//  Copyright © 2020 Pierre Dewatre. All rights reserved.
//

#ifndef huffman_h
#define huffman_h

#include <stdio.h>

//an ElementOccuChar is data structure linked to an other ElementOccuChar in which we have 2 datas: a lettre (type char) and its number of occurences (type int).

typedef struct ElementOccuChar
{
    int nbChar;
    char cPresent;
    struct ElementOccuChar* next;
}ElementOccuChar;
typedef ElementOccuChar* ListOccuChar;

typedef struct Node
{
    ElementOccuChar* data;
    struct Node* left;
    struct Node* right;
}Node;
typedef Node* Tree;

typedef struct ElementNode
{
    Tree data;
    struct ElementNode* next;
}ElementNode;
typedef ElementNode* ListOfNode;

void txt_to_bin(FILE* input, FILE* output);
int* dec_to_bin(int n);

int number_of_char(FILE* input);

ElementOccuChar* create_ElementOccuChar(char c, int nb);
ListOccuChar create_list_char(FILE* input);

void swap(ListOccuChar a, ListOccuChar b);
void list_bubble_sort(ListOccuChar l);
int list_sum(ListOccuChar l);
Tree create_node(ElementOccuChar* val);

ListOfNode create_list_node(ListOccuChar l);
Tree min_heap(ListOfNode l);
Tree pop_list(ListOfNode* l);
void add_Node(ListOfNode* l, Tree t);

void huffman_dico_in_txt(Tree ht, FILE* huffmanDico);
char go_trough_ht_1(Tree* ht);
void go_trough_ht_2(Tree* ht, FILE* huffmanDico);

void rewrite_text(FILE* output, FILE* dictionnary, FILE* input);

void compress_text_file(FILE* input);

void decompress_text(FILE* compressed);
char find_value_in_dico(char* word, FILE* dico, int max);
char* str_cat(char* word1, char c);

#endif /* huffman_h */