#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQ_BIN "jogadores.dat"
#define ARQ_TXT "relatorio_jogadores.txt"

typedef struct {
    int id;
    char nick[40];
    char funcao[30];
    int pontos;
    int ativo;
} Jogador;

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int id_existe(int id) {
    FILE *f = fopen(ARQ_BIN, "rb");
    if (f == NULL) return 0;

    Jogador j;
    while (fread(&j, sizeof(Jogador), 1, f) == 1) {
        if (j.id == id && j.ativo == 1) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int contar_ativos() {
    FILE *f = fopen(ARQ_BIN, "rb");
    if (f == NULL) return 0;

    Jogador j;
    int total = 0;
    while (fread(&j, sizeof(Jogador), 1, f) == 1) {
        if (j.ativo == 1) {
            total++;
        }
    }
    fclose(f);
    return total;
}

void cadastrar() {
    Jogador j;
    printf("\n--- NOVO CADASTRO ---\n");
    printf("ID do jogador: ");
    scanf("%d", &j.id);
    limpar_buffer();

    if (id_existe(j.id)) {
        printf("Erro: Ja existe um jogador ativo com esse ID.\n");
        return;
    }

    printf("Nick: ");
    fgets(j.nick, sizeof(j.nick), stdin);
    j.nick[strcspn(j.nick, "\n")] = '\0';

    printf("Funcao / Rota: ");
    fgets(j.funcao, sizeof(j.funcao), stdin);
    j.funcao[strcspn(j.funcao, "\n")] = '\0';

    printf("Pontuacao: ");
    scanf("%d", &j.pontos);
    limpar_buffer();

    j.ativo = 1;

    FILE *f = fopen(ARQ_BIN, "ab");
    if (f == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    fwrite(&j, sizeof(Jogador), 1, f);
    fclose(f);
    printf("Jogador cadastrado!\n");
}

void consultar() {
    int id_busca, achou = 0;
    printf("\n--- CONSULTAR POR CODIGO ---\n");
    printf("Digite o ID: ");
    scanf("%d", &id_busca);
    limpar_buffer();

    FILE *f = fopen(ARQ_BIN, "rb");
    if (f == NULL) {
        printf("Nenhum registro encontrado.\n");
        return;
    }

    Jogador j;
    while (fread(&j, sizeof(Jogador), 1, f) == 1) {
        if (j.id == id_busca && j.ativo == 1) {
            printf("\nID: %d\n", j.id);
            printf("Nick: %s\n", j.nick);
            printf("Funcao: %s\n", j.funcao);
            printf("Pontos: %d\n", j.pontos);
            achou = 1;
            break;
        }
    }

    if (!achou) {
        printf("Jogador nao encontrado ou inativo.\n");
    }
    fclose(f);
}

void excluir() {
    int id_del, achou = 0;
    printf("\n--- EXCLUIR REGISTRO ---\n");
    printf("Digite o ID para remover: ");
    scanf("%d", &id_del);
    limpar_buffer();

    FILE *f = fopen(ARQ_BIN, "r+b");
    if (f == NULL) {
        printf("Arquivo nao encontrado.\n");
        return;
    }

    Jogador j;
    while (fread(&j, sizeof(Jogador), 1, f) == 1) {
        if (j.id == id_del && j.ativo == 1) {
            j.ativo = 0;
            fseek(f, -(long)sizeof(Jogador), SEEK_CUR);
            fwrite(&j, sizeof(Jogador), 1, f);
            achou = 1;
            printf("Registro removido com sucesso.\n");
            break;
        }
    }

    if (!achou) {
        printf("ID nao encontrado.\n");
    }
    fclose(f);
}

void exportar_txt() {
    FILE *fb = fopen(ARQ_BIN, "rb");
    if (fb == NULL) {
        printf("Sem dados para exportar.\n");
        return;
    }

    FILE *ft = fopen(ARQ_TXT, "w");
    if (ft == NULL) {
        printf("Erro ao criar arquivo texto.\n");
        fclose(fb);
        return;
    }

    fprintf(ft, "=== LISTA DE JOGADORES ATIVOS ===\n\n");
    Jogador j;
    int qtd = 0;

    while (fread(&j, sizeof(Jogador), 1, fb) == 1) {
        if (j.ativo == 1) {
            fprintf(ft, "ID: %d | Nick: %s | Funcao: %s | Pontos: %d\n",
                    j.id, j.nick, j.funcao, j.pontos);
            qtd++;
        }
    }

    fprintf(ft, "\nTotal de registros ativos: %d\n", qtd);
    fclose(fb);
    fclose(ft);
    printf("Exportado para '%s' com sucesso (%d registros).\n", ARQ_TXT, qtd);
}

int main() {
    int op;

    do {
        printf("\n=====================================\n");
        printf("  GERENCIAMENTO DE EQUIPE E-SPORTS\n");
        printf("  Registros ativos no sistema: %d\n", contar_ativos());
        printf("=====================================\n");
        printf("1. Cadastrar jogador\n");
        printf("2. Consultar por codigo (ID)\n");
        printf("3. Excluir registro\n");
        printf("4. Exportar para arquivo TXT\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &op);
        limpar_buffer();

        switch (op) {
            case 1:
                cadastrar();
                break;
            case 2:
                consultar();
                break;
            case 3:
                excluir();
                break;
            case 4:
                exportar_txt();
                break;
            case 0:
                printf("Encerrando o sistema...\n");
                break;
            default:
                printf("Opcao invalida.\n");
        }
    } while (op != 0);

    return 0;
}
