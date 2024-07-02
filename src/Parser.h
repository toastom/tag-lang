#pragma once

#include "Tokenizer.h"

/*
// NodeTerm is either...
NodeExpr -> {
	token int
	string
}
because we're doing "is either" we can prob use a union
*/
/*
typedef struct node_disp {
	token_t str;
} NodeDisp;
*/

typedef struct {
	token_t int_lit;
} NodeExprIntLit;

typedef struct {
	token_t ident;
} NodeExprIdent;

typedef union {
	NodeExprIntLit node_int_lit;
	NodeExprIdent node_ident;
} NodeExpr;

typedef struct {
	// which keyword was used to declare a variable
	token_type_t type;
	token_t ident;
	NodeExpr expr;
} NodeStmtTypedef;

/*
typedef struct {
	token_t int_lit;
} NodeExpr;
*/

typedef struct {
	token_type_t type;
	NodeExpr expr;
} NodeExit;

typedef struct {
	token_t string;
} NodeDisp;

typedef union {
	NodeExit node_exit;
	NodeDisp node_disp;
	NodeStmtTypedef node_typedef;
} NodeStmt;

token_t p_peek(token_t* tokens, int offset);
token_t p_consume(token_t* tokens, int offset);
void* parse_program(token_t* tokens);
NodeExpr parse_expr(token_t* tokens);

