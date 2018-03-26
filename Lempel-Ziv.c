#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "trieADT.c"




void compress(char *buffer, trie_t dictionary){
  char* compressed = calloc(sizeof buffer);
  char* current_str = malloc(2); //find something better. Maybe limit dic size
  while(*buffer != '\0'){
    char ch = *buffer++;
    debug_print("%c\n", ch);

 
    int code = search(&dictionary,current_str);
    debug_print("Code: %d\n", code);

    //String already exist so keep adding chars until we find new one
    if(code!=0){
      current_str[strlen(current_str)] = ch;
      current_str[strlen(current_str) + 1] = '\0';
      buffer++;
    }
    //String is not in dict so encode string with code and look for new 
    else{
            debug_print("dictionary.: %d\n", dictionary.count);

      int code_size = (int)ceil(log2(dictionary.count));
      debug_print("code_size: %d\n", code_size);

      char char_code[code_size];
      debug_print("code_size: %d\n", code_size);

      sprintf(char_code,"%d", code);

      strncpy(compressed + strlen(compressed), char_code, code_size);

      current_str[strlen(current_str)] = ch;
      current_str[strlen(current_str) + 1] = '\0';
      insert(&dictionary, current_str);

      current_str[0] = ch;
      current_str[1] = '\0';
    }
    printf("%s\n", compressed);
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

void initilize_dictionary(trie_t dictionary, char* alphabet){
  while (*alphabet != '\0'){
    char key[] = {*alphabet++, '\0'};
    insert(&dictionary,key );
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
  initilize_dictionary(dictionary, alphabet);

    
  fprintf(stderr, "YAH!\n" );
  compress(buffer, dictionary);
}