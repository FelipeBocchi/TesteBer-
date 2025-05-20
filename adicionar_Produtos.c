#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <math.h>
#include <time.h> // biblioteca para utilizar hora atual

// definição de constantes
#define TAM 50
#define MAX_CAT 3

// Declaração das structs
typedef struct { // struct para cadastro de produtos
    int Cod;
    char descricaoProd[TAM];
    char Categoria[TAM];
    float precoCompra;
    float percentLucro;
    float precoVenda;
    int estoqueMinimo;
    int Estoque;
} cadProdutos;

typedef struct { // struct que armazena informações dos itens vendidos para o carrinho
    int Codigo;
    int codC;
    int Quantidade;
    int cpf;
    char Descricao[TAM];
    char nomeC[TAM];
    float precoItem;
    float precoTotal;
} vendaProdutos;

typedef struct { // struct para armazenar informações da venda
    int numVenda;
    float valorPago;
    char tipoVenda;
    int codCliente;
    char nomeCliente[TAM];
    int cpf;
} relatVendas;

typedef struct{ // struct para cadastro de clientes
int codigo;
int cpf;
int Nrua;
int Ncelular;
char nome[TAM];
char nomeS[TAM];
char rua [TAM];
char bairro[TAM];
}cadcliente;

/*typedef struct{ // struct para guardar informações da venda
    char nome[TAM];
    int codCliente;
    int cpf;
    int vendaNum;
    float pag;
    char vendaTipo;

} Vendas;*/

// Declaração de structs e variáveis globais
cadcliente atualizarC;
cadcliente *cliente;
cadProdutos *produtos = NULL;
vendaProdutos *carrinho;
relatVendas *infoVendas;
//Vendas *relatorioV;

float totalCompra = 0, inicioCx = 0, caixa = 0, caixa_cartao = 0, fatDinheiro = 0, valorSangria = 0, fatTotal = 0;
int qtdProdutos, qtd_Carrinho = 0, qtdVendas , tcad = 0, numeroVend = 0, qtd_Compra = 0;

// Declaração das funções
void menuPrincipal();
int menuCadastros(int espaco);
int cadastro_clientes();
int Cadastro_Estoque();
void atualizaEstoque();
void atualizaPreco();
void menuVendas(int espaco);
float vendas(int espaco);
void carregaCarrinho();
int gravarProdutos();
int gravarClientes();
int gravarVendas();
int lerProdutos();
int lerClientes();
int lerVendas();
void ordenarProdutos();
void Sangria();
void aberturaCaixa();
void fechamento_caixa();
void facaPagamento(int espaco);
char pagDinheiro(float total ,char metodoPag, int espaco);
char pagCartao(float total, char metodoPag, int espaco);
char divisaoPag(float troco, char metodoPag, int espaco, float dinheiro);
void menuRelatorios();
void relatorioClientes();
void relatorioProdutos();
void relatorioVendas();
void vendasAbertas(int espaco);
void sair();

int main() {
    setlocale(LC_ALL, "portuguese"); // define a linguagem do sistema para português
    //struct tm *infoTempo; // padrão da biblioteca time.h - struct com informações para conseguimos hora e  data

    int op_Menu = 0;
    int espaco = 1;

    //time(&tempo_atual); // Obter o tempo atual
   // info_tempo = localtime(&tempo_atual);// Converter o tempo atual para uma estrutura tm local

    do {
        system("cls");
        menuPrincipal();
        printf("\n\nInforme a opção desejada: ");
        scanf("%d", &op_Menu);
        switch(op_Menu) {
        case 1:
            menuCadastros(espaco);
        break;
        case 2:
            menuVendas(espaco);
        break;
        case 3:
            aberturaCaixa();
        break;
        case 4:
            fechamento_caixa();
        break;
        case 5:
            menuRelatorios();
        break;
        case 6:

            sair();
        default:
            printf("\nOpção inválida!\n");
            system("pause");
        break;
        }
    } while (op_Menu != 6);

    return 0;
}

void menuPrincipal() {
    printf("\n--- MERCEARIA DONA BERÊ ----\n");
    printf("\n============================");
    printf("\n| [1] CADASTROS            |");
    printf("\n----------------------------");
    printf("\n| [2] VENDAS               |");if (qtd_Carrinho > 0){printf("  º");}
    printf("\n----------------------------");
    printf("\n| [3] ABERTURA DE CAIXA    |");
    printf("\n----------------------------");
    printf("\n| [4] FECHAMENTO DE CAIXA  |");
    printf("\n----------------------------");
    printf("\n| [5] RELATÓRIOS           |");
    printf("\n----------------------------");
    printf("\n| [6] SAIR                 |");
    printf("\n============================\n");
}

int menuCadastros(int espaco) { // Mostra as opções de cadastro
    int op_menuCad = 0; // variável para controlar a opção escolhida

    while (op_menuCad != 3) {
        system("cls");
        printf("\n----------- CADASTROS -----------\n\n");
        printf("=================================\n");
        printf("| [1] CLIENTES                  |\n");
        printf("---------------------------------\n");
        printf("| [2] PRODUTOS                  |\n");
        printf("---------------------------------\n");
        printf("| [3] VOLTAR AO MENU PRINCIPAL  |\n");
        printf("=================================\n\n");

        printf("Informe a opção desejada: ");
        scanf("%d", &op_menuCad);

        switch(op_menuCad) {
        case 1:
            cadastro_clientes(); // chamada da função para cadastro de clientes
            break;
        case 2:
            Cadastro_Estoque(espaco); // chamada da função para cadastrar produtos
            break;
        case 3:
            return 0; // retorna ao menu
            break;
        default:
            printf("\nOpção inválida!\n");
            system("pause");
            break;
        }
    }
    return 0;
}

