#include "Tokenizer.h"
#include "Parser.h"

#define CODE_GEN_SIZE 2048 // 2 kB file size for now??

//void generate_program(token_t* tokens, NodeExit root);
void generate_program(token_t* tokens, void* root);
void generate_asm(void* root, char* data, char* text);
char* tokens_to_asm(token_t* tokens, char* data, char* text);
