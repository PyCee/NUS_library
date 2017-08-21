#include <stdio.h>
#include <stdlib.h>

#include <NUS/NUS_library.h>

int main(int argc, char *argv[])
{
  NUS_binary_model binary_model;
  if(argc < 3){
    printf("NUSM_ERROR::incorrect number of arguments: %d\n", argc);
    return -1;
  }
  printf("Loading information from file using assimp\n");
  if(nusm_load(argv[1], &binary_model) != NUS_SUCCESS){
    printf("NUSM_ERROR::failed to load file data\n");
    return -1;
  }
  printf("File information loaded\nStoring information in file\n");
  if(nusm_store(argv[2], &binary_model) != NUS_SUCCESS){
    printf("NUSM_ERROR::failed to store data into binary file\n");
    return -1;
  }
  printf("Information stored\nValidating created file\n");
  if(nusm_validate(argv[2], &binary_model) != NUS_SUCCESS){
    printf("NUSM_ERROR::failed to validate created file\n");
    return -1;
  }
  nus_binary_model_free(&binary_model);
  printf("Validation Successful\nFile Successfully Created\n");
  return 0;
}
