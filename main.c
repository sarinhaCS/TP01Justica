#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

int main(){
    Lista lista;

    int opcao; //variável resposta do menu
    char nomeArquivoDestino[100];
    char nomeArquivoResumo[100];
    char municipio[50];

    inicializarLista(&lista);

    do {
        printf("\n#### MENU ####\n");
        printf("1 - Concatenar arquivos\n");
        printf("2 - Gerar resumo por tribunal\n");
        printf("3 - Filtrar arquivos por municipio\n");
        printf("4 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        getchar(); // limpa o espaço vazio do buffer 

        switch (opcao) {
            case 1:
                /* Chama a função do item 1 */
                printf("Digite o nome do arquivo de destino: ");
                fgets(nomeArquivoDestino, sizeof(nomeArquivoDestino), stdin);
                nomeArquivoDestino[strcspn(nomeArquivoDestino, "\n")] = '\0'; 
                concatenarArquivos(&lista, nomeArquivoDestino);
                break;

            case 2:
                /* Chama a função do item 2 */
                gerarResumoMetas(&lista, "resumo.csv");
                break;

            case 3:
                /* Chama a função do item 3 */
                printf("Digite o nome do municipio: ");
                fgets(municipio, sizeof(municipio), stdin);
                municipio[strcspn(municipio, "\n")] = '\0';
                filtrarPorMunicipio(&lista, municipio);
                break;

            case 4:
                printf("Saida realizada\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
