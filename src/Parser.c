#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Parser.h"
#include "Tokenizer.h"

static size_t p_index = 0;
static size_t s_index = 0;

static void* prog_statements;

token_t p_peek(token_t* tokens, int offset){
	token_t tok;
	token_t* p_token = &tok;
//	tok.value = (tokens + p_index + offset)->value;
	set_token_value(p_token, (tokens + p_index + offset)->value);
	tok.type = (tokens + p_index + offset)->type;
	
	return tok;
}

token_t p_consume(token_t* tokens, int offset){
	token_t tok;
	token_t* p_token = &tok;
// 	tok.value = (tokens + p_index + offset)->value;
	set_token_value(p_token, (tokens + p_index + offset)->value);
 	tok.type = (tokens + p_index + offset)->type;
 	p_index++;
 	
 	return tok;
}

void* parse_program(token_t* tokens){
	NodeExit exit_node = {};
	//NodeStmt node = {};
	while(p_peek(tokens, 0).type != k_tag_stop) {
		if(p_peek(tokens, 0).type == k_ret) {
			p_consume(tokens, 0); // consume ret
			p_consume(tokens, 0); // consume space
			if(p_peek(tokens, 1).type != semi) {
				printf("ERROR: Expected `;` after `ret <int>`\n");
				exit(3);
			}
			
			NodeExpr node_expr = parse_expr(tokens);
			if(node_expr.node_int_lit.int_lit.value) {
				exit_node = (NodeExit){.type = k_ret, .expr = node_expr};
			}
			else {
				printf("ERROR: invalid expression\n");
				exit(3);
			}
		}
		p_consume(tokens, 0);
	}
 	// might change this func to void instead of void ptr and add nodes to static statements global like we're doing for the tokenizer
 	void* v = &exit_node;
	return v;
	//return exit_node;
}

NodeExpr parse_expr(token_t* tokens){
	if(p_peek(tokens, 0).type == int_lit) {	
		return (NodeExpr){.node_int_lit = (NodeExprIntLit){.int_lit = p_consume(tokens, 0)}};
	}
	else if(p_peek(tokens, 0).type == ident) {
		return (NodeExpr){.node_ident = (NodeExprIdent){.ident = p_consume(tokens, 0)}};
	}
	else {
		return (NodeExpr){};
	}
}

 //parse_stmt







