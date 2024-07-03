#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>

#include "Tokenizer.h"
#include "Parser.h"
#include "Generation.h"

int main(int argc, char* argv[]) {
	if(argc < 1) {
		printf("ERROR: missing argument for tag source code file");
		exit(EX_USAGE);
	}
	char* filename = argv[1];
	FILE *file = fopen(filename, "r");
	if(file == NULL) {
		printf("ERROR: .tag source code file not found\n");
		exit(EX_IOERR);
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
	// tokenize() operates on the copy of tokens we define here in main
	token_t tokens[file_size];
	tokenize(tokens, file_str);
	// Debug text to check and see if tokenizer worked
	int n = 0;
	for(n = 0; n < file_size && tokens[n].type != k_tag_stop; n++) {
		if(tokens[n].type == eof)
			break;
		printf("token[%d] TYPE is:\t%s\n", n, type_to_str(tokens[n].type));
		if(tokens[n].value)
			printf("token[%d] VALUE is:\t%s\n", n, tokens[n].value);
	}
	printf("\n");
	n++; // TODO: figure out if I need this to include tagstop
	printf("number of tokens: %d\n", n);

	//void* program_stmts[n];
	NodeStmt program_stmts[n];
	parse_program(tokens, program_stmts);

//	if(root == NULL) {
//		printf("ERROR: missing `exit <return_code>;` statement\n");
//		exit(EX_DATAERR);
//	}
	
	//generate_program(tokens, root);
	generate_program(tokens, program_stmts, n);

	// TODO: Change all these relative paths to absolute paths, or at least do it based on 
	// current user dir
	system("nasm -felf64 build/out.asm");
	system("ld -o build/out build/out.o");

	return EX_OK;
}

