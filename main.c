#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define TAM_MOCHILA 5 // 20
#define TAM_NOME 30
#define TAM_TIPO 20

struct Componente {
	char nome[TAM_NOME];
	char tipo[TAM_TIPO];
	int quantidade;
	int prioridade;
};

void exibirMenu(int quantidadeItens, int bubbleStatus);
void inserirItem(struct Componente* inventario, int* quantidadeItens);
void removerItem(struct Componente** inventario, int* quantidadeItens);
void listarItens(struct Componente* inventario, int quantidadeItens);
void buscarItem(struct Componente* inventario, int quantidadeItens, int bubbleStatus);
void exibirMenuSort();
const char* checarBubbleSort(int bubbleStatus);
void bubbleSortPorNome(struct Componente* inventario, int quantidadeItens);
void insertionSortPorTipo(struct Componente* inventario, int quantidadeItens);
void selectionSortPorPrioridade(struct Componente* inventario, int quantidadeItens);
void liberarmemoria(struct Componente* inventario);
void limparBufferEntrada();

// Main
int main() {
	struct Componente *inventario;
    inventario = (struct Componente *)calloc(TAM_MOCHILA, sizeof(struct Componente));
    
	if (inventario == NULL) {
        printf("Erro na alocacao de memoria!\n");
        return 1; // Sai do programa com erro
    }

	// Variaveis de controle
	int opcao;
	int opcaoSort;
	int bubbleStatus = 0;
	int quantidadeItens = 0;

	do {
		exibirMenu(quantidadeItens, bubbleStatus);
		scanf("%d", &opcao);
		limparBufferEntrada();

		switch (opcao) {
			case 1: // Adicionar
				inserirItem(inventario, &quantidadeItens);
				break;
			
			case 2: // Remover
				removerItem(&inventario, &quantidadeItens);
				break;
			
			case 3: // Listar
				listarItens(inventario, quantidadeItens);
				break;
			
			case 4: // Organizar
				exibirMenuSort();
				scanf("%d", &opcaoSort);
				limparBufferEntrada();
					switch (opcaoSort) {
						case 1: // Bubble (Nome)
							bubbleSortPorNome(inventario, quantidadeItens);
							bubbleStatus = 1;
							break;
						case 2: // Insertion (Tipo)
							insertionSortPorTipo(inventario, quantidadeItens);
							bubbleStatus = 0;
							break;
						case 3: // Selection (Prioridade)
							selectionSortPorPrioridade(inventario, quantidadeItens);
							bubbleStatus = 0;
							break;
						default:
							printf("Opcao invalida. Tente novamente.");
							break;
					}
				break;

			case 5: // Buscar
				buscarItem(inventario, quantidadeItens, bubbleStatus);
				break;
			
			case 0: // Sair
				printf("Encerrando o jogo...\n");
				break;
			
			default:
				printf("Opcao invalida. Tente novamente.");
				break;
		}

		if (opcao != 0) {
			printf("\nPressione ENTER para continuar...");
        	getchar();

			if (opcao != 3) {
				listarItens(inventario, quantidadeItens);
			}
		}

	} while (opcao != 0);

	liberarmemoria(inventario);

	return 0;
}

