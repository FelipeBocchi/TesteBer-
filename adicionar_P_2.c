//                     SEGUNDA VERÇÃO DO ADICIONAR pRODUTO, PORÉM SIMPLIFICADO;
/*
    Ao cadastrar os produtos, a Dona Berê deseja guardar: um código para o produto, a descrição
do produto, a categoria (Alimento, Material de Limpeza, Panificação), preço de compra, a
margem de lucro (em percentual a ser calculado como acréscimo ao valor da compra gerando
o valor da venda), preço de venda (deseja que seja calculado no ato da venda em decorrência
da necessidade de modificar a margem de lucro ao longo do tempo), quantidade em estoque e
o estoque mínimo.
Regras aos Produtos:
1. No ato da venda, caso o estoque mínimo tenha sido alcançado, dona Berê
deseja receber uma mensagem de alerta do sistema;
2. No ato da venda, caso a quantidade de itens a serem comprado pelo cliente
seja maior que a quantidade presente em estoque dona Berê deseja receber
uma mensagem de alerta do sistema. Por exemplo: “Você deseja vender 4
produtos. Há somente 3 em estoque! Continuar (s/n)?;
3. No ato da venda, não é possível vender produtos com estoque ZERADO. dona
Berê deseja receber uma mensagem de alerta do sistema quando isso ocorrer
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define max_caracter 100      //max de caracter para a descrição
#define ARQUIVO "cadastrar.txt"  //renomeando o arquivo que vai ser valvo

typedef struct
{
    int id;
    char descricao_Produto[max_caracter];
    int categoria_Produto;
    float Preco_De_Compra;
    float Preco_De_Venda;
    int Quantidade_em_Estoque;
    int estoque_Minimo;
}Cadastrar_Produtos;

void pular_Linha()
{
    printf("\n");
}
void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt);
void salvar_Produtos_Arquivo(Cadastrar_Produtos *pt, FILE *arquivo);
void listar_Produtos();

int main()
{
    setlocale(LC_ALL, "Portuguese");

    int opcao;
    Cadastrar_Produtos *pt_Produtos = NULL;
    int quantidade = 0;

    do
    {
        printf("\n======= MENU =======\n");
        printf("1. Cadastrar Produto\n");
        printf("2. Listar Produtos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // Limpa buffer

        switch(opcao)
        {
            case 1:
                //Aqui usei realloc para sempre termos memória e espaço no "deposito" e não passar por cima do estoque antigo
                pt_Produtos = realloc(pt_Produtos, (quantidade + 1) * sizeof(Cadastrar_Produtos));
                if(pt_Produtos == NULL)
                {
                    printf("erro ao alocar memória!!!\n");
                    return  1;
                }

                cadastrar_Informacao_Produtos(&pt_Produtos[quantidade]);

                FILE *arquivo = fopen(ARQUIVO, "a+");
                if(arquivo == NULL)
                {
                    printf("ERRO AO ABRIR O ARQUIVO!!!\n");
                    return 1;
                }

                salvar_Produtos_Arquivo(&pt_Produtos[quantidade], arquivo);
                fclose(arquivo);

                quantidade++;
                break;

            case 2:
                listar_Produtos();
                break;

            case 3:
                printf("SAINDA DA PROGRAMA...\n");
                break;

            default:
                printf("Opcao invalida!!!\n");
                break;
        }

    }while(opcao != 0);

    free(pt_Produtos);
    return 0;
}

void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt)
{
 printf("\n-- Cadastrar Produto --\n");

    printf("ID: ");
    scanf("%d", &pt->id);
    getchar();
    pular_Linha();

    printf("Descrição: ");
    fgets(pt->descricao_Produto, max_caracter, stdin);
    strtok(pt->descricao_Produto, "\n"); // Remove o \n do final
    pular_Linha();

    printf("Categoria (1-Alimentos, 2-Limpeza, 3-Panificação): ");
    scanf("%d", &pt->categoria_Produto);
    pular_Linha();

    printf("Preço de Compra: ");
    scanf("%f", &pt->Preco_De_Compra);
    pular_Linha();

    printf("Preço de Venda: ");
    scanf("%f", &pt->Preco_De_Venda);
    pular_Linha();

    printf("Quantidade em Estoque: ");
    scanf("%d", &pt->Quantidade_em_Estoque);
    pular_Linha();

    printf("Estoque Mínimo: ");
    scanf("%d", &pt->estoque_Minimo);
    pular_Linha();
}

void salvar_Produtos_Arquivo(Cadastrar_Produtos *pt, FILE *arquivo)
{
    fprintf(arquivo, "%d;%s;%d;%.2f;%.2f;%d;%d\n",
        pt->id,
        pt->descricao_Produto,
        pt->categoria_Produto,

        pt->Preco_De_Compra,
        pt->Preco_De_Venda,
        pt->Quantidade_em_Estoque,
        pt->estoque_Minimo);
}

void listar_Produtos()
{
    FILE *arquivo = fopen(ARQUIVO, "r");
    if(arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!!!\n");
        return ;
    }

    printf("\n-- Produtos Cadastrados --\n");

    Cadastrar_Produtos temp;
    while (fscanf(arquivo, "%d;%[^;];%d;%f;%f;%d;%d\n",
                &temp.id,
                temp.descricao_Produto,
                &temp.categoria_Produto,
                &temp.Preco_De_Compra,
                &temp.Preco_De_Venda,
                &temp.Quantidade_em_Estoque,
                &temp.estoque_Minimo) != EOF)
    {

        printf("ID: %d\n", temp.id);
        printf("Descrição: %s\n", temp.descricao_Produto);
        printf("Categoria: %d\n", temp.categoria_Produto);
        printf("Compra: R$ %.2f | Venda: R$ %.2f\n", temp.Preco_De_Compra, temp.Preco_De_Venda);
        printf("Estoque: %d | Mínimo: %d\n", temp.Quantidade_em_Estoque, temp.estoque_Minimo);
        printf("-------------------------\n");
    }

    fclose(arquivo);
}


