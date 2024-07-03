#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "Parser.h"

static size_t p_index = 0;
static size_t stmt_idx = 0;

// change to array with len = how many tokens we have??
//static void* prog_statements[];

token_t p_peek(token_t* tokens, int offset){
	token_t tok;
	token_t* p_token = &tok;
	set_token_value(p_token, (tokens + p_index + offset)->value);
	tok.type = (tokens + p_index + offset)->type;
	
	return tok;
}

token_t p_consume(token_t* tokens, int offset){
	token_t tok;
	token_t* p_token = &tok;
	set_token_value(p_token, (tokens + p_index + offset)->value);
 	tok.type = (tokens + p_index + offset)->type;
 	p_index++;
 	
 	return tok;
}

NodeStmt* parse_program(token_t* tokens, NodeStmt* program){
	while(p_peek(tokens, 0).type != k_tag_stop) {
//		if(p_peek(tokens, 0).type == space)
//			p_consume(tokens, 0);
		printf("%s\n", type_to_str(p_peek(tokens, 0).type));
		NodeStmt stmt = parse_stmt(tokens);
		if(stmt.keyword && stmt.keyword != k_tag_stop) {
			program[stmt_idx] = stmt;
		}
		else if(p_peek(tokens, 0).type == space) {
			p_consume(tokens, 0);
		}
		else {
			printf("ERROR: Expected statement\n");
			exit(EX_DATAERR);
		}
		stmt_idx++;
	}

	return program;
}

NodeExpr parse_expr(token_t* tokens){
	// These should only ever be expression tokens, not keywords
	if(p_peek(tokens, 0).type == int_lit) {	
		return (NodeExpr){.int_lit = p_consume(tokens, 0)};
	}
	else if(p_peek(tokens, 0).type == ident) {
		return (NodeExpr){.ident = p_consume(tokens, 0)};
	}
	else if(p_peek(tokens, 0).type == str) {
		return (NodeExpr){.string = p_consume(tokens, 0)};
	}
	else {
		return (NodeExpr){};
	}
}

NodeStmt parse_stmt(token_t* tokens){
	// These will only ever be my keywords that begin statements
	// exit
	if(p_peek(tokens, 0).type == k_exit && p_peek(tokens, 2).type == int_lit) {
		token_t token_exit = p_consume(tokens, 0);
		p_consume(tokens, 0); // space

		NodeStmt stmt;
		stmt.expr = parse_expr(tokens);
		stmt.keyword = token_exit.type;

		try_consume(tokens, semi);
		p_consume(tokens, 0);
		return stmt;
	}
	// disp
	if(p_peek(tokens, 0).type == k_disp) {
		token_t token_disp = p_consume(tokens, 0);
		try_consume(tokens, open_paren);

		NodeStmt stmt;
		stmt.expr = parse_expr(tokens);
		stmt.keyword = token_disp.type;

		try_consume(tokens, close_paren);
		try_consume(tokens, semi);
		return stmt;
	}
	else {
		return (NodeStmt){};
	}
}


token_t try_consume(token_t* tokens, token_type_t type){
	if(p_peek(tokens, 0).type == type) {
			token_t this_token = p_consume(tokens, 0); // consume this type
			//p_consume(tokens, 0); // consume space after
			return this_token;
	} else {
			printf("Expected `%s`\n", type_to_str(type));
			exit(EX_DATAERR);
	}
}




