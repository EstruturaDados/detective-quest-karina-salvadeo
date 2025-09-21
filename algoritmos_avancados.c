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
    char pista[50];           // Pista encontrada na sala (se houver)
    char suspeito[50];        // Suspeito relacionado à pista
    struct Sala *esquerda;    // Caminho à esquerda
    struct Sala *direita;     // Caminho à direita
} Sala;

// Cria uma sala com nome, pista e suspeito
Sala* criarSala(const char *nome, const char *pista, const char *suspeito) {
    Sala *nova = (Sala*) malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    strcpy(nova->pista, pista);
    strcpy(nova->suspeito, suspeito);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

// ===================== Estrutura da BST para Pistas =====================
typedef struct NodoBST {
    char pista[50];
    struct NodoBST *esq;
    struct NodoBST *dir;
} NodoBST;

// Cria novo nodo na BST
NodoBST* novoNodoBST(const char *pista) {
    NodoBST *novo = (NodoBST*) malloc(sizeof(NodoBST));
    strcpy(novo->pista, pista);
    novo->esq = NULL;
    novo->dir = NULL;
    return novo;
}

// Insere pista na BST em ordem alfabética
NodoBST* inserirBST(NodoBST *raiz, const char *pista) {
    if (raiz == NULL) return novoNodoBST(pista);
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirBST(raiz->esq, pista);
    else if (strcmp(pista, raiz->pista) > 0)
        raiz->dir = inserirBST(raiz->dir, pista);
    return raiz;
}

// Percorre a BST em ordem
void emOrdem(NodoBST *raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esq);
        printf("- %s\n", raiz->pista);
        emOrdem(raiz->dir);
    }
}

// ===================== Estrutura da Tabela Hash =====================
#define TAM 10 // Tamanho fixo da tabela hash

typedef struct HashNode {
    char pista[50];             // Nome da pista
    char suspeito[50];          // Suspeito associado
    struct HashNode *prox;      // Encadeamento em caso de colisão
} HashNode;

HashNode* tabela[TAM]; // A tabela hash em si (vetor de ponteiros)

// Função hash simples (soma dos caracteres mod TAM)
int funcaoHash(const char *str) {
    int soma = 0;
    for (int i = 0; str[i] != '\0'; i++) soma += str[i];
    return soma % TAM;
}

// Insere pista → suspeito na hash
void inserirHash(const char *pista, const char *suspeito) {
    int idx = funcaoHash(pista);

    HashNode *novo = (HashNode*) malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[idx];  // Encadeia na lista
    tabela[idx] = novo;
}

// Mostra todas as pistas e seus suspeitos
void mostrarHash() {
    printf("\n📌 Pistas e suspeitos relacionados:\n");
    for (int i = 0; i < TAM; i++) {
        HashNode *atual = tabela[i];
        while (atual != NULL) {
            printf("- %s → %s\n", atual->pista, atual->suspeito);
            atual = atual->prox;
        }
    }
}

// Retorna o suspeito mais citado na hash
void suspeitoMaisCitado() {
    char suspeitos[50][50]; // Lista simples de nomes
    int contagem[50];       // Contador de ocorrências
    int total = 0;

    // Percorre a hash coletando suspeitos
    for (int i = 0; i < TAM; i++) {
        HashNode *atual = tabela[i];
        while (atual != NULL) {
            int encontrado = 0;
            for (int j = 0; j < total; j++) {
                if (strcmp(suspeitos[j], atual->suspeito) == 0) {
                    contagem[j]++;
                    encontrado = 1;
                    break;
                }
            }
            if (!encontrado) {
                strcpy(suspeitos[total], atual->suspeito);
                contagem[total] = 1;
                total++;
            }
            atual = atual->prox;
        }
    }

    // Descobre o mais citado
    int max = 0;
    char culpado[50];
    strcpy(culpado, "Ninguém");
    for (int i = 0; i < total; i++) {
        if (contagem[i] > max) {
            max = contagem[i];
            strcpy(culpado, suspeitos[i]);
        }
    }

    printf("\n🕵️ Suspeito mais citado: %s (%d pistas)\n", culpado, max);
}

// ===================== Exploração da Mansão =====================
void explorarSalas(Sala *atual, NodoBST **pistas) {
    char escolha;

    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome);

        // Se a sala contém uma pista
        if (strlen(atual->pista) > 0) {
            printf("Você encontrou uma pista: '%s' (Suspeito: %s)\n", atual->pista, atual->suspeito);
            *pistas = inserirBST(*pistas, atual->pista);
            inserirHash(atual->pista, atual->suspeito);
            atual->pista[0] = '\0'; // Evita coletar a mesma pista de novo
        }

        // Mostra opções
        printf("\nO que deseja fazer?\n");
        if (atual->esquerda != NULL) printf("  (e) Ir para a esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf("  (d) Ir para a direita -> %s\n", atual->direita->nome);
        printf("  (p) Ver todas as pistas coletadas (ordem alfabética)\n");
        printf("  (h) Ver suspeitos relacionados às pistas\n");
        printf("  (f) Finalizar investigação e ver suspeito mais citado\n");
        printf("  (s) Sair da exploração\n");
        printf("Escolha: ");
        scanf(" %c", &escolha);

        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda;
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;
        } else if (escolha == 'p') {
            printf("\n📜 Pistas coletadas:\n");
            emOrdem(*pistas);
        } else if (escolha == 'h') {
            mostrarHash();
        } else if (escolha == 'f') {
            suspeitoMaisCitado();
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
    // Inicializa a hash
    for (int i = 0; i < TAM; i++) tabela[i] = NULL;

    // Constrói a mansão
    Sala *hall = criarSala("Hall de Entrada", "", "");
    Sala *salaEstar = criarSala("Sala de Estar", "Chave dourada", "Sr. Black");
    Sala *biblioteca = criarSala("Biblioteca", "Página rasgada", "Sra. White");
    Sala *cozinha = criarSala("Cozinha", "", "");
    Sala *jardim = criarSala("Jardim", "Pegadas misteriosas", "Srta. Green");
    Sala *quarto = criarSala("Quarto Secreto", "Diário antigo", "Coronel Mustard");

    // Ligações
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    biblioteca->direita = quarto;

    // Raiz da BST de pistas
    NodoBST *pistas = NULL;

    // Inicia exploração
    explorarSalas(hall, &pistas);

    return 0;
}
