#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dictionary.h"
#include <sys/stat.h>

// check for valid input file
int file_isreg(const char *path) {
    struct stat st;
    if (stat(path, &st) < 0)
        return -1;
    return S_ISREG(st.st_mode);
}
/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
  FILE* fp;
  // validate input file
  if (file_isreg(dictionary_file)){
    // open the dictionary_file text file.
    fp = fopen(dictionary_file, "r");
  }

  // return false if file opening failed
  if (fp == NULL) {
      printf("Failed to load the dictionary file!");
      return false;
  }
  // initialize null values in hash table

  for(int i = 0; i < HASH_SIZE; i++) {
      hashtable[i] = NULL;
  }
  // loop through file until EOF
  char buff[LENGTH+1];
  while (fscanf(fp, "%s", buff) > 0) {
      // allocate memory
      node* new_node = malloc(sizeof(node));
      // null ptr to next
      new_node->next = NULL;
      // copy word value to new node
      strcpy(new_node->word, buff);
      // get bucket value of word
      int bucket = hash_function(new_node->word);
      // if the first node added
      if (hashtable[bucket] == NULL) {
          hashtable[bucket] = new_node;
      }
      // or add to next node
      else {
          new_node->next = hashtable[bucket];
          hashtable[bucket] = new_node;
      }
  }
  // close the file
  fclose(fp);
  // return true
  return true;
}
/**
 * Returns true if word is in dictionary else false.
 */
bool check_word(const char* word, hashmap_t hashtable[]){
  // word and word length variables
  int word_len = strlen(word);
  char word_lwr[LENGTH+1];
  // convert word to lowercase
  for (int i = 0; i < word_len; i++) {
      if(isupper(word[i])){
          word_lwr[i] = tolower(word[i]) ;
      }
      else{
          word_lwr[i] = word[i];
      }
  }
  // add null character to end of char
  word_lwr[word_len] = '\0';
  // get bucket value of word
  int bucket = hash_function(word_lwr);
  // cursor set to first node in bucket
  node* cursor = hashtable[bucket];
  // compare each word stored in each node to word_lwr
  while (cursor != NULL){
      // word is the same, return true
      if (strcmp(word_lwr, cursor->word) == 0){
          return true;
      }
      // move cursor to next node
      cursor = cursor->next;
  }
  // else return false
  return false;
}
/**
 * Array misspelled is populated with words that are misspelled. Returns the length of misspelled.
 */
int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
  // variables
  int num_misspelled = 0;
  int index = 0;
  char word[LENGTH+1];
  // exit if file opening failed
  if (fp == NULL){
      printf("Could not open file %s.\n", fp);
      return 1;
  }
  // spell-check words
  for (int c = fgetc(fp); c != EOF; c = fgetc(fp)){
    // if alpha characters or apostrophe
    if (isalpha(c) || (c == '\'' && index > 0)){
        // append character to word
        word[index] = c;
        index++;
        // ignore values longer than length
        if (index > LENGTH){
            while ((c = fgetc(fp)) != EOF && isalpha(c));
            index = 0;
        }
    }
    // if word contains numbers
    else if (isdigit(c)){
        while ((c = fgetc(fp)) != EOF && isalnum(c));
          index = 0;
    }
    // handle found word
    else if (index > 0){
        // add null terminate on word
        word[index] = '\0';
        // check spelling
        bool misspelled = !check_word(word, hashtable);
        // print misspelled word
        if (misspelled){
            printf("%s\n", word);
            // increment number misspelled
            num_misspelled++;
        }
        index = 0;
    }
  }
  fclose(fp);
  return num_misspelled;
}
© 2019 GitHub, Inc.
Terms
Privacy
Security
Status
Help
Contact GitHub
Pricing
API
Training
Blog
About

