
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define max_caracter 100      //max de caracter para a descrição
#define ARQUIVO "estoque.bin"  //renomeando o arquivo que vai ser valvo
#define COMPRAS "vendas.txt"    //criando um novo arquivo para salvar as compras de cada cliente;
#define TEMP "temp.txt"

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

typedef struct
{
    int id_produto;
    char descricao[max_caracter];
    float preco_unitario;
    int quantidade;
    float subtotal;
} ItemCarrinho;

typedef struct
{
    ItemCarrinho *itens;
    int qtd_itens;
    int ID_Cliente;
    float total_final;
    char categoria_Pagamento;
    int num_Venda;
    char situacao_Do_pagamento;
    float valor_Recebido;
}Carrinho;

Carrinho *clientes = NULL;
int qtd_clientes = 0;
int idx;

void pular_Linha()
{
    printf("\n");
}
void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt);
void salvar_Todos_Produtos_Arquivo_Binario(Cadastrar_Produtos *produtos, int total);
void listar_Produtos_Binario();
void mostrar_Produtos_Compra_Binario();
void venda_de_Produtos();
void atualizarEstoque(int idProduto, int quantidadeComprada);
void nota_E_Desconto();
void nota_fiscal();
void salvar_Compra();
void pagamento();

int main()
{
    setlocale(LC_ALL, "Portuguese");

    int opcao, opcao2, opcao3;
    Cadastrar_Produtos *pt_Produtos = NULL;
    int quantidade = 0;

    printf("\tMENU PRINCIPAL\n\n");
    printf("1-Cadastrar\n2-Vendas\n");
    scanf("%d", &opcao2);
    printf("\n\n");
    getchar();

    switch (opcao2)
    {
        case 1:
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

                        FILE *arquivo = fopen(ARQUIVO, "ab"); // adicionar ao final
                        //salvar_Produtos_Arquivo_Binario(&pt_Produtos[quantidade], arquivo);
                        fwrite(&pt_Produtos[quantidade], sizeof(Cadastrar_Produtos), 1, arquivo);
                        fclose(arquivo);

                        quantidade++;
                        break;

                    case 2:
                        listar_Produtos_Binario();
                        break;

                    case 3:
                        printf("SAINDA DA PROGRAMA...\n");
                        break;

                    default:
                        printf("Opcao invalida!!!\n");
                        break;
                }

            }while(opcao != 3);
            break;

        case 2:
            do
            {

            printf("\tÁREA  DE VENDAS\n");
            printf("========================\n");
            printf("21 - NOVA VENDA\n22 - RETIRANDA DE CAIXA(SANGUIA)\n23 - PAGAMENTO\n24 - RETORNAR OA MENU PRÍNCIPAL\n");
            scanf("%d", &opcao3);
            getchar();

            switch (opcao3)
            {
                case 21:
                    venda_de_Produtos();
                    nota_E_Desconto();

                break;

                case 22:
                    //retirada de caixa
                break;

                case 23:
                    pagamento();
                    //free(carrinho);
                break;

                case 24:
                    //volta ao menu anterior
                break;

                default:
                break;
            }
            } while (opcao3 != 24);

        break;
        default:
            printf("OPÇÃO INVALIDA!!!\n\n");
        break;
    }

    free(pt_Produtos);

     return 0;
}

