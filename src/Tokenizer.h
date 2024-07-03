#pragma once

#include <stddef.h>

#define WORD_SIZE 32

typedef enum TokenType {
	eof,
	unknown,
	int_lit,
	str,
	ident,
	space,
	semi,
	eq,
	colon,
	quote,
	open_paren,
	close_paren,
	plus,
	// keywords
	k_int,
	k_disp,
	k_str,
	k_ret,
	k_exit,
	k_tag_stop
} token_type_t;

typedef struct Token {
	token_type_t type;
	char value[WORD_SIZE];
} token_t;

char peek(char* file_str, int offset);
char consume(char* file_str, int offset);
token_t t_peek(token_t* tokens, int offset);
token_t t_consume(token_t* tokens, int offset);
void tokenize(token_t* tokens, char* file_str);
char* type_to_str(token_type_t type);

size_t get_index();
void set_token_value(token_t* token, char* value);
