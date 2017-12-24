/**
 * Implements a dictionary's functionality.
 */
#include<stdio.h>
#include<cs50.h>
#include <stdbool.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include<ctype.h>
#include "dictionary.h"
FILE *dct;
char word[46];
int num=0;
//Creating a structure of trie
typedef struct node {
                     bool is_word;
                     struct node* children[27];
                     }
                     node;
node *root;
node *cursor;
node *temp;
void freenode(node *);
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int i=0;
    temp=root;
    while(word[i]!='\0'){
        int index=abs(tolower(word[i])-'a');
        if(index>26){
                index=26;
        }
        temp=temp->children[index];
        if(temp==NULL){
          return false;
        }
        i++;
 }
 if(temp->is_word==true){

     return true;
 }
 else{
     return false;
 }
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{   //open dictionary file
    dct = fopen(dictionary,"rb");
    if(dct==NULL){
            printf("couldn't open dictionary");
                return false;
    }
    //Allocate memory to root node and initializes it.
    root=malloc(sizeof(node));
    memset(root,0,sizeof(node));

    //read the words from the file until EOF file is reached
    while(fscanf(dct,"%s",word)!=EOF){
        cursor=root;
        for(int i=0;word[i]!='\0';i++){
              //returns location in array for the word[i]
              int index=abs(tolower(word[i])-'a');
              if(index>26){
                      index=26;
              }

              //Check if letter is present in root node
              if(cursor->children[index]==NULL){

                    //Allocates memory to new_node and intializes it.
                    node *new_node=malloc(sizeof(node));
                    memset(new_node,0,sizeof(node));

                    //check if malloc can allocate memory for pointer
                    if(new_node==NULL){
                                printf("couldn't allocate memory");
                                unload();
                                return false;
                                }

                        cursor->children[index]=new_node;
                        cursor=cursor->children[index];
                }
             else {

                cursor=cursor->children[index];
            }
        }
    cursor->is_word=true;
    num++;
    }
return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
        return num;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{       freenode(root);
        fclose(dct);
        return 1;
}
void freenode(node *ptr){

for(int i=0;i<27;i++){
        if(ptr->children[i]!=NULL){
                freenode(ptr->children[i]);
        }
}
free(ptr);
}