int cadastro_clientes(){
    int i , ver , ncad = 0 , op_cliente , mudar , cliente_encontrado = 0;
    char continua = 's', altclienteC;

    tcad = lerClientes();


    system("cls");
    printf("\n----- CADASTROS CLIENTES-----\n");
    printf("=================================\n");
    printf("| [1] CADASTRAR CLIENTES        |\n");
    printf("---------------------------------\n");
    printf("| [2] ATUALIZAR CLIENTES        |\n");
    printf("---------------------------------\n");
    printf("| [3] SAIR                      |\n");;
    printf("=================================\n\n");
    printf("Selecione a opção desejada: ");
    scanf("%d", &op_cliente);

    switch(op_cliente) {
        case 1:


            system("cls");
            printf("\n--------- CADASTRO DE CLIENTES ---------\n\n");
            printf("QUANTIDADE DE CLIENTES CADASTRADOS: %d\n\n", tcad);
            printf("\t ...::informe quantos clientes serao cadastrados::...\n\n");
            scanf("%d",&ncad);
            for(i=0;i<ncad;i++){

                printf("\n- Informe o código do cliente %03d :", i + 1);
                scanf("%d", &cliente[i].codigo);
                fflush(stdin);//limpa o buffer
                printf("- Informe o nome completo do cliente %03d :", i + 1);
                fgets(cliente[i].nome, sizeof(cliente[i].nome), stdin);
                cliente[i].nome[strcspn(cliente[i].nome, "\n")] = '\0';
                printf("- Informe o nome social do cliente %03d :", i + 1);
                fgets(cliente[i].nomeS, sizeof(cliente[i].nomeS), stdin);
                cliente[i].nomeS[strcspn(cliente[i].nomeS, "\n")] = '\0';
                printf("- Informe o CPF do cliente %03d :", i + 1);
                scanf("%d", &cliente[i].cpf);
                fflush(stdin);
                printf("- Informe a rua do cliente %03d :", i + 1);
                fgets(cliente[i].rua, sizeof(cliente[i].rua), stdin);
                cliente[i].rua[strcspn(cliente[i].rua, "\n")] = '\0';
                printf("- Informe o número da rua do cliente %03d :", i + 1);
                scanf("%d", &cliente[i].Nrua);
                fflush(stdin);
                printf("- Informe o bairro do cliente %03d :", i + 1);
                fgets(cliente[i].bairro, sizeof(cliente[i].bairro), stdin);
                cliente[i].bairro[strcspn(cliente[i].bairro, "\n")] = '\0';
                printf("- Informe o celular/whats do cliente %03d :", i + 1);
                scanf("%d", &cliente[i].Ncelular);
                fflush(stdin);

            }

            tcad+=ncad;
            gravarClientes();
            cadastro_clientes();
            break;
        case 2:
            if(tcad == 0){
                system("cls");
                printf("-----------------------------------------");
                printf("\n NÃO HA NENHUM CLIENTE CADASTRADO!");
                printf("\n-----------------------------------------\n");
                system("pause");
                break;
            }
            ordenarClientes();
            system("cls");
            printf("\t...::Lista dos clientes cadastrados::...\n\n");
            printf("\tcodigo\tnome\n");
            for(int i=0; i<tcad;i++){
                printf("\t%d \t%s \n",cliente[i].codigo,cliente[i].nome);
            }

            while (continua == 's' || continua == 'S') {
            printf("\n- Informe o código da pessoa cadastrada que deseja ver mais informaçoes: ");
                scanf("%d", &ver);
                for (int i = 0; i < tcad; i++) {
                    if (ver == cliente[i].codigo) {
                        system("cls");
                        printf("- [1] CODIGO: %d\n",cliente[i].codigo);
                        printf("- [2] NOME: %s\n",cliente[i].nome);
                        printf("- [3] NOME SOCIAL: %s\n",cliente[i].nomeS);
                        printf("- [4] CPF: %d\n",cliente[i].cpf);
                        printf("- [5] RUA: %s\n",cliente[i].rua);
                        printf("- [6] NUMERO DA RUA: %d\n",cliente[i].Nrua);
                        printf("- [7] BAIRRO: %s\n",cliente[i].bairro);
                        printf("- [8] CELULAR/WHATS: %d\n\n",cliente[i].Ncelular);
                        printf("- INFORME QUAL INFORMAÇÃO GOSTARIA DE ATUALIZAR:\n");
                        scanf("%d",&mudar);
                        switch(mudar){
                            case 1:
                                printf("- INFORME QUAL É O NOVO CODIGO: ");
                                scanf("%d", &atualizarC.codigo);
                                cliente[i].codigo = atualizarC.codigo;
                                break;
                            case 2:
                                printf("- INFORME QUAL É O NOVO NOME: ");
                                fflush(stdin);
                                fgets(atualizarC.nome, sizeof(atualizarC.nome), stdin);
                                atualizarC.nome[strcspn(atualizarC.nome, "\n")] = '\0';
                                strcpy(cliente[i].nome, atualizarC.nome);
                                break;
                            case 3:
                                printf("- INFORME QUAL É O NOVO NOME SOCIAL: ");
                                fflush(stdin);
                                fgets(atualizarC.nomeS, sizeof(atualizarC.nomeS), stdin);
                                atualizarC.nomeS[strcspn(atualizarC.nomeS, "\n")] = '\0';
                                strcpy(cliente[i].nomeS, atualizarC.nomeS);
                                break;
                            case 4:
                                printf("- INFORME QUAL É O NOVO CPF: ");
                                scanf("%d", &atualizarC.cpf);
                                cliente[i].cpf = atualizarC.cpf;
                                break;
                            case 5:
                                printf("- INFORME QUAL É O NOVO NOME DA RUA: ");
                                fflush(stdin);
                                fgets(atualizarC.rua, sizeof(atualizarC.rua), stdin);
                                atualizarC.rua[strcspn(atualizarC.rua, "\n")] = '\0';
                                strcpy(cliente[i].rua, atualizarC.rua);
                                break;
                            case 6:
                                printf("- INFORME QUAL É O NOVO NUMERO DA RUA: ");
                                scanf("%d", &atualizarC.Nrua);
                                cliente[i].Nrua = atualizarC.Nrua;
                                break;
                            case 7:
                                printf("- INFORME QUAL É O NOVO BAIRRO: ");
                                fflush(stdin);
                                fgets(atualizarC.bairro, sizeof(atualizarC.bairro), stdin);
                                atualizarC.bairro[strcspn(atualizarC.bairro, "\n")] = '\0';
                                strcpy(cliente[i].bairro, atualizarC.bairro);
                                break;
                            case 8:
                                printf("- INFORME QUAL É O NOVO NUMERO/WHATS: ");
                                scanf("%d", &atualizarC.Ncelular);
                                cliente[i].Ncelular = atualizarC.Ncelular;
                                break;
                            default:
                                printf("Opção inválida!\n");
                                break;
                        }
                    }
                }

                printf("\n\nDeseja ver os detalhes de mais algum cliente? [S/N]: ");
                getchar();
                scanf("%c", &continua);
            }


        gravarClientes();
        break;
        case 3:
            printf("Voltando ao menu principal...\n");
            break;

        default:
            printf("\nOpção inválida!\n");
            system("pause");
            break;
        }
}
/*
    Função para cadastrar novos produtos, recupera os registros do arquivo binario e aloca na struct cadProdutos produtos.
    Também é capaz atualizar o estoque do produto escolhido
*/
int Cadastro_Estoque(int espaco) {

    int op_estoque = 0;
    char continuar = 's';
    char continua = 's';

    qtdProdutos = lerProdutos(); // chama a função responsável por fazer a leitra do arquivo com registro dos produtos
    if (qtdProdutos > 0) { // se houver registros no arquivo qtdProdutos será maior que 0, então espaco recebe o valor de qtd produtos
        espaco = qtdProdutos;
    } else {
        espaco = 1;
        produtos = (cadProdutos *)malloc(espaco * sizeof(cadProdutos)); // se não, a alocação inicial é feita aqui
        if (produtos == NULL) { // testa se memória foi alocada corretamente
            printf("Erro ao alocar memória.\n");
            exit(1);
        }
    }
    while (op_estoque != 4) {
        system("cls");
        printf("\n----- CADASTRO DE PRODUTOS -----");
        printf("\n================================");
        printf("\n| [1] CADASTRAR PRODUTOS       |");
        printf("\n--------------------------------");
        printf("\n| [2] ATUALIZAR ESTOQUE        |");
        printf("\n--------------------------------");
        printf("\n| [3] ATUALIZAR PREÇO DE VENDA |");
        printf("\n--------------------------------");
        printf("\n| [4] VOLTAR AO MENU           |");
        printf("\n================================\n\n");
        printf("Selecione a opção desejada: ");
        scanf("%d", &op_estoque);

        switch(op_estoque) {
        case 1:
                do {
                if (qtdProdutos == espaco) {
                    espaco *= 5;
                    cadProdutos *temp = (cadProdutos *)realloc(produtos, espaco * sizeof(cadProdutos));
                    if (temp == NULL) {
                        printf("\n !ERRO NA REALOCAÇÃO DE MEMÓRIA!\n");
                        system("pause");
                        free(produtos);
                        exit(1);
                    }
                    produtos = temp;
                }

                cadProdutos *novoProduto = &produtos[qtdProdutos];

                refazer:
                system("cls");
                printf("\n--------- CADASTRO DE PRODUTOS ---------\n\n");
                printf("QUANTIDADE DE PRODUTOS CADASTRADOS: %d\n\n", qtdProdutos);

                printf("- Informe o código do produto: ");
                scanf("%d", &novoProduto->Cod);
                for (int i=0; i<qtdProdutos; i++){
                    if(novoProduto->Cod == produtos[i].Cod){
                        printf("Código já existe\n- Informe um código válido: ");
                        system("pause");
                        goto refazer;
                    }
                }
                printf("- Informe a descrição do produto: ");
                fflush(stdin);
                fgets(novoProduto->descricaoProd, sizeof(novoProduto->descricaoProd), stdin);
                novoProduto->descricaoProd[strcspn(novoProduto->descricaoProd, "\n")] = '\0';
                printf("- Informe a categoria do produto: ");
                fflush(stdin);
                fgets(novoProduto->Categoria, sizeof(novoProduto->Categoria), stdin);
                novoProduto->Categoria[strcspn(novoProduto->Categoria, "\n")] = '\0';
                printf("- Informe o preço de compra do produto (unidade): R$");
                scanf("%f", &novoProduto->precoCompra);
                printf("- Informe a margem de lucro desejada: ");
                scanf("%f", &novoProduto->percentLucro);
                printf("- Informe a quantidade de produtos: ");
                scanf("%d", &novoProduto->Estoque);
                printf("- Informe a quantidade mínima permitida em estoque: ");
                scanf("%d", &novoProduto->estoqueMinimo);
                novoProduto->percentLucro /= 100;
                novoProduto->precoVenda = novoProduto->precoCompra + (novoProduto->precoCompra * novoProduto->percentLucro);
                qtdProdutos++;

                printf("\nDeseja cadastrar mais algum produto [S/N]?\n");
                scanf(" %c", &continuar);
                printf("\n\n");
                system("pause");

            } while (continuar == 's' || continuar == 'S');
            gravarProdutos();
        break;
        case 2: // atualização de estoque
            atualizaEstoque();
            gravarProdutos();
        break;
        case 3: // atualizar preço de venda
            atualizaPreco();
            gravarProdutos();
        break;
        case 4:
            return 0;
        break;
        default:
            printf("\nOpção inválida!\n");
            system("pause");
        break;
        }
    }
    free(produtos);
    return 0;
}

