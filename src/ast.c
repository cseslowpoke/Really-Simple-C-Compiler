#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "parser.tab.h"

// 錯誤處理函數
void yyerror(char *s);

// 全局變量，用於控制縮排深度
int indent = 0;

// 打印當前縮排
void printIndent() {
  for (int i = 0; i < indent; i++) {
    printf("  ");
  }
}

// 增加縮排深度
void incrementIndent() {
  indent++;
}

// 減少縮排深度
void decrementIndent() {
  indent--;
}

/* 構建 AST 節點 */
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

// 構建數字節點
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

// 構建變量引用節點
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

// 構建變量聲明節點
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

// 構建賦值節點
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

// 構建返回節點
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

// 構建打印節點
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

// 構建掃描節點
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

// 構建程序節點
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

/* 遍歷並打印 AST */
void dump_ast(struct ast *node) {
  printIndent();
  incrementIndent();
  switch (node->type) {
    case ast_PROGRAM:
      printf("main\n"); break;
    case ast_VAR_DECL:
      printf("var_decl: %s\n", node->name); break;
    case ast_VAR_REF:
      printf("var_ref: %s\n", node->name); break;
    case ast_ASSIGN:
      printf("assign\n"); break;
    case ast_ADD:
      printf("add\n"); break;
    case ast_SUB:
      printf("sub\n"); break;
    case ast_MUL:
      printf("mul\n"); break;
    case ast_DIV:
      printf("div\n"); break;
    case ast_NUM:
      printf("num: %d\n", node->value); break;
    case ast_RETURN:
      printf("return\n"); break;
    case ast_PRINT:
      printf("print\n"); break;
    case ast_SCAN:
      printf("scan\n"); break;
  }
  // 遍歷子節點
  for (int i = 0; i < node->num_children; i++) {
    dump_ast(node->children[i]);
  }
  decrementIndent();
}

/* 生成彙編代碼 */
void gen(struct ast *node) {
  switch (node->type) {
    case ast_PROGRAM:
      return;
    case ast_VAR_DECL:
      return;
    case ast_VAR_REF:
      printf("  mov rax, rbp\n");
      printf("  sub rax, %d\n", (node->name[0] - 'a' + 1) * 8);
      printf("  push rax\n");
      printf("  pop rax\n");
      printf("  mov rax, [rax]\n");
      printf("  push rax\n");
      return;
    case ast_ASSIGN:
      printf("  mov rax, rbp\n");
      printf("  sub rax, %d\n", (node->children[0]->name[0] - 'a' + 1) * 8);
      printf("  push rax\n");
      gen(node->children[1]);
      printf("  pop rdi\n");
      printf("  pop rax\n");
      printf("  mov [rax], rdi\n");
      printf("  push rdi\n");
      return;
    case ast_ADD:
      gen(node->children[0]);
      gen(node->children[1]);
      printf("  pop rdi\n");
      printf("  pop rax\n");
      printf("  add rax, rdi\n");
      printf("  push rax\n");
      return;
    case ast_SUB:
      gen(node->children[0]);
      gen(node->children[1]);
      printf("  pop rdi\n");
      printf("  pop rax\n");
      printf("  sub rax, rdi\n");
      printf("  push rax\n");
      return;
    case ast_MUL:
      gen(node->children[0]);
      gen(node->children[1]);
      printf("  pop rdi\n");
      printf("  pop rax\n");
      printf("  imul rax, rdi\n");
      printf("  push rax\n");
      return;
    case ast_DIV:
      gen(node->children[0]);
      gen(node->children[1]);
      printf("  pop rdi\n");
      printf("  pop rax\n");
      printf("  cqo\n");
      printf("  idiv rdi\n");
      printf("  push rax\n");
      return;
    case ast_NUM:
      printf("  push %d\n", node->value);
      return;
    case ast_RETURN:
      gen(node->children[0]);
      printf("  pop rax\n");
      printf("  leave\n");
      printf("  ret\n");
      return;
    case ast_PRINT:
      // printNumber(int)
      gen(node->children[0]);
      printf("  pop rdi\n");
      printf("  call printNumber\n");
      return;
    case ast_SCAN:
      // scanNumber(int *)
      printf("  mov rax, rbp\n");
      printf("  sub rax, %d\n", (node->children[0]->name[0] - 'a' + 1) * 8);
      printf("  push rax\n");
      printf("  pop rdi\n");
      printf("  call scanNumber\n");
      return;
    }
}

/* 釋放 AST 節點內存 */
void free_ast(struct ast *node) {
// 遍歷並釋放子節點
    for (int i = 0; i < node->num_children; i++) {
        free_ast(node->children[i]);
    }
    free(node->children);
    free(node);
}