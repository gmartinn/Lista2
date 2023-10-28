#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char artista[100];
    char musica[100];
    struct Node* prev;
    struct Node* next;
} Node;

Node* createNode(char* artista, char* musica) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->artista, artista);
    strcpy(newNode->musica, musica);
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

void insertNode(Node** head, char* artista, char* musica) {
    Node* newNode = createNode(artista, musica);
    if (*head == NULL) {
        *head = newNode;
        (*head)->next = *head;
        (*head)->prev = *head;
    } else {
        Node* last = (*head)->prev;
        newNode->next = *head;
        newNode->prev = last;
        last->next = newNode;
        (*head)->prev = newNode;
    }
    printf("Musica inserida com sucesso!\n");

    // Atualiza o arquivo de entrada com o novo registro
    FILE* file = fopen("musicas.txt", "a");
    if (file != NULL) {
        fprintf(file, "%s;%s\n", artista, musica);
        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo.\n");
    }
}

void removeNode(Node** head, char* musica) {
    if (*head == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }
    Node* curr = *head;
    Node* prevNode = NULL;
    int found = 0;
    do {
        if (strcmp(curr->musica, musica) == 0) {
            found = 1;
            break;
        }
        prevNode = curr;
        curr = curr->next;
    } while (curr != *head);

    if (found) {
        if (curr->next == curr) {
            *head = NULL;
        } else {
            prevNode->next = curr->next;
            curr->next->prev = prevNode;
            if (curr == *head) {
                *head = curr->next;
            }
        }
        free(curr);
        printf("Musica removida com sucesso!\n");

        // Atualiza o arquivo de entrada removendo o registro
        FILE* file = fopen("musicas.txt", "r");
        if (file != NULL) {
            FILE* tempFile = fopen("temp.txt", "w");
            if (tempFile != NULL) {
                char line[256];
                while (fgets(line, sizeof(line), file)) {
                    line[strcspn(line, "\n")] = '\0'; // Remove o caractere de nova linha
                    char artista[100];
                    char musica[100];
                    sscanf(line, "%[^;];%s", artista, musica);
                    if (strcmp(musica, curr->musica) != 0) {
                        fprintf(tempFile, "%s\n", line);
                    }
                }
                fclose(tempFile);
                fclose(file);
                remove("musicas.txt");
                rename("temp.txt", "musicas.txt");
            } else {
                printf("Erro ao abrir o arquivo temporario.\n");
            }
        } else {
            printf("Erro ao abrir o arquivo.\n");
        }
    } else {
        printf("Musica nao encontrada.\n");
    }
}

void displayPlaylistByOrder(Node* head) {
    if (head == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }
    Node* curr = head;
    printf("Playlist pela ordem de cadastro:\n");
    do {
        printf("Artista: %s | Musica: %s\n", curr->artista, curr->musica);
        curr = curr->next;
    } while (curr != head);
}

void displayPlaylistByMusic(Node* head) {
    if (head == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }
    Node* curr = head;
    printf("Playlist ordenada pelo nome das musicas:\n");

    int count = 0;
    Node** nodes = NULL;
    do {
        nodes = (Node**)realloc(nodes, (count + 1) * sizeof(Node*));
        nodes[count] = curr;
        count++;
        curr = curr->next;
    } while (curr != head);

    // Ordena a lista de nós pelo nome das músicas usando o algoritmo de classificação "Bubble Sort"
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(nodes[j]->musica, nodes[j+1]->musica) > 0) {
                Node* temp = nodes[j];
                nodes[j] = nodes[j+1];
                nodes[j+1] = temp;
            }
        }
    }

    // Exibe a playlist ordenada
    for (int i = 0; i < count; i++) {
        printf("Artista: %s | Musica: %s\n", nodes[i]->artista, nodes[i]->musica);
    }

    free(nodes);
}

Node* loadPlaylistFromFile() {
    Node* head = NULL;
    FILE* file = fopen("musicas.txt", "r");
    if (file != NULL) {
        char line[256];
        while (fgets(line, sizeof(line), file)) {
            line[strcspn(line, "\n")] = '\0'; // Remove o caractere de nova linha
            char artista[100];
            char musica[100];
            sscanf(line, "%[^;];%s", artista, musica);
            insertNode(&head, artista, musica);
        }
        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo.\n");
    }
    return head;
}

void searchMusic(Node* head, char* musica) {
    if (head == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }
    Node* curr = head;
    int found = 0;
    do {
        if (strcmp(curr->musica, musica) == 0) {
            found = 1;
            break;
        }
        curr = curr->next;
    } while (curr != head);

    if (found) {
        printf("Musica encontrada:\n");
        printf("Artista: %s | Musica: %s\n", curr->artista, curr->musica);
    } else {
        printf("Musica nao encontrada.\n");
    }
}

void playNext(Node** head) {
    if (*head == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }
    *head = (*head)->next;
    printf("Tocando musica:\n");
    printf("Artista: %s | Musica: %s\n", (*head)->artista, (*head)->musica);
}

void playPrevious(Node** head) {
    if (*head == NULL) {
        printf("A playlist esta vazia.\n");
        return;
    }
    *head = (*head)->prev;
    printf("Tocando musica:\n");
    printf("Artista: %s | Musica: %s\n", (*head)->artista, (*head)->musica);
}

void printMenu() {
    printf("\n--- MENU ---\n");
    printf("1. Exibir playlist pela ordem de cadastro\n");
    printf("2. Exibir playlist ordenada pelo nome das musicas\n");
    printf("3. Inserir nova musica\n");
    printf("4. Remover uma musica\n");
    printf("5. Buscar uma musica\n");
    printf("6. Avancar para a proxima musica\n");
    printf("7. Voltar para a musica anterior\n");
    printf("8. Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    Node* head = loadPlaylistFromFile();
    int choice;
    char musica[100];
    char artista[100];

    do {
        printMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayPlaylistByOrder(head);
                break;
            case 2:
                displayPlaylistByMusic(head);
                break;
            case 3:
                printf("Digite o nome do artista: ");
                scanf(" %[^\n]s", artista);
                printf("Digite o nome da musica: ");
                scanf(" %[^\n]s", musica);
                insertNode(&head, artista, musica);
                break;
            case 4:
                printf("Digite o nome da musica a ser removida: ");
                scanf(" %[^\n]s", musica);
                removeNode(&head, musica);
                break;
            case 5:
                printf("Digite o nome da musica a ser buscada: ");
                scanf(" %[^\n]s", musica);
                searchMusic(head, musica);
                break;
            case 6:
                playNext(&head);
                break;
            case 7:
                playPrevious(&head);
                break;
            case 8:
                printf("Encerrando o programa.\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }
    } while (choice != 8);

    // Libera a memória alocada pela lista
    Node* curr = head;
    Node* nextNode = NULL;
    if (curr != NULL) {
        do {
            nextNode = curr->next;
            free(curr);
            curr = nextNode;
        } while (curr != head);
    }

    return 0;
}