void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt)
{
    printf("\n-- Cadastrar Produto --\n");
    int validacao_de_Resposta = 0;

        do
        {
            if(validacao_de_Resposta > 0)
                printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

            printf("ID: ");
            scanf("%d", &pt->id);
            getchar();
            validacao_de_Resposta++;
        } while (pt->id <= 0);
        pular_Linha();
        validacao_de_Resposta =  0;

        printf("Descrição: ");
        fgets(pt->descricao_Produto, max_caracter, stdin);
        strtok(pt->descricao_Produto, "\n"); // Remove o \n do final
        pular_Linha();

        do
        {
            if(validacao_de_Resposta > 0)
                printf("Valor digitado não é 1 ou 2 ou 3! Digite um valor valido...\n");

            printf("Categoria (1-Alimentos, 2-Limpeza, 3-Panificação): ");
            scanf("%d", &pt->categoria_Produto);
            validacao_de_Resposta++;
        } while (pt->categoria_Produto != 1 && pt->categoria_Produto != 2 && pt->categoria_Produto != 3);
        pular_Linha();
        validacao_de_Resposta =  0;


        do
        {
            if(validacao_de_Resposta > 0)
                printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

            printf("VALOR DE COMPRA: ");
            scanf("%f", &pt->Preco_De_Compra);
            getchar();
            validacao_de_Resposta++;
        } while (pt->Preco_De_Compra <= 0);
        pular_Linha();
        validacao_de_Resposta =  0;

        do
        {
            if(validacao_de_Resposta > 0)
                printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

            printf("PREÇO DE VENDA: ");
            scanf("%f", &pt->Preco_De_Venda);
            getchar();
            validacao_de_Resposta++;
        } while (pt->Preco_De_Venda <= 0);
        pular_Linha();
        validacao_de_Resposta =  0;

        do
        {
            if(validacao_de_Resposta > 0)
                printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

            printf("QUANTIDADE ESTOQUE: ");
            scanf("%d", &pt->Quantidade_em_Estoque);
            getchar();
            validacao_de_Resposta++;
        } while (pt->Quantidade_em_Estoque <= 0);
        pular_Linha();
        validacao_de_Resposta =  0;

        do
        {
            if(validacao_de_Resposta > 0)
                printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

            printf("ESTOQUE MINÍMO: ");
            scanf("%d", &pt->estoque_Minimo);
            getchar();
            validacao_de_Resposta++;
        } while (pt->estoque_Minimo <= 0);
        pular_Linha();
        validacao_de_Resposta =  0;
}

void salvar_Todos_Produtos_Arquivo_Binario(Cadastrar_Produtos *produtos, int total) {
    FILE *arquivo = fopen("produtos.bin", "wb"); // "wb" para sobrescrever o arquivo
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo binário");
        return;
    }

    fwrite(produtos, sizeof(Cadastrar_Produtos), total, arquivo);
    fclose(arquivo);

    printf("Todos os produtos foram salvos no arquivo binário com sucesso!\n");
}

void listar_Produtos_Binario() {
    FILE *arquivo = fopen(ARQUIVO, "rb"); // abrindo em modo leitura binária
    if (arquivo == NULL) {
        printf("ERRO AO ABRIR ARQUIVO!!!\n");
        return;
    }

    printf("\n-- Produtos Cadastrados --\n");

    Cadastrar_Produtos temp;
    while (fread(&temp, sizeof(Cadastrar_Produtos), 1, arquivo) == 1) {
        printf("ID: %d\n", temp.id);
        printf("Descrição: %s\n", temp.descricao_Produto);
        printf("Categoria: %d\n", temp.categoria_Produto);
        printf("Compra: R$ %.2f | Venda: R$ %.2f\n", temp.Preco_De_Compra, temp.Preco_De_Venda);
        printf("Estoque: %d | Mínimo: %d\n", temp.Quantidade_em_Estoque, temp.estoque_Minimo);
        printf("-------------------------\n");
    }

    fclose(arquivo);
}

void mostrar_Produtos_Compra_Binario() {
    FILE *arquivo = fopen(ARQUIVO, "rb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de produtos.\n");
        return;
    }

    Cadastrar_Produtos produto;

    printf("\nProdutos disponíveis:\n");
    printf("ID\tDescrição\t\tEstoque\t\tPreço de Venda\n");
    printf("---------------------------------------------------------------\n");

    while (fread(&produto, sizeof(Cadastrar_Produtos), 1, arquivo) == 1) {
        printf("%-5d %-25s %-10d R$ %.2f\n",
               produto.id,
               produto.descricao_Produto,
               produto.Quantidade_em_Estoque,
               produto.Preco_De_Venda);
    }

    fclose(arquivo);
}