/// @brief Exibe o menu principal
/// @param inventario Necessario para contagem
/// @param bubbleStatus Estado da ordenação por bolha (nome)
void exibirMenu(int quantidadeItens, int bubbleStatus) {
    printf("\n=============================================\n");
    printf("  MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
    printf("=============================================\n");
    printf("Itens na Mochila: %d/%d\n", quantidadeItens, TAM_MOCHILA);
    printf("Status da Ordenação por Nome: %s\n\n", checarBubbleSort(bubbleStatus));

	printf("1. Adicionar Componente (Loot)\n");
	printf("2. Descartar Componente\n");
	printf("3. Listar Componentes (Inventario)\n");
	printf("4. Organizar Mochila (Ordenar Componentes)\n");
	printf("5. Buscar Binaria por Componente-Chave (por nome)\n");
	printf("0. ATIVAR TORRE DE FUGA (Sair)\n");
	printf("---------------------------------------------\n");
	printf("Escolha uma opcao: ");
}

/// @brief Insere um item na mochila se houver espaco
/// @param inventario A mochila em questão
/// @param quantidadeItens A posicao que o item ocupará, também o contador de itens para display
void inserirItem(struct Componente* inventario, int* quantidadeItens) {
    if (*quantidadeItens >= TAM_MOCHILA) {
        printf("Inventario cheio! Nao e possivel inserir mais componentes.\n");
        return;
    }

    printf("\n--- Coletando Novo Componente ---\n");

    printf("Nome: ");
    fgets(inventario[*quantidadeItens].nome, TAM_NOME, stdin);
    inventario[*quantidadeItens].nome[strcspn(inventario[*quantidadeItens].nome, "\n")] = 0;

    printf("Tipo (Estrutural, Eletronico, Energia): ");
    fgets(inventario[*quantidadeItens].tipo, TAM_TIPO, stdin);
    inventario[*quantidadeItens].tipo[strcspn(inventario[*quantidadeItens].tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &inventario[*quantidadeItens].quantidade);
    limparBufferEntrada();

    printf("Prioridade de Montagem (1-5): ");
    scanf("%d", &inventario[*quantidadeItens].prioridade);
    limparBufferEntrada();

    printf("\nComponente '%s' adicionado com sucesso!\n\n", inventario[*quantidadeItens].nome);

    (*quantidadeItens)++; // incrementa o contador
}

/// @brief Remove um item da mochila
/// @param inventario O começo da lista
void removerItem(struct Componente** inventario, int *quantidadeItens) {
	char* nomeAlvo;
	int i = 0;

	printf("Digite o nome do item a ser removido: ");
	fgets(nomeAlvo, TAM_NOME, stdin);
	nomeAlvo[strcspn(nomeAlvo, "\n")] = '\0';

	for (inventario[i]->nome; i <= sizeof(inventario); i++) {
        if (strcmp(inventario[i]->nome, nomeAlvo) == 0) {

            free(inventario[i]);
			printf("Componente '%s' removido com sucesso!\n", nomeAlvo);

			(*quantidadeItens)--;
            return;
		}
	}

    printf("Componente '%s' não encontrado.\n", nomeAlvo);
	return;
}

/// @brief Lista os itens da mochila
/// @param inventario O começo da lista
void listarItens(struct Componente* inventario, int quantidadeItens) {
    printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", quantidadeItens, TAM_MOCHILA);
    printf("-----------------------------------------------------------------------------\n");
    printf("%-*s| %-*s| %-11s| %s\n", TAM_NOME, "NOME", TAM_TIPO, "TIPO", "QUANTIDADE", "PRIORIDADE");
    printf("-----------------------------------------------------------------------------\n");

    if (quantidadeItens == 0) {
        printf("A mochila está vazia.\n");
        return;
    }

    for (int i = 0; i < quantidadeItens; i++) {
        printf("%-*s| %-*s| %-11d| %d\n", TAM_NOME, inventario[i].nome, TAM_TIPO, inventario[i].tipo, inventario[i].quantidade, inventario[i].prioridade);
    printf("-----------------------------------------------------------------------------\n");
    }
}

/// @brief Busca itens por nome
/// @param inventario O começo da lista
void buscarItem(struct Componente* inventario, int quantidadeItens, int bubbleStatus) {
    if (quantidadeItens == 0) {
        printf("A mochila está vazia. Nenhum item para buscar.\n");
        return;
    }

    if (bubbleStatus == 0) {
        printf("ALERTA: A busca binária requer que a mochila esteja ordenada por NOME.\n");
        printf("Use a opção 4 para organizar a mochila primeiro.\n\n");
        return;
    }

    char nomeAlvo[TAM_NOME];
    printf("\n--- Busca Binária por Componente-Chave ---\n");
    printf("Nome do componente a buscar: ");
    fgets(nomeAlvo, TAM_NOME, stdin);
    nomeAlvo[strcspn(nomeAlvo, "\n")] = '\0';

    int inicio = 0;
    int fim = quantidadeItens - 1;
    int encontrado = -1;
	int etapa = 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int comparacao = strcmp(inventario[meio].nome, nomeAlvo);

        if (comparacao == 0) {
            encontrado = meio;
            break;
        } else if (comparacao < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
		(etapa)++;
    }

	if (encontrado != -1) {
		printf("\n--- Componente-Chave Encontrado! ---\n");
		printf("Nome: %s, Tipo: %s, Qtd: %d, Prio: %d\n", inventario[encontrado].nome, inventario[encontrado].tipo, inventario[encontrado].quantidade, inventario[encontrado].prioridade);
	}
	else {
	printf("\nResultado: Componente '%s' NAO foi encontrado na mochila.\n", nomeAlvo);
	}
	printf("Etapas percorridas: %d", etapa);
}

void exibirMenuSort() {
	printf("--- Estrategia de Organizacao ---\n");
	printf("Como deseja ordenar os componentes?\n");
	printf("1. Por Nome (Ordem Alfabetica)\n");
	printf("2. Por Tipo\n");
	printf("3. Por Prioridade de Montagem\n");
	printf("0. Cancelar\n");
	printf("Escolha o critério: ");
}

/// @brief Transforma int em string para o menu
/// @param bubbleStatus O estado da ordenação por bolha (nome)
const char* checarBubbleSort(int bubbleStatus) {
	if (bubbleStatus == 1) {
		return "ORDENADO";
	}
	return "NAO ORDENADO";
}

void bubbleSortPorNome(struct Componente* inventario, int quantidadeItens) {
    clock_t inicio = clock();

    for (int i = 0; i < quantidadeItens - 1; i++) {
        for (int j = 0; j < quantidadeItens - i - 1; j++) {
            if (strcmp(inventario[j].nome, inventario[j + 1].nome) > 0) {
                struct Componente temp = inventario[j];
                inventario[j] = inventario[j + 1];
                inventario[j + 1] = temp;
            }
        }
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Bubble Sort por Nome concluído em %.6f segundos.\n", tempo);
}

void insertionSortPorTipo(struct Componente* inventario, int quantidadeItens) {
    clock_t inicio = clock();

    for (int i = 1; i < quantidadeItens; i++) {
        struct Componente chave = inventario[i];
        int j = i - 1;

        while (j >= 0 && strcmp(inventario[j].tipo, chave.tipo) > 0) {
            inventario[j + 1] = inventario[j];
            j--;
        }
        inventario[j + 1] = chave;
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Insertion Sort por Tipo concluído em %.6f segundos.\n", tempo);
}

void selectionSortPorPrioridade(struct Componente* inventario, int quantidadeItens) {
    clock_t inicio = clock();

    for (int i = 0; i < quantidadeItens - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < quantidadeItens; j++) {
            if (inventario[j].prioridade > inventario[maxIndex].prioridade) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            struct Componente temp = inventario[i];
            inventario[i] = inventario[maxIndex];
            inventario[maxIndex] = temp;
        }
    }

    clock_t fim = clock();
    double tempo = ((double)(fim - inicio)) / CLOCKS_PER_SEC;
    printf("Selection Sort por Prioridade concluído em %.6f segundos.\n", tempo);
}

/// @brief Libera a memoria alocada
/// @param inventario A struct a ser liberada
void liberarmemoria(struct Componente* inventario) {
	free(inventario);
	printf("Memoria liberada com sucesso! Ate a proxima!");
}

/// @brief Limpa o \\n do scanf
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}