void atualizaPreco(){

    char continua = 's', alterarVcompra = 's';
    int itemPreco = 0;
    float novoPreco = 0, precoCompra = 0;

    system("cls");
    printf("\n------------------------ PRODUTOS CADASTRADOS ------------------------\n\n");
    printf("| CÓDIGO |      DESCRIÇÃO      |  CATEGORIA  | PREÇO VENDA | ESTOQUE |\n");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < qtdProdutos; i++) { // percore os produtos cadastrados
        printf("| %-6d | %-19s | %-11s | R$%-9.2f | %-7d |\n", produtos[i].Cod, produtos[i].descricaoProd, produtos[i].Categoria, produtos[i].precoVenda, produtos[i].Estoque);
    }

    while (continua == 's' || continua == 'S') {
        printf("\nInforme o código do item que deseja atualizar o preço: ");
        scanf("%d", &itemPreco);
        // percorre os produtos cadastrados até encontrar o código digitado
        for (int i = 0; i < qtdProdutos; i++) {
            if (itemPreco == produtos[i].Cod) {
                printf("\n- ITEM: %s\n", produtos[i].descricaoProd);
                printf("- PREÇO ATUAL: R$%.2f\n\n", produtos[i].precoVenda);
                printf("- Deseja alterar o preço de compra do produto? ");
                getchar();
                scanf(" %c", &alterarVcompra);
                if(alterarVcompra == 's' || alterarVcompra == 'S'){
                    printf("\nInforme o novo preço de compra do produto: R$");
                    scanf("%f", &precoCompra);
                }
                else {
                    precoCompra = produtos[i].precoCompra;
                }
                printf("\nInforme a nova porcentagem de lucro que deseja obter: ");
                scanf("%d", &novoPreco);
                produtos[i].precoVenda = precoCompra + (precoCompra * novoPreco);
            }
        }
        printf("\nPreço de venda atualizado com sucesso!");
        printf("\n\nDeseja atualizar mais algum item? [S/N]:");
        getchar();
        scanf(" %c", &continua);
        printf("\n\n");
    }
    //gravarProdutos();
    //free(produtos);

}

void atualizaEstoque(){

    int item = 0, novoEstoque = 0;
    char continua = 's';

    system("cls");
    printf("\n------------------------ PRODUTOS CADASTRADOS ------------------------\n\n");
    printf("| CÓDIGO |      DESCRIÇÃO      |  CATEGORIA  | PREÇO VENDA | ESTOQUE |\n");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < qtdProdutos; i++) { // percore os produtos cadastrados
        printf("| %-6d | %-19s | %-11s | R$%-9.2f | %-7d |\n", produtos[i].Cod, produtos[i].descricaoProd, produtos[i].Categoria, produtos[i].precoVenda, produtos[i].Estoque);
    }

    while (continua == 's' || continua == 'S') {
        printf("\nInforme o código do item que deseja atualizar o estoque: ");
        scanf("%d", &item);
        // percorre os produtos cadastrados até encontrar o código digitado
        for (int i = 0; i < qtdProdutos; i++) {
            if (item == produtos[i].Cod) {
                printf("\n- ITEM: %s\n\n", produtos[i].descricaoProd);
                printf("Informe a nova quantidade de itens no estoque: ");
                scanf("%d", &novoEstoque);
                produtos[i].Estoque = novoEstoque;// altera a quantidade de estoque na struct
            }
        }
        printf("\nEstoque atualizado com sucesso!");
        printf("\n\nDeseja atualizar mais algum item? [S/N]:");
        getchar();
        scanf(" %c", &continua);
        printf("\n\n");
    }
    //gravarProdutos(); // grava as alterações em arquivo
    //free(produtos); // libera a memória alocada
}

