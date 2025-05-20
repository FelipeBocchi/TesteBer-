#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ctype.h> // Para função toupper()

/* Defines ---------------------------*/

#define max_caracter 100               // max de caracter para a descrição
#define max_cpf 12
#define max_inf 50
#define ARQUIVO "estoque.bin"          // renomeando o arquivo que vai ser valvo
#define COMPRAS "vendas.bin"           // criando um novo arquivo para salvar as compras de cada cliente;
#define DADOSCLIENTES "DadosClientes.txt"

/* Fim Defines ----------------------*/

/* Structs --------------------------*/

typedef struct
{
    int id;
    char descricao_Produto[max_caracter];
    int categoria_Produto;
    float Preco_De_Compra;
    float Preco_De_Venda;
    int Quantidade_em_Estoque;
    int estoque_Minimo;
} Cadastrar_Produtos;

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
} Carrinho;

typedef struct
{
    int cod;
    char nome[max_caracter];
    char nome_social[max_caracter];
    char cpf[max_cpf];
    char rua_num[max_inf];
    char bairro[max_inf];
    char celular[max_inf];
} Cliente;

/* Fim Structs ----------------------------*/

/* Variaveis Globais ----------------------*/

Carrinho *clientes = NULL;
int qtd_clientes = 0;
int idx;
float saldo_caixa = 0.0;

/* Fim Variaveis Globais ------------------*/

/* Inicializando Funcoes ------------------*/

void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt);
void mostrar_Produtos_Compra_Binario();
void venda_de_Produtos();
void atualizar_Estoque(int idProduto, int quantidadeComprada);
void nota_E_Desconto();
void nota_fiscal();
void salvar_Compra();
void pagamento();
void cadastrarCliente(Cliente *);
int contadorCaracterFile(char *, char);
void limparBuffer();
int verificaArquivo(FILE *arquivo);
void removeEnterTexto(char *);
int exibirMenu();
void exibirVendas();
void exibirCadastros();
void pular_Linha();

void abrirCaixa();
void fecharCaixa();
void menuRelatorios();
void relatorioProdutos();
void relatorioVendas();
void relatorioClientes();
void listarClientes();

/* Fim Inicializando Funcoes --------------*/

int main()
{
    setlocale(LC_ALL, "Portuguese");
    int escolha;

    do // Pelo menos uma vez vai ser mostrado a menu principal
    {
        //escolha recebe uma atribuição que cham a função exibirMenu
        escolha = exibirMenu();
        switch (escolha)
        {
            case 1:
                exibirCadastros();
            break;
            case 2:
                exibirVendas();
            break;
            case 3:
                abrirCaixa();
            break;
            case 4:
                fecharCaixa();
            break;
            case 5:
                menuRelatorios();
            break;
            case 6:
                printf("\n\n\t\tSaindo do programa\n\n");
            break;
            default:
                //caso o usuário tenha informado uma opção invalida
                printf(" \n          Opção invalida\n\n");
            break;
        }

    } while (escolha != 6); // Se a escolha for diferente de 6 vai repetir se não vai parar o programa
    return 0;
}

int exibirMenu()
{
    char opcao[10]; // alterado para string, pois caso o usuario inserisse um caracter, ele bugava o codigo
    printf("\n");
    printf("%50s\n", "=======================================");
    printf("%50s\n", "|            MENU PRINCIPAL           |");
    printf("%50s\n", "=======================================");
    printf("%50s\n", "| 1) Cadastros                        |");
    printf("%50s\n", "| 2) Vendas                           |");
    printf("%50s\n", "| 3) Abertura de caixa                |");
    printf("%50s\n", "| 4) Fechamento de caixa              |");
    printf("%50s\n", "| 5) Relatórios                       |");
    printf("%51s\n", "| 6) Sair                             |");
    printf("%50s\n", "======================================");
    printf("%32s", "Escolha uma opção: ");
    fgets(opcao, sizeof(opcao), stdin);
    return atoi(opcao); // Para transformar um valor de texto em um inteiro em C, a função atoi() da biblioteca stdlib.h pode ser usada (Google)
}

void pular_Linha()
{
    printf("\n");
}

