#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Generation.h"
#include "Parser.h"

//static void* root_node;

// TODO: Use node parsing in code generation
// - working on that now

//void generate_program(token_t* tokens, NodeExit root){
void generate_program(token_t* tokens, void* root){
	char out_asm[CODE_GEN_SIZE] = {};
	strcat(out_asm, "global _start\n");

	char data[CODE_GEN_SIZE];
	strcat(data, "section .data\n");
	char text[CODE_GEN_SIZE];
	strcat(text, "section .text\n");

	
	//tokens_to_asm(tokens, data, text);
	generate_asm(root, data, text);
	strncat(out_asm, data, strlen(data));
	strncat(out_asm, text, strlen(text));
	strcat(out_asm, "\0");
	
	printf("Output ASM code:\n%s\n", out_asm);

	// Save code to out.asm file
	FILE* out_file = fopen("out.asm", "w");
	if(!out_file) {
		printf("ERROR: Could not write to file out.asm\n");
		exit(1);
	}
	fputs(out_asm, out_file);
	fclose(out_file);

	printf("Saved to file: ./out.asm\n");
}

// This func will be a replacement for tokens_to_asm
// Go through the nodes now to generate asm code instead of individual tokens
void generate_asm(void* root, char* data, char* text){
	strcat(text, "_start:\n");
	size_t text_len = CODE_GEN_SIZE - strlen(data);
	int str_count = 0;

	//root = (NodeExit*)root;
	NodeExit* root_exit = root;
	// need some sort of generic Node* ptr
	
	while(root_exit) {
		if(root_exit->type == k_ret) {
			strcat(text, "\tmov rax, 60\n"); // rax -> 60 for exit syscall
			char tmp[WORD_SIZE+12]; //FIXME: magic number 12 for mov rdi
			sprintf(tmp, "\tmov rdi, %s\n", root_exit->expr.node_int_lit.int_lit.value);
			strcat(text, tmp);
			strcat(text, "\tsyscall\n");
			break;
		}
	}
}

// TODO: fix hardcoded error codes and put em all in #defines or an enum
char* tokens_to_asm(token_t* tokens, char* data, char* text){
	strcat(text, "_start:\n");
	size_t text_len = CODE_GEN_SIZE - strlen(data);
	int str_count = 0;
	
	while(p_peek(tokens, 0).type != k_tag_stop && strlen(text) < text_len-1) {
		// ret <int_lit>;
		if(p_peek(tokens, 0).type == k_ret) {
			p_consume(tokens, 0);
			if(p_peek(tokens, 0).type == space && p_peek(tokens, 1).type == int_lit) {
				p_consume(tokens, 0);
				token_t ret_code = p_consume(tokens, 0);
				if(p_peek(tokens, 0).type != semi) {
					printf("ERROR: Expected `;` after keyword `ret`\n");
					exit(2);
				}
				p_consume(tokens, 0);
				strcat(text, "\tmov rax, 60\n"); // rax -> 60 for exit syscall
				char tmp[WORD_SIZE+12]; //FIXME: magic number 12 for mov rdi
				sprintf(tmp, "\tmov rdi, %s\n", ret_code.value);
				strcat(text, tmp);
				strcat(text, "\tsyscall\n");
			}
		}
		// disp("string");
		else if(p_peek(tokens, 0).type == k_disp) {
			p_consume(tokens, 0);
			if(p_peek(tokens, 0).type != open_paren) {
				printf("ERROR: Expected `(` after keyword `disp`\n");
				exit(2);
			}
			p_consume(tokens, 0);
			if(p_peek(tokens, 0).type != str) {
				printf("ERROR: Expected string after `(`\n");
				exit(2);
			}
			token_t str = p_consume(tokens, 0);
			if(p_peek(tokens, 0).type != close_paren) {
				printf("ERROR: Expected `)` after string`\n");
				exit(2);
			}
			p_consume(tokens, 0);
			if(p_peek(tokens, 0).type != semi) {
				printf("ERROR: Expected `;`\n");
				exit(2);
			}
			p_consume(tokens, 0);

			char tmp[WORD_SIZE+12]; //FIXME: magic number 12 for mov rdi str
			sprintf(tmp, "\tstr%d:\tdb \"%s\"\n", str_count, str.value);
			strcat(data, tmp);
			
			strcat(text, "\tmov rax, 1\n"); // rax -> 1 for write syscall
			strcat(text, "\tmov rdi, 1\n"); // rdi -> 1 for write to stdout
			//char tmp[WORD_SIZE+12];
			sprintf(tmp, "\tmov rsi, str%d\n", str_count);
			strcat(text, tmp);
			sprintf(tmp, "\tmov rdx, %ld\n", strlen(str.value)); // rdx -> strlen
			strcat(text, tmp);
			strcat(text, "\tsyscall\n");

			str_count++;
		}

		p_consume(tokens, 0);
		text_len = CODE_GEN_SIZE - strlen(data);
	}
	return text;
}