// Função responsável por gravar os produtos cadastrados em arquivo binario
int gravarProdutos(){
    FILE *arquivoProd = fopen("produtos.dat", "wb"); // abertura do arquivo binario
    if (arquivoProd == NULL){ // testa se o arquivo foi aberto
        printf("Erro ao abrir arquivo!\n");
        system("pause");
        return 0;
    }
    fwrite(&qtdProdutos, sizeof(int), 1, arquivoProd); // escreve a quantidade de produtos no arquivo
    fwrite(produtos, sizeof(cadProdutos), qtdProdutos, arquivoProd); // grava os produtos
    fclose(arquivoProd); // fecha o arquivo
    return 0;
}

int gravarClientes() {
    FILE *arquivo = fopen("clientes.dat", "wb"); // abre o arquivo binário para escrita
    if (arquivo == NULL) { // verifica se o arquivo foi aberto corretamente
        printf("Erro ao abrir arquivo!\n");
        system("pause");
        return 0;
    }
    fwrite(&tcad, sizeof(int), 1, arquivo); // escreve a quantidade de clientes no arquivo
    fwrite(cliente, sizeof(cadcliente), tcad, arquivo); // grava os clientes no arquivo
    fclose(arquivo); // fecha o arquivo
    return 0;
}

int gravarVendas() {
    FILE *arquivoVendas = fopen("vendas.dat", "wb"); // abre o arquivo binário para escrita
    if (arquivoVendas == NULL) { // verifica se o arquivo foi aberto corretamente
        printf("Erro ao abrir arquivo!\n");
        system("pause");
        return 0;
    }
    fwrite(&qtdVendas, sizeof(int), 1, arquivoVendas); // escreve a quantidade de clientes no arquivo
    fwrite(infoVendas, sizeof(relatVendas), qtdVendas, arquivoVendas); // grava os clientes no arquivo
    fclose(arquivoVendas); // fecha o arquivo
    return 0;
}

// função para a leitura dos registros no arquivo
int lerProdutos() {
    FILE *arquivoProd;
    int capacidade, i=0;

    arquivoProd = fopen("produtos.dat", "rb"); // abre o arquivo

    if (arquivoProd == NULL) { // testa se arquivo existe
        system("cls");
        printf("... Criando novo arquivo ...\n\n");
        qtdProdutos = 0;
        system("pause");
        return 0; // Retorna 0 indicando falha ao abrir o arquivo
    }

    fread(&qtdProdutos, sizeof(int), 1, arquivoProd); // lê a quantidade de produtos

    if (qtdProdutos == 0) { // testa se há registros no arquivo
        fclose(arquivoProd);
        printf("Não foi possível recuperar os registros arquivados\n");
        return 0; // Retorna 0 indicando falha ao ler a quantidade de produtos
    }
    // Aloca memória para receber os registros recuperados na struct de cadastro de produtos
    produtos = (cadProdutos *)malloc(qtdProdutos * sizeof(cadProdutos));
    if (produtos == NULL) {
        fclose(arquivoProd);
        printf("Não foi possível alocar memória para os registros arquivados\n");
        return 0; // Retorna 0 indicando falha ao alocar memória
    }

    while (!feof(arquivoProd) && i < qtdProdutos) { // faz a leitura de cada item até chegar ao fim do arquivo
        fread(&produtos[i], sizeof(cadProdutos), 1, arquivoProd); // lê cada produto
        i++;
    }

    capacidade = qtdProdutos;
    fclose(arquivoProd); // fecha o arquivo
    return capacidade;
}

int lerClientes() {
    FILE *arquivo;
    int cadastrados, i = 0;

    arquivo = fopen("clientes.dat", "rb"); // abre o arquivo binário para leitura
    if (arquivo == NULL) { // verifica se o arquivo foi aberto corretamente
        system("cls");
        printf("... Criando novo arquivo ...\n\n");
        tcad = 0;
        system("pause");
        return 0; // retorna 0 indicando falha ao abrir o arquivo
    }

    fread(&tcad, sizeof(int), 1, arquivo); // lê a quantidade de clientes do arquivo
    if (tcad == 0) { // verifica se há registros no arquivo
        fclose(arquivo);
        printf("Não foi possível recuperar os registros arquivados\n");
        return 0; // retorna 0 indicando falha ao ler a quantidade de clientes
    }

    cliente = (cadcliente *)malloc(tcad * sizeof(cadcliente));
    if (cliente == NULL) {
        fclose(arquivo);
        printf("Não foi possível alocar memória para os registros arquivados\n");
        return 0; // Retorna 0 indicando falha ao alocar memória
    }

    while (!feof(arquivo) && i < tcad) { // lê cada cliente do arquivo
        fread(&cliente[i], sizeof(cadcliente), 1, arquivo);
        i++;
    }

    cadastrados = tcad;
    fclose(arquivo); // fecha o arquivo
    return cadastrados;
}

int lerVendas() {
    FILE *arquivoVendas;
    int capacidade, i=0;

    arquivoVendas = fopen("vendas.dat", "rb"); // abre o arquivo

    if (arquivoVendas == NULL) { // testa se arquivo existe
        system("cls");
        printf("... Criando novo arquivo ...\n\n");
        qtdVendas = 0;
        system("pause");
        return 0; // Retorna 0 indicando falha ao abrir o arquivo
    }

    fread(&qtdVendas, sizeof(int), 1, arquivoVendas); // lê a quantidade de produtos

    if (qtdVendas == 0) { // testa se há registros no arquivo
        fclose(arquivoVendas);
        printf("Não foi possível recuperar os registros arquivados\n");
        return 0; // Retorna 0 indicando falha ao ler a quantidade de produtos
    }
    // Aloca memória para receber os registros recuperados na struct de cadastro de produtos
    infoVendas = (relatVendas *)malloc(qtdVendas * sizeof(relatVendas));
    if (infoVendas == NULL) {
        fclose(arquivoVendas);
        printf("Não foi possível alocar memória para os registros arquivados\n");
        return 0; // Retorna 0 indicando falha ao alocar memória
    }

    while (!feof(arquivoVendas) && i < qtdVendas) { // faz a leitura de cada item até chegar ao fim do arquivo
        fread(&infoVendas[i], sizeof(relatVendas), 1, arquivoVendas); // lê cada produto
        i++;
    }

    numeroVend = qtdVendas; // atualiza número venda
    capacidade = qtdVendas;
    fclose(arquivoVendas); // fecha o arquivo
    return capacidade;
}


