#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "Tokenizer.h"

static size_t t_index = 0;
static size_t c_index = 0;

char* type_to_str(token_type_t type){
	char* type_str;
	switch(type) {
		case colon:
			type_str = ":";
			break;
		case eq:
			type_str = "=";
			break;
		case semi:
			type_str = ";";
			break;
		case quote:
			type_str = "\"";
			break;
		case plus:
			type_str = "+";
			break;
		case open_paren:
			type_str = "(";
			break;
		case close_paren:
			type_str = ")";
			break;
		case space:
			type_str = "space";
			break;
		case int_lit:
			type_str = "int_lit";
			break;
		case str:
			type_str = "string";
			break;
		case k_int:
			type_str = "int";
			break;
		case k_disp:
			type_str = "disp";
			break;
		case k_ret:
			type_str = "ret";
			break;
		case k_tag_stop:
			type_str = "tag_stop";
			break;
		case eof:
            type_str = "eof";
            break;
		case ident:
			type_str = "ident";
			break;
		default:
			type_str = "unknown";
	}
	return type_str;
}


void tokenize(token_t* tokens, char* file_str){
    int t = 0, w = 0;
    char word[WORD_SIZE] = {};
    memset(word, 0, sizeof(word));
	const size_t list_size = strlen(file_str);

    while(peek(file_str, 0) != EOF && t < list_size && w < WORD_SIZE) {
        if( isalpha(peek(file_str, 0)) ) {
            word[w] = consume(file_str, 0);
            w++;
            char next = peek(file_str, 0);
            while(w < WORD_SIZE && next != EOF && isalnum(next)) {
                word[w] = consume(file_str, 0);
                w++;
                next = peek(file_str, 0);
            }
            w++;
            word[w] = '\0';
            // keywords checking
            if(strcmp(word, "int") == 0) {
//                printf("FOUND keyword %s\n", word);
                tokens[t] = (token_t){.type = k_int, .value = "int"};
                memset(word, 0, sizeof(word));
                w = 0;
                t++;
                continue;
            }
            else if(strcmp(word, "disp") == 0) {
//                printf("FOUND keyword %s\n", word);
                tokens[t] = (token_t){.type = k_disp, .value = "disp"};
                memset(word, 0, sizeof(word));
                w = 0;
                t++;
                continue;
            }
            else if(strcmp(word, "ret") == 0) {
//                printf("FOUND keyword %s\n", word);
                tokens[t] = (token_t){.type = k_ret, .value = "ret"};
                memset(word, 0, sizeof(word));
                w = 0;
                t++;
                continue;
            }
			else if(strcmp(word, "tagstop") == 0) {
				tokens[t] = (token_t){.type = k_tag_stop, .value = "tag_stop"};
                memset(word, 0, sizeof(word));
                w = 0;
                t++;
                return;
			}
            else {
//                printf("FOUND identifier %s\n", word);
                //memset(temp_ident, 0, strlen(temp_ident));
//                strncpy(temp_ident, word, strlen(word));
                tokens[t] = (token_t){.type = ident};
				set_token_value(&tokens[t], word);
                memset(word, 0, sizeof(word));
                w = 0;
                t++;
                continue;
            }

        }

        else if( isdigit(peek(file_str, 0)) ) {
            word[w] = consume(file_str, 0);
            w++;
            char next = peek(file_str, 0);
            while(w < WORD_SIZE && next != EOF && isdigit(next)) {
                word[w] = consume(file_str, 0);
                w++;
                next = peek(file_str, 0);
            }
            w++;
            word[w] = '\0';
            // got the whole number
//            printf("FOUND int literal %s\n", word);
            tokens[t] = (token_t){.type = int_lit};
            set_token_value(&tokens[t], word);

            memset(word, 0, sizeof(word));
            w = 0;
            t++;
            continue;
        }
        else if(peek(file_str, 0) == ' ' || peek(file_str, 0) == '\t' || peek(file_str, 0) == '\n') {
//            printf("FOUND space %s\n", word);
			char c = consume(file_str, 0);
            tokens[t] = (token_t){.type = space};
            char temp[2] = {c, '\0'};
            set_token_value(&tokens[t], temp);

            w = 0;
            t++;
            continue;
        }
        else if(peek(file_str, 0) == '=') {
            char c = consume(file_str, 0);
//            printf("FOUND equals sign %c\n", c);
            tokens[t] = (token_t){.type = eq, .value = "="};
			t++;
            continue;
        }
        else if(peek(file_str, 0) == ':') {
            char c = consume(file_str, 0);
//            printf("FOUND colon %c\n", c);
            tokens[t] = (token_t){.type = colon, .value = ":"};
            t++;
            continue;
        }
        else if(peek(file_str, 0) == '+') {
            char c = consume(file_str, 0);
//            printf("FOUND plus %c\n", c);
            tokens[t] = (token_t){.type = plus, .value = "+"};
            t++;
            continue;
        }
        else if(peek(file_str, 0) == '"') {
			char c = consume(file_str, 0);
			char next = peek(file_str, 0);
			while(next != EOF && next != '"') {
				// single-line strings only
				if(c == ';') {
					printf("ERROR: strings must be contained within one line\n");
					exit(1);
				}
				word[w] = consume(file_str, 0);
				w++;
				next = peek(file_str, 0);
			}
//            printf("FOUND string %s\n", word);
            tokens[t] = (token_t){.type = str};
			set_token_value(&tokens[t], word);
//			printf("set token value to %s\n", tokens[t].value);

			memset(word, 0, sizeof(word));
			consume(file_str, 0);
			w = 0;
            t++;
            continue;
        }
        // comments! single line
        else if(peek(file_str, 0) == '-' && peek(file_str, 1) == '-') {
        	//consume(file_str, 0);
        	//consume(file_str, 0);
        	while(peek(file_str, 0) != '\n') {
        		consume(file_str, 0);
        	}
        	continue;
        }
        else if(peek(file_str, 0) == '(') {
            char c = consume(file_str, 0);
//            printf("FOUND open paren %c\n", c);
            tokens[t] = (token_t){.type = open_paren, .value = "("};
            t++;
            continue;
        }
        else if(peek(file_str, 0) == ')') {
            char c = consume(file_str, 0);
//            printf("FOUND close paren %c\n", c);
            tokens[t] = (token_t){.type = close_paren, .value = ")"};
            t++;
            continue;
        }
        else if(peek(file_str, 0) == ';') {
            char c = consume(file_str, 0);
//            printf("FOUND semicolon %c\n", c);
            tokens[t] = (token_t){.type = semi, .value = ";"};
            t++;
            continue;
        }
        else if(peek(file_str, 0) != EOF) {
            char c = consume(file_str, 0);
//            printf("Unknown identifier: %c\n ", c);
            tokens[t] = (token_t){.type = unknown, .value = ""};
            t++;
            continue;
        }
        else {
//            printf("ERROR: unknown tokenization error\n");
            break;
        }

        memset(word, 0, sizeof(word));
        w = 0;
        t++;
    }
    //printf("\n");
}

char peek(char* file_str, int offset){
	return *(file_str + c_index + offset);
}

char consume(char* file_str, int offset){
	char c = *(file_str + c_index + offset);
	c_index++;
	return c;
}

size_t get_index(){
	return t_index;
}

void set_token_value(token_t* token, char* value){
	//const int length = strlen(token.value);
	for(int i = 0; i < WORD_SIZE; i++) {
		token->value[i] = value[i];
	}
}
