// ============================================================================
//         Desafio Detective Quest
// ============================================================================
// Karina Salvadeo Pereira - Engenharia de Software (Estrutura de Dados)       
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa cada sala da mansão (nó da árvore binária)
typedef struct Sala {
    char nome[50];            // Nome da sala
    struct Sala *esquerda;    // Ponteiro para sala à esquerda
    struct Sala *direita;     // Ponteiro para sala à direita
} Sala;

// Função que cria uma nova sala dinamicamente
Sala* criarSala(const char *nome) {
    Sala *nova = (Sala*) malloc(sizeof(Sala)); // Aloca memória para a sala
    strcpy(nova->nome, nome);                  // Copia o nome para a sala
    nova->esquerda = NULL;                     // Inicializa ponteiro esquerdo como vazio
    nova->direita = NULL;                      // Inicializa ponteiro direito como vazio
    return nova;                               // Retorna ponteiro para a nova sala
}

// Função que permite o jogador explorar a mansão
void explorarSalas(Sala *atual) {
    char escolha; // Variável para armazenar a escolha do jogador

    // Enquanto houver uma sala válida
    while (atual != NULL) {
        printf("\nVocê está na sala: %s\n", atual->nome); // Exibe nome da sala atual

        // Caso a sala seja um nó-folha (sem caminhos)
        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho! Você não pode seguir adiante.\n");
            break; // Sai do laço pois não há para onde ir
        }

        // Mostra opções de movimentação
        printf("Escolha para onde ir:\n");
        if (atual->esquerda != NULL) printf("  (e) Ir para a esquerda -> %s\n", atual->esquerda->nome);
        if (atual->direita != NULL)  printf("  (d) Ir para a direita -> %s\n", atual->direita->nome);
        printf("  (s) Sair da exploração\n");
        printf("Digite sua escolha: ");
        scanf(" %c", &escolha); // Lê opção (ignora espaços)

        // Verifica a escolha do jogador
        if (escolha == 'e' && atual->esquerda != NULL) {
            atual = atual->esquerda; // Move para a esquerda
        } else if (escolha == 'd' && atual->direita != NULL) {
            atual = atual->direita;  // Move para a direita
        } else if (escolha == 's') {
            printf("Você decidiu sair da exploração.\n");
            break; // Sai do jogo
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }
    }
}

int main() {
    // Construção manual da mansão (árvore binária fixa)

    Sala *hall = criarSala("Hall de Entrada"); // Raiz da árvore
    Sala *salaEstar = criarSala("Sala de Estar");
    Sala *biblioteca = criarSala("Biblioteca");
    Sala *cozinha = criarSala("Cozinha");
    Sala *jardim = criarSala("Jardim");
    Sala *quarto = criarSala("Quarto Secreto");

    // Conectando as salas (definindo caminhos)
    hall->esquerda = salaEstar;
    hall->direita = biblioteca;
    salaEstar->esquerda = cozinha;
    salaEstar->direita = jardim;
    biblioteca->direita = quarto;

    // Inicia a exploração a partir do Hall
    explorarSalas(hall);

    // Liberação de memória (boa prática)
    free(hall);
    free(salaEstar);
    free(biblioteca);
    free(cozinha);
    free(jardim);
    free(quarto);

    return 0; // Fim do programa
}
