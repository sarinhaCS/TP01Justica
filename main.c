#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

int main(){
    int opcao; //variável resposta do menu

    Lista *l;
    inicializarLista(&l);

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Concatenar arquivos\n");
        printf("2 - Gerar resumo por tribunal\n");
        printf("3 - Filtrar arquivos por municipio\n");
        printf("4 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                /* Chama a função do item 1 */
                //Lista * registro = 
                concatenarArquivos(&l,"registro.csv");
                break;

            case 2:
                /* Chama a função do item 2 */
                gerarResumoMetas(&l, "resumo.csv");
                break;

            case 3:
                /* Chama a função do item 3 */
                filtrarPorMunicipio(&l, municipio);
                break;

            case 4:
                printf("Saida solicitada\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}
