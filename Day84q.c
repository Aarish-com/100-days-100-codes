/ Recursive-descent expression parser -> AST -> evaluator.
// Supports + - * / ^ (right-assoc), unary minus, parentheses, sqrt/sin/cos/log/abs, constants pi & e.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <setjmp.h>
 
typedef enum { NUM, BIN, NEG, CALL } Kind;
typedef struct Node {
    Kind kind; char op; double num; char fn[8];
    struct Node *l, *r, *chain;          // chain: every node is linked so we can free all at once
} Node;
 
static const char *p, *src_start;
static jmp_buf err_jmp;
static char err_msg[96];
static Node *alloc_list;
 
static void fail(const char *msg) { snprintf(err_msg, sizeof err_msg, "%s at column %d", msg, (int)(p - src_start) + 1); longjmp(err_jmp, 1); }
static void skip(void) { while (isspace((unsigned char)*p)) p++; }
static Node *mk(Kind k, Node *l, Node *r) {
    Node *n = calloc(1, sizeof *n); n->kind = k; n->l = l; n->r = r;
    n->chain = alloc_list; alloc_list = n; return n;
}
static void free_all(void) { while (alloc_list) { Node *n = alloc_list->chain; free(alloc_list); alloc_list = n; } }
 
static Node *expr(void);
 
static Node *primary(void) {
    skip();
    if (isdigit((unsigned char)*p) || *p == '.') {
        char *end; Node *n = mk(NUM, NULL, NULL); n->num = strtod(p, &end); p = end; return n;
    }
    if (*p == '(') {
        p++; Node *n = expr(); skip();
        if (*p != ')') fail("expected ')'");
        p++; return n;
    }
    if (isalpha((unsigned char)*p)) {
        char name[8]; int i = 0;
        while (isalpha((unsigned char)*p) && i < 7) name[i++] = *p++;
        name[i] = 0; skip();
        if (!strcmp(name, "pi")) { Node *n = mk(NUM, NULL, NULL); n->num = M_PI; return n; }
        if (!strcmp(name, "e"))  { Node *n = mk(NUM, NULL, NULL); n->num = M_E;  return n; }
        if (*p != '(') fail("unknown identifier");
        p++; Node *arg = expr(); skip();
        if (*p != ')') fail("expected ')'");
        p++; Node *n = mk(CALL, arg, NULL); strcpy(n->fn, name); return n;
    }
    fail("unexpected token"); return NULL;
}
 
static Node *unary(void);
 
static Node *power(void) {                     // primary ^ unary  => right-assoc, and -2^2 == -(2^2)
    Node *base = primary(); skip();
    if (*p == '^') { p++; Node *n = mk(BIN, base, unary()); n->op = '^'; return n; }
    return base;
}
 
static Node *unary(void) {
    skip();
    if (*p == '-') { p++; return mk(NEG, unary(), NULL); }
    if (*p == '+') { p++; return unary(); }
    return power();
}
 
static Node *term(void) {
    Node *n = unary();
    for (skip(); *p == '*' || *p == '/'; skip()) { char op = *p++; n = mk(BIN, n, unary()); n->op = op; }
    return n;
}
 
static Node *expr(void) {
    Node *n = term();
    for (skip(); *p == '+' || *p == '-'; skip()) { char op = *p++; n = mk(BIN, n, term()); n->op = op; }
    return n;
}
 
static double eval(const Node *n) {
    switch (n->kind) {
    case NUM: return n->num;
    case NEG: return -eval(n->l);
    case BIN: {
        double a = eval(n->l), b = eval(n->r);
        switch (n->op) {
        case '+': return a + b;  case '-': return a - b;  case '*': return a * b;
        case '/': if (b == 0) fail("division by zero"); return a / b;
        case '^': return pow(a, b);
        }
    } break;
    case CALL: {
        double a = eval(n->l);
        if (!strcmp(n->fn, "sqrt")) return sqrt(a);
        if (!strcmp(n->fn, "sin"))  return sin(a);
        if (!strcmp(n->fn, "cos"))  return cos(a);
        if (!strcmp(n->fn, "log"))  return log(a);
        if (!strcmp(n->fn, "abs"))  return fabs(a);
        fail("unknown function");
    }
    }
    return 0;
}
 
static void run(const char *src) {
    p = src_start = src; alloc_list = NULL;
    if (setjmp(err_jmp)) { printf("%-28s error: %s\n", src, err_msg); free_all(); return; }
    Node *root = expr(); skip();
    if (*p) fail("trailing characters");
    printf("%-28s = %.10g\n", src, eval(root));
    free_all();
}
 
int main(void) {
    const char *tests[] = { "1 + 2 * 3", "(1 + 2) * 3", "2 ^ 3 ^ 2", "-2 ^ 2", "sqrt(16) + abs(-3.5)",
                            "sin(pi/2) + cos(0)", "10 / (5 - 5)", "2 * (3 + ", "foo(2)" };
    for (size_t i = 0; i < sizeof tests / sizeof *tests; i++) run(tests[i]);
    return 0;
}
 
