// ============================================================================
//         Desafio Detective Quest
// ============================================================================
// Karina Salvadeo Pereira - Engenharia de Software (Estrutura de Dados)       
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===================== Estrutura da Mansão (Árvore Binária) =====================
typedef struct Sala {
    char nome[50];            // Nome da sala
    char pista[50];           // Pista que pode estar na sala (ou vazia)
    struct Sala *esquerda;    // Ponteiro para sala à esquerda
    struct Sala *direita;     // Ponteiro para sala à direita
} Sala;

// Cria uma sala da mansão
Sala* criarSala(const char *nome, const char *pista) {
    Sala *nova = (Sala*) malloc(sizeof(Sala)); // Aloca memória para a sala
    strcpy(nova->nome, nome);                  // Define o nome da sala
    strcpy(nova->pista, pista);                // Define a pista (se existir)
    nova->esquerda = NULL;                     // Inicializa ponteiros
    nova->direita = NULL;
    return nova;                               // Retorna ponteiro da sala criada
}

// ===================== Estrutura da BST para Pistas =====================
typedef struct NodoBST {
    char pista[50];            // Nome da pista
    struct NodoBST *esq;       // Filho à esquerda
    struct NodoBST *dir;       // Filho à direita
} NodoBST;

// Cria um novo nodo da BST
NodoBST* novoNodoBST(const char *pista) {
    NodoBST *novo = (NodoBST*) malloc(sizeof(NodoBST));
    strcpy(novo->pista, pista); // Copia a string da pista
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

// Insere uma pista na BST (ordenada alfabeticamente)
NodoBST* inserir(NodoBST *raiz, const char *pista) {
    if (raiz == NULL) return novoNodoBST(pista); // Caso base: insere aqui
    if (strcmp(pista, raiz->pista) < 0) {        // Se for menor, vai à esquerda
        raiz->esq = inserir(raiz->esq, pista);
    } else if (strcmp(pista, raiz->pista) > 0) { // Se for maior, vai à direita
        raiz->dir = inserir(raiz->dir, pista);
    }
    return raiz; // Retorna a raiz (pode ter mudado apenas em níveis abaixo)
}

// Percorre a BST em ordem e imprime todas as pistas
void emOrdem(NodoBST *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("- %s\n", raiz->pista);
        emOrdem(raiz->dir);
    }
}

// ===================== Exploração da Mansão =====================
void explorarSalas(Sala *atual, NodoBST **pistas) {
    char escolha; // Armazena a escolha do jogador

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Se a sala contém uma pista, adiciona na BST
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: '%s'!\n", atual->pista);
            *pistas = inserir(*pistas, atual->pista);
            // Evita inserir a mesma pista mais de uma vez se voltar
            atual->pista[0] = '\0';
        }

        // Caso a sala seja um nó-folha
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho nesta sala.\n");
        }

        // Mostra opções
        printf("\nO que deseja fazer?\n");
        if (atual->esquerda != NULL) printf("  (e) Ir para a esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf("  (d) Ir para a direita -> %s\n", atual->direita->nome);
        printf("  (p) Ver todas as pistas coletadas\n");
        printf("  (s) Sair da exploração\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 'p') {
            printf("\n📜 Pistas coletadas até agora:\n");
            emOrdem(*pistas);
        } else if (escolha == 's') {
            printf("Você decidiu encerrar a exploração.\n");
            break;
        } else {
            printf("Opção inválida!\n");
        }
    }
}

// ===================== Função Principal =====================
int main() {
    // Construindo a mansão (árvore binária fixa)
    Sala *hall = criarSala("Hall de Entrada", "");
    Sala *salaEstar = criarSala("Sala de Estar", "Chave dourada");
    Sala *biblioteca = criarSala("Biblioteca", "Página rasgada");
    Sala *cozinha = criarSala("Cozinha", "");
    Sala *jardim = criarSala("Jardim", "Pegadas misteriosas");
    Sala *quarto = criarSala("Quarto Secreto", "Diário antigo");

    // Conexões
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    biblioteca->direita = quarto;

    // Raiz da BST de pistas
    NodoBST *pistas = NULL;

    // Inicia a exploração
    explorarSalas(hall, &pistas);

    // Liberação da memória das salas
    free(hall);
    free(salaEstar);
    free(biblioteca);
    free(cozinha);
    free(jardim);
    free(quarto);

    // Obs: aqui não implementei a liberação da BST de pistas (poderia ser feito com pós-ordem)

    return 0;
}