void ordenarProdutos () {

    cadProdutos prodtemp;

    // Algoritmo de ordenação por bolha - ordena os produtos pela categoria
    for(int i = 0; i < qtdProdutos - 1; i++) {
        for(int j = 0; j < qtdProdutos - i - 1; j++) {
            if (strcmp(produtos[j].Categoria, produtos[j + 1].Categoria) > 0) {
                // Troca produtos[j] e produtos[j + 1]
                prodtemp = produtos[j];
                produtos[j] = produtos[j + 1];
                produtos[j + 1] = prodtemp;
            }
        }
    }
}

void ordenarClientes() {

    cadcliente clienttemp;
    // ordena os clientes por nome
    for(int i = 0; i < tcad - 1; i++) {
        for(int j = 0; j < tcad - i - 1; j++) {
            if (strcmp(cliente[j].nome, cliente[j + 1].nome) > 0) {
                // Troca cliente[j] e cliente[j + 1]
                clienttemp = cliente[j];
                cliente[j] = cliente[j + 1];
                cliente[j + 1] = clienttemp;
            }
        }
    }
}

// Função que mostra as opções em vendas
void menuVendas(int espaco) {
    int op_vendas = 0;

    // testa se o caixa foi aberto
    if (inicioCx == 0) {
        system("cls");
        printf("-----------------------------------------");
        printf("\n\t OPERAÇÃO INVÁLIDA!\t");
        printf("\n É NECESSÁRIO INICIALIZAR CAIXA PRIMEIRO");
        printf("\n-----------------------------------------\n");
        system("pause");
        return;
    }

    while(op_vendas != 4){

        system("cls");
        printf("\n----------- VENDAS -----------\n\n");
        printf("================================\n");
        printf("| [1] NOVA VENDA               |\n");
        printf("--------------------------------\n");
        printf("| [2] RETIRADA DE CAIXA        |\n");
        printf("--------------------------------\n");
        printf("| [3] PAGAMENTOS               |\n");
        printf("--------------------------------\n");
        printf("| [4] CARRINHO                 |");if (qtd_Carrinho > 0){printf("  º");} // indica se há produtos no carrinho
        printf("\n--------------------------------\n");
        printf("| [5] VOLTAR AO MENU PRINCIPAL |\n");
        printf("================================\n\n");

        printf("Selecione a opção desejada: ");
        getchar();
        scanf("%d", &op_vendas);

        switch(op_vendas) {
        case 1:
            lerProdutos(); // faz a leitura do arquivo
            ordenarProdutos();
            //lerClientes();
            totalCompra = vendas(espaco); // função de vendas
        break;
        case 2:
            Sangria(); // Função de retirada de caixa
        break;
        case 3:
            //lerProdutos();
            facaPagamento(espaco); // função para pagamento
            gravarVendas();
        break;
        case 4:
            system("cls");
            carregaCarrinho(); // mostra os itens no carrinho
            system("pause");
        case 5:
            return;
        default:
            printf("\nOpção inválida!\n");
            system("pause");
            break;
        }
    }
}

float vendas(int espaco) {

    int cod_Compra = 0, encontrado = 0, vendaCliente = 0, codCliente = 0;
    char continua = 's', cont_Compra = 's';

    carrinho = (vendaProdutos *)malloc(espaco * sizeof(vendaProdutos)); // Alocação inicial de memória da struct carrinho
        if (carrinho == NULL) {
            printf("Erro ao alocar memória.\n");
            system("pause");
            exit(1);
        }

    tenteNovamente: // goto
    while (continua == 's' || continua == 'S'){

        if (qtd_Carrinho == espaco){ //realocação de memoria
            espaco *= 2;
            vendaProdutos *temp = (vendaProdutos *)realloc(carrinho, espaco * sizeof(vendaProdutos));
            if (temp == NULL) {
                printf("\n !ERRO NA REALOCAÇÃO DE MEMÓRIA!\n");
                system("pause");
                free(carrinho);
                exit(1);
            }
        carrinho = temp;
        }

        // cria novo ponteiro para adicionar informação no ponteiro de struct carrinho
        // vendaProdutos(struct) *novoItem(Novo pontteiro) = &carrinho(endereço do ponteiro carrinho)[ns posição da var qtd_Carrinho]
        vendaProdutos *novoItem = &carrinho[qtd_Carrinho];

        system("cls");

        if(qtd_Carrinho == 0){
            printf("\nDeseja adicionar um cliente à venda?");
            printf("\n[1] SIM\n[2] NÃO\n");
            printf("- ");
            scanf("%d", &vendaCliente);
            if(vendaCliente == 1){
                printf("\n---------------------------------------\n");
                printf("\n| CÓDIGO |           CLIENTE          |");
                printf("\n---------------------------------------\n");
                for(int i=0; i<tcad; i++){
                    printf("| %-6d | %-26s |\n", cliente[i].codigo, cliente[i].nome);
                }
                printf("\n\n- Informe o código do cliente: ");
                scanf("%d", &codCliente);
                for(int i=0; i<tcad; i++){
                    if(cliente[i].codigo == codCliente){
                        printf("\n- Cliente: %s", cliente[i].nome);
                        strcpy(novoItem->nomeC, cliente[i].nome);
                        novoItem->codC = cliente[i].codigo;
                        novoItem->cpf = cliente[i].cpf;
                    }
                }
            }
            else{
                novoItem->codC = 1;
                strcpy(novoItem->nomeC, "CONSUMIDOR FINAL");
                novoItem->cpf = 0;
            }
        }

        system("cls");
        printf("\n------------------------------ PRODUTOS ------------------------------\n\n");
        printf("| CÓDIGO |      DESCRIÇÃO      |  CATEGORIA  | PREÇO VENDA | ESTOQUE |\n");
        printf("----------------------------------------------------------------------\n");
        for (int i = 0; i < qtdProdutos; i++) {
            printf("| %-6d | %-19s | %-11s | R$%-9.2f | %-7d |\n", produtos[i].Cod, produtos[i].descricaoProd, produtos[i].Categoria, produtos[i].precoVenda, produtos[i].Estoque);
        }

        printf("\nInforme o código do produto desejado: ");
        getchar();
        scanf("%d", &cod_Compra);

        for (int i = 0; i < qtdProdutos; i++){
            if(cod_Compra == 0){
                return 0;
            }
            if (cod_Compra == produtos[i].Cod){
                encontrado = 1; // modifica o valor da variável em caso do item encontrado
                printf("\n- ITEM: %s\n", produtos[i].descricaoProd);
                printf("\nInforme a quantidade desejada: ");
                getchar();
                scanf("%d", &qtd_Compra);
                if (qtd_Compra > produtos[i].Estoque){ // verifica se a quantidade adicionada no carrinho é maior do que o estoque do produto
                    printf("\nQuantidade selecionada maior que quantidade em estoque.\n Deseja continuar? [S/N]: ");
                    scanf(" %c", &cont_Compra);
                    if(cont_Compra == 'n' || cont_Compra == 'N'){ // cancela a operação caso dona Berê não queira continar com a compra
                        printf("\nOperação cancelada!\n");
                        break; // sai do laço
                    }
                }
                if (produtos[i].Estoque <= produtos[i].estoqueMinimo){ // verifica se produto atingiu o estoque minimo
                    printf("\n! ATENÇÃO, PRODUTO ATINGIU O ESTOQUE MINÍMO !\n");
                }
                if (produtos[i].Estoque <= 0){ // verifica se a qtd adicionada no carrinho é maior que o estoque disponível
                    printf("\nProduto com estoque zerado!\n Deseja continuar? [S/N]: ");
                    fflush(stdin);
                    scanf(" %c", &cont_Compra);
                    if(cont_Compra == 'n' || cont_Compra == 'N');{ // cancela a operação caso dona Berê não queira continar com a compra
                        printf("\nOperação cancelada\n");
                        system("pause");
                        break;
                    }
                }
                produtos[i].Estoque -= qtd_Compra;
                // Grava na struct carrinho os dados dos produtos selecionados na hora da compra
                novoItem->Codigo = produtos[i].Cod;
                strcpy(novoItem->Descricao, produtos[i].descricaoProd);
                novoItem->precoItem = produtos[i].precoVenda;
                novoItem->Quantidade = qtd_Compra;
                novoItem->precoTotal = produtos[i].precoVenda * qtd_Compra;

                totalCompra += (produtos[i].precoVenda * qtd_Compra);
            }
        }
        if (encontrado == 0){ // caso produto não seja encontrado
            printf("\nProduto não encontrado, tente novamente.\n\n");
            system ("pause");
            goto tenteNovamente;
        }
        qtd_Carrinho++;
        printf("\nTOTAL: %.2f\n\n", totalCompra);
        printf("\n\nDeseja adicionar mais algum item no carrinho? [S/N]: ");
        fflush(stdin);
        scanf(" %c", &continua);
    }
    system("cls");
    if (cont_Compra != 'n' || cont_Compra != 'N'){
        carregaCarrinho();
    }
    printf("\n\nO total da compra é: R$%.2f\n\n", totalCompra);
    system("pause");
    gravarProdutos(); // Grava as alterações do estoque
    free(produtos);
    return totalCompra;
}

