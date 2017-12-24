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
int words=0;
FILE *dct;
typedef struct node {
                    char value[46];
                    struct node* next;
                }
                node;
                node *hashtable[26]={NULL};
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int cindex=hash(word[0]);
    int check;
    node *cursor;
    cursor =hashtable[cindex];
    while(cursor!=NULL)
    {   const char *cword=cursor->value;
        check=strcasecmp(cword,word);
        if(check==0){
            return true;
        }
        cursor=cursor->next;
    }
    return false;
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


    int index;
    char word[46];
    //read the words from the file until EOF file is reached
    while(fscanf(dct,"%s",word)!=EOF){
        node *new_node= malloc(sizeof(node));
        //check if malloc is able to allocate memory for the pointer
        if(new_node==NULL){
            printf("couldn't allocate memory");
            unload();
            return false;
        }
        strcpy(new_node->value,word);
        words++;
        index=hash(word[0]);
        if(hashtable[index]==0){
           hashtable[index]=new_node;
           new_node->next=NULL;
        }
        else{
            node *tptr;
            tptr=hashtable[index];
            while(tptr->next!=NULL){
                tptr=tptr->next;
            }

        new_node->next=NULL;
        tptr->next=new_node;
        }
    }
 return true;

}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{

    return words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for(int i=0;i<26;i++)
    {
        node *cursor=hashtable[i];
        while(cursor!=NULL){
            node *temp=cursor;
            cursor=cursor->next;
            free(temp);
        }
    }
    fclose(dct);
    return true;
}
int hash(char word){
  char alpha = tolower(word);
  int  temp= alpha - 'a';
  return temp;
}
