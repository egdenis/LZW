#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "trieADT.c"
#include "helpers.c"






void compress(char *buffer, trie_t *dictionary){
  char* compressed = calloc(1,sizeof buffer);
  char* current_str = calloc(1,1); //find something better. Maybe limit dic size
  char current_char;

  printf("Original size: %d\n", (int)strlen(buffer)*8);

  while(*buffer != '\0'){

    current_char = *buffer++;
    
    //Keep on appending new chars from buffer until we create a string that has not been seen previously
    current_str = realloc(current_str, strlen(current_str)+1);
    append(current_str,current_char);
    
    debug_print("current char: %c\n", current_char);
    debug_print("current char: %d\n", current_char);
    debug_print("current string: %s\n", current_str);
    debug_print("dictionary search: %d\n\n", search(dictionary,current_str));


    if(!search(dictionary,current_str)){

      char* str_to_write = calloc(strlen(current_str),sizeof(char));
      int str_to_write_code;
      char* str_to_write_binary_code = malloc((int)ceil(str_to_write_code)+1);

      str_to_write = strncpy(str_to_write, current_str, strlen(current_str)-1);
      str_to_write_code = search(dictionary, str_to_write);
      str_to_write_binary_code = itoa(str_to_write_code,str_to_write_binary_code, 2);

      debug_print("str_to_write: %s\n", str_to_write);
      debug_print("str_to_write_code: %d\n", str_to_write_code);
      debug_print("str_to_write_binary_code: %s\n", str_to_write_binary_code);


      compressed = realloc(compressed, strlen(compressed)+strlen(str_to_write_binary_code)+1);
      strcpy(compressed + strlen(compressed), str_to_write_binary_code);

      insert(dictionary, current_str);

      current_str = realloc(current_str, 2);
      current_str[0] = current_char;
      current_str[1] = '\0';

      debug_print("dictionary count: %d\n", dictionary->count);
      debug_print("compressed string: %s\n\n\n\n", compressed);
    }


  }

    printf("Compressed size: %d\n", (int)strlen(compressed));

}
void decompress(char *compressed){

}



void initilize_dictionary(trie_t *dictionary, char* alphabet){
  while (*alphabet != '\0'){
    char key[] = {*alphabet++, '\0'};
    insert(dictionary,key );
  }
}



int main( int argc, char* argv[] ){
  if (argc != 2){
    fprintf(stderr, "No file has been given to compress");
  }

  char* buffer = readFile(argv[1]);
  trie_t dictionary;

  initialize(&dictionary);
  char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  initilize_dictionary(&dictionary, alphabet);

    
  compress(buffer, &dictionary);
}