void carregaCarrinho(){
    if (qtd_Carrinho == 0) {
        printf("-----------------------------------------");
        printf("\n\tO CARRINHO ESTÁ VAZIO!");
        printf("\n-----------------------------------------\n");
        return;
    }
    printf("\n----------------------------- CARRINHO -------------------------------\n\n");
    printf("| CÓDIGO |      DESCRIÇÃO      | PREÇO VENDA  | QUANTIDADE |  TOTAL  |\n");
    printf("----------------------------------------------------------------------\n");
    for (int i = 0; i < qtd_Carrinho; i++) {
        printf("| %-6d | %-19s | R$ %-9.2f | %-10d | R$%-3.2f |\n", carrinho[i].Codigo, carrinho[i].Descricao, carrinho[i].precoItem, carrinho[i].Quantidade, carrinho[i].precoTotal);
    }
}
// Função para retirada de dinheiro no caixa
void Sangria(){

        float valorRetirada = 0;
        valorSangria = caixa - 50; // a variável recebe o valor em caixa - 50, garantindo que fique 50 para troco

        if (0 >= valorSangria) { // testa se há valor para a retirada
            system("cls");
            printf("\n--------------------------------------------------------------------------");
            printf("\n\tNÃO É POSSIVEL REALIZAR RETIRADA DO CAIXA VALOR INFERIOR A 50!");
            printf("\n--------------------------------------------------------------------------\n");
            system("pause");
            return;
        }
        refazer: // goto
        system("cls");
        printf("\n-------------- RETIRADA CAIXA --------------\n\n");
        printf("\n- Valor disponível para retirada (salvo R$50,00 para caixa): R$%.2f", valorSangria);
        printf("\n- Informe a quantia que deseja retirar: R$");
        scanf("%f", &valorRetirada);

        if (valorRetirada > valorSangria) { // testa se o valor indicado é maior do que o valor disponível em caixa
            printf("-----------------------------------------");
            printf("\nO VALOR DESEJA UTRAPASSA O PERMITIDO !");
            printf("\n---------------------------------------\n");
            system("pause");
            goto refazer; // vai ao inicio da função, para refazer
        }
        caixa = caixa - valorRetirada; // grava no caixa o valor retirado
        printf("\n\nValor atual no caixa: R$%.2f\n\n", caixa); // mostra o valor restante
        system("pause");
}

void aberturaCaixa(){ // Função para abertura do caixa

        int confirmar = 0;

        if (inicioCx > 0) { // testa se caixa já não foi aberto
            system("cls");
            printf("-----------------------------------------");
            printf("\n\t OPERAÇÃO INVÁLIDA!\t");
            printf("\n\tO CAIXA JÁ ESTÁ ABERTO!");
            printf("\n-----------------------------------------\n");
            system("pause");
            return;
        }
        ajustar:
        system ("cls");
        printf("\n--------------- ABERTURA DE CAIXA ---------------\n");
        printf("\nPOR FAVOR INFORME O VALOR INICIAL NO CAIXA: R$");
        scanf("%f",&inicioCx);
        system("cls");
        printf("\n==============================");
        printf("\nO VALOR NO CAIXA É: R$%.2f\n",inicioCx);
        printf("\nConfirmar valor?");
        printf("\n[1] SIM");
        printf("\n[2] NÃO\n\n");
        scanf("%d", &confirmar);
        printf("===============================\n");
        if (confirmar == 2){
            goto ajustar;
        }
        caixa += inicioCx; // adiciona no caixa o valor do inicio
        system("pause");
    }

