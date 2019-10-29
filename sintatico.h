#ifndef __SIN__
#define __SIN__

#include "lexico.h"
void next_token();

int erro();

int reconhece_tipo();

int reconhece_arg();

int reconhece_listaArgs();

int reconhece_funcao();

int reconhece_lista_identificadores();

int reconhece_declaracao();

int comparacao();

int operador();

int operador_duplo();

int operador_composto();

int expressao_02();

int literal();

int fator();

int termo_linha();

int termo();

int magnitude_linha();

int magnitude();

int valor_r_linha();

int valor_r();

int expressao();

int lista_statement_linha();

int lista_statement();

int expressao_opcional();

int statement_escopo();

int statement_return();

int statement();

int statement_for();

int statement_while();

int statement_do_while();

int statement_else();

int statement_if();

int switch_args();

int lista_case_linha();

int lista_case();

int statement_switch();

#endif //__SIN__
