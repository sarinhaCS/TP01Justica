#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

int main(){
  
    int opcao; //variável resposta do menu

    //&lista é a variável de exemplo da struct, pode ser mudado depois

    do {
        printf("\n#### MENU ####\n");
        printf("1 - Juntar arquivos\n");
        printf("2 - Gerar resumo por tribunal\n");
        printf("3 - Gerar arquivo por municipio\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
          //os nomes das funções são de exmplo
            case 1:
                /* Chama a função do item 1 */
                //juntarArquivos(&lista);
                break;

            case 2:
                /* Chama a função do item 2 */
                //gerarResumo(&lista);
                break;

            case 3:
                /* Chama a função do item 3 */
                //gerarArquivoMunicipio(&lista);
                break;

            case 0:
                printf("Saída solicitada\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

  return 0;
}
