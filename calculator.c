#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "calculator.h"

calculator_data calc_data;

ast *new_ast(int nodetype, ast *l, ast *r) {
    ast *a = malloc(sizeof(ast));
    if (!a) {
        fprintf(stderr, "%d: error: malloc failed", calc_data.current_line);
        exit(0);
    }
    a->nodetype = nodetype;
    a->l = l;
    a->r = r;
    return a;
}

ast *new_num(double d) {
    numval *a = malloc(sizeof(numval));
    if (!a) {
        fprintf(stderr, "%d: error: malloc failed", calc_data.current_line);
        exit(0);
    }
    a->nodetype = 'N';
    a->number = d;
    return (ast *)a;
}

double eval(ast *a) {
    double v;
    switch (a->nodetype) {
    case 'N': // Число
        v = ((numval *)a)->number;
        break;
    case '+': // Додавання
        v = eval(a->l) + eval(a->r);
        break;
    case '-': // Віднімання
        v = eval(a->l) - eval(a->r);
        break;
    case '*': // Множення
        v = eval(a->l) * eval(a->r);
        break;
    case '/': // Ділення
        if (eval(a->r) == 0) {
            fprintf(stderr, "Error: division by zero\n");
            exit(EXIT_FAILURE);
        }
        v = eval(a->l) / eval(a->r);
        break;
    case '^': // Степінь
        v = pow(eval(a->l), eval(a->r));
        break;
    case '%': // Остача від ділення
        if (eval(a->r) == 0) {
            fprintf(stderr, "Error: modulo by zero\n");
            exit(EXIT_FAILURE);
        }
        v = fmod(eval(a->l), eval(a->r));
        break;
    case '|': // Модуль
        v = eval(a->l);
        if (v < 0) v = -v;
        break;
    case 'M': // Унарний мінус
        v = -eval(a->l);
        break;
    default:
        fprintf(stderr, "internal error: bad node %c\n", a->nodetype);
        exit(EXIT_FAILURE);
    }
    return v;
}

void free_tree(ast *a) {
    switch (a->nodetype) {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '%':
        free_tree(a->r);
    case '|':
    case 'M':
        free_tree(a->l);
    case 'N':
        free(a);
        break;
    default:
        printf("internal error: free bad node %c\n", a->nodetype);
    }
}

void report_error(const char *msg) {
    fprintf(stderr, "Error: %s\n", msg);
    exit(EXIT_FAILURE);
}

void print_ast_json(ast *a, FILE *out) {
    if (!a) return;

    fprintf(out, "{\n");
    fprintf(out, "  \"type\": \"%c\",\n", a->nodetype);

    if (a->nodetype == 'N') {
        numval *n = (numval *)a;
        fprintf(out, "  \"value\": %f\n", n->number);
    } else {
        fprintf(out, "  \"left\": ");
        if (a->l) {
            print_ast_json(a->l, out);
        } else {
            fprintf(out, "null");
        }
        fprintf(out, ",\n  \"right\": ");
        if (a->r) {
            print_ast_json(a->r, out);
        } else {
            fprintf(out, "null");
        }
        fprintf(out, "\n");
    }

    fprintf(out, "}");
}

void generate_json(ast *a, const char *filename) {
    FILE *out = fopen(filename, "w");
    if (!out) {
        perror("fopen");
        return;
    }

    print_ast_json(a, out);
    fprintf(out, "\n");

    fclose(out);
}

void print_ast_xml(ast *a, FILE *out) {
    if (!a) return;

    fprintf(out, "<node type=\"%c\"", a->nodetype);

    if (a->nodetype == 'N') {
        numval *n = (numval *)a;
        fprintf(out, " value=\"%f\" />\n", n->number);
    } else {
        fprintf(out, ">\n");

        if (a->l) {
            fprintf(out, "  <left>\n");
            print_ast_xml(a->l, out);
            fprintf(out, "  </left>\n");
        } else {
            fprintf(out, "  <left>null</left>\n");
        }

        if (a->r) {
            fprintf(out, "  <right>\n");
            print_ast_xml(a->r, out);
            fprintf(out, "  </right>\n");
        } else {
            fprintf(out, "  <right>null</right>\n");
        }

        // Закриваємо вузол
        fprintf(out, "</node>\n");
    }
}

void generate_xml(ast *a, const char *filename) {
    FILE *out = fopen(filename, "w");
    if (!out) {
        perror("fopen");
        return;
    }

    fprintf(out, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(out, "<ast>\n");

    print_ast_xml(a, out);

    fprintf(out, "</ast>\n");

    fclose(out);
}