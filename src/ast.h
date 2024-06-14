/* node in the abstract syntax tree */
#ifndef ast_h
#define ast_h
struct ast {
    enum {
        ast_PROGRAM,
        ast_VAR_DECL,
        ast_VAR_REF,
        ast_ASSIGN,
        ast_ADD,
        ast_SUB,
        ast_MUL,
        ast_DIV,
        ast_NUM,
        ast_RETURN,
        ast_PRINT,
        ast_SCAN
    } type;
    struct ast **children;
    int num_children;
    int value;
    char *name;
};
/* build an AST node */

struct ast *expr_ast(int type, struct ast *, struct ast *);

struct ast *num_ast(int value);

struct ast *var_ast(char *name);

struct ast *decl_ast(char *name);

struct ast *assign_ast(struct ast *var,struct ast *expr);

struct ast *ret_ast(struct ast *expr);

struct ast *print_ast(struct ast *expr);

struct ast *scan_ast(struct ast *var);

struct ast *program(struct ast **stmts, int len);

/* evaluate an AST */
// int indent = 0;
void printIndent();
void incrementIndent();
void decrementIndent();
void dump_ast(struct ast *);
void gen(struct ast *);
/* delete and free an AST */
void free_ast(struct ast *);

#endif