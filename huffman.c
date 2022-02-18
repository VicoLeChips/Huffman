//
//  huffman.c
//  Huffman Coding Project
//
//  Created by pierre dewatre on 25/10/2020.
//  Copyright © 2020 Pierre Dewatre. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "huffman.h"




// A) ​⭐⭐​: Write a function that reads text from one file,
// and translates it to its 0 and 1 equivalent in another file.

// This fonction read the input file and put the binary ASCII code in the output file
void txt_to_bin(FILE* input, FILE* output)
{
    // We try to open the files
    if (input == NULL || output == NULL)
    {
        printf("Error! Could not open file\n");
        exit(-1);
    }
    char c;
    // We create an array to receive the ASCII char code on 8 int
    int* decimal;
    // loop to go through the file and translate all the ASCII code to bin
    while((c=fgetc(input)) != EOF)
    {
        decimal = dec_to_bin((int)c);
        for (int i = 0; i < 8; i++)
            fprintf(output, "%d", decimal[i]);
    }
}

// This function translate the ASCII code to binaray number under the 
int* dec_to_bin(int n)
{
    // here, we allocate the array because we want to return it
    int* binaryNum = (int*)malloc(8*sizeof(int));
    for (int i = 7; i >= 0; i--)
    {
        if (n >= pow(2, i))
        {
            n -= pow(2, i);
            binaryNum[7-i] = 1;
        }
        else
            binaryNum[7-i] = 0;
    }
    return binaryNum;
}


// B) ​⭐​: Write a function that displays the number of characters in a text file.

int number_of_char(FILE* input)
{
    int i = 0;
    fseek(input, 0, SEEK_SET);
    while(fgetc(input) != EOF)
        i++;
    return i;
}


// C) ​⭐⭐​: Write a function that returns a list containing each character present in the text, as well as the number of occurrences of that character.

//this function generates an element of type ElementOccuChar and initialize it. 
ElementOccuChar* create_ElementOccuChar(char c, int nb)
{  
    ElementOccuChar* newChar = (ElementOccuChar*)malloc(sizeof(ElementOccuChar));
    newChar->cPresent = c;
    newChar->nbChar = nb;
    newChar->next = NULL;
    return newChar;
}

// this function creates a list containing all the characters present in the text and their number of occurences. 
ListOccuChar create_list_char(FILE* input)     
{
    //first we initialize a the list of the occurence of all charactere and we create a runner. 
    char c;
    fseek(input, 0, SEEK_SET);
    ElementOccuChar* tmp = create_ElementOccuChar(fgetc(input), 1);
    ListOccuChar ListOccuChar = tmp;
    //here we go through the file and at each letter we check if it is already stored in our list, if it is we add 1 to the number of occurences, else we create a new element in the list
    while((c = fgetc(input)) != EOF)
    {
        while(tmp->cPresent != c && tmp->next != NULL)
            tmp = tmp->next;
        if(tmp->next == NULL && tmp->cPresent != c)
        {
            tmp->next = create_ElementOccuChar(c, 1);
        }
        else
            tmp->nbChar += 1;
        tmp = ListOccuChar;
    }
    return ListOccuChar;
}



//D) ⭐⭐⭐: Write a function that returns a Huffman tree, from a list of occurrences.

// This function is used to swap the datas of two different ElementOccuChars structues of a list.
// Parameters are the two ElementOccuChars.
void swap(ListOccuChar a, ListOccuChar b) 
{
    // We save the datas of the first ElementOccuChar in temporaries variables.
    int tmpInt = a->nbChar;
    char tmpChar = a->cPresent;
    // We replace the datas of the first ElementOccuChar by the datas of the second one.
    a->nbChar = b->nbChar;
    a->cPresent = b->cPresent;
    // We replace the datas of the second ElementOccuChar by the old datas of the first one (temporary saved).
    b->nbChar = tmpInt;
    b->cPresent = tmpChar;
}

// This function is used to sort the ElementOccuChar of a list using the bubble sorting and it can be optimized because its time complexity is only Θ(n²).
// Parameter: the list of ElementOccuChar.
void list_bubble_sort(ListOccuChar l)
// Sorts iteratively (if the list exists) in a descending order all ElementOccuChars of the list depending on their nbChar parameters (which is the number of occurences of the lettre of the ElementOccuChar).
{
    if(l != NULL) {
        // We use two temporary variables to travel into the list.
        ListOccuChar runner = l;
        ListOccuChar runner2 = runner;
        // runner checks for each element if it is at the good place relatively to others.
        while (runner->next != NULL) {
            // runner2 travels the whole list and exchanges its place with the next ElementOccuChar of the list when its number of occurrences parameter is smaller than the next one.
            while (runner2->next != NULL) {
                if (runner2->nbChar < runner2->next->nbChar) {
                    swap(runner2, runner2->next);
                }
                runner2 = runner2->next;
            }
        // Do it for all ElementOccuChars of the list.
        runner2 = l;
        runner = runner->next;
        }
    }
}

