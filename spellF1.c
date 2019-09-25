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
	FILE *dictlist = fopen(dictionary_file, "r");
 	if(dictlist == NULL){
      		return false;             
   	}
	for (int i=0; i<HASH_SIZE; i++) { //initialize the hashtable to null
        	hashtable[i]=NULL;
	}
	char wordsize[LENGTH + 1];
	while (fscanf(dictlist, "%45s", wordsize) > 0) {	
		Uppercase(wordsize);
		int hash = hash_function(wordsize);
		hashmap_t newNode = (hashmap_t) malloc(sizeof(node));
		newNode->next = NULL;
		strncpy(newNode->word, wordsize, LENGTH);
		if (hashtable[hash] == NULL ) {
		    hashtable[hash] = newNode;
		}	
		else {
		    hashmap_t curPtr = hashtable[hash];
		    hashmap_t nextPtr = curPtr->next;
		    while (nextPtr != NULL) {
			curPtr = nextPtr;
			nextPtr = curPtr->next;
		    } 
		    curPtr->next = newNode;
		} 
		char * Buffer2 = NULL;
		char nextChar = fgetc(dictlist); 
		fseek(dictlist,ftell(dictlist)-1,0); 
		if (nextChar != '\n') {
			while (fscanf(dictlist, "%500ms", &Buffer2) > 0) {
			    char next = fgetc(dictlist);    
			    fseek(dictlist, ftell(dictlist)-1,0); 
			    free(Buffer2);
			    if (next == '\n') 
				break;
			     } 
		}  
	}
	fclose(dictlist);
	return true;
}

//Function 2 -  Function called check_words to tie them all together

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {

	int IncorrectCount = 0;
	char *Buffer1 = NULL;
	char * Buffer2 = NULL;

	while (fscanf(fp, "%80ms", &Buffer1) > 0) {
		if (strlen(Buffer1) == 80) {
			char nextChar = fgetc(fp);
			fseek(fp,ftell(fp)-1,0); 
			if (nextChar != '\n' && nextChar != ' ') {
				while (0 < fscanf(fp, "%80ms", &Buffer2)) {
				     if (strlen(Buffer2) < 80)
					     break;
				     else if (strlen(Buffer2) == 80) {
					char next = fgetc(fp);    
					fseek(fp, ftell(fp)-1,0); 
					if (next == '\n' || next == ' ')
						break;
				     }
				     free(Buffer2);
			     }
			     free(Buffer2);
			}
		}
		int LTAlph = -1;
		int RTAlph = -1;

		//Using the isalpha function to ensure it is always an alphabetic letter

		for (int position = 0; position <  strlen(Buffer1); position++) {
		    if (isalpha(Buffer1[position])) {
			LTAlph = position;
			break;
		    }
		} 
		for (int position = strlen(Buffer1) - 1; position >= 0; position--) {
		    if (isalpha(Buffer1[position])) {
			RTAlph = position;
			break;
		    }
		}
		if (LTAlph >= 0) {
			int bytesToCopy = (RTAlph - LTAlph) + 1;
			bool truncated = false;
			if (bytesToCopy > LENGTH) {
				bytesToCopy = LENGTH;
				truncated = true;
			}
			char * Buffer3 = malloc(LENGTH + 1);
			strncpy(Buffer3, &Buffer1[LTAlph], bytesToCopy);
			Buffer3[bytesToCopy] = '\0';
			if (truncated || (!check_word(Buffer3, hashtable))) {
			    misspelled[IncorrectCount++] = Buffer3; 
			} 
			else {
		   	     free(Buffer3);
			}
		}
		free(Buffer1);
	}
	return IncorrectCount;
}

//Function 3 - A function to check if a word is correctly spelled

bool check_word(const char* word, hashmap_t hashtable[]) {
	if ((word == NULL) || (hashtable == NULL)) {
		return false; // NULL values not to be used
	}
	char UpperWord[strlen(word)]; 
	sprintf(UpperWord, "%s", word); //Stored as a C string in the buffer 
	Uppercase(UpperWord);
	int hash = hash_function(UpperWord);
	hashmap_t hashes = hashtable[hash];
	if (hashes == NULL)  {
		return false; // NO MATCH
	}
	bool CorrectSpell = false;
	hashmap_t currentNode = hashes;
	int match;
	while (currentNode->next != NULL) {
		match = strncmp(UpperWord, currentNode->word, strlen(currentNode->word));
		if (match == 0) {
		    CorrectSpell = true;
	            break;
		} 
		else {
		CorrectSpell = false;		
		}
		currentNode = currentNode->next;
	}
	if (!CorrectSpell) {
		match = strncmp(UpperWord, currentNode->word, strlen(currentNode->word));
		if (match == 0) {
	 	   CorrectSpell = true;
		}
		else {
		   CorrectSpell = false;	
		}
	}
	return CorrectSpell;
}


