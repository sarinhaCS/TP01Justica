#ifndef LISTA_H
#define LISTA_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Struct com os 33 campos

typedef struct registro {

    char sigla_tribunal[20];
    char procedimento[50];
    char ramo_justica[30];
    char sigla_grau[10];
    char uf_oj[3];
    char municipio_oj[50];
    int id_ultimo_oj;
    char nome[200];

    char mesano_cnm1[20];
    char mesano_sent[20];

    int casos_novos_2026;
    int julgados_2026;
    int prim_sent2026;
    int suspensos_2026;
    int dessobrestados_2026;
    float cumprimento_meta1;

    int distm2_a;
    int julgm2_a;
    int suspm2_a;
    float cumprimento_meta2a;

    int distm2_ant;
    int julgm2_ant;
    int suspm2_ant;
    int desom2_ant;
    float cumprimento_meta2ant;

    int distm4_a;
    int julgm4_a;
    int suspm4_a;
    float cumprimento_meta4a;

    int distm4_b;
    int julgm4_b;
    int suspm4_b;
    float cumprimento_meta4b;

} Registro;

//Estruturas da lista encadeada:

typedef struct no {
    Registro dado;
    struct no *prox;
} No;

typedef struct lista {
    No *inicio;
    int tamanho;
} Lista;

//Funções básicas da lista:

void inicializarLista(Lista *l);
void inserirFim(Lista *l, Registro r);
int removerPorId(Lista *l, int id);
No* buscarPorId(Lista *l, int id);
void liberarLista(Lista *l);


// ​Função para concatenar arquivos.  
//Função para gerar o resumo com as fórmulas das Metas.  
//​Função para filtrar por município

#endif