// Função para fechamento de caixa, com relatório de faturamento
void fechamento_caixa() { //funcao para fechar caixa
    int op1;
    float valorFechamento = 0;

    if (inicioCx == 0) {  //bloqueio da abertura de caixa
        system("cls");
        printf("-----------------------------------------");
        printf("\n\t OPERAÇÃO INVÁLIDA!\t");
        printf("\n É NECESSÁRIO INICIALIZAR CAIXA PRIMEIRO");
        printf("\n-----------------------------------------\n");
        system("pause");
        return;
    }

    if (qtd_Carrinho > 0){
        system("cls");
        printf("-----------------------------------------");
        printf("\n\t OPERAÇÃO INVÁLIDA!\t");
        printf("\n HÁ VENDAS QUE NÃO FORAM FINALIZADAS");
        printf("\n-----------------------------------------\n");
        system("pause");
        return;
    }

    system("cls");
    printf("\n----- FECHAMENTO DE CAIXA-----\n");
    printf("=================================\n");
    printf("| [1] FECHAR CAIXA              |\n");
    printf("---------------------------------\n");
    printf("| [2] RETORNAR AO MENU          |\n");
    printf("=================================\n");
    scanf("%d", &op1);

    switch(op1){
    case 1:
    system("cls");
    printf("\n============ FATURAMENTOS ===========");
    printf("\n--------------------------------------");
    printf("\n| QUANTIDADE DE VENDAS:        %d", qtdVendas);
    printf("\n| FATURAMENTO TOTAL:         R$%.2f", fatTotal);
    printf("\n| FATURAMENTO EM DINHEIRO:   R$%.2f", fatDinheiro);
    printf("\n| FATURAMENTO EM CARTÃO:     R$%.2f", caixa_cartao);
    printf("\n=====================================\n");
    printf("\n=====================================");
    printf("\n| VALOR EM CAIXA [DINHEIRO]: R$%.2f", caixa);
    printf("\n| VALOR RETIRADO DO CAIXA:   R$%.2f", valorSangria);
    printf("\n|====================================");
    printf("\n| VALOR INICIAL DO CAIXA:    R$%.2f", inicioCx);
    printf("\n|====================================\n");

    valorFechamento = caixa - fatDinheiro - caixa_cartao;
    printf("\nFECHAMENTO: %.2f", valorFechamento);
    system("pause");

    //Reinicializa as variáveis de vendas e valor inicial do caixa para o próximo expediente
    inicioCx = 0;
    fatTotal = 0;
    fatDinheiro = 0;
    caixa_cartao = 0;
    qtdVendas = 0;

    printf("\nCaixa fechado com sucesso!\n");
    system("pause");
    break;
    case 2:
        printf("\nRetornando ao menu...\n\n");
        system("pause");
        main();
    break;
    default:
    fechamento_caixa(); // retorna ao inicio da função
    system ("cls");
    break;
}
}
// Função para pagamento
void facaPagamento(int espaco){

    char desconto = 's', formaPag, finalizarVenda, cpfNota = 's';
    float porcentDesc = 0, totalFinal = 0;
    int opcaoPag = 0, novocpf = 0;


    qtdVendas = lerVendas(); // chama a função responsável por fazer a leitra do arquivo com registro das vendas
    if (qtdVendas > 0) { // se houver registros no arquivo qtdVendas será maior que 0, então espaco recebe o valor de qtdVendas
        espaco = qtdVendas;

    } else {
        espaco = 1;
        infoVendas = (relatVendas *)malloc(espaco * sizeof(relatVendas)); // se não, a alocação inicial é feita aqui
        if (infoVendas == NULL) { // testa se memória foi alocada corretamente
            printf("Erro ao alocar memória.\n");
            exit(1);
        }
    }
    vendasAbertas(espaco); // Verifica se há vendas abertas

    if (totalCompra == 0){ // verifica se foi adicionado algum item no carrinho para prosseguir com a compra
        printf("\n------------------------------------------\n");
        printf("\nNão há itens no carrinho para pagamento \nAdicione itens para prosseguir com a venda\n\n");
        printf("------------------------------------------\n\n");
        system("pause");
        return;
    }

    if (qtdVendas == espaco) { // realoca memória
        espaco *= 2;
        relatVendas *temp = (relatVendas *)realloc(infoVendas, espaco * sizeof(relatVendas));
        if (temp == NULL) {
            printf("\n !ERRO NA REALOCAÇÃO DE MEMÓRIA!\n");
            system("pause");
            free(infoVendas);
            exit(1);
        }
        infoVendas = temp;
    }


    // Criação de novo ponteiro para a atualização dos vendas e incremento a cada repetição do laço
    //relatVendas *novaVenda = &infoVendas[qtdVendas];

    tenteNov: // goto
    system("cls");
    carregaCarrinho(); // mostra os itens no carrinho
    printf("\n\n- O total da compra é: R$%.2f\n\n", totalCompra);
    if(carrinho->codC == 1){
        printf("\n- Deseja incluir CPF na nota? [S/N]: ");
        getchar();
        scanf(" %c", &cpfNota);
        if(cpfNota == 's' || cpfNota == 'S'){
            printf("\nInforme o CPF do cliente [00000000000]: ");
            getchar();
            scanf("%d", &novocpf);
            carrinho->cpf = novocpf;
        }
    }
    printf("\n- Deseja conceder desconto ao cliente? [S/N]: ");
    getchar(); // limpa buffer
    scanf(" %c", &desconto);
    if(desconto == 's' || desconto == 'S'){ // se sim - lê desconto
        printf("\nInforme a porcentagem de desconto: ");
        getchar();
        scanf("%f", &porcentDesc);
        porcentDesc /= 100; // calcula a porcentagem de desconto
        totalFinal = totalCompra - (totalCompra * porcentDesc); // calcula o valor total da compra aplicando o desconto
    }
    else{ // se não - sem desconto
        totalFinal = totalCompra;
    }
    printf("\nTotal da compra: R$%.2f", totalFinal);

    printf("\n\n---- PAGAMENTO ----");
    printf("\n [1] Dinheiro");
    printf("\n [2] Cartão");
    printf("\n [3] Cancelar");
    printf("\n [4] Voltar ao menu\n\n- ");
    scanf("%d", &opcaoPag);

    switch(opcaoPag){
        case 1:
            formaPag = pagDinheiro(totalFinal ,formaPag, espaco); // função para pagamento em dinheiro
        break;
        case 2:
            formaPag = pagCartao(totalFinal, formaPag, espaco); // função pagamento no cartão
        break;
        case 3: // cancela a compra e zera variáveis
            printf("\n A compra foi cancelada");
            printf("\n Retornando ao menu\n");
            // devolve a quantidade de produtos que estava no carrinho para o estoque
            for(int i=0; i<qtd_Carrinho; i++){
                for(int j=0; j<qtdProdutos; j++){
                    if(carrinho[i].Codigo == produtos[j].Cod){
                        produtos[j].Estoque += carrinho[i].Quantidade;
                    }
                }
            }
            totalCompra = 0;
            totalFinal = 0;
            formaPag = 'x';
            gravarProdutos();
            system("pause");
        break;
        case 4: // compra gravada como aberta - não finalizada
            formaPag = 'a';
            printf("\nRetornando ao menu...\n\n");
            system("pause");
        break;
        default:
            printf("\nOpção inválida, tente novamente.\n\n");
            system("pause");
            goto tenteNov;
        break;
    }
    //gravarProdutos();
    // Grava informações na struct de vendas
    numeroVend ++;
    // grava as informações na struct infoVenda
    infoVendas[qtdVendas].numVenda = numeroVend;
    infoVendas[qtdVendas].tipoVenda = formaPag;
    infoVendas[qtdVendas].valorPago = totalFinal;
    infoVendas[qtdVendas].codCliente = carrinho->codC;
    infoVendas[qtdVendas].cpf = carrinho->cpf;
    strcpy(infoVendas[qtdVendas].nomeCliente, carrinho->nomeC);
    qtdVendas++; // incrementa quantidade de vendas

    // zera variáveis
    qtd_Carrinho = 0;
    totalCompra = 0;
    totalFinal = 0;
    free(carrinho);
}
// Função pagamento em dinehiro
char pagDinheiro(float total ,char metodoPag, int espaco){

    float dinheiro = 0, troco = 0;

    system("cls");
    printf("- Valor total da compra: R$%.2f", total);
    printf("\n\n- Informe o valor pago: R$ ");
    scanf("%f", &dinheiro);
    troco = dinheiro - total; //calculo do troco
    if(dinheiro >= total){
        if(caixa < troco){ // se o valor a retornar de troco for maior que o disponivel em caixa. retorna para nova tentativa
            printf("\nNão há saldo suficiente no caixa \nEscolha outra forma de pagamento\n\n");
            system("pause");
            facaPagamento(espaco);
        }
        printf("- O valor a retornar de troco é: R$%.2f", troco);
        metodoPag = 'd';
        printf("\n\nCompra finalizada com sucesso!\n");
        caixa = (caixa + dinheiro) - troco; // adiciona e subtrai valores do caixa
        fatDinheiro += (caixa - inicioCx);
        fatTotal += fatDinheiro;
    }
    else {
        metodoPag = divisaoPag(troco, metodoPag, espaco, dinheiro); // caso o valor pago em dinheiro não seja suficiente para pagamento ocorre divisão
    }
    system("pause");
    return metodoPag;
}

