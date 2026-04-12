#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
void inicializarLista(Lista *l) {
    l->inicio = NULL;
    l->tamanho = 0;
}
 
void inserirFim(Lista *l, Registro r) {
    No *novo = (No *)malloc(sizeof(No));
    novo->dado = r;
    novo->prox = NULL;
 
    if (l->inicio == NULL) {
        l->inicio = novo;
    } else {
        No *atual = l->inicio;
        while (atual->prox != NULL)
            atual = atual->prox;
        atual->prox = novo;
    }
    l->tamanho++;
}
 
int removerPorId(Lista *l, int id) {
    No *atual = l->inicio, *ant = NULL;
    while (atual) {
        if (atual->dado.id_ultimo_oj == id) {
            if (ant) ant->prox = atual->prox;
            else     l->inicio = atual->prox;
            free(atual);
            l->tamanho--;
            return 1;
        }
        ant = atual;
        atual = atual->prox;
    }
    return 0;
}
 
No *buscarPorId(Lista *l, int id) {
    No *atual = l->inicio;
    while (atual) {
        if (atual->dado.id_ultimo_oj == id) return atual;
        atual = atual->prox;
    }
    return NULL;
}
 
void liberarLista(Lista *l) {
    No *atual = l->inicio;
    while (atual) {
        No *prox = atual->prox;
        free(atual);
        atual = prox;
    }
    l->inicio = NULL;
    l->tamanho = 0;
}
 
// ─────────────────────────────────────────────
//  Leitura de linha CSV
// ─────────────────────────────────────────────
 
static void lerCampo(const char **src, char *dest, int max) {
    int i = 0;
    while (**src && **src != ';' && **src != '\n' && i < max - 1)
        dest[i++] = *(*src)++;
    dest[i] = '\0';
    if (**src == ';') (*src)++;
}
 
static void parsearLinha(const char *linha, Registro *r) {
    char buf[256];
    const char *p = linha;
 
    lerCampo(&p, r->sigla_tribunal,  sizeof(r->sigla_tribunal));
    lerCampo(&p, r->procedimento,    sizeof(r->procedimento));
    lerCampo(&p, r->ramo_justica,    sizeof(r->ramo_justica));
    lerCampo(&p, r->sigla_grau,      sizeof(r->sigla_grau));
    lerCampo(&p, r->uf_oj,           sizeof(r->uf_oj));
    lerCampo(&p, r->municipio_oj,    sizeof(r->municipio_oj));
 
    lerCampo(&p, buf, sizeof(buf)); r->id_ultimo_oj = atoi(buf);
    lerCampo(&p, r->nome,            sizeof(r->nome));
    lerCampo(&p, r->mesano_cnm1,     sizeof(r->mesano_cnm1));
    lerCampo(&p, r->mesano_sent,     sizeof(r->mesano_sent));
 
    lerCampo(&p, buf, sizeof(buf)); r->casos_novos_2026    = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgados_2026       = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->prim_sent2026       = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspensos_2026      = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->dessobrestados_2026 = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta1   = atof(buf);
 
    lerCampo(&p, buf, sizeof(buf)); r->distm2_a   = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgm2_a   = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspm2_a   = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta2a = atof(buf);
 
    lerCampo(&p, buf, sizeof(buf)); r->distm2_ant  = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgm2_ant  = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspm2_ant  = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->desom2_ant  = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta2ant = atof(buf);
 
    lerCampo(&p, buf, sizeof(buf)); r->distm4_a   = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgm4_a   = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspm4_a   = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta4a = atof(buf);
 
    lerCampo(&p, buf, sizeof(buf)); r->distm4_b   = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgm4_b   = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspm4_b   = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta4b = atof(buf);
}
 
// ─────────────────────────────────────────────
//  1. Concatenar arquivos
//  O usuário informa quantos arquivos e seus nomes.
//  O cabeçalho é copiado só do primeiro arquivo.
//  Todos os dados são carregados na lista e gravados no destino.
// ─────────────────────────────────────────────
 
