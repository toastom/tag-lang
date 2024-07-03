#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "Generation.h"
#include "Parser.h"

// TODO: Use node parsing in code generation
// - working on that now

//void generate_program(token_t* tokens, NodeExit root){
void generate_program(token_t* tokens, NodeStmt* root, const int prog_len){
	char out_asm[CODE_GEN_SIZE];
	memset(out_asm, 0, strlen(out_asm));
	strncat(out_asm, "global _start\n", 15);

	char data[CODE_GEN_SIZE];
	memset(data, 0, strlen(out_asm));
	strncat(data, "section .data\n", 15);
	char text[CODE_GEN_SIZE];
	memset(text, 0, strlen(out_asm));
	strncat(text, "section .text\n", 15);

	
	//tokens_to_asm(tokens, data, text);
	generate_asm(data, text, root, prog_len);
	strncat(out_asm, data, strlen(data));
	strcat(out_asm, "\0");
	strncat(out_asm, text, strlen(text));
	strcat(out_asm, "\0");
	
	printf("Output ASM code:\n%s\n", out_asm);

	// Save code to out.asm file
	FILE* out_file = fopen("build/out.asm", "w");
	if(!out_file) {
		printf("ERROR: Could not write to file out.asm\n");
		exit(EX_CANTCREAT);
	}
	fputs(out_asm, out_file);
	fclose(out_file);

	printf("Saved to file: build/out.asm\n");
}

// This func will be a replacement for tokens_to_asm
// Go through the nodes now to generate asm code instead of individual tokens
void generate_asm(char* data, char* text, NodeStmt* root, const int prog_len){
	strcat(text, "_start:\n");
	size_t text_len = CODE_GEN_SIZE - strlen(data);
	int str_count = 0;

	//root = (NodeExit*)root;
	//NodeExit* root_ptr = (NodeExit*)root;
	//NodeStmt* root_ptr = &root;
	
	int prog_idx = 0;
	while(prog_idx <= prog_len && strlen(text) < text_len-1) {
		printf("prog_stmt[%d]->type is:\t%s\n", prog_idx, type_to_str(root[prog_idx].keyword));

		if(root[prog_idx].keyword == k_exit) {
			strcat(text, "\tmov rax, 60\n"); // rax -> 60 for exit syscall
			char tmp[WORD_SIZE+12]; //FIXME: magic number 12 for mov rdi
			sprintf(tmp, "\tmov rdi, %s\n", root[prog_idx].expr.int_lit.value);
			strcat(text, tmp);
			strcat(text, "\tsyscall\n");
			//break;
		}
		else if(root[prog_idx].keyword == k_disp) {
			char tmp[WORD_SIZE+12]; //FIXME: magic number 12 for mov rdi str
			sprintf(tmp, "\tstr%d:\tdb \"%s\", 0xA\n", str_count, root[prog_idx].expr.string.value);
			strcat(data, tmp);
			
			strcat(text, "\tmov rax, 1\n"); // rax -> 1 for write syscall
			strcat(text, "\tmov rdi, 1\n"); // rdi -> 1 for write to stdout
			//char tmp[WORD_SIZE+12];
			sprintf(tmp, "\tmov rsi, str%d\n", str_count);
			strncat(text, tmp, strlen(tmp));
			sprintf(tmp, "\tmov rdx, %ld\n", strlen(root[prog_idx].expr.string.value)); // rdx -> strlen
			strncat(text, tmp, strlen(tmp));
			strcat(text, "\tsyscall\n");

			str_count++;
		}
		prog_idx++;
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
					exit(EX_DATAERR);
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
				exit(EX_DATAERR);
			}
			p_consume(tokens, 0);
			if(p_peek(tokens, 0).type != str) {
				printf("ERROR: Expected string after `(`\n");
				exit(EX_DATAERR);
			}
			token_t str = p_consume(tokens, 0);
			if(p_peek(tokens, 0).type != close_paren) {
				printf("ERROR: Expected `)` after string`\n");
				exit(EX_DATAERR);
			}
			p_consume(tokens, 0);
			if(p_peek(tokens, 0).type != semi) {
				printf("ERROR: Expected `;`\n");
				exit(EX_DATAERR);
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

