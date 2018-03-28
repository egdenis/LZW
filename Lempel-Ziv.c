#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "trieADT.c"
#include "helpers.c"


char* compress(char *buffer, char* alphabet){
  char* compressed = calloc(1,1);
  char* current_str = calloc(1,1); //find something better. Maybe limit dic size
  char current_char;
  trie_t dictionary;

  initialize(&dictionary);
  initilize_dictionary(&dictionary, alphabet);
  while(*buffer){

    current_char = *buffer++;
    
    //Keep on appending new chars from buffer until we create a string that has not been seen previously
    current_str = realloc(current_str, strlen(current_str)+1);
    append(current_str,current_char);
    
    debug_print("current char: %c\n", current_char);
    debug_print("current char: %d\n", current_char);
    debug_print("current string: %s\n", current_str);
    debug_print("dictionary search: %d\n\n", search(&dictionary,current_str));


    if(!search(&dictionary,current_str)){

      int code_length = ((int)ceil(log2(dictionary.count)));
      char* str_to_write = calloc(strlen(current_str),sizeof(char));
      int str_to_write_code;
      char* str_to_write_binary_code = malloc((int)ceil(str_to_write_code)+1);
      char* str_to_write_binary_code_padding = calloc(code_length+1,1);
      


      str_to_write = strncpy(str_to_write, current_str, strlen(current_str)-1);
      str_to_write_code = search(&dictionary, str_to_write);
      str_to_write_binary_code = itoa(str_to_write_code,str_to_write_binary_code, 2);

      char* write_address_padding = str_to_write_binary_code_padding + code_length - strlen(str_to_write_binary_code);
      memset(str_to_write_binary_code_padding, 48, code_length); 

      write_address_padding = strncpy(write_address_padding, str_to_write_binary_code, strlen(str_to_write_binary_code));

      debug_print("code_length: %d\n", code_length);
      debug_print("str_to_write: %s\n", str_to_write);
      debug_print("str_to_write_code: %d\n", str_to_write_code);
      debug_print("str_to_write_binary_code: %s\n", str_to_write_binary_code);
      debug_print("str_to_write_binary_code_padding: %s\n", str_to_write_binary_code_padding);


      compressed = realloc(compressed, strlen(compressed)+code_length+1);
      strcpy(compressed + strlen(compressed), str_to_write_binary_code_padding);

      insert(&dictionary, current_str);

      current_str = realloc(current_str, 2);
      current_str[0] = current_char;
      current_str[1] = '\0';

      debug_print("dictionary count: %d\n", dictionary.count);
      debug_print("compressed string: %s\n\n\n\n", compressed);
    }


  }
  return compressed;

}
void decompress(char* src, int src_len, char* dest, int dest_len, char* alphabet){  
  int src_index = 0;
  int dest_index = 0;

  char* current_code_bin; 
  int current_code;
  int prev_code;

  char* entry;

  char** dictionary =   initilize_decode_dictionary( alphabet);
  int dictionary_length = strlen(alphabet)*2;
  int dictionary_index = strlen(alphabet);


  while(src_index < src_len){
    int code_length = ((int)ceil(log2(dictionary_index)));

    //Get next code from src 
    current_code_bin = calloc(code_length+1,1); 
    current_code_bin = strncpy(current_code_bin, src + src_index, code_length);
    current_code = strtol(current_code_bin, NULL, 2)-1;
    src_index += code_length;

    printf("\nsrc_index: %d\n", src_index);
    printf("code_length: %d\n", code_length);
    printf("current_code_bin: %s\n", current_code_bin);
    printf("current_code: %d\n", current_code);

    //dest = strcpy(dest + strlen(dest), oldcode);

    if (current_code < dictionary_length){
      entry = dictionary[current_code];
      strcpy(dest+dest_index, entry);
      dest_index+=strlen(entry);
      printf("entry: %s\n", entry);
      printf("Entry in dictionary#%d: %s\n",current_code ,dictionary[current_code] );
      printf("decompressed: %s\n",dest );
      for (int i = 0; i < dictionary_index; ++i)
      {
        printf("%s\n",dictionary[i] );
      }

      //Add code to dictionary (we know that the previous code plus the first letter of the next code make a new definition)
      dictionary[dictionary_index] = malloc(strlen(dictionary[prev_code])+2);
      strcpy(dictionary[dictionary_index],dictionary[prev_code]);
      strncat(dictionary[dictionary_index], entry, 1);
      printf("Dict Entry #%d: %s\n",dictionary_index ,dictionary[dictionary_index] );

      dictionary_index++;

      prev_code = current_code; //****OUTSIDE OF IF???
    }
    else{
      //dictionary[dictPos++] = (unsigned char*) strncat((char*)old_code, (char*)d, 1);
    }

    if(dictionary_index>=dictionary_length){
        dictionary_length*=2;
        dictionary = realloc(dictionary, dictionary_length);
    }
  }
}





int main( int argc, char* argv[] ){
  if (argc != 2){
    fprintf(stderr, "No file has been given to compress");
  }

  char* buffer = readFile(argv[1]);
  char* alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  char* compressed_buffer;
  char* uncompressed_buffer = calloc(strlen(buffer),1);
  double compression_ratio;

  printf("pre compress\n");

  compressed_buffer = compress(buffer, alphabet);
  compression_ratio = ((double)strlen(compressed_buffer))/((double)(strlen(buffer)*8));

  printf("Original size: %d\n", (int)strlen(buffer)*8);
  printf("Compressed size: %d\n", (int)strlen(compressed_buffer));
  printf("Compression ratio: %f\n", compression_ratio);

  decompress(compressed_buffer, strlen(compressed_buffer), uncompressed_buffer, strlen(buffer), alphabet);



}