void concatenarArquivos(Lista *l, const char *nomeArquivoDestino) {
    int qtd;
    printf("Quantos arquivos deseja concatenar? ");
    scanf("%d", &qtd);
    getchar();
 
    FILE *dest = fopen(nomeArquivoDestino, "w");
    if (!dest) { printf("Erro ao criar arquivo destino.\n"); return; }
 
    liberarLista(l);
 
    for (int i = 0; i < qtd; i++) {
        char caminho[200];
        printf("Nome do arquivo %d: ", i + 1);
        fgets(caminho, sizeof(caminho), stdin);
        caminho[strcspn(caminho, "\n")] = '\0';
 
        FILE *src = fopen(caminho, "r");
        if (!src) { printf("Erro ao abrir: %s\n", caminho); continue; }
 
        char linha[2048];
 
        // Cabeçalho: copia só do primeiro, descarta dos demais
        if (fgets(linha, sizeof(linha), src)) {
            if (i == 0) fprintf(dest, "%s", linha);
        }
 
        while (fgets(linha, sizeof(linha), src)) {
            fprintf(dest, "%s", linha);
            Registro r;
            parsearLinha(linha, &r);
            inserirFim(l, r);
        }
 
        fclose(src);
    }
 
    fclose(dest);
    printf("Arquivo '%s' gerado com %d registros.\n", nomeArquivoDestino, l->tamanho);
}
 
// ─────────────────────────────────────────────
//  2. Gerar resumo por tribunal
//
//  Fórmulas do PDF (subtrações viram adições conforme instrução):
//  Meta1    = sum(julgados) / (sum(casos_novos) + sum(dessobrestados) + sum(suspensos)) * 100
//  Meta2A   = sum(julgm2_a) / (sum(distm2_a) + sum(suspm2_a)) * 1000/7
//  Meta2Ant = sum(julgm2_ant) / (sum(distm2_ant) + sum(suspm2_ant) + sum(desom2_ant)) * 100
//  Meta4A   = sum(julgm4_a) / (sum(distm4_a) + sum(suspm4_a)) * 100
//  Meta4B   = sum(julgm4_b) / (sum(distm4_b) + sum(suspm4_b)) * 100
// ─────────────────────────────────────────────
 
