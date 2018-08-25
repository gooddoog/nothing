#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "script/expr.h"

static char *string_duplicate(const char *str,
                              const char *str_end)
{
    if (str_end != NULL && str > str_end) {
        return NULL;
    }

    const size_t n = str_end == NULL ? strlen(str) : (size_t) (str_end - str);
    char *dup_str = malloc(sizeof(char) * (n + 1));
    if (dup_str == NULL) {
        return NULL;
    }

    memcpy(dup_str, str, n);
    return dup_str;
}

struct Expr atom_as_expr(struct Atom *atom)
{
    struct Expr expr = {
        .type = EXPR_ATOM,
        .atom = atom
    };

    return expr;
}

struct Expr cons_as_expr(struct Cons *cons)
{
    struct Expr expr = {
        .type = EXPR_CONS,
        .cons = cons
    };

    return expr;
}

void print_atom_as_sexpr(struct Atom *atom)
{
    assert(atom);

    switch (atom->type) {
    case ATOM_SYMBOL:
        printf("%s", atom->sym);
        break;

    case ATOM_NUMBER:
        printf("%f", atom->num);
        break;

    case ATOM_STRING:
        printf("\"%s\"", atom->str);
        break;
    }
}

void print_cons_as_sexpr(struct Cons *cons)
{
    assert(cons);

    printf("(");
    print_expr_as_sexpr(cons->car);
    printf(" . ");
    print_expr_as_sexpr(cons->cdr);
    printf(")");
}

void print_expr_as_sexpr(struct Expr expr)
{
    switch (expr.type) {
    case EXPR_ATOM:
        print_atom_as_sexpr(expr.atom);
        break;

    case EXPR_CONS:
        print_cons_as_sexpr(expr.cons);
        break;
    }
}

void destroy_expr(struct Expr expr)
{
    switch (expr.type) {
    case EXPR_ATOM:
        destroy_atom(expr.atom);
        break;

    case EXPR_CONS:
        destroy_cons(expr.cons);
        break;
    }
}

struct Cons *create_cons(struct Expr car, struct Expr cdr)
{
    struct Cons *cons = malloc(sizeof(struct Cons));
    if (cons == NULL) {
        return NULL;
    }

    cons->car = car;
    cons->cdr = cdr;

    return cons;
}

void destroy_cons(struct Cons *cons)
{
    destroy_expr(cons->car);
    destroy_expr(cons->cdr);
    free(cons);
}

struct Atom *create_number_atom(float num)
{
    struct Atom *atom = malloc(sizeof(struct Atom));
    if (atom == NULL) {
        return NULL;
    }
    atom->type = ATOM_NUMBER;
    atom->num = num;
    return atom;
}

struct Atom *create_string_atom(const char *str, const char *str_end)
{
    struct Atom *atom = malloc(sizeof(struct Atom));
    if (atom == NULL) {
        return NULL;
    }
    atom->type = ATOM_STRING;
    atom->str = string_duplicate(str, str_end);
    if (atom->str == NULL) {
        free(atom);
        return NULL;
    }

    return atom;
}

struct Atom *create_symbol_atom(const char *sym, const char *sym_end)
{
    struct Atom *atom = malloc(sizeof(struct Atom));
    if (atom == NULL) {
        return NULL;
    }
    atom->type = ATOM_SYMBOL;
    atom->sym = string_duplicate(sym, sym_end);
    if (atom->sym == NULL) {
        free(atom);
        return NULL;
    }

    return atom;
}

void destroy_atom(struct Atom *atom)
{
    switch (atom->type) {
    case ATOM_SYMBOL:
    case ATOM_STRING: {
        free(atom->str);
    } break;

    case ATOM_NUMBER: {
        /* Nothing */
    } break;
    }

    free(atom);
}
