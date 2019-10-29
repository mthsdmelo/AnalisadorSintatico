#include "sintatico.h"


Token token;
FILE *arq;
FILE *arq_erro;
int n_erros;

void next_token(){
    fread(&token,sizeof(Token),1,arq);
    printf("%d\n",token.id);
}

int erro(){
  fprintf(arq_erro,"Erro Linha: %d, Coluna %d\n",token.linha,token.coluna);
  n_erros++;
  return 0;
}

int reconhece_tipo(){
  return(token.id==5||token.id==9||token.id==11||token.id==14||token.id==23);
}

int reconhece_arg(){
  if(reconhece_tipo()){
    next_token();
    if(token.id == ID){
        return 1;
    }
  }
  return 0;
}

int reconhece_listaArgs(){
  if(reconhece_arg()){
    next_token();
  }
  if(token.id==VIRGULA){
    next_token();
    if(reconhece_listaArgs())
      return 1;
  }
  return 1;
}

int reconhece_funcao(){
  if(reconhece_tipo()){
    next_token();
    if(token.id==ID){
      next_token();
      if(token.id==O_PARENTESES){
        next_token();
        if(reconhece_listaArgs()){
          next_token();
          if(token.id==C_PARENTESES){
            next_token();
            if(statement_escopo()){
              return 1;
            }
          }
        }
      }
    }
  }
  return 0;
}

int reconhece_lista_identificadores(){
  if(token.id==ID){
    next_token();
    if(token.id==VIRGULA){
      next_token();
      if(reconhece_lista_identificadores()){
        return 1;
      }
      else return 0;
    }
    else return 1;
  }
  return 0;
}

int reconhece_declaracao(){
  if(reconhece_tipo()){
    next_token();
    if(reconhece_lista_identificadores()){
      return 1;
    }
    else return erro();
  }
  return 1;
}

int comparacao(){
  return(token.id==IGUAL_IGUAL||token.id==MENOR||token.id==MAIOR||token.id==MAIOR_IGUAL||token.id==MENOR_IGUAL||token.id==DIFERENTE);
}

int operador(){
  return(token.id==ADD||token.id==SUB||token.id==MUL||token.id==DIV);
}

int operador_duplo(){
  return(token.id==ADD_ADD||token.id==SUB_SUB);
}

int operador_composto(){
  return(token.id==ADD_IGUAL||token.id==SUB_IGUAL||token.id==MUL_IGUAL||token.id==DIV_IGUAL);
}

int expressao_02(){
  if(token.id==IGUAL||comparacao()||operador()||operador_composto()){
    next_token();
    if(expressao()){
      return 1;
    }
    return 0;
  }
  else if(operador_duplo()){
      return 1;
  }
  return 0;
}

int literal(){
  if(token.id==ASPAS_S){
    next_token();
    if(token.id==LITERAL){
      next_token();
      if(token.id==ASPAS_S){
        return 1;
      }
    }
    return 0;
  }
  if(token.id==ASPAS_D){
    next_token();
    if(token.id==LITERAL){
      next_token();
      if(token.id==ASPAS_D){
        return 1;
      }
    }
  }
  return 0;
}

int fator(){
  if(token.id==O_PARENTESES){
    next_token();
    if(expressao()){
      next_token();
      if(token.id==C_PARENTESES){
        return 1;
      }
      return 0;
    }
    return 0;
  }
  else if(token.id == SUB||token.id == ADD){
    next_token();
    if(fator()){
      return 1;
    }
    return 0;
  }
  else if(token.id == ID||token.id == NUM||literal()){
    return 1;
  }
  return 0;
}

int termo_linha(){
  if(token.id==MUL||token.id==DIV){
    next_token();
    if(fator()){
      next_token();
      if(termo_linha()){
        return 1;
      }
    }
    return 0;
  }
  return 1;
}

int termo(){
  if(fator()){
    next_token();
    if(termo_linha()){
      return 1;
    }
  }
  return 0;
}

int magnitude_linha(){
  if(token.id==ADD||token.id==SUB){
    next_token();
    if(termo()){
      next_token();
      if(magnitude_linha()){
        return 1;
      }
    }
    return erro();
  }
  return 1;
}

int magnitude(){
  if(termo()){
    next_token();
    if(magnitude_linha()){
      return 1;
    }
  }
  return 0;
}

int valor_r_linha(){
  if(comparacao()){
    next_token();
    if(magnitude()){
      next_token();
      if(valor_r_linha()){
        return 1;
      }
    }
    return erro();
  }
  return 1;
}

int valor_r(){
  if(comparacao()){
    next_token();
    if(magnitude()){
      next_token();
      if(valor_r_linha()){
        return 1;
      }
    }
    return erro();
  }
  else if(magnitude()){
    return 1;
  }
  return erro();
}

int expressao(){
  if(token.id==ID){
    next_token();
    if(expressao_02()){
      return 1;
    }
    return 0;
  }
  else if(valor_r()){
    return 1;
  }
  return 0;
}

int lista_statement_linha(){
  if(statement()){
    next_token();
    if(lista_statement_linha()){
      return 1;
    }
  }
  return 1;
}

