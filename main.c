#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_MOCHILA 10
#define TAM_NOME 30
#define TAM_TIPO 20

struct Item {
	char nome[TAM_NOME];
	char tipo[TAM_TIPO];
	int quantidade;
	struct Item* proximo;
};

void exibirMenu(struct Item* inicio);
void inserirItem(struct Item** inicio);
void removerItem(struct Item** inicio);
void listarItens(struct Item* inicio);
int contarItens(struct Item* inicio);
void buscarItem(struct Item* inicio);
void liberarmemoria(struct Item* inicio);
void limparBufferEntrada();

// Main
int main() {
	// Variaveis de controle
	int opcao;
	struct Item* inicio = NULL;

	do {
		exibirMenu(inicio);
		scanf("%d", &opcao);
		limparBufferEntrada();

		switch (opcao)
		{
		case 1: // Adicionar
			inserirItem(&inicio);
			break;
		
		case 2: // Remover
			removerItem(&inicio);
			break;
		
		case 3: // Listar
			listarItens(inicio);
			break;
		
		case 4:
			buscarItem(inicio);
			break;
		
		case 0: // Sair
			printf("Encerrando o jogo...\n");
			break;
		
		default:
			break;
		}

		if (opcao != 0 && opcao != 3 && opcao != 4) {
			listarItens(inicio);
		}

		if (opcao != 0) {
			printf("\nPressione ENTER para continuar...");
        	getchar();
		}

	} while (opcao != 0);

	liberarmemoria(inicio);

	return 0;
}

/// @brief Exibe o menu principal
/// @param inicio O começo da lista
void exibirMenu(struct Item* inicio) {
	printf("\n=============================================\n");
	printf("  MOCHILA DE SOBREVIVENCIA - CODIGO DA ILHA\n");
	printf("=============================================\n");
	printf("Itens na Mochila: %d/%d\n\n", contarItens(inicio), TAM_MOCHILA);

	printf("1. Adicionar Item (Loot)\n");
	printf("2. Remover Item\n");
	printf("3. Listar Itens na Mochila\n");
	printf("4. Buscar Item por Nome\n");
	printf("0. Sair\n");
	printf("---------------------------------------------\n");
	printf("Escolha uma opcao: ");
}

/// @brief Insere um item na mochila se houver espaco
/// @param inicio O começo da lista
void inserirItem(struct Item** inicio) {
	if (contarItens(*inicio) == TAM_MOCHILA) {
		printf("Mochila cheia! Nao e possivel inserir mais itens.\n");
		return;
	}

	char nome[TAM_NOME];
	char tipo[TAM_TIPO];
	int quantidade;

	printf("\n--- Adicionar Novo Item ---\n");
	printf("Nome do item: ");
	fgets(nome, TAM_NOME, stdin);
	printf("Tipo do item: ");
	fgets(tipo, TAM_TIPO, stdin);
	printf("Quantidade: ");
	scanf("%d", &quantidade);
	limparBufferEntrada();
	printf("\n");

	nome[strcspn(nome, "\n")] = '\0';
    tipo[strcspn(tipo, "\n")] = '\0';

	struct Item* novo = (struct Item*) malloc(sizeof(struct Item));
	if (novo == NULL) {
		printf("Erro: Falha ao alocar memória.\n");
		return;
	}
	strncpy(novo->nome, nome, TAM_NOME - 1);
    novo->nome[TAM_NOME - 1] = '\0';
    strncpy(novo->tipo, tipo, TAM_TIPO - 1);
    novo->tipo[TAM_TIPO - 1] = '\0';
	novo->quantidade = quantidade;
    novo->proximo = *inicio;
    *inicio = novo;

	printf("Item '%s' adicionado com sucesso!\n\n", nome);
	return;
}

/// @brief Remove um item da mochila
/// @param inicio O começo da lista
void removerItem(struct Item** inicio) {
    struct Item* atual = *inicio;
    struct Item* anterior = NULL;
	char* nomeAlvo;

	printf("Digite o nome do item a ser removido: ");
	fgets(nomeAlvo, TAM_NOME, stdin);
	nomeAlvo[strcspn(nomeAlvo, "\n")] = '\0';

    while (atual != NULL) {
        if (strcmp(atual->nome, nomeAlvo) == 0) {
            if (anterior == NULL) { // primeiro item
                *inicio = atual->proximo;
            } else { // meio ou fim
                anterior->proximo = atual->proximo;
            }

            free(atual);
			printf("Item '%s' removido com sucesso!\n", nomeAlvo);
            return;
        }

        anterior = atual;
        atual = atual->proximo;
    }

    printf("Item '%s' não encontrado.\n", nomeAlvo);
}

/// @brief Lista os itens da mochila
/// @param inicio O começo da lista
void listarItens(struct Item* inicio) {
    struct Item* atual = inicio;

	printf("\n--- ITENS NA MOCHILA (%d/%d) ---\n", contarItens(inicio), TAM_MOCHILA);
	printf("----------------------------------------------------------------\n");
	printf("%-*s| %-*s| %s\n", TAM_NOME, "NOME", TAM_TIPO, "TIPO", "QUANTIDADE");
	printf("----------------------------------------------------------------\n");

	if (atual == NULL) {
		printf("A mochila está vazia.\n");
		return;
	}

    while (atual != NULL) {
        printf("%-*s| %-*s| %d\n", TAM_NOME, atual->nome, TAM_TIPO, atual->tipo,atual->quantidade);
		printf("----------------------------------------------------------------\n");
        atual = atual->proximo;
    }
}

/// @brief Conta os itens da mochila
/// @param inicio O começo da lista
/// @return A quantidade de itens dentro da mochila
int contarItens(struct Item* inicio) {
	struct Item* atual = inicio;
	int contagem = 0;

	while (atual != NULL) {
		contagem = contagem + 1;
		atual = atual->proximo;
	}
	return contagem;
}

/// @brief Busca itens por nome
/// @param inicio O começo da lista
void buscarItem(struct Item* inicio) {
	struct Item* atual = inicio;
	char nomeAlvo[TAM_NOME];
	int encontrado = 0;

	//printf("Resultado: Item '' NAO foi encontrado na mochila.\n");

	printf("Digite o nome do item a ser buscado: ");
	fgets(nomeAlvo, TAM_NOME, stdin);
	nomeAlvo[strcspn(nomeAlvo, "\n")] = '\0';

	while (atual != NULL) {
		if (strcmp(atual->nome, nomeAlvo) == 0) {
			printf("\n--- Item Encontrado! ---\n");
			printf("Nome: %s\n", atual->nome);
			printf("Tipo: %s\n", atual->tipo);
			printf("Quantidade: %d\n", atual->quantidade);
			printf("-------------------------\n");
			encontrado = 1;
		}
		atual = atual->proximo;
	}

	if (!encontrado) {
		printf("\nResultado: Item '%s' NAO foi encontrado na mochila.\n", nomeAlvo);
	}
}

/// @brief Libera a memoria alocada
/// @param inicio O começo da lista
void liberarmemoria(struct Item* inicio) {
	struct Item* atual = inicio;
	struct Item* temp;

	while (atual != NULL) {
		temp = atual;
		atual = atual->proximo;
		free(temp);
	}
	printf("Memoria liberada com sucesso! Ate a proxima!");
}

/// @brief Limpa o \\n do scanf
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}