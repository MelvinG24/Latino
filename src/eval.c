#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "latino.h"

/* tabla de simbolos */
static long
symhash(char *sym)
{
    long hash = 0;
    int len = strlen(sym);
    while (len--) hash = (hash << 5) - hash + *sym++;
    return abs(hash);
}

static char *
strdup0(const char *s)
{
    size_t len = strlen(s);
    char *p;
    p = (char *)malloc(len + 1);
    if (p) {
        strncpy(p, s, len);
    }
    p[len] = '\0';
    return p;
}

struct symbol *
lookup(char *sym, lat_value *v)
{
    symbol *sp = &symtab[symhash(sym) % NHASH];
    int scount        = NHASH; /* how many have we looked at */
    while (--scount >= 0) {
        if (sp->name && !strcmp(sp->name, sym)) {
            if (v != NULL) {
                /* set value to symbol if exists */
                sp->value = v;
            }
            return sp;
        }
        if (!sp->name) { /* new entry*/
            sp->name  = strdup0(sym);
            sp->value = NULL;
            sp->func  = NULL;
            sp->syms  = NULL;
            return sp;
        }
        if (++sp >= symtab + NHASH)
            sp = symtab; /* try the next entry */
    }
    yyerror("desbordamiento de la tabla de simbolos\n");
    abort();    /* tried them all, table is full */
}