void venda_de_Produtos()
{
    int codigo, quantidade;
    char linha[256], escolha_continuar_Comprando;
    idx = qtd_clientes; //aqui da para alterar para o id do cliente
    Cadastrar_Produtos temp;

    clientes = realloc(clientes, (qtd_clientes + 1) * sizeof(Carrinho));
    if (clientes == NULL)
    {
        printf("Erro ao alocar memória para carrinhos!\n");
    }

    clientes[qtd_clientes].itens = NULL;
    clientes[qtd_clientes].qtd_itens = 0;
    clientes[qtd_clientes].ID_Cliente = qtd_clientes + 1; // ou outro ID único
    clientes[qtd_clientes].total_final = 0.0;


    do
    {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif

        mostrar_Produtos_Compra_Binario();

        // Mostrar resumo da compra
        printf("\n\n============= NOTA FISCAL (Prévia) =============\n");
        printf("ITEM\tQTD\tUNITÁRIO\tSUBTOTAL\n");

        for (int i = 0; i < clientes[idx].qtd_itens; i++)
        {
            printf("%s\t%d\tR$ %.2f\t\tR$ %.2f\n",
                clientes[idx].itens[i].descricao,
                clientes[idx].itens[i].quantidade,
                clientes[idx].itens[i].preco_unitario,
                clientes[idx].itens[i].subtotal);
        }

        //Abrindo o arquivo que está nosso aetoque para poder verificar os dados dos produtos;
        FILE *arquivo = fopen(ARQUIVO, "rb"); // abrindo binário
        if (arquivo == NULL)
        {
            printf("\nErro ao abrir o arquivo de produtos!!!\n");
            return;
        }

        printf("\nCarrinho de compras: ");

        printf("\nInforme o codigo do produto a ser comprado: ");
        scanf("%d", &codigo);

        int produto_encontrado = 0;

        while (fread(&temp, sizeof(Cadastrar_Produtos), 1, arquivo) == 1)
        {
            if (codigo == temp.id)
            {
                produto_encontrado = 1;

                printf("\nProduto encontrado: %s", temp.descricao_Produto);
                printf("\nQuantidade em estoque: %d", temp.Quantidade_em_Estoque);
                printf("\nInforme a quantidade desejada: ");
                scanf("%d", &quantidade);

                if (quantidade <= temp.Quantidade_em_Estoque) {
                    float valor = temp.Quantidade_em_Estoque - quantidade;
                    if (valor <= temp.estoque_Minimo)
                        printf("\nAviso: estoque no limite mínimo!!!!");

                    atualizarEstoque(codigo, quantidade); // essa função também precisa estar adaptada ao binário

                    clientes[idx].itens = realloc(clientes[idx].itens,
                                                (clientes[idx].qtd_itens + 1) * sizeof(ItemCarrinho));
                    if (clientes[idx].itens == NULL) {
                        printf("Erro ao alocar memória!\n");
                        fclose(arquivo);
                        return;
                    }

                    ItemCarrinho *item = &clientes[idx].itens[clientes[idx].qtd_itens];

                    item->id_produto = temp.id;
                    strcpy(item->descricao, temp.descricao_Produto);
                    item->preco_unitario = temp.Preco_De_Venda;
                    item->quantidade = quantidade;
                    item->subtotal = quantidade * temp.Preco_De_Venda;

                    clientes[idx].total_final += item->subtotal;
                    clientes[idx].qtd_itens++;

                    printf("\nItem adicionado ao carrinho!");
                } else {
                    printf("\nQuantidade no estoque insuficiente!");
                }

                break; // já encontrou o produto
            }
        }

        if (!produto_encontrado)
            printf("\nCódigo do produto inválido ou não encontrado.");

        fclose(arquivo);

        do {
            printf("\nNovo item no carrinho de compra (s/n): ");
            scanf(" %c", &escolha_continuar_Comprando);
            if (escolha_continuar_Comprando != 's' && escolha_continuar_Comprando != 'n')
                printf("\nOpção inválida!");

        } while (escolha_continuar_Comprando != 's' && escolha_continuar_Comprando != 'n');

    } while (escolha_continuar_Comprando == 's');

}

