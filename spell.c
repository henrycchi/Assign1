/**
Henry Chi
Application Security
CS-GY 9163
Assignment #1
*/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

// Turns all letters to upper case using toupper function.

void Uppercase(char * str) {
	if (str != NULL) {
		int len = strlen(str);
		for (int place = 0; place < len; place++) {
			str[place] = toupper(str[place]);
		}
	}
}

// Function 1 -  A function to load the list of words into the hash map (called load_dictionary).  Returns true if successful else false.

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
  FILE* wordlist;
  wordlist = fopen(dictionary_file, "r");
  if (wordlist == NULL) {
      return false;
  }
  for(int i = 0; i < HASH_SIZE; i++) {
      hashtable[i] = NULL;
  }
  char buff[LENGTH+1];
  while (fscanf(wordlist, "%s", buff) > 0) {
      Uppercase(buff);
      node* new_node = malloc(sizeof(node));
      new_node->next = NULL;
      strcpy(new_node->word, buff);
      int hash = hash_function(new_node->word);
      if (hashtable[hash] == NULL) {
          hashtable[hash] = new_node;
      }
      else {
          new_node->next = hashtable[hash];
          hashtable[hash] = new_node;
      }
  }
  fclose(wordlist);
  return true;
}

//Function 2 -  Function called check_words to tie them all together

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
	int WrongCount = 0;
        char line[4096];
        while(fgets(line,4096,fp)){
        char * token = strtok(line," .,-!?\n");
        while(token!= NULL){
            if (!check_word(token, hashtable)){
                misspelled[WrongCount] = token;
                WrongCount++;
            }
            token = strtok(NULL," .,-!?\n");
        }
    }
    return WrongCount;
}
//Function 3 - A function to check if a word is correctly spelled returns TRUE or FALSE

bool check_word(const char* word, hashmap_t hashtable[]) {
	if ((word == NULL) || (hashtable == NULL)) {
		return false; // NULL values not to be used
	}
	char UpperWord[strlen(word)]; 
	sprintf(UpperWord, "%s", word); //Stored as a string in the buffer 
	Uppercase(UpperWord);
	int hash = hash_function(UpperWord);
	hashmap_t hashes = hashtable[hash];
	if (hashes == NULL)  {
		return false; // NO MATCH
	}
	bool CorrectSpell = false;
	hashmap_t currentNode = hashes;
	while (currentNode->next != NULL) {
		if (strncmp(UpperWord, currentNode->word, strlen(currentNode->word)) == 0) {
			CorrectSpell = true;
			break;
	        } 
		else {
			CorrectSpell = false;		
		}
		currentNode = currentNode->next;
	}
	return CorrectSpell;
}