ast *
newast(node_type nodetype, ast *l, ast *r)
{
    ast *a = malloc(sizeof(ast));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

ast *
newint(long i)
{
    node *a = malloc(sizeof(node));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = NODE_INT;
    lat_value *val = malloc(sizeof(lat_value));
    val->t = VALUE_INT;
    val->v.i = i;
    a->value = val;
    return (ast *)a;
}

ast *
newnum(double d)
{
    node *a = malloc(sizeof(node));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = NODE_DECIMAL;
    lat_value *val = malloc(sizeof(lat_value));
    val->t = VALUE_DOUBLE;
    val->v.d = d;
    a->value = val;
    return (ast *)a;
}

ast *
newbool(char *b)
{
    node *a = malloc(sizeof(node));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = NODE_BOOLEAN;
    lat_value *val = malloc(sizeof(lat_value));
    val->t = VALUE_BOOL;
    char *t = "verdadero";
    int ret = strncmp(t, b, 5);
    if (ret == 0) {
        val->v.b = 1;
    } else {
        val->v.b = 0;
    }
    a->value = val;
    return (ast *)a;
}

static unsigned char *
strndup0(const unsigned char *s, size_t n)
{
    size_t i;
    const unsigned char *p = s;
    unsigned char *new;
    for (i = 0; i < n && *p; i++, p++)
        ;
    new = (unsigned char *)malloc(i + 1);
    if (new) {
        memcpy(new, s, i);
        new[i] = '\0';
    }
    return new;
}

ast *
newchar(lat_string *c, size_t l)
{
    node *a = malloc(sizeof(node));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = NODE_CHAR;
    lat_value *val = malloc(sizeof(lat_value));
    val->t = VALUE_CHAR;
    val->v.c = c[0];
    a->value = val;
    return (ast *)a;
}

ast *
newstr(lat_string *s, size_t l)
{
    node *a = malloc(sizeof(node));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = NODE_STRING;
    lat_value *val = malloc(sizeof(lat_value));
    val->t = VALUE_STRING;
    val->v.s = strndup0(s, l);
    a->value = val;
    return (ast *)a;
}

ast *
newfunc(int functype, ast *l)
{
    fncall *a = malloc(sizeof(fncall));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = NODE_BUILTIN_FUNCTION;
    a->l = l;
    a->functype = functype;
    return (ast *)a;
}

ast *
newcall(struct symbol *s, ast *l)
{
    ufncall *a = malloc(sizeof(ufncall));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = NODE_USER_FUNCTION;
    a->l = l;
    a->s = s;
    return (ast *)a;
}

ast *
newref(struct symbol *s)
{
    symref *a = malloc(sizeof(symref));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = NODE_SYMBOL;
    a->s = lookup(s->name, NULL);
    return (ast *)a;
}

ast *
newasgn(struct symbol *s, ast *v)
{
    symasgn *a = malloc(sizeof(symasgn));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = NODE_ASSIGMENT;
    a->s = s;
    a->v = v;
    return (ast *)a;
}

ast *
newflow(node_type nodetype, ast *cond, ast *tl, ast *el)
{
    flow *a = malloc(sizeof(flow));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = nodetype;
    a->cond = cond;
    a->tl = tl;
    a->el = el;
    return (ast *)a;
}

ast *
newfor(node_type nodetype, ast *begin, ast *end, ast *stmts, ast *step)
{
    node_for *a = malloc(sizeof(node_for));
    if (!a) {
        yyerror("sin espacio\n");
        exit(0);
    }
    a->nodetype = nodetype;
    a->begin = begin;
    a->end = end;
    a->stmts = stmts;
    a->step = step;
    return (ast *)a;
}

/* free a tree of ASTs */
void
treefree(ast *a)
{

    switch (a->nodetype) {
    /* two subtrees */
    case NODE_ADD:
    case NODE_SUB:
    case NODE_MULT:
    case NODE_DIV:
    case NODE_MOD:
    case NODE_AND:
    case NODE_OR:
    case NODE_EQ:
    case NODE_NEQ:
    case NODE_GT:
    case NODE_LT:
    case NODE_GE:
    case NODE_LE:
    case NODE_BLOCK:
        if(a->r)
            treefree(a->r);
        if(a->l)
            treefree(a->l);
        break;
    /* one subtree */
    case NODE_NEG:
    case NODE_UNARY_MINUS:
    case NODE_USER_FUNCTION:
    case NODE_LIST_SYMBOLS:
    case NODE_RETURN:
    case NODE_BUILTIN_FUNCTION:
        if(a->l)
            treefree(a->l);
        break;
    /* no subtree */
    case NODE_INT:
    case NODE_CHAR:
    case NODE_DECIMAL:
    case NODE_SYMBOL:
    case NODE_BOOLEAN:
        break;
    case NODE_ASSIGMENT:
        if(((symasgn *)a)->v)
            treefree(((symasgn *)a)->v);
        free(((symasgn *)a)->s);
        break;
    /* up to three subtrees */
    case NODE_IF:
    case NODE_DO:
    case NODE_WHILE:
    case NODE_SWITCH:
    case NODE_CASES:
    case NODE_CASE:
    case NODE_DEFAULT:
        if (((flow *)a)->el)
            treefree(((flow *)a)->el);
        if (((flow *)a)->tl)
            treefree(((flow *)a)->tl);
        if (((flow *)a)->cond)
            treefree(((flow *)a)->cond);
        break;
    case NODE_FROM:
        if (((node_for *)a)->stmts)
            treefree(((node_for *)a)->stmts);
        if (((node_for *)a)->step)
            treefree(((node_for *)a)->step);
        free(((node_for *)a)->begin);
        free(((node_for *)a)->end);
        break;
    case NODE_STRING:
        free(((node *)a)->value->v.s);
        break;
    default:
        printf("error interno: nodo mal liberado %i\n", a->nodetype);
        break;
    }
    free(a); /* always free the node itself */
}

symlist *
newsymlist(struct symbol *sym, symlist *next)
{
    symlist *sl = malloc(sizeof(symlist));
    if (!sl) {
        yyerror("sin espacio\n");
        exit(0);
    }
    sl->sym = sym;
    sl->next = next;
    return sl;
}

/* free a list of symbols */
void
symlistfree(symlist *sl)
{
    symlist *nsl;
    while (sl) {
        nsl = sl->next;
        free(sl);
        sl = nsl;
    }
}

static lat_string *
concat(lat_string *s1, lat_string *s2)
{
    lat_string *s3 = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(s3, s1);
    strcat(s3, s2);
    return s3;
}

static lat_string *
int2str(long i)
{
    char s[255];
    lat_string *r = malloc(strlen(s) + 1);
    snprintf(s, 255, "%ld", i);
    strcpy(r, s);
    return r;
}

static lat_string *
double2str(double d)
{
    char s[64];
    lat_string *r = malloc(strlen(s) + 1);
    snprintf(s, 64, "%g", (float)d);
    strcpy(r, s);
    return r;
}

static lat_string *
char2str(char c)
{
    char s[2];
    lat_string *r = malloc(2);
    snprintf(s, 2, "%c", c);
    strcpy(r, s);
    return r;
}

static lat_string *
bool2str(int i)
{
    char s[10];
    lat_string *r = malloc(11);
    if (i) {
        snprintf(s, 10, "%s", "verdadero");
        strcpy(r, s);
    } else {
        snprintf(s, 10, "%s", "falso");
        strcpy(r, s);
    }
    return r;
}

lat_value *
eval_node_add(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    switch (left->t) {
    case VALUE_BOOL:
        if (right->t == VALUE_STRING) {
            result->t = VALUE_STRING;
            result->v.s = concat(bool2str(left->v.b), right->v.s);
            return result;
        }
        break;
    case VALUE_INT:
        if (right->t == VALUE_INT) {
            result->t = VALUE_INT;
            result->v.i  = left->v.i + right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->t = VALUE_DOUBLE;
            result->v.d  = left->v.i + right->v.d;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->t = VALUE_CHAR;
            result->v.c  = left->v.i + right->v.c;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->t   = VALUE_STRING;
            result->v.s = concat(int2str(left->v.i), right->v.s);
            return result;
        }
        break;
    case VALUE_DOUBLE:
        if (right->t == VALUE_INT) {
            result->t = VALUE_DOUBLE;
            result->v.d  = left->v.d + right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->t = VALUE_DOUBLE;
            result->v.d  = left->v.d + right->v.d;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->t   = VALUE_STRING;
            result->v.s = concat(int2str(left->v.d), right->v.s);
            return result;
        }
        break;
    case VALUE_CHAR:
        if (right->t == VALUE_INT) {
            result->t = VALUE_CHAR;
            result->v.c  = left->v.c + right->v.i;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->t = VALUE_CHAR;
            result->v.c  = left->v.c + right->v.c;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->t   = VALUE_STRING;
            result->v.s = concat(char2str(left->v.c), right->v.s);
            return result;
        }
        break;
    case VALUE_STRING:
        if (right->t == VALUE_BOOL) {
            result->t = VALUE_STRING;
            result->v.s = concat(left->v.s, bool2str(right->v.b));
            return result;
        }
        if (right->t == VALUE_INT) {
            result->t = VALUE_STRING;
            result->v.s  = concat(left->v.s, int2str(right->v.i));
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->t = VALUE_STRING;
            result->v.s  = concat(left->v.s, double2str(right->v.d));
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->t = VALUE_STRING;
            result->v.s  = concat(left->v.s, char2str(right->v.c));
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->t   = VALUE_STRING;
            result->v.s = concat(left->v.s, right->v.s);
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("+ tipos incompatibles");
    return result;
}

lat_value *
eval_node_sub(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    switch (left->t) {
    case VALUE_INT:
        if (right->t == VALUE_INT) {
            result->t = VALUE_INT;
            result->v.i  = left->v.i - right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->t = VALUE_DOUBLE;
            result->v.d  = left->v.i - right->v.d;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->t = VALUE_CHAR;
            result->v.c  = left->v.i - right->v.c;
            return result;
        }
        break;
    case VALUE_DOUBLE:
        if (right->t == VALUE_INT) {
            result->t = VALUE_DOUBLE;
            result->v.d  = left->v.d - right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->t = VALUE_DOUBLE;
            result->v.d  = left->v.d - right->v.d;
            return result;
        }
        break;
    case VALUE_CHAR:
        if (right->t == VALUE_INT) {
            result->t = VALUE_CHAR;
            result->v.c  = left->v.c - right->v.i;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->t = VALUE_CHAR;
            result->v.c  = left->v.c - right->v.c;
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("- tipos incompatibles");
    return result;
}

lat_value *
eval_node_mult(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    switch (left->t) {
    case VALUE_INT:
        if (right->t == VALUE_INT) {
            result->t = VALUE_INT;
            result->v.i  = left->v.i * right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->t = VALUE_DOUBLE;
            result->v.d  = left->v.i * right->v.d;
            return result;
        }
        break;
    case VALUE_DOUBLE:
        if (right->t == VALUE_INT) {
            result->t = VALUE_DOUBLE;
            result->v.d  = left->v.d * right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->t = VALUE_DOUBLE;
            result->v.d  = left->v.d * right->v.d;
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("* tipos incompatibles");
    return result;
}

lat_value *
eval_node_div(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    switch (left->t) {
    case VALUE_INT:
        if (right->t == VALUE_INT) {
            result->t = VALUE_INT;
            if (right->v.i == 0) {
                yyerror("/ division por cero");
            } else {
                result->v.i  = left->v.i / right->v.i;
            }
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->t = VALUE_DOUBLE;
            if (right->v.d == 0) {
                yyerror("/ division por cero");
            } else {
                result->v.d  = left->v.i / right->v.d;
            }
            return result;
        }
        break;
    case VALUE_DOUBLE:
        if (right->t == VALUE_INT) {
            result->t = VALUE_DOUBLE;
            if (right->v.i == 0) {
                yyerror("/ division por cero");
            } else {
                result->v.d  = left->v.d / right->v.i;
            }
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->t = VALUE_DOUBLE;
            if (right->v.d == 0) {
                yyerror("/ division por cero");
            } else {
                result->v.d  = left->v.d / right->v.d;
            }
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("/ tipos incompatibles");
    return result;
}

lat_value *
eval_node_mod(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    switch (left->t) {
    case VALUE_INT:
        if (right->t == VALUE_INT) {
            result->t = VALUE_INT;
            if (right->v.i == 0) {
                yyerror("\% division por cero");
            } else {
                result->v.i  = left->v.i % right->v.i;
            }
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("\% tipos incompatibles");
    return result;
}

lat_value *
eval_node_unary_minus(lat_value *left)
{
    lat_value *result = malloc(sizeof(lat_value));
    if (left->t == VALUE_INT) {
        result->t = VALUE_INT;
        result->v.i = - (left->v.i);
        return result;
    }
    if (left->t == VALUE_DOUBLE) {
        result->t = VALUE_DOUBLE;
        result->v.d  = - (left->v.d);
        return result;
    }
    yyerror("- tipos incompatible");
    return result;
}

lat_value *
eval_node_and(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    if (left->t == VALUE_BOOL && right->t == VALUE_BOOL) {
        result->t = VALUE_BOOL;
        result->v.b = left->v.b && right->v.b;
        return result;
    }
    yyerror("&& tipos incompatible");
    return result;
}

lat_value *
eval_node_or(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    if (left->t == VALUE_BOOL && right->t == VALUE_BOOL) {
        result->t = VALUE_BOOL;
        result->v.b = left->v.b || right->v.b;
        return result;
    }
    yyerror("|| tipos incompatible");
    return result;
}

lat_value *
eval_node_neg(lat_value *left)
{
    lat_value *result = malloc(sizeof(lat_value));
    if (left->t == VALUE_BOOL) {
        result->t = VALUE_BOOL;
        result->v.b = !(left->v.b);
        return result;
    }
    yyerror("! tipos incompatible");
    return result;
}

lat_value *
eval_node_gt(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    result->t = VALUE_BOOL;
    switch (left->t) {
    case VALUE_CHAR:
        if (right->t == VALUE_INT) {
            result->v.b = left->v.c > right->v.i;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(char2str(left->v.c), right->v.s) > 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_INT:
        if (right->t == VALUE_CHAR) {
            result->v.b = left->v.i > right->v.c;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b = left->v.i > right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b = left->v.i > right->v.d;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(int2str(left->v.i), right->v.s) > 0 ?  1 : 0;
            return result;
        }
        break;
    case VALUE_DOUBLE :
        if (right->t == VALUE_INT) {
            result->v.b = left->v.i > right->v.d;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b = left->v.d > right->v.d;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(double2str(left->v.d), right->v.s) > 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_STRING:
        if (right->t == VALUE_INT) {
            result->v.b  = strcmp(left->v.s, int2str(right->v.i)) > 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b  = strcmp(left->v.s, double2str(right->v.d)) > 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->v.b  = strcmp(left->v.s, char2str(right->v.c)) > 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(left->v.s, right->v.s) > 0 ? 1 : 0;
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("> tipos incompatibles");
    return result;
}

lat_value *
eval_node_lt(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    result->t = VALUE_BOOL;
    switch (left->t) {
    case VALUE_CHAR:
        if (right->t == VALUE_INT) {
            result->v.b = left->v.c < right->v.i;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(char2str(left->v.c), right->v.s) < 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_INT:
        if (right->t == VALUE_CHAR) {
            result->v.b = left->v.i < right->v.c;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b = left->v.i < right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b = left->v.i < right->v.d;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(int2str(left->v.i), right->v.s) < 0 ?  1 : 0;
            return result;
        }
        break;
    case VALUE_DOUBLE :
        if (right->t == VALUE_INT) {
            result->v.b = left->v.i < right->v.d;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b = left->v.d < right->v.d;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(double2str(left->v.d), right->v.s) < 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_STRING:
        if (right->t == VALUE_INT) {
            result->v.b  = strcmp(left->v.s, int2str(right->v.i)) < 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b  = strcmp(left->v.s, double2str(right->v.d)) < 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->v.b  = strcmp(left->v.s, char2str(right->v.c)) < 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(left->v.s, right->v.s) < 0 ? 1 : 0;
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("> tipos incompatibles");
    return result;
}

lat_value *
eval_node_neq(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    result->t = VALUE_BOOL;
    switch (left->t) {
    case VALUE_BOOL:
        if (right->t == VALUE_BOOL) {
            result->v.b = left->v.b != right->v.b;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b = left->v.b != right->v.i;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(bool2str(left->v.b), right->v.s) != 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_CHAR:
        if (right->t == VALUE_CHAR) {
            result->v.b = left->v.c == right->v.c;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b = left->v.c != right->v.i;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(char2str(left->v.c), right->v.s) != 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_INT:
        if (right->t == VALUE_BOOL) {
            result->v.b = left->v.i != right->v.b;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->v.b = left->v.i != right->v.c;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b = left->v.i != right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b = left->v.i != right->v.d;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(int2str(left->v.i), right->v.s) != 0 ?  1 : 0;
            return result;
        }
        break;
    case VALUE_DOUBLE:
        if (right->t == VALUE_INT) {
            result->v.b = left->v.i != right->v.d;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b = left->v.d != right->v.d;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(double2str(left->v.d), right->v.s) != 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_STRING:
        if (right->t == VALUE_BOOL) {
            result->v.b = strcmp(left->v.s, bool2str(right->v.b)) != 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b  = strcmp(left->v.s, int2str(right->v.i)) != 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b  = strcmp(left->v.s, double2str(right->v.d)) != 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->v.b  = strcmp(left->v.s, char2str(right->v.c)) != 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(left->v.s, right->v.s) != 0 ? 1 : 0;
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("> tipos incompatibles");
    return result;
}

lat_value *
eval_node_eq(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    result->t = VALUE_BOOL;
    switch (left->t) {
    case VALUE_BOOL:
        if (right->t == VALUE_BOOL) {
            result->v.b = left->v.b == right->v.b;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b = left->v.b == right->v.i;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(bool2str(left->v.b), right->v.s) == 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_CHAR:
        if (right->t == VALUE_CHAR) {
            result->v.b = left->v.c == right->v.c;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b = left->v.c == right->v.i;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(char2str(left->v.c), right->v.s) == 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_INT:
        if (right->t == VALUE_BOOL) {
            result->v.b = left->v.i == right->v.b;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->v.b = left->v.i == right->v.c;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b = left->v.i == right->v.i;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b = left->v.i == right->v.d;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(int2str(left->v.i), right->v.s) == 0 ?  1 : 0;
            return result;
        }
        break;
    case VALUE_DOUBLE :
        if (right->t == VALUE_INT) {
            result->v.b = left->v.i == right->v.d;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b = left->v.d == right->v.d;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(double2str(left->v.d), right->v.s) == 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_STRING:
        if (right->t == VALUE_BOOL) {
            result->v.b = strcmp(left->v.s, bool2str(right->v.b)) == 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_INT) {
            result->v.b  = strcmp(left->v.s, int2str(right->v.i)) == 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_DOUBLE) {
            result->v.b  = strcmp(left->v.s, double2str(right->v.d)) == 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_CHAR) {
            result->v.b  = strcmp(left->v.s, char2str(right->v.c)) == 0 ? 1 : 0;
            return result;
        }
        if (right->t == VALUE_STRING) {
            result->v.b = strcmp(left->v.s, right->v.s) == 0 ? 1 : 0;
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("> tipos incompatibles");
    return result;
}

lat_value *
eval_node_ge(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    result->t = VALUE_BOOL;
    switch (left->t) {
    case VALUE_CHAR:
        if (right->t >= VALUE_INT) {
            result->v.b = left->v.c >= right->v.i;
            return result;
        }
        if (right->t >= VALUE_STRING) {
            result->v.b = strcmp(char2str(left->v.c), right->v.s) >= 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_INT:
        if (right->t >= VALUE_CHAR) {
            result->v.b = left->v.i >= right->v.c;
            return result;
        }
        if (right->t >= VALUE_INT) {
            result->v.b = left->v.i >= right->v.i;
            return result;
        }
        if (right->t >= VALUE_DOUBLE) {
            result->v.b = left->v.i >= right->v.d;
            return result;
        }
        if (right->t >= VALUE_STRING) {
            result->v.b = strcmp(int2str(left->v.i), right->v.s) >= 0 ?  1 : 0;
            return result;
        }
        break;
    case VALUE_DOUBLE :
        if (right->t >= VALUE_INT) {
            result->v.b = left->v.i >= right->v.d;
            return result;
        }
        if (right->t >= VALUE_DOUBLE) {
            result->v.b = left->v.d >= right->v.d;
            return result;
        }
        if (right->t >= VALUE_STRING) {
            result->v.b = strcmp(double2str(left->v.d), right->v.s) >= 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_STRING:
        if (right->t >= VALUE_INT) {
            result->v.b  = strcmp(left->v.s, int2str(right->v.i)) >= 0 ? 1 : 0;
            return result;
        }
        if (right->t >= VALUE_DOUBLE) {
            result->v.b  = strcmp(left->v.s, double2str(right->v.d)) >= 0 ? 1 : 0;
            return result;
        }
        if (right->t >= VALUE_CHAR) {
            result->v.b  = strcmp(left->v.s, char2str(right->v.c)) >= 0 ? 1 : 0;
            return result;
        }
        if (right->t >= VALUE_STRING) {
            result->v.b = strcmp(left->v.s, right->v.s) >= 0 ? 1 : 0;
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("> tipos incompatibles");
    return result;
}

lat_value *
eval_node_le(lat_value *left, lat_value *right)
{
    lat_value *result = malloc(sizeof(lat_value));
    result->t = VALUE_BOOL;
    switch (left->t) {
    case VALUE_CHAR:
        if (right->t <= VALUE_INT) {
            result->v.b = left->v.c <= right->v.i;
            return result;
        }
        if (right->t <= VALUE_STRING) {
            result->v.b = strcmp(char2str(left->v.c), right->v.s) <= 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_INT:
        if (right->t <= VALUE_CHAR) {
            result->v.b = left->v.i <= right->v.c;
            return result;
        }
        if (right->t <= VALUE_INT) {
            result->v.b = left->v.i <= right->v.i;
            return result;
        }
        if (right->t <= VALUE_DOUBLE) {
            result->v.b = left->v.i <= right->v.d;
            return result;
        }
        if (right->t <= VALUE_STRING) {
            result->v.b = strcmp(int2str(left->v.i), right->v.s) <= 0 ?  1 : 0;
            return result;
        }
        break;
    case VALUE_DOUBLE :
        if (right->t <= VALUE_INT) {
            result->v.b = left->v.i <= right->v.d;
            return result;
        }
        if (right->t <= VALUE_DOUBLE) {
            result->v.b = left->v.d <= right->v.d;
            return result;
        }
        if (right->t <= VALUE_STRING) {
            result->v.b = strcmp(double2str(left->v.d), right->v.s) <= 0 ? 1 : 0;
            return result;
        }
        break;
    case VALUE_STRING:
        if (right->t <= VALUE_INT) {
            result->v.b  = strcmp(left->v.s, int2str(right->v.i)) <= 0 ? 1 : 0;
            return result;
        }
        if (right->t <= VALUE_DOUBLE) {
            result->v.b  = strcmp(left->v.s, double2str(right->v.d)) <= 0 ? 1 : 0;
            return result;
        }
        if (right->t <= VALUE_CHAR) {
            result->v.b  = strcmp(left->v.s, char2str(right->v.c)) <= 0 ? 1 : 0;
            return result;
        }
        if (right->t <= VALUE_STRING) {
            result->v.b = strcmp(left->v.s, right->v.s) <= 0 ? 1 : 0;
            return result;
        }
        break;
    default:
        break;
    }
    yyerror("> tipos incompatibles");
    return result;
}

/* evaluar ast */
lat_value *
eval(ast *a)
{
    double v;
    lat_value *val = malloc(sizeof(lat_value));
    lat_value *val1 = malloc(sizeof(lat_value));
    if (!a) {
        yyerror("eval es nulo\n");
        return val;
    }
    switch (a->nodetype) {
    /* constant */
    case NODE_BOOLEAN:
    case NODE_CHAR:
    case NODE_DECIMAL:
    case NODE_STRING:
    case NODE_INT:
        return ((node *)a)->value;
        break;
    /* name reference */
    case NODE_SYMBOL:
        if (((symref *)a)->s->value == NULL) {
            yyerror("variable sin definir");
        } else {
            return ((symref *)a)->s->value;
        }
        return NULL;
        break;
    /* assignment */
    case NODE_ASSIGMENT:
        ((symasgn *)a)->s->value = eval(((symasgn *)a)->v);
        return ((symasgn *)a)->s->value;
        break;
    case NODE_UNARY_MINUS:
        val = eval_node_unary_minus(eval(a->l));
        return val;
        break;
    /* expressions */
    case NODE_ADD:
        val = eval_node_add(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_SUB:
        val = eval_node_sub(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_MULT:
        val = eval_node_mult(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_DIV:
        val = eval_node_div(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_MOD:
        val = eval_node_mod(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_AND:
        val = eval_node_and(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_OR:
        val = eval_node_or(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_NEG:
        val = eval_node_neg(eval(a->l));
        return val;
        break;
    /* comparisons */
    case NODE_GT:
        val = eval_node_gt(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_LT:
        val = eval_node_lt(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_NEQ:
        val = eval_node_neq(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_EQ:
        val = eval_node_eq(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_GE:
        val = eval_node_ge(eval(a->l), eval(a->r));
        return val;
        break;
    case NODE_LE:
        val = eval_node_le(eval(a->l), eval(a->r));
        return val;
        break;
    /* control flow */
    /* if/then/else */
    /* null expressions allowed in the grammar, so check for them */
    case NODE_IF:
        if ((eval(((flow *)a)->cond))->v.b != 0) {
            if (((flow *)a)->tl != NULL) {
                eval(((flow *)a)->tl);
            }
        } else {
            if (((flow *)a)->el != NULL) {
                eval(((flow *)a)->el);
            }
        }
        return val;
        break;
    /* while */
    case NODE_WHILE:
        while ((eval(((flow *)a)->cond))->v.b != 0) {
            eval(((flow *)a)->tl);
        }
        return val;
        break;
    /* do */
    case NODE_DO:
        do {
            eval(((flow *)a)->tl);
        } while ((eval(((flow *)a)->cond))->v.b != 0);
        return val;
        break;
    case NODE_SWITCH:
        val = eval(((flow *)a)->cond);
        ((flow *)(((flow *)a)->tl))->cond = ((flow *)a)->cond;
        /* evaluar los casos */
        val = eval(((flow *)a)->tl);
        /* si ningun caso se evaluo */
        if (!val->v.b) {
            /* evaluar default si existe */
            if ((((flow *)a)->el)) {
                val = eval(((flow *)a)->el);
            }
        }
        return val;
        break;
    case NODE_CASES:
        val = eval(((flow *)a)->cond);
        /* pasa la condicion a los nodos hijos */
        ((flow *)(((flow *)a)->tl))->cond = ((flow *)a)->cond;
        ((flow *)(((flow *)a)->el))->cond = ((flow *)a)->cond;
        /* evaluar caso */
        val = eval(((flow *)a)->el);
        if (!val->v.b) {
            /* evaluar otros casos */
            val = eval(((flow *)a)->tl);
        }
        return val;
        break;
    case NODE_CASE:
        val = eval(((flow *)a)->cond);
        /* evaluar valor del caso */
        val1 = eval(((flow *)a)->tl);
        /*si el caso es igual a la condicion */
        if ((eval_node_eq(val, val1))->v.b) {
            /* evaluar bloque de codigo */
            eval(((flow *)a)->el);
            val1->v.b = 1;
        } else {
            val1->v.b = 0;
        }
        return val1;
        break;
    case NODE_DEFAULT:
        eval(((flow *)a)->tl);
        break;
    /* list of statements */
    case NODE_FROM:
        if ((eval(((node_for *)a)->begin))->t == VALUE_INT && (eval(((node_for *)a)->end))->t == VALUE_INT) {
            int old_begin = (eval(((node_for *)a)->begin))->v.i;
            int begin = (eval(((node_for *)a)->begin))->v.i;
            int end   = (eval(((node_for *)a)->end))->v.i;
            int step = 1;
            if (((node_for *)a)->step != NULL) {
                step = (eval(((node_for *)a)->step))->v.i;
            }
            if (begin < end) {
                while (begin < end) {
                    eval(((node_for *)a)->stmts);
                    (eval(((node_for *)a)->begin))->v.i += step;
                    begin += step;
                }
            }
            if (begin > end) {
                while (begin > end) {
                    eval(((node_for *)a)->stmts);
                    (eval(((node_for *)a)->begin))->v.i -= step;
                    begin -= step;
                }
            }
            //restore begin value
            (eval(((node_for *)a)->begin))->v.i = old_begin;
        }
        return val;
        break;
    case NODE_BLOCK:
        if (a->l) {
            if (a->l->nodetype == NODE_RETURN) {
                val = eval(a->l);
                return val;
                break;
            }
            eval(a->l);
        }
        if (a->r) {
            val = eval(a->r);
        }
        return val;
        break;
    case NODE_BUILTIN_FUNCTION:
        v = callbuiltin((fncall *)a);
        break;
    case NODE_USER_FUNCTION:
        return calluser((ufncall *)a);
        break;
    case NODE_LIST_SYMBOLS:
        val = eval(a->l);
        //val = eval(a->r);
        return val;
        break;
    case NODE_RETURN:
        if (a->l) {
            val = eval(a->l);
        }
        return val;
        break;
    default:
        printf("nodo incorrecto %i\n", a->nodetype);
        break;
    }
    return val;
}

void
imprimir(lat_value *val)
{
    if (val != NULL) {
        switch (val->t) {
        case VALUE_BOOL:
            printf("%s\n", bool2str(val->v.b));
            break;
        case VALUE_INT:
            printf("%ld\n", val->v.i);
            break;
        case VALUE_CHAR:
            printf("%c\n", val->v.c);
            break;
        case VALUE_DOUBLE:
            printf("%g\n", val->v.d);
            break;
        case VALUE_STRING:
            printf("%s\n", val->v.s);
            break;
        case VALUE_NULL:
            printf("%s\n", "nulo");
            break;
        }
    }
}

double
callbuiltin(fncall *f)
{
    double v = 0;
    switch (f->functype) {
    case B_sqrt:
        return sqrt(v);
    case B_exp:
        return exp(v);
    case B_log:
        return log(v);
    case B_print: {
        lat_value *val;
        val =  eval(f->l);
        imprimir(val);
    }
    return v;
    default:
        yyerror("definicion de funcion desconocida\n");
    }
    return v;
}

/* define a function */
void
dodef(struct symbol *s, symlist *syms, ast *func)
{
    s->syms = syms;
    s->func = func;
}

lat_value *
calluser(ufncall *f)
{
    struct symbol *fn = f->s; /* function name */
    symlist *sl; /* dummy arguments */
    ast *args = f->l; /* actual arguments */
    lat_value **oldval;
    lat_value **newval; /* saved arg values */
    lat_value *v = NULL;
    int nargs;
    int i;
    if (!fn->func) {
        yyerror("llamada a funcion indefinida\n");
        return v;
    }
    /* count the arguments */
    sl = fn->syms;
    for (nargs = 0; sl; sl = sl->next)
        nargs++;
    /* prepare to save them */
    oldval = malloc(nargs * sizeof(lat_value));
    newval = malloc(nargs * sizeof(lat_value));
    if (!oldval || !newval) {
        yyerror("sin espacio en %s", fn->name);
        return v;
    }
    /* evaluate the arguments */
    for (i = 0; i < nargs; i++) {
        if (!args) {
            yyerror("muy pocos argumentos en llamada a funcion");
            free(oldval);
            free(newval);
            return v;
        }
        if (args->nodetype == NODE_LIST_SYMBOLS) { /* if this is a list node */
            newval[i] = eval(args->l);
            args = args->r;
        } else { /* if it's the end of the list */
            newval[i] = eval(args);
            args = NULL;
        }
    }
    /* save old values of dummies, assign new ones */
    sl = fn->syms;
    for (i = 0; i < nargs; i++) {
        struct symbol *s = sl->sym;
        oldval[i] = s->value;
        s->value = newval[i];
        sl = sl->next;
    }
    free(newval);
    /* evaluate the function */
    v = eval(fn->func);
    /* put the real values of the dummies back */
    sl = fn->syms;
    for (i = 0; i < nargs; i++) {
        struct symbol *s = sl->sym;
        s->value = oldval[i];
        sl = sl->next;
    }
    free(oldval);
    return v;
}