// This function is used to know the numbr of characters in the text (the sum of numbers of occurrences of all letters).
// The parameter is the list of all ElementOccuChars.
int list_sum(ListOccuChar l)
// Adds recursively to the total sum numbers of occurences of all lettres.
{
    if (l == NULL) 
        return 0; 
    return (list_sum(l->next) + l->nbChar);
}

// This function is used to create a node with a given data of type ElementOccuChar (to convert SLL into Tree).
Tree create_node(ElementOccuChar* val)
// Allocates a place in the memory for the node, gives its value, links it to nothing and returns it.
{
    Tree node = (Tree)malloc(sizeof(Node));
    node->data = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Tree ht_from_list_of_occ(ListOccuChar l)
{
    return min_heap(create_list_node(l));
}

// This function will recursively create a list containing nodes of ListOccuChar.
ListOfNode create_list_node(ListOccuChar l)
{
    // Stopping condition
    if (l == NULL)
    {
        return NULL;
    }
    else
    {
        // We'll create a new element, and we'll call the function again for the -> next of this element.
        ListOfNode newElement = (ListOfNode)malloc(sizeof(ElementNode));
        newElement->data = create_node(l);
        newElement->data->data = l;
        if (l->next != NULL)
        {
            newElement->next = create_list_node(l->next);
        }
        else
        {
            newElement->next = NULL;
        }
        return newElement;
    }
}

// This is the function that will truly create the Huffman Tree.
Tree min_heap(ListOfNode l)
{
    // Stopping condition: If the list has only 1 element left, then this element contains the root of the huffman tree: we
    if (l == NULL || l->next == NULL)
    {
        return l->data;
    }
    else
    {
        Tree newNode = create_node(create_ElementOccuChar('~', 0));
        // We pop 2 elements from the list and add them as children of the new node.
        newNode->right = pop_list(&l);
        newNode->left = pop_list(&l);
        // The new node has the value of the sum of its children.
        newNode->data->nbChar = newNode->right->data->nbChar + newNode->left->data->nbChar;
        // We add the new node back in the list
        add_Node(&l, newNode);
        Tree Tmp = min_heap(l);
        return Tmp;
    }
}

// This function will remove the last element of the list, and returns the node that it contains.
Tree pop_list(ListOfNode* l)
{
    Tree Tmp2;
    if ((*l)->next == NULL)
    {
        Tmp2 = (*l)->data;
        free((*l)->next);
        *l = NULL;
        return Tmp2;
    }
    else
    {
        ListOfNode Tmp = *l;
        while (Tmp->next->next != NULL)
            Tmp = Tmp->next;
        Tmp2 = Tmp->next->data;
        free(Tmp->next);
        Tmp->next = NULL;
        return Tmp2;
    }
}

// This function adds the tree given in parameter into the list.
void add_Node(ListOfNode* l, Tree t)
{
    if (*l == NULL)
    {
        *l = (ListOfNode)malloc(sizeof(ElementNode));
        (*l)->data = t;
        (*l)->next = NULL;
    }
    else
    {
        ListOfNode Tmp = *l, Tmp2;
        if (t->data->nbChar >= Tmp->data->data->nbChar)
        {
            *l = (ListOfNode)malloc(sizeof(ElementNode));
            (*l)->next = Tmp;
            (*l)->data = t;
        }
        else
        {
            // We run trough the list as long as our value is smaller than the one of the current element, or as long as we didn't reach the end.
            while(Tmp->next != NULL && Tmp->data->data->nbChar > t->data->nbChar)
            {
                Tmp2 = Tmp;
                Tmp = Tmp->next;
            }
            // Then, we create a new element, with the tree as the data
            Tmp2->next = (ListOfNode)malloc(sizeof(ElementNode));
            Tmp2->next->next = Tmp;
            Tmp2->next->data = t;
        }
    }
}

//E) ⭐⭐: Write a function that stores the dictionary from the Huffman tree in a txt file.

void huffman_dico_in_txt(Tree ht, FILE* huffmanDico)
{
    if (ht != NULL)
    {
        // We get the character by going trough the ht. If it was a '~' (internal node), it will be deleted and we won't go trough the list again.
        char c = go_trough_ht_1(&ht);
        if (c != '~')
        {
            // If it was not a "~", we'll go trough the list again.
            fputc(c, huffmanDico);
            go_trough_ht_2(&ht, huffmanDico);
            fputc('\n', huffmanDico);
        }
        huffman_dico_in_txt(ht, huffmanDico);
    }
}

// This function simply go through the tree a first time, and returns the character in the end. In case of an internal node, it frees it
char go_trough_ht_1(Tree* ht)
{
    Tree t = *ht;
    if (t == NULL)
        return '\0';
    else if (t->left != NULL)
        return go_trough_ht_1(&(t->left));
    else if (t->right != NULL)
        return go_trough_ht_1(&(t->right));
    else
    {
        char c = t->data->cPresent;
        if (c == '~')
        {
            free(t->left);
            free(t->right);
            *ht = NULL;
        }
        return c;
    }
}

// This function goes through the tree a second time and writes its path on the dico, with 0's and 1's. Then, it it deletes the leaf of the character.
void go_trough_ht_2(Tree* ht, FILE* huffmanDico)
{
    Tree t = *ht;
    if (t != NULL)
    {
        if (t->left != NULL)
        {
            fputc('0', huffmanDico);
            go_trough_ht_2(&(t->left), huffmanDico);
        }
        else if (t->right != NULL)
        {
            fputc('1', huffmanDico);
            go_trough_ht_2(&(t->right), huffmanDico);
        }
        else
        {
            free(t->left);
            free(t->right);
            *ht = NULL;
        }
    }
}
 
// F) ​⭐⭐​: Write a function that translates a text into a binary sequencebased on a Huffman dictionary.

// This function will compress the text and write it in a new file, using the dictionnary.
void rewrite_text(FILE* output, FILE* dictionnary, FILE* input) 
{
    // We make sure to be at the beginning of both files
    fseek(input, 0, SEEK_SET);
    fseek(output, 0, SEEK_SET); 
    char c, x;
    // The variable c will take the values of each character of the input.
    while ((c = fgetc(input)) != EOF)
    {
        fseek(dictionnary, 0, SEEK_SET);
        int z = 1;
        while (z == 1)
        {
            // The variable x will go trough the dictionnary until it finds the right value. We do not need to put any instructions inside the loop since everything happens in the condition
            while ((x = fgetc(dictionnary)) !=  c);
            if (c == '\n')
            {
                x = fgetc(dictionnary);
                if(x == '0' || x == '1')
                {
                    z = 0;
                }
                fseek(dictionnary, -1, SEEK_CUR);  
            }
            else
                z = 0;
        }
        // Once we've found the letter to compress, we'll go trough its binary version in the dictionnary, and write each character in the output file.
        while ((x = fgetc(dictionnary)) != '\n')
            fputc(x, output);
    }
}

//G) ⭐: Write a function that compresses a text file. The input file will not be modified, another file, containing the compressed text, will be created.


void compress_text_file(FILE* input){
    FILE* compressed = fopen("compressed.bin", "w"); // write only
    FILE* huffmanDico = fopen("huffmanDico.txt", "w+"); // write/lecture
    ListOccuChar ListOccuChar = create_list_char(input);
    list_bubble_sort(ListOccuChar);
    Tree ht = ht_from_list_of_occ(ListOccuChar);
    huffman_dico_in_txt(ht, huffmanDico);
    rewrite_text(compressed, huffmanDico, input);
    fclose(huffmanDico);
    fclose(compressed);
}


//H) ⭐⭐: Write a function that decompresses a text file from a Huffman tree. The input file will not be modified, another file, containing the decompressed text, will be created.
//Note : We will not be able to decompress a file outside of our compression program, since we need the tree in memory.

// This function decompress a compressed file, writing in a new output file with ASCII characters
void decompress_text(FILE* compressed)
{
    FILE* huffmanDico = fopen("huffmanDico.txt", "r");
    FILE* decompressed = fopen("decompressed.txt", "w");
    char c, Tmp;
    int max = 0;
    while ((c = fgetc(huffmanDico)) != EOF)
    {
        if (c == '\n')
            max += 1;
    }
    char* word = (char*)malloc((max)*sizeof(char));
    *word = '\0';
    while ((c = fgetc(compressed)) != EOF)
    {
        // we'll go trough the compressed file, and try to find each character in the dictionary (with find_value_in_dico).
        // If no value is found, we'll add a second character and look again. The string "word" will grow until it corresponds to a letter in the dictionary.
        // when this happens, we enter on the next "if" bloc, we print the character on the text file, and we reset the string "word" to start again with the next character.
        word = str_cat(word, c);
        Tmp = find_value_in_dico(word, huffmanDico, max);
        if (Tmp != '\0')
        {
            fputc(Tmp, decompressed);
            *word = '\0';
        }
    }
    free(word);
    fclose(decompressed);
    fclose(huffmanDico);
}

// This fonction checks if a string exists in the dico. 
char find_value_in_dico(char* word, FILE* dico, int max)
{
    char c, character;
    char* word2 = (char*)malloc(max * sizeof(char));
    *word2 = '\0';
    fseek(dico, 0, SEEK_SET);
    while ((c = fgetc(dico)) != EOF)
    {
        character = c;
        while ((c = fgetc(dico)) != '\n')
        {
            str_cat(word2, c);
        }
        if (strncmp(word, word2, max) == 0)
        {
            free(word2);
            // If the string is in the dictionary, we return the character.
            return character;
        }
        *word2 = '\0';
    }
    // If the string is not in the dictionary, we return \0
    free(word2);
    return '\0';
}

// This function is used on the previous ones, to concatenate two strings.
char* str_cat(char* word1, char c)
{
    char* start = word1;
    while(*start != '\0')
    {
        start++;
    }
    *start = c;
    start++;
    *start = '\0';
    return word1;
}