void atualizarEstoque(int idProduto, int quantidadeComprada) {
    FILE *arquivo = fopen(ARQUIVO, "rb+");

    if (!arquivo) {
        printf("Erro ao abrir o arquivo de produtos para atualizar estoque.\n");
        return;
    }

    Cadastrar_Produtos temp;

    while (fread(&temp, sizeof(Cadastrar_Produtos), 1, arquivo) == 1) {
        if (temp.id == idProduto) {
            // Atualiza a quantidade em estoque
            temp.Quantidade_em_Estoque -= quantidadeComprada;
            if (temp.Quantidade_em_Estoque < 0) temp.Quantidade_em_Estoque = 0;

            // Volta o ponteiro para reescrever o produto atualizado
            fseek(arquivo, -sizeof(Cadastrar_Produtos), SEEK_CUR);
            fwrite(&temp, sizeof(Cadastrar_Produtos), 1, arquivo);
            break;
        }
    }

    fclose(arquivo);
}

void nota_E_Desconto()
{
    float total_compra = 0, desconto_no_totalC;

    printf("\n\n============= NOTA FISCAL =============\n");
    printf("ITEM\tQTD\tUNITÁRIO\tSUBTOTAL\n");

    for (int i = 0; i < clientes[idx].qtd_itens; i++)
    {
        printf("%s\t%d\tR$ %.2f\t\tR$ %.2f\n",
               clientes[idx].itens[i].descricao,
               clientes[idx].itens[i].quantidade,
               clientes[idx].itens[i].preco_unitario,
               clientes[idx].itens[i].subtotal);

        total_compra += clientes[idx].itens[i].subtotal;
    }

    printf("\nTotal da compra: R$ %.2f", total_compra);

    printf("\nHá desconto? Informe a porcentagem (ou 0 para nenhum): ");
    if (scanf("%f", &desconto_no_totalC) == 1 && desconto_no_totalC > 0)
    {
        float valor_desconto = total_compra * (desconto_no_totalC / 100.0);
        total_compra -= valor_desconto;
        printf("\nDesconto aplicado: R$ %.2f (%.2f%%)", valor_desconto, desconto_no_totalC);
    }

    printf("\nTotal final com desconto: R$ %.2f\n", total_compra);
}

void nota_fiscal()
{
    float total_compra = 0;  // inicializado para evitar lixo de memória

    printf("\n\n============= NOTA FISCAL =============\n");
    printf("ITEM\tQTD\tUNITÁRIO\tSUBTOTAL\n");

    for (int i = 0; i < clientes[idx].qtd_itens; i++)
    {
        printf("%s\t%d\tR$ %.2f\t\tR$ %.2f\n",
            clientes[idx].itens[i].descricao,
            clientes[idx].itens[i].quantidade,
            clientes[idx].itens[i].preco_unitario,
            clientes[idx].itens[i].subtotal);

        total_compra += clientes[idx].itens[i].subtotal;
    }

    printf("\nTotal da compra: R$ %.2f\n", total_compra);
}

void salvar_Compra()
{
    FILE *file = fopen("venda.bin", "ab"); // modo binário append
    if (file == NULL) {
        printf("\nErro ao abrir o arquivo de compras\n");
        return;
    }

    // Salva os dados do cliente/venda
    fwrite(&clientes[idx].ID_Cliente, sizeof(int), 1, file);
    fwrite(&clientes[idx].num_Venda, sizeof(int), 1, file);
    fwrite(&clientes[idx].categoria_Pagamento, sizeof(char), 1, file);
    fwrite(&clientes[idx].situacao_Do_pagamento, sizeof(char), 1, file);
    fwrite(&clientes[idx].valor_Recebido, sizeof(float), 1, file);

    // Salva a quantidade de itens no carrinho para poder ler depois
    fwrite(&clientes[idx].qtd_itens, sizeof(int), 1, file);

    // Salva os itens
    for(int i = 0; i < clientes[idx].qtd_itens; i++)
    {
        fwrite(&clientes[idx].itens[i].id_produto, sizeof(int), 1, file);
        fwrite(clientes[idx].itens[i].descricao, sizeof(char), max_caracter, file);
        fwrite(&clientes[idx].itens[i].preco_unitario, sizeof(float), 1, file);
        fwrite(&clientes[idx].itens[i].subtotal, sizeof(float), 1, file);
    }

    fclose(file);

    // Libera memória após salvar
    for (int i = 0; i < qtd_clientes; i++)
    {
        free(clientes[i].itens);
    }
    free(clientes);
}

