#pragma once

#include "Tokenizer.h"

/*
typedef struct node_disp {
	token_t str;
} NodeDisp;
*/
/*
typedef struct {
	token_t int_lit;
} NodeExprIntLit;
*/

typedef struct {
	token_t ident;
} NodeExprIdent;

typedef union {
	token_t int_lit;
	token_t ident;
	token_t string;
} NodeExpr;

typedef struct {
	token_type_t keyword; // only put the keyword associated with statement in here
	token_t ident; // ident is optional, for ex -> int: ident = expr;
				   // but statement can also be disp(expr);
	NodeExpr expr;
} NodeStmt;

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

/*
typedef union {
	NodeExit node_exit;
	NodeDisp node_disp;
	NodeStmtType node_type;
} NodeStmt;
*/

token_t p_peek(token_t* tokens, int offset);
token_t p_consume(token_t* tokens, int offset);
token_t try_consume(token_t* tokens, token_type_t type);
NodeStmt* parse_program(token_t* tokens, NodeStmt* program);
NodeExpr parse_expr(token_t* tokens);
NodeStmt parse_stmt(token_t* tokens);
