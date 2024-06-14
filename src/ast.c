#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.tab.h"
void yyerror(char *s);


int indent = 0;
void printIndent() {
    for (int i = 0; i < indent; i++) {
        printf("  ");
    }
}
void incrementIndent() {
    indent++;
}
void decrementIndent() {
    indent--;
}

/* build an AST node */
struct ast *expr_ast(int type, struct ast *l, struct ast *r) {
    struct ast *node = malloc(sizeof(struct ast));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->type = type;
    node->children = malloc(2 * sizeof(struct ast *));
    if (!node->children) {
        yyerror("out of space");
        exit(0);
    }
    node->children[0] = l;
    node->children[1] = r;
    node->num_children = 2;
    return node;
}

struct ast *num_ast(int value) {
    struct ast *node = malloc(sizeof(struct ast));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->type = ast_NUM;
    node->value = value;
    node->num_children = 0;
    return node;
}

struct ast *var_ast(char *name) {
    struct ast *node = malloc(sizeof(struct ast));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->type = ast_VAR_REF;
    node->name = name;
    node->num_children = 0;
    return node;
}


struct ast *decl_ast(char *name) {
    struct ast *node = malloc(sizeof(struct ast));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->type = ast_VAR_DECL;
    node->name = name;
    node->num_children = 0;
    return node;

}

struct ast *assign_ast(struct ast* ref, struct ast *expr) {
    struct ast *node = malloc(sizeof(struct ast));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->type = ast_ASSIGN;
    node->children = malloc(sizeof(struct ast *) * 2);
    if (!node->children) {
        yyerror("out of space");
        exit(0);
    }
    node->children[0] = ref;
    node->children[1] = expr;
    node->num_children = 2;
    return node;
}

struct ast *ret_ast(struct ast *expr) {
    struct ast *node = malloc(sizeof(struct ast));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->type = ast_RETURN;
    node->children = malloc(sizeof(struct ast *));
    if (!node->children) {
        yyerror("out of space");
        exit(0);
    }
    node->children[0] = expr;
    node->num_children = 1;
    return node;
}

struct ast *print_ast(struct ast *expr) {
    struct ast *node = malloc(sizeof(struct ast));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->type = ast_PRINT;
    node->children = malloc(sizeof(struct ast *));
    if (!node->children) {
        yyerror("out of space");
        exit(0);
    }
    node->children[0] = expr;
    node->num_children = 1;
    return node;
}

struct ast *scan_ast(struct ast *var) {
    struct ast *node = malloc(sizeof(struct ast));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->type = ast_SCAN;
    node->children = malloc(sizeof(struct ast *));
    if (!node->children) {
        yyerror("out of space");
        exit(0);
    }
    node->children[0] = var;
    node->num_children = 1;
    return node;
}


struct ast *program(struct ast **stmts, int len) {
    struct ast *node = malloc(sizeof(struct ast));
    if (!node) {
        yyerror("out of space");
        exit(0);
    }
    node->type = ast_PROGRAM;
    node->children = stmts;
    node->num_children = len;
    return node;
}


/* evaluate an AST */
void dump_ast(struct ast *node)
{
    printIndent();
    incrementIndent();
    switch (node->type) {
        case 0:
            printf("main\n"); break;
        case 1:
            printf("var_decl: %s\n", node->name); break;
        case 2:
            printf("var_ref: %s\n", node->name); break;
        case 3:
            printf("assign\n"); break;
        case 4:
            printf("add\n"); break;
        case 5:
            printf("sub\n"); break;
        case 6:
            printf("mul\n"); break;
        case 7:
            printf("div\n"); break;
        case 8:
            printf("num: %d\n", node->value); break;
        case 9:
            printf("return\n"); break;
        case 10:
            printf("print\n"); break;
        case 11:
            printf("scan\n"); break;
    }
    // printf("Evaluating node of type %d\n", node->type);
    for (int i = 0; i < node->num_children; i++)
    {
        dump_ast(node->children[i]);
    }
    decrementIndent();
}

void gen(struct ast *node) {
    switch (node->type) {
        case 0:
            return;
        case 1:
            return;
        case 2:
            printf("  mov rax, rbp\n");
            printf("  sub rax, %d\n", (node->name[0] - 'a' + 1) * 8);
            printf("  push rax\n");
            printf("  pop rax\n");
            printf("  mov rax, [rax]\n");
            printf("  push rax\n");
            return;
        case 3:
            printf("  mov rax, rbp\n");
            printf("  sub rax, %d\n", (node->children[0]->name[0] - 'a' + 1) * 8);
            printf("  push rax\n");
            gen(node->children[1]);
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  mov [rax], rdi\n");
            printf("  push rdi\n");
            return;
        case 4:
            gen(node->children[0]);
            gen(node->children[1]);
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  add rax, rdi\n");
            printf("  push rax\n");
            return;
        case 5:
            gen(node->children[0]);
            gen(node->children[1]);
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  sub rax, rdi\n");
            printf("  push rax\n");
            return;
        case 6:
            gen(node->children[0]);
            gen(node->children[1]);
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  imul rax, rdi\n");
            printf("  push rax\n");
            return;
        case 7:
            gen(node->children[0]);
            gen(node->children[1]);
            printf("  pop rdi\n");
            printf("  pop rax\n");
            printf("  cqo\n");
            printf("  idiv rdi\n");
            printf("  push rax\n");
            return;
        case 8:
            printf("  push %d\n", node->value);
            return;
        case 9:
            gen(node->children[0]);
            printf("  pop rax\n");
            printf("  leave\n");
            printf("  ret\n");
            return;
        case 10: 
            // printNumber(int)
            gen(node->children[0]);
            printf("  pop rdi\n");
            printf("  call printNumber\n");
            return;
        case 11:
            // scanNumber(int *)
            printf("  mov rax, rbp\n");
            printf("  sub rax, %d\n", (node->children[0]->name[0] - 'a' + 1) * 8);
            printf("  push rax\n");
            printf("  pop rdi\n");
            printf("  call scanNumber\n");
            return;
    }
}



/* delete and free an AST */
void free_ast(struct ast *node)
{
   
}