void gerarResumoMetas(Lista *l, const char *nomeArquivoSaida) {
    if (!l->inicio) { printf("Lista vazia. Concatene os arquivos primeiro.\n"); return; }
 
    char tribunais[200][20];
    int qtdTribunais = 0;
 
    No *atual = l->inicio;
    while (atual) {
        int existe = 0;
        for (int i = 0; i < qtdTribunais; i++)
            if (strcmp(tribunais[i], atual->dado.sigla_tribunal) == 0) { existe = 1; break; }
        if (!existe)
            strcpy(tribunais[qtdTribunais++], atual->dado.sigla_tribunal);
        atual = atual->prox;
    }
 
    FILE *f = fopen(nomeArquivoSaida, "w");
    if (!f) { printf("Erro ao criar arquivo de resumo.\n"); return; }
 
    fprintf(f, "sigla_tribunal;total_julgados_2026;Meta1;Meta2A;Meta2Ant;Meta4A;Meta4B\n");
 
    for (int i = 0; i < qtdTribunais; i++) {
        long julgados = 0, casos_novos = 0, dessobr = 0, susp = 0;
        long julgm2a = 0, distm2a = 0, suspm2a = 0;
        long julgm2ant = 0, distm2ant = 0, suspm2ant = 0, desom2ant = 0;
        long julgm4a = 0, distm4a = 0, suspm4a = 0;
        long julgm4b = 0, distm4b = 0, suspm4b = 0;
 
        atual = l->inicio;
        while (atual) {
            Registro *r = &atual->dado;
            if (strcmp(r->sigla_tribunal, tribunais[i]) == 0) {
                julgados    += r->julgados_2026;
                casos_novos += r->casos_novos_2026;
                dessobr     += r->dessobrestados_2026;
                susp        += r->suspensos_2026;
                julgm2a     += r->julgm2_a;
                distm2a     += r->distm2_a;
                suspm2a     += r->suspm2_a;
                julgm2ant   += r->julgm2_ant;
                distm2ant   += r->distm2_ant;
                suspm2ant   += r->suspm2_ant;
                desom2ant   += r->desom2_ant;
                julgm4a     += r->julgm4_a;
                distm4a     += r->distm4_a;
                suspm4a     += r->suspm4_a;
                julgm4b     += r->julgm4_b;
                distm4b     += r->distm4_b;
                suspm4b     += r->suspm4_b;
            }
            atual = atual->prox;
        }
 
        double meta1    = (casos_novos + dessobr + susp)        ? (double)julgados  / (casos_novos + dessobr + susp)        * 100      : 0;
        double meta2a   = (distm2a  + suspm2a)                  ? (double)julgm2a   / (distm2a  + suspm2a)                  * 1000.0/7 : 0;
        double meta2ant = (distm2ant + suspm2ant + desom2ant)   ? (double)julgm2ant / (distm2ant + suspm2ant + desom2ant)   * 100      : 0;
        double meta4a   = (distm4a  + suspm4a)                  ? (double)julgm4a   / (distm4a  + suspm4a)                  * 100      : 0;
        double meta4b   = (distm4b  + suspm4b)                  ? (double)julgm4b   / (distm4b  + suspm4b)                  * 100      : 0;
 
        fprintf(f, "%s;%ld;%.2f;%.2f;%.2f;%.2f;%.2f\n",
                tribunais[i], julgados, meta1, meta2a, meta2ant, meta4a, meta4b);
    }
 
    fclose(f);
    printf("Resumo gerado em '%s'.\n", nomeArquivoSaida);
}
 
// ─────────────────────────────────────────────
//  3. Filtrar por município
//  Gera MUNICIPIO.txt com todas as linhas daquele município.
// ─────────────────────────────────────────────
 
void filtrarPorMunicipio(Lista *l, const char *municipioEspecifico) {
    if (!l->inicio) { printf("Lista vazia. Concatene os arquivos primeiro.\n"); return; }
 
    char nomeArquivo[60];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.txt", municipioEspecifico);
 
    FILE *f = fopen(nomeArquivo, "w");
    if (!f) { printf("Erro ao criar arquivo.\n"); return; }
 
    int cont = 0;
    No *atual = l->inicio;
    while (atual) {
        Registro *r = &atual->dado;
        if (strcmp(r->municipio_oj, municipioEspecifico) == 0) {
            fprintf(f, "%s;%s;%s;%s;%s;%s;%d;%s;%s;%s;"
                       "%d;%d;%d;%d;%d;%.2f;"
                       "%d;%d;%d;%.2f;"
                       "%d;%d;%d;%d;%.2f;"
                       "%d;%d;%d;%.2f;"
                       "%d;%d;%d;%.2f\n",
                r->sigla_tribunal, r->procedimento, r->ramo_justica,
                r->sigla_grau, r->uf_oj, r->municipio_oj,
                r->id_ultimo_oj, r->nome, r->mesano_cnm1, r->mesano_sent,
                r->casos_novos_2026, r->julgados_2026, r->prim_sent2026,
                r->suspensos_2026, r->dessobrestados_2026, r->cumprimento_meta1,
                r->distm2_a, r->julgm2_a, r->suspm2_a, r->cumprimento_meta2a,
                r->distm2_ant, r->julgm2_ant, r->suspm2_ant, r->desom2_ant, r->cumprimento_meta2ant,
                r->distm4_a, r->julgm4_a, r->suspm4_a, r->cumprimento_meta4a,
                r->distm4_b, r->julgm4_b, r->suspm4_b, r->cumprimento_meta4b);
            cont++;
        }
        atual = atual->prox;
    }
 
    fclose(f);
    printf("%d Registros salvos em '%s'.\n", cont, nomeArquivo);
}
 
