/**
Henry Chi
Application Security
CS-GY 9163
Assignment #1
*/

#include "dictionary.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//nonvalue-returning function for changing case to lowercase

void Lowercase(char * lowstring) {
	if (lowstring != NULL) {
		int len = strlen(lowstring);
		for (int i = 0; i < len; i++) {
			lowstring[i] = tolower(lowstring[i]);
		}
	}
}

// Function 1 -  A function to load the list of words into the bucket map (called load_dictionary).  Returns true if successful else false.

/**
function load_dictionary(string dictionary, hashmap hashtable[])
{
    Initialize all values in bucket table to NULL.
    Open dict_file from path stored in dictionary.
    If dict_file is NULL:
        return false.
    While word in dict_file is not EOF (end of file):
        Set hashmap_t new_node to a new node.
        Set new_node->next to NULL.
        Set new_node->word equal to word.
        Set int bucket to hash_function(word).
        if hashtable[bucket] is NULL:
            Set hashtable[bucket] to new_node.
        else:
            Set new_node->next to hashtable[bucket].
            Set hashtable[bucket] to new_node.
    Close dict_file.
}
*/

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){   
	FILE *dict_file = fopen(dictionary_file, "r");
	if (dict_file == NULL) {
		return false;
	}
	else {
        	for(int i = 0; i < HASH_SIZE; i++) {
      		hashtable[i] = NULL;
		}
        	int bucket;
        	char line[LENGTH];
        	while(fgets(line,LENGTH,dict_file)){
         		if(line[strlen(line)-1] =='\n') {
				line[strlen(line)-1]='\0';
			}
            		Lowercase(line);
       			bucket = hash_function(line);
            	if(hashtable[bucket] == NULL){
                    node * cursor = malloc(sizeof(node));
                    strncpy(cursor->word,line,LENGTH);
                    cursor->next = NULL;
                    hashtable[bucket] = cursor;
            	}
            	else {
                    node * cursor = malloc(sizeof(node));
                    strncpy(cursor->word,line,LENGTH);
                    cursor->next =hashtable[bucket];
                    hashtable[bucket] = cursor;
                }
        }
        fclose(dict_file);
        return true;
        }
    }

//Function 2 -  Function called check_words to tie them all together

/**
function check_words(file fp, hashmap hashtable[], string misspelled[])
{
    Set int num_misspelled to 0.
    While line in fp is not EOF (end of file):
        Read the line.
        Split the line on spaces.
        For each word in line:
            Remove punctuation from beginning and end of word.
            If not check_word(word):
                Append word to misspelled.
                Increment num_misspelled.
    Return num_misspelled.
}
*/

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]){ 
	if(!fp) {
		return false;
	}
    	else {
    		int num_misspelled = 0 ;
		char BuffW[LENGTH];
		char* placeholder = strtok(BuffW," ");
		while(fgets(BuffW,LENGTH,fp) != NULL) {
			placeholder = strtok(NULL," ");
        		while(placeholder != NULL) {
            			if(placeholder[strlen(placeholder)-1] =='\n') {
					placeholder[strlen(placeholder)-1]='\0';
				}
				placeholder[strlen(placeholder)]='\0';           
         			bool punctright = ispunct(placeholder[strlen(placeholder)-1]);
            			if(punctright) {
					while(punctright) {
                			placeholder[strlen(placeholder)-1]='\0';
			                punctright = ispunct(placeholder[strlen(placeholder)-1]);
                    			}
                		}
				bool punctleft = ispunct(placeholder[0]);
         	       		if(punctleft) {
					while(punctleft) {
                       				placeholder++;
                    				punctleft = ispunct(placeholder[0]);
                    			}
                		}
				bool wordCheckResult = check_word(placeholder,hashtable);
            			if(!(wordCheckResult == 1 && strlen(BuffW) < LENGTH)) {
                			misspelled[num_misspelled] = malloc(strlen(placeholder));
                			strcpy(misspelled[num_misspelled], placeholder);
                			num_misspelled ++;
                		}
            			placeholder = strtok(NULL," ");
            		}
           	return num_misspelled;
        	}
    	}
        return true;
}

//Function 3 - A function to check if a word is correctly spelled returns TRUE or FALSE
/**
fuction check_word(string word, hashmap hashtable[])
{
    Set int bucket to the output of hash_function(word).
    Set hashmap_t cursor equal to hashmap[bucket].
    While cursor is not NULL:
        If word equals cursor->word:
            return True.
        Set curosr to cursor->next.
    Set int bucket to the output of hash_function(word).
    Set hashmap_t cursor equal to hashmap[bucket].
    While cursor is  not NULL:
        If lower_case(word) equals curosr->word:
            return True.
        Set curosr to cursor->next.
    return False.
}
*/
bool check_word(const char* word, hashmap_t hashtable[])
{ int WordLength = strlen(word);
    if (WordLength > LENGTH){
    	return 0;
    }
    int numeric = 1;
    for (int i = 0; i < WordLength; i++){
        if (!isdigit(word[i])){
            numeric = 0;
        }
    }
    if (numeric == 1){
        return 1;
    }
    int ascii = 1;
    for (int i = 0; i < WordLength; i++){
        if (isascii(word[i]) == 0){
            ascii = 0;
        }
    }
    if (ascii == 0){
        return 0;
    }
    int bucket = hash_function(word);
    hashmap_t cursor = hashtable[bucket];
        if (strcmp(word, cursor->word) == 0) {
            return 1;
        }
        else {
            char word_lower[LENGTH+1];
            for (int i=0; i <= strlen(word); i++) {
                word_lower[i] = tolower(word[i]);
            }
            int bucket2 = hash_function(word_lower);
            hashmap_t cursor2 = hashtable[bucket2];
            while(cursor2 != NULL) { 
                if (strcmp(word_lower, cursor2->word) == 0) {
	                return 1;
                }
                cursor2 = cursor2->next;
            }
        }
    return 0;
}