int lista_statement(){
  if(statement()){
    next_token();
    if(lista_statement_linha()){
      return 1;
    }
  }
  return 0;
}

int expressao_opcional(){
  if(token.id==ID){
    next_token();
    if(expressao_02()){
      return 1;
    }
    return erro();
  }
  else if(valor_r()){
    return 1;
  }
  return 1;
}

int statement_escopo(){
  if(token.id==O_CHAVE){
    next_token();
    if(lista_statement()){
      next_token();
      if(token.id==C_CHAVE){
        return 1;
      }
    }
  }
  return 0;
}

int statement_return(){
  return(token.id==NUM||token.id==ID||literal());
}

int statement(){
  if(statement_if()||statement_for()||statement_do_while()||statement_while()||statement_switch()||statement_escopo()||reconhece_declaracao()){
    return 1;
  }
  if(expressao()){
    next_token();
    if(token.id==PONTO_VIRGULA){
      return 1;
    }
    return erro();
  }
  if(token.id==16){//return
    next_token();
    if(statement_return()){
      next_token();
    }
    if(token.id==PONTO_VIRGULA){
        return 1;
    }
  }
  if(token.id==3){//break
    next_token();
    if(token.id==PONTO_VIRGULA){
      return 1;
    }
  }
  if(token.id==PONTO_VIRGULA){
    return 1;
  }
  return erro();
}

int statement_for(){
  if(token.id == 12){
    next_token();
    if(token.id==O_PARENTESES){
      next_token();
      if(expressao()){
        next_token();
        if(token.id==PONTO_VIRGULA){
          next_token();
          if(expressao_opcional()){
            next_token();
            if(token.id==PONTO_VIRGULA){
              next_token();{
                if(expressao_opcional()){
                  next_token();
                  if(token.id==C_PARENTESES){
                    next_token();
                    if(statement()){
                      return 1;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    return erro();
  }
  return 0;
}

int statement_while(){
  if(token.id==24){ //while
    next_token();
    if(token.id==O_PARENTESES){
      next_token();{
        if(expressao()){
          next_token();
          if(token.id==C_PARENTESES){
            next_token();
            if(statement()){
              return 1;
            }
          }
        }
      }
    }
    return erro();
  }
  return 0;
}

int statement_do_while(){
  if(token.id==8){ //do
    next_token();
    if(statement_escopo()){
      next_token();
      if(token.id==24){//while
        next_token();
        if(token.id==O_PARENTESES){
          next_token();
          if(expressao()){
            next_token();
            if(token.id==C_PARENTESES){
              return 1;
            }
          }
        }
      }
    }
    return erro();
  }
  return 0;
}

int statement_else(){
  if(token.id==10){ //else
    next_token();
    if(statement()){
      return 1;
    }
    return erro();
  }
  return 1;
}

int statement_if(){
  if(token.id==13){ //if
    next_token();
    if(token.id==O_PARENTESES){
      next_token();
      if(expressao()){
        next_token();
        if(token.id==C_PARENTESES){
          next_token();
          if(statement()){
            next_token();
            if(statement_else()){
              return 1;
            }
          }
        }
      }
    }
    return erro();
  }
  return 0;
}

int switch_args(){
  return(token.id==NUM||token.id==ID||literal());
}

int lista_case_linha(){
   if(token.id==4){//case
    next_token();
    if(switch_args()){
      next_token();
      if(token.id==DOIS_PONTOS){
        next_token();
        if(lista_statement()){
          next_token();
          if(lista_case_linha()){
            return 1;
          }
        }
      }
    }
    return 0;
  }
  else if(token.id==7){//default
    next_token();
    if(token.id==DOIS_PONTOS){
      next_token();
      if(lista_statement()){
        return 1;
      }
    }
    return 0;
  }
  return 1;
}

int lista_case(){
  if(token.id==4){//case
    next_token();
    if(switch_args()){
      next_token();
      if(token.id==DOIS_PONTOS){
        next_token();
        if(lista_statement()){
          next_token();
          if(lista_case_linha()){
            return 1;
          }
        }
      }
    }
  }
  return 0;
}

int statement_switch(){
  if(token.id==20){ //switch
    next_token();
    if(token.id==O_PARENTESES){
      next_token();
      if(switch_args()){
        next_token();
        if(token.id==C_PARENTESES){
          next_token();
          if(token.id==O_CHAVE){
            next_token();
            if(lista_case()){
              next_token();
              if(token.id==C_CHAVE){
                return 1;
              }
            }
          }
        }
      }
    }
  }
  return erro();
}

int main(int argc, char *argv[]){
    gera_tokens(argv[1]);
    n_erros=0;
    arq = fopen("lista_tokens","rb");
    arq_erro = fopen("log_erros_sintatico.txt","w");
    while(!feof(arq)){
      next_token();
      reconhece_funcao();
    }
    if(n_erros==0){
      printf("OK\n");
    }
    else{
      printf("Numero de erros: %d\n",n_erros);
    }
    fclose(arq);
    fclose(arq_erro);
}
