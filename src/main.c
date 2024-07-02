#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Tokenizer.h"
#include "Parser.h"
#include "Generation.h"

int main(int argc, char* argv[]) {
//	char *filename = "code.tag";
	if(argc < 1) {
		printf("ERROR: missing argument for tag source code file");
		exit(-1);
	}
	char* filename = argv[1];
	FILE *file = fopen(filename, "r");
	if(file == NULL) {
		printf("ERROR: .tag source code file not found\n");
		exit(-1);
	}

	// Get file size as number of characters
	fseek(file, 0, SEEK_END);
	const size_t file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	// Now get the file as a char string
	char file_str[file_size];
	char c;
	int i = 0;
	while((c = fgetc(file)) != EOF) {
		file_str[i] = c;
		i++;
	}
	file_str[i+1] = EOF;
	fclose(file);

	// And now actually turn that str into tokens
	// tokenize() operates on that copy of tokens we define here in main
	token_t tokens[file_size];
	tokenize(tokens, file_str);
	// Check to see if tokenizer worked
	for(int i = 0; i < file_size && tokens[i].type != unknown; i++) {
		if(tokens[i].type == eof)
			break;
		printf("token[%d] TYPE is:\t%s\n", i, type_to_str(tokens[i].type));
		if(tokens[i].value)
			printf("token[%d] VALUE is:\t%s\n", i, tokens[i].value);
	}
	printf("\n");

	//NodeExit root = parse_program(tokens);
	void* root = parse_program(tokens);
	//if(root.expr.node_int_lit.int_lit == NULL) {
	if(!root) {
		printf("ERROR: missing `ret <int>;` statement\n");
		exit(3);
	}
	
	generate_program(tokens, root);

	// TODO: Change all these relative paths to absolute paths, or at least do it based on 
	// current user dir
	system("nasm -felf64 out.asm");
	system("ld -o out out.o");

	return 0;
}