void exibirCadastros()
{
    char opcao[10];
    Cadastrar_Produtos *pt_Produtos = NULL;
    int quantidade = 0;
    FILE *arquivo;
    while (1)
    {
        printf("1. Menu Cadastros\n");
        printf("\t\t1 - Cadastro de Clientes\n");
        printf("\t\t2 - Cadastro de Produtos\n");
        printf("\t\t3 - Retornar ao Menu Principal\n");
        printf("Escolha uma opção: ");

        fgets(opcao, sizeof(opcao), stdin);

        // caso nao exista cria, se nao so atualiza;
        Cliente cliente;
        switch (atoi(opcao))
        {
            case 1:
                cadastrarCliente(&cliente);
            break;

            case 2:
                //atribui ao ponteiro a função REALLOC que almenta nosso espaço na memória de modo que não ficamos sem "espaço"
                pt_Produtos = realloc(pt_Produtos, (quantidade + 1) * sizeof(Cadastrar_Produtos));
                if (pt_Produtos == NULL)
                {
                    printf("erro ao alocar memória!!!\n");
                    return;
                }

                //chama a função para cadastrar os produtos em nosso arquivo binário
                cadastrar_Informacao_Produtos(&pt_Produtos[quantidade]);

                FILE *arquivo = fopen(ARQUIVO, "ab"); // adicionar ao final
                if (arquivo == NULL)
                {
                    printf("ERRO AO ABRIR O ARQUIVO!!!\n");
                    return;
                }

                fwrite(&pt_Produtos[quantidade], sizeof(Cadastrar_Produtos), 1, arquivo);
                fclose(arquivo);

                quantidade++;
            break;

            case 3:
                printf("Voltando ao menu principal...\n");
                return;
            break;

            default:
                printf("Opcao Invalida!!");
            break;
        }

        printf("\n");
    }
}

void cadastrarCliente(Cliente *new_cliente)
{

    char *nome_arquivo = DADOSCLIENTES; // declarando como ponteiro, para usar dentro do fopen da funcao a seguir
    int contador = 0;
    FILE *arquivo;

    // Verifica se o arquivo existe e conta as linhas
    arquivo = fopen(nome_arquivo, "a+");
    if (verificaArquivo(arquivo))
        return;
    fclose(arquivo);

    contador = contadorCaracterFile(nome_arquivo, '\n');
    new_cliente->cod = contador + 1;

    limparBuffer(); // verifica a entrada/limpa buffer // ajuste da funcao do Wesley

    printf("Insira o Nome: \t");
    fgets(new_cliente->nome, sizeof(new_cliente->nome), stdin);
    removeEnterTexto(new_cliente->nome); // removendo o ("enter",\n) da string

    printf("Insira um nome social: \t");
    fgets(new_cliente->nome_social, sizeof(new_cliente->nome_social), stdin);
    removeEnterTexto(new_cliente->nome_social);

    do
    { // verificacao simples para o tamanho do cpf
        printf("Insira o cpf: \t");
        fgets(new_cliente->cpf, sizeof(new_cliente->cpf), stdin);
        removeEnterTexto(new_cliente->cpf);

    } while ((strlen(new_cliente->cpf) != 11));
    limparBuffer();

    printf("Insira o nome da rua ou o numero da residencia: \t");
    fgets(new_cliente->rua_num, sizeof(new_cliente->rua_num), stdin);
    removeEnterTexto(new_cliente->rua_num);

    printf("Insira o bairro: \t");
    fgets(new_cliente->bairro, sizeof(new_cliente->bairro), stdin);
    removeEnterTexto(new_cliente->bairro);

    printf("Insira o numero de telefone: \t");
    fgets(new_cliente->celular, sizeof(new_cliente->celular), stdin);
    removeEnterTexto(new_cliente->celular);

    arquivo = fopen(nome_arquivo, "a");
    if (verificaArquivo(arquivo))
    {
        return;
    }

    fprintf(arquivo, "%d,%s,%s,%s,%s,%s,%s\n", // salvando as informacoes no arquivo
            new_cliente->cod, new_cliente->nome, new_cliente->nome_social,
            new_cliente->cpf, new_cliente->rua_num, new_cliente->bairro, new_cliente->celular);

    fclose(arquivo); // fechando o arquivo
}

int verificaArquivo(FILE *arquivo)
{
    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo"); // mostra erro mais detalhado
        return 1;                          // indica erro
    }
    return 0; // tudo certo
}

