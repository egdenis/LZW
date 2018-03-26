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

  while(*buffer != '\0'){

    current_char = *buffer++;
    debug_print("current char: %c\n", current_char);

    
    //Keep on appending new chars from buffer until we create a string that has not been seen previously
    current_str = realloc(current_str, strlen(current_str)+1);
    append(current_str,current_char);
        debug_print("current string: %s\n", current_str);

    debug_print("dictionary search: %d\n\n", search(dictionary,current_str));


    if(!search(dictionary,current_str)){

      char* str_to_write = malloc(strlen(current_str)-1);
      str_to_write = strncpy(str_to_write, current_str, strlen(current_str)-1);
      
      int str_to_write_code = search(dictionary, str_to_write);

      char* str_to_write_binary_code = malloc((int)ceil(str_to_write_code)+1);
      

      itoa(str_to_write_code,str_to_write_binary_code, 2);
      
      debug_print("str_to_write: %s\n", str_to_write);
      debug_print("str_to_write_code: %d\n", str_to_write_code);
      debug_print("str_to_write_binary_code: %s\n", str_to_write_binary_code);

      strncpy(compressed + strlen(compressed), str_to_write_binary_code, strlen(str_to_write_binary_code));

      insert(dictionary, current_str);

      current_str = realloc(current_str, 2);
      current_str[0] = current_char;
      current_str[1] = '\0';
      debug_print("dictionary count: %d\n", dictionary->count);
      debug_print("compressed string: %s\n\n\n\n", compressed);
    }
  }


}
void decompress(char *compressed){

}

char* readFile(char *filename)
{
   char *buffer = NULL;
   int string_size, read_size;
   FILE *handler = fopen(filename, "r");

   if (handler)
   {
       // Seek the last byte of the file
       fseek(handler, 0, SEEK_END);
       // Offset from the first to the last byte, or in other words, filesize
       string_size = ftell(handler);
       // go back to the start of the file
       rewind(handler);

       // Allocate a string that can hold it all
       buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

       // Read it all in one operation
       read_size = fread(buffer, sizeof(char), string_size, handler);

       // fread doesn't set it so put a \0 in the last position
       // and buffer is now officially a string
       buffer[string_size] = '\0';

       if (string_size != read_size)
       {
           // Something went wrong, throw away the memory and set
           // the buffer to NULL
           free(buffer);
           buffer = NULL;
       }

       // Always remember to close the file.
       fclose(handler);
    }

    return buffer;
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