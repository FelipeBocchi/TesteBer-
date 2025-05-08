#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
typedef struct{
    int id; //4
    char nome[50]; //50
    float preco; //8
    int quantidade; //4
} Produto;


void cadastrarProduto(Produto p){
    printf("\nID: ");
    scanf("%d", &p.id);
    getchar();
    printf("\nNome: ");
    fgets(p.nome, 50, stdin);
    printf("Preco: ");
    scanf("%f", &p.preco);
    printf("\nQuantidade: ");
    scanf("%d", &p.quantidade);
}

void mostrarProduto(Produto p){
    printf("\n%d", p.id);
    printf("\n%s", p.nome);
    printf("\n%.2f", p.preco);
    printf("\n%d", p.quantidade);
}


int main(){

    int n;
    printf("Quantos produtos quer: ");
    scanf("%d", &n);
    getchar();

    Produto *estoque = malloc(n * sizeof(Produto));

    for(int i=0;i<n;i++){
        printf("\nProduto #%d : \n", i+1);
        cadastrarProduto(*estoque);
    }

    printf("\n=-=-=-ESTOQUE-=-=-=\n");
    for(int i=0;i<n;i++){
        mostrarProduto(*estoque);
    }


    free(estoque);
    return 0;
}
*/

/*                                                  PRIMEIRA TENTATIVA DE MALLOC NO MERCADO
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    int id; //4
    char nome[50]; //50
    float preco; //8
    int quantidade; //4
} Produto;

void redefinir_tamanho(Produto **pt, int tam)
{
    *pt = malloc(tam * sizeof(Produto));
}

void cadastrarProduto(Produto *pn){
    printf("ID: ");
    scanf("%d", &pn->id);
    getchar();

    printf("Nome: ");
    scanf(" %[^\n]", pn->nome);
    getchar();

    printf("Quantidade: ");
    scanf("%d", &pn->quantidade);
    getchar();

    printf("Preco: ");
    scanf("%f", &pn->preco);
    getchar();
}

void mostrarProduto(const Produto *pn){
    printf("ID: %d // Nome: %s // Quantidade: %d // Preco: %.2f\n",
            pn->id, pn->nome, pn->quantidade, pn-> preco);
}

int main(){

    int n=1;
    char opcao;

    //printf("Quantos produtos quer: ");
    //scanf("%d", &n);
    //getchar();

    Produto *estoque;

    redefinir_tamanho(&estoque, n);

    do
    {
        for(int i=0;i<n;i++){                           MEU ERRO FOI TER COLOCADO UM FOR ONDE NÃO PRECISAVA
            printf("\nProduto #%d : \n", i+1);
            cadastrarProduto(&estoque[i]);
            redefinir_tamanho(&estoque, n);
            n++;
            printf("Deseja sair (x):");
            scanf("%c", &opcao);
            if(opcao == 'x')
                n = i - 1;
        }

    } while (opcao != 'x');


    printf("\n=-=-=-ESTOQUE-=-=-=\n");
    n=1;
    for(int i=0;i<n;i++){
        mostrarProduto(&estoque[i]);
    }

    free(estoque);
    return 0;
}
*/

/*                                              SEGUNDA TENTATIVA, TROCANDO MALLOC POR REALLOC E TIRANDO O FOR
typedef struct {
    int id;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

void cadastrarProduto(Produto *pn) {
    printf("ID: ");
    scanf("%d", &pn->id);
    getchar();

    printf("Nome: ");
    scanf(" %[^\n]", pn->nome);
    getchar();

    printf("Quantidade: ");
    scanf("%d", &pn->quantidade);
    getchar();

    printf("Preco: ");
    scanf("%f", &pn->preco);
    getchar();
}

void mostrarProduto(const Produto *pn) {
    printf("ID: %d // Nome: %s // Quantidade: %d // Preco: %.2f\n",
           pn->id, pn->nome, pn->quantidade, pn->preco);
}

int main() {
    Produto *estoque = NULL;
    int n = 0;
    char opcao;

    do {
        Produto *temp = realloc(estoque, (n + 1) * sizeof(Produto));
        if (temp == NULL) {
            printf("Erro de alocação.\n");
            free(estoque);
            return 1;
        }
        estoque = temp;

        printf("\nProduto #%d :\n", n + 1);
        cadastrarProduto(&estoque[n]);
        n++;

        printf("Deseja adicionar outro produto? (s/n): ");
        scanf(" %c", &opcao);
        getchar();

    } while (opcao != 'n');

    printf("\n=-=-=- ESTOQUE -=-=-=\n");
    for (int i = 0; i < n; i++) {
        mostrarProduto(&estoque[i]);
    }

    free(estoque);
    return 0;
}
*/


typedef struct {
    int id;
    char nome[50];
    float preco;
    int quantidade;
} Produto;

#define ARQUIVO "produtos.txt"

Produto *carregarProdutos(int *n) {
    FILE *fp = fopen(ARQUIVO, "r");
    Produto *lista = NULL;
    Produto temp;
    *n = 0;

    if (!fp) return NULL;

    while (fscanf(fp, "%d\n%[^\n]\n%d\n%f\n", &temp.id, temp.nome, &temp.quantidade, &temp.preco) == 4) {
        Produto *novaLista = realloc(lista, (*n + 1) * sizeof(Produto));
        if (!novaLista) {
            printf("Erro de memória.\n");
            free(lista);
            fclose(fp);
            return NULL;
        }
        lista = novaLista;
        lista[*n] = temp;
        (*n)++;
    }

    fclose(fp);
    return lista;
}

void salvarProdutos(Produto *produtos, int n) {
    FILE *fp = fopen(ARQUIVO, "w");
    if (!fp) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d\n%s\n%d\n%.2f\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco);
    }

    fclose(fp);
}

void cadastrarProduto(Produto *pn) {
    printf("ID: ");
    scanf("%d", &pn->id);
    getchar();

    printf("Nome: ");
    scanf(" %[^\n]", pn->nome);
    getchar();

    printf("Quantidade: ");
    scanf("%d", &pn->quantidade);
    getchar();

    printf("Preco: ");
    scanf("%f", &pn->preco);
    getchar();
}

void mostrarProduto(const Produto *pn) {
    printf("ID: %d // Nome: %s // Quantidade: %d // Preco: %.2f\n",
           pn->id, pn->nome, pn->quantidade, pn->preco);
}

int main() {
    Produto *estoque = NULL;
    int n = 0;
    char opcao;

    estoque = carregarProdutos(&n);

    do {
        Produto *temp = realloc(estoque, (n + 1) * sizeof(Produto));
        if (!temp) {
            printf("Erro de memória.\n");
            free(estoque);
            return 1;
        }
        estoque = temp;

        printf("\nProduto #%d:\n", n + 1);
        cadastrarProduto(&estoque[n]);
        n++;

        salvarProdutos(estoque, n);  // Salva após cada cadastro

        printf("Deseja adicionar outro produto? (s/n): ");
        scanf(" %c", &opcao);
        getchar();

    } while (opcao != 'n');

    printf("\n=-=-=- ESTOQUE ATUAL -=-=-=\n");
    for (int i = 0; i < n; i++) {
        mostrarProduto(&estoque[i]);
    }

    free(estoque);
    return 0;
}
