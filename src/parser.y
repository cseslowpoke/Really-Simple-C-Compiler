/* declarations */
%{
#include <stdio.h>
#include <string.h>
#include "ast.h"
int yylex();
void yyerror(char *);
struct ast *root;
int cnt = 0;
int symbol[256] = {};
int semaError = 0;
%}
%union {
    struct ast *node;
    struct ast *nodes[100];
    char *identifier;
    int value;
}


%token LPAREN RPAREN LBRCKT RBRCKT
%token KW_INT KW_RET KW_MAIN KW_PRINT KW_SCAN
%left ADD SUB 
%left MUL DIV 
%token SEMI COMMA ASSN 
%token <identifier> ID
%token <value> NUMBER
%type <node> expr
%type <nodes> stmts
%type <node> stmt
%type <node> Varref
%start main

%%

main:
    KW_INT KW_MAIN LPAREN RPAREN LBRCKT stmts RBRCKT {
        root = program($6, cnt);
    }

stmts:
    eps {
        
    }
    | stmts stmt {
        $$[cnt++] = $2;
    }

stmt:
    KW_INT ID SEMI {
        if (symbol[(int)$2[0]]) {
            printf("Error: Redefined Variable\n");
            semaError = 1;
        }
        symbol[(int)$2[0]] = 1;
        $$ = decl_ast($2);
    }
    | Varref ASSN expr SEMI{
        $$ = assign_ast($1, $3); 
    }
    | KW_RET expr SEMI{
        $$ = ret_ast($2);
    }
    | KW_PRINT LPAREN expr RPAREN SEMI{
        $$ = print_ast($3);
    }
    | KW_SCAN LPAREN Varref RPAREN SEMI{
        $$ = scan_ast($3);
    }
    ;

Varref:
    ID {
        if (!symbol[(int)$1[0]]) {
            printf("Error: Not Define Variable\n");
            semaError = 1;
        }
        $$ = var_ast($1);
    }

expr:
    NUMBER {
        $$ = num_ast($1);
    }
    | Varref {
        $$ = $1;
    }
    | expr MUL expr {
        $$ = expr_ast(6, $1, $3);
    }
    | expr DIV expr {
        $$ = expr_ast(7, $1, $3);
    }
    | expr ADD expr {
        $$ = expr_ast(4, $1, $3);
    }
    | expr SUB expr {
        $$ = expr_ast(5, $1, $3);
    }
    | LPAREN expr RPAREN {
        $$ = $2;
    }
    ;

eps: %empty 
    ;


%%

/* C code */
int main(int argv, char **argc) {
    if (argv < 2) {
        printf("Usage: %s <filename> [-o <output> / --ast]\n", argc[0]);
        return 1;
    }
    freopen(argc[1], "r", stdin);
    yyparse(); 
    if (semaError) {
        printf("Semantic Error!!!\n");
        return 1;
    }
    if (argv == 3 && strcmp(argc[2], "--ast") == 0){
        dump_ast(root);
        return 0;
    }
    if (argv == 4 && strcmp(argc[2], "-o") == 0) {
        freopen(argc[3], "w", stdout);
        // 輸出前半段組合語言指令
        printf("section .text\n");
        printf("global main\n");
        printf("extern printNumber\n");
        printf("extern scanNumber\n");
        printf("main:\n");

        // 前言
        // 預留26個變數的空間
        printf("  push rbp\n");
        printf("  mov rbp, rsp\n");
        printf("  sub rsp, 208\n");
        for (int i = 0; i < root->num_children; i++) {
            gen(root->children[i]);
        }
        // 結束
        return 0;
    }
    else if (argv == 2) {
        freopen("a.asm", "w", stdout);
        // 輸出前半段組合語言指令
        printf("section .text\n");
        printf("global main\n");
        printf("extern printNumber\n");
        printf("extern scanNumber\n");
        printf("main:\n");
        // 前言
        // 預留26個變數的空間
        printf("  push rbp\n");
        printf("  mov rbp, rsp\n");
        printf("  sub rsp, 208\n");
        for (int i = 0; i < root->num_children; i++) {
            gen(root->children[i]);
        }
        // 結束
        return 0;
    }
    printf("Usage: %s <filename> [-o <output> / --ast]\n", argc[0]);
    return 0;
}

void yyerror(char *s) {
    fprintf(stderr, "error: %s\n", s);
}