// Quando o valor pago em dinheiro não foi suficiente para pagamento total
char divisaoPag(float troco, char metodoPag, int espaco, float dinheiro){

    float debito = 0;
    int op_debito=0;

    retorno1: //quando cair no default o programa retorna aqui
    debito = fabs(troco); //a função "fabs" serve para mostrar o absoluto do número apontado entre parenteces. fabs serve para float
    system("cls");
    printf("\n!--------------ATENÇÃO--------------!");
    printf("\nValor pago menor do que o valor devido \nFaltam: R$%.2f\n", debito);
    printf("\n[1] Finalizar o pagamento no cartão");
    printf("\n[2] Cancelar a compra \n");
    scanf("%d", &op_debito);

    switch(op_debito){
        case 1:// Finaliza no cartão
            system("cls");
            metodoPag = pagCartao(debito, metodoPag, espaco);
            metodoPag = 'b';
            caixa += dinheiro; // Grava em caixa cartão o valor pago
            fatDinheiro += (caixa - inicioCx); // Grava o faturamenro em dinheiro no caixa descontando o valor inicial do caixa
            fatTotal += fatDinheiro + caixa_cartao; // Grava no faturamento total
        break;
        case 2: // cancela compra
            system("cls");
            printf("\nA compra foi cancelada");
            printf("\nRetornando para o menu inicial\n");
            for(int i=0; i<qtd_Carrinho; i++){
                for(int j=0; j<qtdProdutos; j++){
                    if(carrinho[i].Codigo == produtos[j].Cod){
                        produtos[j].Estoque += carrinho[i].Quantidade;
                    }
                }
            }
            gravarProdutos();
            totalCompra = 0;
            metodoPag = 'x';
            qtd_Carrinho = 0;
            system("pause");
        break;
        default:
            printf("Opção inválida");
            goto retorno1;
        break;
    }
    return metodoPag;
}
// Pagamento no cartão
char pagCartao(float total, char metodoPag, int espaco){

    int cartao = 0;

    system("cls");
    printf("\n- Valor a pagar: R$%.2f \n\n", total);
    printf("\n---------------------------------");
    printf("\n|[0] PAGAMENTO NO CARTAO -NÃO OK-");
    printf("\n|[1] PAGAMENTO NO CARTAO -OK-    ");
    printf("\n---------------------------------");
    printf("\nSelecione a opção: ");
    scanf("%d", &cartao);

    if (cartao == 1) {

        printf("\nTudo certo :)\n");
        metodoPag = 'c';
        caixa_cartao += total;
        fatTotal += caixa_cartao;
    }
    else  { // retorna ao inicio da função para que ocorra nova tentativa de pagamento
        facaPagamento(espaco);
    }

    return metodoPag;
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

void vendasAbertas(int espaco){

    char finalizarVenda, formaPag;
    int opPag = 0;
    float totalPagar = 0;

    for (int i=0; i<qtdVendas; i++){ // Varre a struct para encontrar vendas abertas
        if (infoVendas[i].tipoVenda == 'a'){
            printf("\nHá vendas que não foram finalizadas!\nDeseja finalizar? [S/N]: ");
            scanf(" %c", &finalizarVenda);
            if (finalizarVenda == 's' || finalizarVenda == 'S'){
                printf("------------------------\n");
                printf("| CÓDIGO | PREÇO TOTAL |\n");
                printf("------------------------\n");
                printf("| %-6d | R$%-9.2f |\n", infoVendas[i].numVenda, infoVendas[i].valorPago);
                totalPagar = infoVendas[i].valorPago;
                formaPag = 'a';
                // Pagamento igual ao pagamento de (faca pagamento)
                printf("\n\n---- PAGAMENTO ----");
                printf("\n [1] Dinheiro");
                printf("\n [2] Cartão");
                printf("\n [3] Cancelar");
                printf("\n [4] Voltar ao menu\n\n- ");
                scanf("%d", &opPag);
                getchar();

                switch(opPag){
                case 1:
                    formaPag = pagDinheiro(totalPagar ,formaPag, espaco);
                break;
                case 2:
                    formaPag = pagCartao(totalPagar ,formaPag, espaco);
                break;
                case 3:
                    printf("\n A compra foi cancelada\n\n");
                    totalCompra = 0;
                    totalPagar = 0;
                    formaPag = 'x';
                    infoVendas[i].valorPago = 0;
                    // devolve a quantidade de produtos que estava no carrinho para o estoque
                    for(int i=0; i<qtd_Carrinho; i++){
                        for(int j=0; j<qtdProdutos; j++){
                            if(carrinho[i].Codigo == produtos[j].Cod){
                                produtos[j].Estoque += carrinho[i].Quantidade;
                            }
                        }
                    }
                    gravarProdutos();
                    system("pause");
                break;
                case 4:
                    formaPag = 'a';
                    printf("\nRetornando ao menu...\n\n");
                    system("pause");
                    return;
                break;
                default:
                    printf("\nOpção inválida, tente novamente.\n\n");
                    system("pause");
                    return;
                break;
            }

            infoVendas[i].tipoVenda = formaPag; // Atualiza a forma de pagamento
            }
        }
    }
    system("cls");
}

void sair() {

    //declaração de variáveis
    int sair = 0;

    if (inicioCx > 0) {
            system("cls");
            printf("-----------------------------------------");
            printf("\n\t OPERAÇÃO INVÁLIDA!\t");
            printf("\n\tO CAIXA ESTA ABERTO!");
            printf("\n-----------------------------------------\n");
            system("pause");
            main();
        }
    system("cls");
        printf("\nTem certeza que deseja sair?\n");
        printf("\n[1] SIM");
        printf("\n[2] NÃO\n");
        scanf("%d", &sair);
        getchar();

        if (sair == 1) {
            printf("\nATÉ LOGO :)\n");
            free(produtos);
            free(carrinho);
            free(infoVendas);
            exit(0);
        }
        else {
            main();
        }
    }