void pagamento()
{
    if (clientes[idx].total_final == 0)
    {
        printf("\nNenhum produto foi comprado...\n");
        return;
    }

    int opcao;
    int aprovacao_maquina;
    float valor_Recebido = 0, valor_restante;
    char opcao_parcial;

    do
    {
        printf("\n===== Forma de Pagamento =====\n");
        printf("1 - Cartão\n");
        printf("2 - Dinheiro\n");
        printf("3 - Retornar ao menu principal\n");
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1)
        {
            printf("Entrada inválida! Tente novamente.\n");
            while (getchar() != '\n'); // limpar buffer
            continue;
        }
        while (getchar() != '\n'); // limpar buffer

        switch (opcao)
        {
            case 1: // Cartão
                nota_fiscal();
                clientes[idx].categoria_Pagamento = 'c';

                do
                {
                    printf("\nPagamento na maquininha (1 = OK, 0 = Não OK): ");
                    if (scanf("%d", &aprovacao_maquina) != 1 || (aprovacao_maquina != 0 && aprovacao_maquina != 1))
                    {
                        printf("Opção inválida! Tente novamente.\n");
                        while (getchar() != '\n');
                        aprovacao_maquina = -1; // força repetir o loop
                        continue;
                    }
                    while (getchar() != '\n');
                } while (aprovacao_maquina == -1);

                if (aprovacao_maquina == 1)
                {
                    clientes[idx].situacao_Do_pagamento = 'f';
                    clientes[idx].valor_Recebido = clientes[idx].total_final;
                    salvar_Compra();
                    printf("\nPagamento efetuado com sucesso! Obrigado pela preferência.\n");
                }
                else
                {
                    printf("\nPagamento no cartão não aprovado.\n");
                }
                break;

            case 2: // Dinheiro
                nota_fiscal();
                clientes[idx].categoria_Pagamento = 'd';

                valor_Recebido = 0;
                valor_restante = clientes[idx].total_final;

                while (valor_Recebido < clientes[idx].total_final)
                {
                    printf("\nTotal a pagar: R$ %.2f", clientes[idx].total_final);
                    printf("\nInforme o valor recebido (dinheiro): R$ ");
                    if (scanf("%f", &valor_Recebido) != 1 || valor_Recebido <= 0)
                    {
                        printf("Valor inválido. Tente novamente.\n");
                        while (getchar() != '\n');
                        continue;
                    }
                    while (getchar() != '\n');

                    if (valor_Recebido < valor_restante)
                    {
                        valor_restante -= valor_Recebido;

                        printf("\nValor insuficiente. Faltam R$ %.2f", valor_restante);
                        printf("\nDeseja pagar o restante com cartão? (s/n): ");
                        scanf(" %c", &opcao_parcial);
                        while (getchar() != '\n');

                        if (opcao_parcial == 's' || opcao_parcial == 'S')
                        {
                            clientes[idx].categoria_Pagamento = 'd'; // ou 'dc' para dinheiro + cartão
                            clientes[idx].situacao_Do_pagamento = 'f';
                            clientes[idx].valor_Recebido = clientes[idx].total_final;
                            salvar_Compra();
                            printf("\nPagamento efetuado com sucesso! Obrigado.\n");
                            break;
                        }
                        else if (opcao_parcial == 'n' || opcao_parcial == 'N')
                        {
                            printf("\nPor favor, informe o valor restante em dinheiro.\n");
                            // valor_Recebido continuará acumulando no próximo loop
                            valor_Recebido = clientes[idx].total_final - valor_restante;
                        }
                        else
                        {
                            printf("Opção inválida, assumindo 'n'.\n");
                        }
                    }
                    else
                    {
                        float troco = valor_Recebido - clientes[idx].total_final;
                        if (troco > 0)
                            printf("\nTroco: R$ %.2f\n", troco);

                        clientes[idx].situacao_Do_pagamento = 'f';
                        clientes[idx].valor_Recebido = valor_Recebido;
                        salvar_Compra();
                        printf("\nPagamento efetuado com sucesso! Obrigado.\n");
                        break;
                    }
                }
                break;

            case 3:
                printf("\nRetornando ao menu principal...\n");
                break;

            default:
                printf("\nOpção inválida! Tente novamente.\n");
                break;
        }

    } while (opcao != 3 && !(opcao == 1 && aprovacao_maquina == 1));
}