int contadorCaracterFile(char *nome_arquivo, char caracter_desejado)
{ // funcao para contar quantos caracteres especificos possuem em um arquivo
    int contador = 0;
    char caracter;
    FILE *arquivo = fopen(nome_arquivo, "r"); // abrindo o arquivo

    if (arquivo == NULL)
    { // caso de um erro ao abrir o arquivo
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }
    // fgetc le um caracter por vez do arquivo, o caracter lido e armazenado na variavel
    // o loop continua ate o final do arquivo
    while ((caracter = fgetc(arquivo)) != EOF)
    {
        if (caracter == caracter_desejado)
        {
            contador++; // incrementando o contador
        }
    }

    fclose(arquivo);

    return contador;
}

void limparBuffer()
{
    int ch;
    // descarte todos os caracter que ainda estao no buffer, como \n
    // garantindo que o próximo comando de entrada comece em uma entrada limpa
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

void removeEnterTexto(char *texto)
{
    texto[strcspn(texto, "\n")] = '\0'; // remove o "enter/ \n" do texto
}

void cadastrar_Informacao_Produtos(Cadastrar_Produtos *pt)
{
    printf("\n-- Cadastrar Produto --\n");
    int validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("ID: ");
        scanf("%d", &pt->id);
        getchar();
        validacao_de_Resposta++;
    } while (pt->id <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;

    printf("Descrição: ");
    fgets(pt->descricao_Produto, max_caracter, stdin);
    strtok(pt->descricao_Produto, "\n"); // Remove o \n do final
    pular_Linha();

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado não é 1 ou 2 ou 3! Digite um valor valido...\n");

        printf("Categoria (1-Alimentos, 2-Limpeza, 3-Panificação): ");
        scanf("%d", &pt->categoria_Produto);
        validacao_de_Resposta++;
    } while (pt->categoria_Produto != 1 && pt->categoria_Produto != 2 && pt->categoria_Produto != 3);
    pular_Linha();
    validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("VALOR DE COMPRA: ");
        scanf("%f", &pt->Preco_De_Compra);
        getchar();
        validacao_de_Resposta++;
    } while (pt->Preco_De_Compra <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("PREÇO DE VENDA: ");
        scanf("%f", &pt->Preco_De_Venda);
        getchar();
        validacao_de_Resposta++;
    } while (pt->Preco_De_Venda <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("QUANTIDADE ESTOQUE: ");
        scanf("%d", &pt->Quantidade_em_Estoque);
        getchar();
        validacao_de_Resposta++;
    } while (pt->Quantidade_em_Estoque <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;

    do
    {
        if (validacao_de_Resposta > 0)
            printf("Valor digitado menor ou igual a zera! Digite outro valor...\n");

        printf("ESTOQUE MINÍMO: ");
        scanf("%d", &pt->estoque_Minimo);
        getchar();
        validacao_de_Resposta++;
    } while (pt->estoque_Minimo <= 0);
    pular_Linha();
    validacao_de_Resposta = 0;
}

