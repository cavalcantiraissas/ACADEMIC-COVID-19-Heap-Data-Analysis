#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Estrutura para armazenar os dados de cada país
typedef struct DadosPais {
    char pais[100];
    int confirmados;
    int mortes;
    int recuperados;
    float taxa_mortalidade;
    float taxa_recuperacao;
    struct DadosPais* proximo;
} DadosPais;

// Estrutura para o Heap
typedef struct Heap {
    DadosPais** dados;
    int tamanho;
    int capacidade;
    char tipo[20];
} Heap;

DadosPais* criar_no(const char* pais, int confirmados, int mortes, int recuperados) {
    DadosPais* novo = (DadosPais*)malloc(sizeof(DadosPais));
    if (!novo) {
        perror("Erro ao alocar memória");
        exit(EXIT_FAILURE);
    }

    strncpy(novo->pais, pais, sizeof(novo->pais) - 1);
    novo->confirmados = confirmados;
    novo->mortes = mortes;
    novo->recuperados = recuperados;
    
    novo->taxa_mortalidade = (confirmados > 0) ? (mortes * 100.0f) / confirmados : 0;
    novo->taxa_recuperacao = (confirmados > 0) ? (recuperados * 100.0f) / confirmados : 0;
    
    novo->proximo = NULL;
    return novo;
}

void liberar_lista(DadosPais* cabeca) {
    while (cabeca) {
        DadosPais* temp = cabeca;
        cabeca = cabeca->proximo;
        free(temp);
    }
}

Heap* criar_heap(int capacidade, const char* tipo) {
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    if (!heap) {
        perror("Erro ao alocar heap");
        exit(EXIT_FAILURE);
    }

    heap->dados = (DadosPais**)malloc(capacidade * sizeof(DadosPais*));
    if (!heap->dados) {
        free(heap);
        perror("Erro ao alocar array do heap");
        exit(EXIT_FAILURE);
    }

    heap->tamanho = 0;
    heap->capacidade = capacidade;
    strncpy(heap->tipo, tipo, sizeof(heap->tipo) - 1);
    return heap;
}

void heapify(Heap* heap, int indice) {
    int maior = indice;
    int esquerda = 2 * indice + 1;
    int direita = 2 * indice + 2;

    if (esquerda < heap->tamanho) {
        int valor_esquerda, valor_maior;
        
        if (strcmp(heap->tipo, "confirmados") == 0) {
            valor_esquerda = heap->dados[esquerda]->confirmados;
            valor_maior = heap->dados[maior]->confirmados;
        } else if (strcmp(heap->tipo, "mortes") == 0) {
            valor_esquerda = heap->dados[esquerda]->mortes;
            valor_maior = heap->dados[maior]->mortes;
        } else {
            valor_esquerda = heap->dados[esquerda]->recuperados;
            valor_maior = heap->dados[maior]->recuperados;
        }

        if (valor_esquerda > valor_maior) {
            maior = esquerda;
        }
    }

    if (direita < heap->tamanho) {
        int valor_direita, valor_maior;
        
        if (strcmp(heap->tipo, "confirmados") == 0) {
            valor_direita = heap->dados[direita]->confirmados;
            valor_maior = heap->dados[maior]->confirmados;
        } else if (strcmp(heap->tipo, "mortes") == 0) {
            valor_direita = heap->dados[direita]->mortes;
            valor_maior = heap->dados[maior]->mortes;
        } else {
            valor_direita = heap->dados[direita]->recuperados;
            valor_maior = heap->dados[maior]->recuperados;
        }

        if (valor_direita > valor_maior) {
            maior = direita;
        }
    }

    if (maior != indice) {
        DadosPais* temp = heap->dados[indice];
        heap->dados[indice] = heap->dados[maior];
        heap->dados[maior] = temp;
        heapify(heap, maior);
    }
}

void carregar_dados(DadosPais** cabeca, const char* nome_arquivo) {
    FILE* arquivo = fopen(nome_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir arquivo");
        return;
    }

    char linha[1024];
    fgets(linha, sizeof(linha), arquivo); // Pular cabeçalho

    while (fgets(linha, sizeof(linha), arquivo)) {
        char* token = strtok(linha, ",");
        if (!token) continue;

        char pais[100];
        strncpy(pais, token, sizeof(pais) - 1);

        // Pular colunas não utilizadas
        for (int i = 0; i < 2; i++) strtok(NULL, ",");

        int confirmados = atoi(strtok(NULL, ","));
        int mortes = atoi(strtok(NULL, ","));
        int recuperados = atoi(strtok(NULL, ","));

        DadosPais* novo = criar_no(pais, confirmados, mortes, recuperados);
        novo->proximo = *cabeca;
        *cabeca = novo;
    }

    fclose(arquivo);
}

void exibir_menu() {
    printf("\n=== MENU DE OPÇÕES ===\n");
    printf("1. Países com mais casos confirmados\n");
    printf("2. Países com mais mortes\n");
    printf("3. Países com mais recuperados\n");
    printf("4. Sair\n");
    printf("Escolha uma opção: ");
}

void exibir_resultados(Heap* heap, int quantidade) {
    printf("\n=== RESULTADOS ===\n");
    printf("%-30s %-12s %-12s %-12s %-12s %-12s\n", 
           "País", "Confirmados", "Mortes", "Recuperados", 
           "Mortalidade%", "Recuperação%");
    printf("--------------------------------------------------------------------------------\n");

    for (int i = 0; i < quantidade && heap->tamanho > 0; i++) {
        DadosPais* pais = heap->dados[0];
        printf("%-30s %-12d %-12d %-12d %-12.2f %-12.2f\n", 
               pais->pais, pais->confirmados, pais->mortes, 
               pais->recuperados, pais->taxa_mortalidade, 
               pais->taxa_recuperacao);

        heap->dados[0] = heap->dados[--heap->tamanho];
        heapify(heap, 0);
    }
}

int main() {
    DadosPais* dados = NULL;
    printf("Carregando dados de country_wise_latest.csv...\n");
    carregar_dados(&dados, "country_wise_latest.csv");

    if (!dados) {
        fprintf(stderr, "Erro: Não foi possível carregar os dados.\n");
        return EXIT_FAILURE;
    }

    while (1) {
        exibir_menu();
        int opcao;
        scanf("%d", &opcao);

        if (opcao == 4) break;

        const char* tipo = NULL;
        switch (opcao) {
            case 1: tipo = "confirmados"; break;
            case 2: tipo = "mortes"; break;
            case 3: tipo = "recuperados"; break;
            default:
                printf("Opção inválida!\n");
                continue;
        }

        printf("Quantos países deseja visualizar? ");
        int quantidade;
        scanf("%d", &quantidade);

        Heap* heap = criar_heap(200, tipo);
        for (DadosPais* atual = dados; atual; atual = atual->proximo) {
            if (heap->tamanho < heap->capacidade) {
                heap->dados[heap->tamanho++] = atual;
            }
        }

        for (int i = heap->tamanho / 2 - 1; i >= 0; i--) {
            heapify(heap, i);
        }

        exibir_resultados(heap, quantidade);
        
        free(heap->dados);
        free(heap);
    }

    liberar_lista(dados);
    printf("Programa encerrado.\n");
    return EXIT_SUCCESS;
}
