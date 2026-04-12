#include "lista.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void carregarArquivo(Lista *l, const char *nomeArquivo);
void escreverRegistro(FILE *arquivo, Registro r);
 
void inicializarLista(Lista *l) {
    l->inicio = NULL;
    l->fim = NULL;
    l->tamanho = 0;
}
 
void inserirFim(Lista *l, Registro r) {
    No *novo = (No*)malloc(sizeof(No));
    novo->dado = r;
    novo->prox = NULL;

    if (l->inicio == NULL) {
        l->inicio = novo;
        l->fim = novo;
    } else {
        l->fim->prox = novo; 
        l->fim = novo; 
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
 
static void lerCampo(const char **src, char *dest, int max) {
    int i = 0;
 
    if (**src == '"') (*src)++;
 
    while (**src && **src != '"' && **src != ',' && **src != '\n' && i < max - 1)
        dest[i++] = *(*src)++;
    dest[i] = '\0';
        if (**src == '"') (*src)++;
        if (**src == ',') (*src)++;
}
 
static void parsearLinha(const char *linha, Registro *r) {
    char buf[256];
    const char *p = linha;
 
    lerCampo(&p, r->sigla_tribunal, sizeof(r->sigla_tribunal));
    lerCampo(&p, r->procedimento, sizeof(r->procedimento));
    lerCampo(&p, r->ramo_justica, sizeof(r->ramo_justica));
    lerCampo(&p, r->sigla_grau, sizeof(r->sigla_grau));
    lerCampo(&p, r->uf_oj, sizeof(r->uf_oj));
    lerCampo(&p, r->municipio_oj, sizeof(r->municipio_oj));
    lerCampo(&p, buf, sizeof(buf)); r->id_ultimo_oj = atoi(buf);
    lerCampo(&p, r->nome, sizeof(r->nome));
    lerCampo(&p, r->mesano_cnm1, sizeof(r->mesano_cnm1));
    lerCampo(&p, r->mesano_sent, sizeof(r->mesano_sent));
    lerCampo(&p, buf, sizeof(buf)); r->casos_novos_2026 = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgados_2026 = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->prim_sent2026 = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspensos_2026 = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->dessobrestados_2026 = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta1 = atof(buf);
    lerCampo(&p, buf, sizeof(buf)); r->distm2_a = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgm2_a = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspm2_a = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta2a = atof(buf);
    lerCampo(&p, buf, sizeof(buf)); r->distm2_ant  = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgm2_ant  = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspm2_ant  = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->desom2_ant  = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta2ant = atof(buf);
    lerCampo(&p, buf, sizeof(buf)); r->distm4_a = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgm4_a = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspm4_a = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta4a = atof(buf);
    lerCampo(&p, buf, sizeof(buf)); r->distm4_b = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->julgm4_b = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->suspm4_b = atoi(buf);
    lerCampo(&p, buf, sizeof(buf)); r->cumprimento_meta4b = atof(buf);
}

void concatenarArquivos(Lista *l, const char *nomeArquivoDestino) {
    int qtd;
    char nomeArquivo[100];

    do {
        printf("Quantos arquivos deseja juntar? ");
        scanf("%d", &qtd);
        getchar();
    } while (qtd <= 0);

    printf("\n");

    for (int i = 0; i < qtd; i++) {
        FILE *teste = NULL; 

        do {
            do {
                printf("Digite o nome do arquivo %d (ex: teste_TRE-AC.csv): ", i + 1);
                fgets(nomeArquivo, sizeof(nomeArquivo), stdin);
                nomeArquivo[strcspn(nomeArquivo, "\n")] = '\0';
            } 
            while (strlen(nomeArquivo) == 0);

            teste = fopen(nomeArquivo, "r");
            
            if (teste == NULL) {
                printf("--> ERRO: O arquivo '%s' nao foi encontrado!\n", nomeArquivo);
                printf("--> Verifique se voce digitou a extensao .csv e tente novamente.\n\n");
            }

        } 
        while (teste == NULL);

        fclose(teste); 

        printf("Carregando %s\n", nomeArquivo);
        carregarArquivo(l, nomeArquivo); 
    }
    FILE *destino = fopen(nomeArquivoDestino, "w");
    if (destino) {
        No *atual = l->inicio;
        while (atual != NULL) {
            escreverRegistro(destino, atual->dado);
            atual = atual->prox;
        }
        fclose(destino);
        printf("\nArquivos unificados em '%s'.\n", nomeArquivoDestino);
    } else {
        printf("\nErro ao criar o arquivo de destino.\n");
    }
}
 
void gerarResumoMetas(Lista *l, const char *nomeArquivoSaida) {
    if (!l->inicio){ 
        printf("Junte os arquivos primeiro.\n"); return; }
 
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
 
        double meta1    = (casos_novos + dessobr + susp) ? (double)julgados  / (casos_novos + dessobr + susp) * 100 : 0;
        double meta2a   = (distm2a  + suspm2a) ? (double)julgm2a   / (distm2a  + suspm2a) * 1000.0/7 : 0;
        double meta2ant = (distm2ant + suspm2ant + desom2ant) ? (double)julgm2ant / (distm2ant + suspm2ant + desom2ant) * 100 : 0;
        double meta4a   = (distm4a  + suspm4a) ? (double)julgm4a / (distm4a  + suspm4a) * 100 : 0;
        double meta4b   = (distm4b  + suspm4b) ? (double)julgm4b  / (distm4b  + suspm4b) * 100 : 0;
 
        fprintf(f, "%s;%ld;%.2f;%.2f;%.2f;%.2f;%.2f\n",
                tribunais[i], julgados, meta1, meta2a, meta2ant, meta4a, meta4b);
    }
 
    fclose(f);
    printf("Resumo gerado em '%s'.\n", nomeArquivoSaida);
}
 
void normalizarString(const char *origem, char *destino) {
    int j = 0;
    for (int i = 0; origem[i] != '\0'; i++) {
        unsigned char c = origem[i];
        if (c == 0xC3 && origem[i+1] != '\0') {
            i++;
            unsigned char c2 = origem[i];
            if (c2 == 0x81 || c2 == 0x82 || c2 == 0x83 || c2 == 0xA1 || c2 == 0xA2 || c2 == 0xA3) destino[j++] = 'A';
            else if (c2 == 0x89 || c2 == 0x8A || c2 == 0xA9 || c2 == 0xAA) destino[j++] = 'E';
            else if (c2 == 0x8D || c2 == 0xAD) destino[j++] = 'I';
            else if (c2 == 0x93 || c2 == 0x94 || c2 == 0x95 || c2 == 0xB3 || c2 == 0xB4 || c2 == 0xB5) destino[j++] = 'O';
            else if (c2 == 0x9A || c2 == 0xBA) destino[j++] = 'U';
            else if (c2 == 0x87 || c2 == 0xA7) destino[j++] = 'C';
        } 
        else if (c == 0xC1 || c == 0xC2 || c == 0xC3 || c == 0xE1 || c == 0xE2 || c == 0xE3) destino[j++] = 'A';
        else if (c == 0xC9 || c == 0xCA || c == 0xE9 || c == 0xEA) destino[j++] = 'E';
        else if (c == 0xCD || c == 0xED) destino[j++] = 'I';
        else if (c == 0xD3 || c == 0xD4 || c == 0xD5 || c == 0xF3 || c == 0xF4 || c == 0xF5) destino[j++] = 'O';
        else if (c == 0xDA || c == 0xFA) destino[j++] = 'U';
        else if (c == 0xC7 || c == 0xE7) destino[j++] = 'C';
        else if (c >= 'a' && c <= 'z') destino[j++] = c - 32; 
        else if (c < 0x80) destino[j++] = c; 
    }
    destino[j] = '\0';
}

void filtrarPorMunicipio(Lista *l, const char *municipioEspecifico) {
    if (!l->inicio){ 
        printf("Lista vazia. Junte os arquivos primeiro.\n"); 
        return; 
    }
    char cidadeBusca[100];
    char cidadeBuscaLimpa[100];
    strcpy(cidadeBusca, municipioEspecifico);
    normalizarString(cidadeBusca, cidadeBuscaLimpa);

    int encontrou;

    do {
        encontrou = 0;
        
        No *atualBusca = l->inicio;
        while (atualBusca) {
            char cidadeArquivoLimpa[100]; 
            normalizarString(atualBusca->dado.municipio_oj, cidadeArquivoLimpa);

            if (strcmp(cidadeArquivoLimpa, cidadeBuscaLimpa) == 0) {
                encontrou = 1;
                break;
            }
            atualBusca = atualBusca->prox;
        }

        if (!encontrou) {
            printf("\n--> Erro: O municipio '%s' nao foi encontrado na base de dados.\n", cidadeBusca);
            
            do {
                printf("\nDigite o nome do municipio novamente (ou digite SAIR para cancelar): ");
                fgets(cidadeBusca, sizeof(cidadeBusca), stdin);
                cidadeBusca[strcspn(cidadeBusca, "\n")] = '\0';
            } while (strlen(cidadeBusca) == 0);

            if (strcmp(cidadeBusca, "SAIR") == 0) {
                printf("Operacao cancelada. Voltando ao menu.\n");
                return; 
            }
            
            normalizarString(cidadeBusca, cidadeBuscaLimpa);
        }
    } while (!encontrou);

    char nomeArquivo[60];
    snprintf(nomeArquivo, sizeof(nomeArquivo), "%s.txt", cidadeBuscaLimpa); 
    FILE *f = fopen(nomeArquivo, "w");
    if (!f){ 
        printf("Erro ao criar arquivo.\n"); 
        return; 
    }

    int cont = 0;
    No *atual = l->inicio;
    while (atual) {
        Registro *r = &atual->dado;
        char cidadeArquivoLimpa[100];
        normalizarString(r->municipio_oj, cidadeArquivoLimpa);

        if (strcmp(cidadeArquivoLimpa, cidadeBuscaLimpa) == 0) {
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
    printf("\n %d registros encontrados e salvos em '%s'.\n", cont, nomeArquivo);
}

void carregarArquivo(Lista *l, const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "r");
    if (!f) {
        printf("Nao foi possivel abrir o arquivo %s\n", nomeArquivo);
        return;
    }

    char linha[1024];
    
    if (fgets(linha, sizeof(linha), f) == NULL) {
        fclose(f);
        return;
    }

    while (fgets(linha, sizeof(linha), f)) {
        Registro r;
        memset(&r, 0, sizeof(Registro)); 
        parsearLinha(linha, &r);
        inserirFim(l, r);
    }
    fclose(f);
}

void escreverRegistro(FILE *arquivo, Registro r) {
    fprintf(arquivo, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,\"%s\",\"%s\",\"%s\","
                     "%d,%d,%d,%d,%d,%.2f,"
                     "%d,%d,%d,%.2f,"
                     "%d,%d,%d,%d,%.2f,"
                     "%d,%d,%d,%.2f,"
                     "%d,%d,%d,%.2f\n",
            r.sigla_tribunal, r.procedimento, r.ramo_justica,
            r.sigla_grau, r.uf_oj, r.municipio_oj,
            r.id_ultimo_oj, r.nome, r.mesano_cnm1, r.mesano_sent,
            r.casos_novos_2026, r.julgados_2026, r.prim_sent2026,
            r.suspensos_2026, r.dessobrestados_2026, r.cumprimento_meta1,
            r.distm2_a, r.julgm2_a, r.suspm2_a, r.cumprimento_meta2a,
            r.distm2_ant, r.julgm2_ant, r.suspm2_ant, r.desom2_ant, r.cumprimento_meta2ant,
            r.distm4_a, r.julgm4_a, r.suspm4_a, r.cumprimento_meta4a,
            r.distm4_b, r.julgm4_b, r.suspm4_b, r.cumprimento_meta4b);
}