void exibirVendas()
{
    int opcao;
    do
    {

        printf("\tÁREA  DE VENDAS\n");
        printf("========================\n");
        printf("21 - NOVA VENDA\n22 - RETIRANDA DE CAIXA(SANGUIA)\n23 - PAGAMENTO\n24 - RETORNAR OA MENU PRÍNCIPAL\n");
        scanf("%d", &opcao);
        getchar();

        switch (opcao)
        {
            case 21:
                venda_de_Produtos();
                nota_E_Desconto();
            break;

            case 22:
                // retirada de caixa
            break;

            case 23:
                pagamento();
            break;

            case 24:
                printf("\n\tVoltando ao menu principal...");
            break;

        default:
            printf("\tOpcao Invalida!!\n");
            break;
        }
    } while (opcao != 24);
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

                    atualizar_Estoque(codigo, quantidade); // essa função também precisa estar adaptada ao binário

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

void atualizar_Estoque(int idProduto, int quantidadeComprada) {
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

//          SANGUIA

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

void abrirCaixa() {
    float valor;
    // Solicita o valor inicial do caixa
    printf("Informe o valor de abertura do caixa: ");
    scanf("%f", &valor);

      // Verifica se o valor é válido (não negativo)
    if (valor >= 0) {

        saldo_caixa = valor;  // Atualiza o saldo do caixa
        printf("Caixa aberto com R$ %.2f\n", valor);
    } else {
        printf("Valor inválido para abertura de caixa!\n");
    }
}

void fecharCaixa() {
      // Variáveis para armazenar os totais
    float vl_abertura = 0.0;
    float vl_pago_d = 0.0;
    float vl_pago_c = 0.0;
    float vl_pago_dc = 0.0;
    float faturamento = 0.0;
    int qtd_vendas_realizadas = 0;

    // Calcula os totais percorrendo todos os movimentos
    for (int i = 0; i < qtd_movimentos; i++) {
        if (movimentos[i].tipo == 'a') {
            vl_abertura = movimentos[i].valor; // Pega o valor de abertura
        } else if (movimentos[i].tipo == 'd') {
            vl_pago_d += movimentos[i].valor; // Soma pagamentos em dinheiro
        } else if (movimentos[i].tipo == 'c') {
            vl_pago_c += movimentos[i].valor; // Soma pagamentos em cartão
        }
    }


     // Exibe o resumo do caixa
    printf("• Quantidade de vendas realizadas: %d\n", qtd_vendas_realizadas);
    printf("• Total do faturamento no dia: R$ %.2f\n", faturamento);
    printf("• Valor da abertura de caixa: R$ %.2f\n", vl_abertura);
    printf("• Valor pago em dinheiro: R$ %.2f\n", vl_pago_d);
    printf("• Valor pago em cartão: R$ %.2f\n", vl_pago_c);
    printf("• Valor pago em dinheiro/cartão: R$ %.2f\n", vl_pago_dc);

     // Verifica se o caixa fecha sem divergências
    float diferenca = faturamento - vl_abertura - vl_pago_d - vl_pago_c - vl_pago_dc;

    if (diferenca == 0) {// Se houver divergência, solicita ajuste
        printf("Caixa fechado corretamente!\n");
    } else {
        printf("ATENÇÃO: O caixa não pode ser fechado porque há divergência de R$ %.2f\n", diferenca);
        printf("Informe o valor de ajuste (positivo para sobra, negativo para falta): ");
        float ajuste;
        scanf("%f", &ajuste);
        getchar();

        if (ajuste == diferenca) {
            printf("Caixa ajustado e fechado!\n");
        } else {
            printf("Ajuste incorreto! Caixa não fechado!\n");
            return;
        }
    }

    // Zerar caixa
    saldo_caixa = 0.0;
}

void menuRelatorios(){

    int opcaoRelat = 0;

    do {
        system("cls");
        printf("-------- RELATÓRIOS --------\n\n");
        printf("- [1] CLIENTES\n");
        printf("- [2] PRODUTOS\n");
        printf("- [3] VENDAS\n");
        printf("- [4] RETORNAR AO MENU\n");
        printf("\nSelecione a opção desejada: ");
        scanf("%d", &opcaoRelat);
        switch(opcaoRelat){
            case 1:
                lerClientes();
                relatorioClientes();
            break;
            case 2:
                lerProdutos();
                ordenarProdutos();
                relatorioProdutos();

            break;
            case 3:
                lerVendas();
                relatorioVendas();
            break;
            case 4:
                printf("Retornando ao menu...\n\n");
                system("pause");
            break;
            default:
                printf("Opção inválida, tente novamente.\n");
                system("pause");
                menuRelatorios();
            break;
        }
    }while(opcaoRelat != 4);
}

void listarClientes(){
        FILE *arquivo = fopen(DADOSCLIENTES, "r");
    if(arquivo == NULL)
    {
        printf("ERRO AO ABRIR ARQUIVO!!!\n");
        return ;
    }

    printf("\n-- Clientes Cadastrados --\n");

    Cliente temp;
    while (fscanf(arquivo, "%d,%99[^,],%99[^,],%11[^,],%49[^,],%49[^,],%49[^\n]\n",
                &temp.cod,
                temp.nome,
                temp.nome_social,
                temp.cpf,
                temp.rua_num,
                temp.bairro,
                temp.celular) != EOF)
    {

        printf("ID: %d\n", temp.cod);
        printf("Nome: %s\n", temp.nome);
        printf("Nome Social: %s\n", temp.nome_social);
        printf("CPF: %s\n", temp.cpf);
        printf("Rua: %s\n", temp.rua_num);
        printf("Bairro: %s\n", temp.bairro);
        printf("Telefone: %s\n", temp.celular);
        printf("-------------------------\n");
    }

    fclose(arquivo);
}
/*
void relatorioClientes(){
    char continua = 's';
    int ver;
    if(tcad == 0){
            menuRelatorios();
    }
    system("cls");
    printf("\t...::Lista dos clientes cadastrados::...\n\n");
    printf("\tcodigo\tnome\n");
    for(int i=0; i<tcad;i++){
        printf("\t%d \t%s \n",cliente[i].codigo,cliente[i].nome);
    }

    printf("\n- Informe o código da pessoa cadastrada que deseja ver mais informaçoes: ");
    while (continua == 's' || continua == 'S') {
        scanf("%d", &ver);
        for (int i = 0; i < tcad; i++) {
            if (ver == cliente[i].codigo) {
                printf("- CODIGO: %d\n",cliente[i].codigo);
                printf("- NOME: %s\n",cliente[i].nome);
                printf("- NOME SOCIAL: %s\n",cliente[i].nomeS);
                printf("- CPF: %d\n",cliente[i].cpf);
                printf("- RUA: %s\n",cliente[i].rua);
                printf("- NUMERO DA RUA: %d\n",cliente[i].Nrua);
                printf("- BAIRRO: %s\n",cliente[i].bairro);
                printf("- CELULAR/WHATS: %d\n",cliente[i].Ncelular);
            }
        }

        printf("\n\nDeseja ver os detalhes de mais algum cliente? [S/N]:");
        fflush(stdin);
        scanf(" %c", &continua);
        printf("\n\n");
    }
    printf("\n ");

    system("pause");
}

void relatorioProdutos(){

    system("cls");
    printf("------------ PRODUTOS CADASTRADOS -------------\n\n");
    printf("| CÓDIGO |      DESCRIÇÃO      |   CATEGORIA   |\n");
    printf("-----------------------------------------------\n");
    for (int i=0; i<qtdProdutos; i++){
        printf("| %-6d | %-19s | %-13s |\n", produtos[i].Cod, produtos[i].descricaoProd, produtos[i].Categoria);
    }

    system("pause");
}

void relatorioVendas(){

    system("cls");
    printf("----------- VENDAS -----------\n\n");
    printf("- LEGENDA:\n");
    printf("- [a] Aberta\n");
    printf("- [b] Dinheiro/Cartão\n");
    printf("- [c] Cartão\n");
    printf("- [d] Dinheiro\n");
    printf("- [x] Venda Cancelada\n\n");
    for (int i=0; i<qtdVendas; i++){
        printf("-------------------------------------------------------------------\n");
        printf("| CLIENTE: [%-4d] | %-25s | CPF: %-11d |\n", infoVendas[i].codCliente, infoVendas[i].nomeCliente, infoVendas[i].cpf);
        printf("| VENDA:   [%-4d] | R$%-23.2f | TIPO: %-10c |\n", infoVendas[i].numVenda, infoVendas[i].valorPago, infoVendas[i].tipoVenda);
        printf("-------------------------------------------------------------------\n");
    }
    system("pause");
}

*/

























/*Teste ---------*/

typedef struct {
    float valor;
    char tipo; // 'a' - abertura, 's' - sangria, 'd' - dinheiro, 'c' - cartão
    int num_venda; // 0 se for abertura/sangria
} MovimentoCaixa;

typedef struct {
    int num_venda;
    float valor_total;
    char forma_pagamento; // 'd'-dinheiro, 'c'-cartão, 'dc'-ambos
    char situacao; // 'a'-aberto, 'p'-pago
} Venda;

typedef struct
{
    int Cod;
    char descricaoProd[max_caracter];
    char Categoria;
    int codCliente;
    char nomeCliente[max_caracter];
    int cpf;
    int numVenda;
    float valorPago;
    char tipoVenda;
}Produtos;


MovimentoCaixa *movimentos = NULL;
int qtd_movimentos = 0;
int qtdProdutos;
int qtdVendas;
Venda *vendas = NULL;
int qtd_vendas = 0;
float saldo_caixa = 0.0;
Produtos produtos[];
Produtos infoVendas[];





