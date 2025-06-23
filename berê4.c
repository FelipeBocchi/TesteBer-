//              CADASTROS
//  ----- INCLUDES/DEFINE ------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#ifdef _WIN32
    #include <windows.h>
    #define delay(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define delay(ms) usleep(ms * 1000)
#endif
#define USUARIO "usuarios.bin"
#define CLIENTE "clientes.bin"
#define PRODUTO "produtos.bin"
#define CATEGORIA "categorias.bin"
#define VENDAS "vendas.bin"
#define PAGAMENTO "pagamento.bin"
#define Max_cpf 12
#define Max_Login 13
#define Max_Senha 9
#define Max_Nome_Comp 100
#define Max_nome_Soc 50
#define Max_endereco 150

//  ---- STRUCT  ------
typedef struct
{
    int id;
    char login[Max_Login];
    char senha[Max_Senha];
    int tipo_Usuario;
}Usuario;

typedef struct
{
    int id;
    char nome_completo[Max_Nome_Comp];
    char nome_social[Max_nome_Soc];
    int cpf;
    char rua[Max_endereco];
    int num;
    char bairro[Max_endereco];
    int telefone;
}Cliente;

typedef struct
{
    int id;
    char desclicao[Max_Nome_Comp];
    int categoria;
    float preco_Compra;
    float margem_Lucro;
    float preco_Venda;
    int quant_Estoque;
    int estoque_Minimo;
}Produto;

typedef struct
{
    char nome[Max_nome_Soc];
    int num_Id;
}Categoria;

typedef struct cliOrdenado
{
    int id;
    int cpf;
    int telefone;
    char nome_completo[Max_Nome_Comp];
    char nome_social[Max_nome_Soc];
    struct cliOrdenado *prox;
}cliOrdenado;

typedef struct prodOrden
{
    int id;
    int estoque;
    float compra;
    float venda;
    char descricao[Max_Nome_Comp];
    struct prodOrden *prox;
}OrdenaProd;


typedef struct
{
    int num_Venda;
    int dia_Venda;
    float total_Pago;
    float valor_Pago_Dinhe;
    float valor_Pago_Cat;
    char tipo_De_Pagamento[2];
    char nome_Cliente[Max_Nome_Comp];
}Pagamento;

typedef struct Itens
{
    int id;
    int quantidade_levada;
    float preco_Venda;
    float valor_total;
    char desclicao[Max_Nome_Comp];
    struct Itens *prox;
}Itens;

typedef struct Carrinho
{
    int num_Da_Venda;
    int id_Cliente;
    int data_Di, data_Mes, data_Ano;
    Itens *iten;
    float total_Venda;
    char pagamento;
    char tipo_Pagamento[2];
    struct Carrinho *prox;
}Carrinho;

//  ---- VARIÁVEIS GLOBAIS -------
int aber_Fecha;
float valor_No_Caixa, valor_Na_Aber, total_D=0, total_C=0, abre_Menu=0;
float margem_De_lucro = 20;
Usuario user_Atual;

//  ---- DECLARAÇÃO DE FUNÇÕES -----
void menu_Cadastros(FILE *, FILE *ptFc, FILE *ptFp, FILE *ptFct);
void cadatro_Usuario(FILE *);
void cadastro_Cliente(FILE *);
void cadastro_Produtos(FILE *, FILE *);
void cadastro_Categorias(FILE *);
void limpar_Terminal(void);
void valida_Abertura_Arq(FILE *);
void limpa_buff(void);
void tira_espaco(char[]);
void menu_pagamento(Carrinho **);
void produtos_Disponivies();
void produtos_No_Carrinho(Carrinho *);
void novas_Vendas(Carrinho**);
void atualizar_Estoque(int, int);
void adicionar_Item(Itens **, Itens *);
void adicionar_Carrinho(Carrinho **, Carrinho *);
void resulmo_Da_Venda(Itens *, Carrinho *);
void excluir_Prod_Carr(char, Itens **);
void sanguia(Carrinho **);
void pagamento(Carrinho **);
void salva_Pagamento(Pagamento);
void registar_Usuario_Atual(void);
void abretura_Caixa(void);
void fechamento_Caixa(Carrinho **);
void relatorio(void);
void listar_Clientes(void);
void listar_Clientes_Ordenadamente(void);
void ordena(cliOrdenado **, Cliente );
void lista_Clientes_Compraram(void);
void lista_Produtos(void);
void lista_Produtos_Orden(void);
void ordena_Produtos(OrdenaProd **, Produto);
void lista_Produtos_Zerados(void);
void lista_Produtos_Mais_Vendidos(void);
void lista_Vendas(void);
void lista_Vendas_Periodo(void);
void faturamento_Consolidado(void);
void liberarMemoriaCarrinho(Carrinho **);
Carrinho *cadastrar_Carr(void);
Itens *salvar_Prod_Carr(Itens , int);
int gera_id(const char *, size_t);
int menu_Principal(void);

//  ----- FUNÇÃO PRINCIPAL -----
int main()
{
    setlocale(LC_ALL, "portuguese");
    int opcao;

    //  ----- ARQUIVOS DE CADASTROS ----
    FILE *ptFu = fopen(USUARIO, "a+b");
    valida_Abertura_Arq(ptFu);
    FILE *ptFc =  fopen(CLIENTE, "a+b");
    valida_Abertura_Arq(ptFc);
    FILE *ptFp = fopen(PRODUTO, "a+b");
    valida_Abertura_Arq(ptFp);
    FILE *ptFct = fopen(CATEGORIA, "a+b");
    valida_Abertura_Arq(ptFct);

    //  ----- LISTAS ENCADEADAS -----
    Carrinho *lista = NULL;

    // ----- login de usuario ----
    user_Atual.tipo_Usuario=0;
    registar_Usuario_Atual();

    do
    {
        switch(opcao = menu_Principal())
        {
            case 1:
                menu_Cadastros(ptFu, ptFc, ptFp, ptFct);
            break;

            case 2:
                if(abre_Menu == 1)
                    menu_pagamento(&lista);
                else
                    printf("\n%50s O caixa ainda nao foi abreto!!!", "");
            break;

            case 3:
                abretura_Caixa();
            break;

            case 4:
                fechamento_Caixa(&lista);
            break;

            case 5:
                relatorio();
            break;

            case 6:
                registar_Usuario_Atual();
            break;

            case 7:
            break;
            default:
            break;
        }
    } while(opcao != 7);

    fclose(ptFu);
    fclose(ptFp);
    fclose(ptFct);
    fclose(ptFc);
    return 0;
}
//  ------                -------

int menu_Principal()
{
    int op;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%50s - MENU PRINCIPAL -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s [1] Cadatro", "");
    printf("\n%50s [2] Vendas", "");
    printf("\n%50s [3] Abreturaa de  caixa", "");
    printf("\n%50s [4] Fechamento de caixa", "");
    printf("\n%50s [5] Relatorio", "");
    printf("\n%50s [6] Trocar de usuario", "");
    printf("\n%50s [7] Sair", "");
    printf("\n%50s Opcao:", "");

    scanf("%d", &op);
    return op;
}

void limpar_Terminal()
{
    //vai verificar qual sistema operacional o user esta usando
    #ifdef _WIN32           //windows
        system("cls");      //limpa o terminal
    #else
        system("clear");
    #endif
}

void valida_Abertura_Arq(FILE *Arq)
{
    if(Arq == NULL)
    {
        printf("\n%50s Erro ao abrir o arquivo", "");
        return;
    }else
    {
        printf("\n%50s Sucesso ao abrir o arquivo", "");
    }

}

void limpa_buff()
{
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}

void tira_espaco(char string[])
{
    string[strcspn(string, "\n")] = '\0';
}

int gera_id(const char *arquivo, size_t tamanho_struct) {
    FILE *arq = fopen(arquivo, "rb");
    if (!arq) return 1;
    fseek(arq, 0, SEEK_END);
    int id = ftell(arq) / tamanho_struct;
    fclose(arq);
    return id + 1;
}

void menu_Cadastros(FILE *ptFu, FILE *ptFc, FILE *ptFp, FILE *ptFct)
{
    Categoria temp;
    int op;

    do
    {
        limpar_Terminal();
        printf("\n%45s ==================================", "");
        printf("\n%53s - MENU CADASTROS -", "");
        printf("\n%45s ==================================", "");
        printf("\n%50s [1] Cadatro de usuarios", "");
        printf("\n%50s [2] Cadastro de clientes", "");
        printf("\n%50s [3] Cadatro de prdutos", "");
        printf("\n%50s [4] Cadastro de categorias", "");
        printf("\n%50s [5] Voltar ao menu principal", "");
        printf("\n%50s Opcao:", "");
        scanf("%d", &op);
        switch(op)
        {
            case 1:
                cadatro_Usuario(ptFu);
            break;

            case 2:
                cadastro_Cliente(ptFc);
            break;

            case 3:
                if((fread(&temp, sizeof(Categoria), 1, ptFct)) == 1)
                    cadastro_Produtos(ptFp, ptFct);//Só pode cadastrar produtos se tiver categoria
                else
                    printf("\n%50s Nao ha categorias cadastradas!!!", "");

            break;

            case 4:
                cadastro_Categorias(ptFct);
            break;

            case 5:
                printf("\n%50s Voltando ao menu principal...", "");
            break;

            default:
                menu_Cadastros(ptFu, ptFc, ptFp, ptFct);
            break;
        }
    } while (op != 5);

}

void cadatro_Usuario(FILE *ptFu)
{
    //int login_len;
    //int senha_len;
    Usuario user;

    printf("\n%45s ==================================", "");
    printf("\n%53s - CADASTRAR USUARIO -", "");
    printf("\n%45s ==================================\n", "");
    printf("\n%50s - INFORMACOES -", "");

    //do {
        printf("\n%50s Login (8-12): ", "");
        limpa_buff();
        fgets(user.login, sizeof(user.login), stdin);
        tira_espaco(user.login);
        //login_len = strlen(user.login);
    //} while(strlen(user.login) < 8 || strlen(user.login) > 12);

    //do {
        printf("\n%50s Senha (6-8): ", "");
        limpa_buff();
        fgets(user.senha, sizeof(user.senha), stdin);
        tira_espaco(user.senha);
       // senha_len = strlen(user.senha);
    //} while(senha_len < 6 || senha_len > 8);

    do
    {
        printf("\n%50s Tipo de usuario", "");
        printf("\n%50s [1] Administador de sistemas", "");
        printf("\n%50s [2] Usuario", "");
        printf("\n%50s Opcao:", "");
        scanf("%d", &user.tipo_Usuario);
        limpa_buff();
    } while (user.tipo_Usuario != 1 && user.tipo_Usuario != 2);

    user.id  = gera_id(USUARIO, sizeof(Usuario));
    fseek(ptFu, 0, SEEK_END);  // Posiciona no fim
    fwrite(&user, sizeof(Usuario), 1, ptFu);
    fflush(ptFu);
}

void cadastro_Cliente(FILE *ptFc)
{
   Cliente person;

    printf("\n%45s ==================================", "");
    printf("\n%53s - CADATRAR CLIENTES -", "");
    printf("\n%45s ==================================\n", "");
    printf("\n%50s - INFORMACOES -", "");

    printf("\n%50s Nome Completo: ", "");
    limpa_buff();
    fgets(person.nome_completo, sizeof(person.nome_completo), stdin);
    tira_espaco(person.nome_completo);

    printf("\n%50s Nome Social: ", "");
    fgets(person.nome_social, sizeof(person.nome_social), stdin);
    tira_espaco(person.nome_social);

    //do
    //{
        printf("\n%50s Cpf: ", "");
        scanf("%d", &person.cpf);
   // } while(person.cpf <= 0 || person.cpf > Max_cpf);


    printf("\n%50s Rua: ", "");
    limpa_buff();
    fgets(person.rua, sizeof(person.rua), stdin);
    tira_espaco(person.rua);

    //do
    //{
        printf("\n%50s Numero da casa: ", "");
        scanf("%d", &person.num);
    //} while (person.num < 0);

    printf("\n%50s Bairro: ", "");
    limpa_buff();
    fgets(person.bairro, sizeof(person.bairro), stdin);
    tira_espaco(person.bairro);

    //do
    //{
       printf("\n%50s Telefone: ", "");
       scanf("%d", &person.telefone);
   // } while (person.telefone < 0 && person.telefone > 9);

    person.id = gera_id(CLIENTE, sizeof(Cliente));

    fseek(ptFc, 0, SEEK_END);  // Posiciona no fim
    fwrite(&person, sizeof(Cliente), 1, ptFc);
    fflush(ptFc);
}

void cadastro_Produtos(FILE *ptFp, FILE *ptFct)
{
    Produto iten;
    Categoria temp;

    printf("\n%45s ==================================", "");
    printf("\n%53s - CADATRAR PRODUTOS -", "");
    printf("\n%45s ==================================\n", "");
    printf("\n%50s - INFORMACOES -", "");

    printf("\n%50s Nome:", "");
    limpa_buff();
    fgets(iten.desclicao, sizeof(iten.desclicao), stdin);
    tira_espaco(iten.desclicao);

    printf("\n%50s Categoria", "");
    rewind(ptFct);
    while((fread(&temp, sizeof(Categoria), 1, ptFct)) == 1)
    {
        printf("\n%50s [%d] %s", "", temp.num_Id, temp.nome);
    }
    printf("\n%50s Opcao:", "");
    scanf("%d", &iten.categoria);

    do
    {
        printf("\n%50s Preco de Compra:", "");
        scanf("%f", &iten.preco_Compra);
    } while (iten.preco_Compra < 0);

    do
    {
        printf("\n%50s Quantidade no Estoque:", "");
        scanf("%d", &iten.quant_Estoque);
    } while (iten.quant_Estoque < 0);

    do
    {
        printf("\n%50s Quantidade Minima de Estoque:", "");
        scanf("%d", &iten.estoque_Minimo);
    } while (iten.estoque_Minimo < 0);

    margem_De_lucro /=100;
    iten.preco_Venda = iten.preco_Compra + (iten.preco_Compra * margem_De_lucro);

    iten.id = gera_id(PRODUTO, sizeof(Produto));

    fseek(ptFp, 0, SEEK_END);  // Posiciona no fim
    fwrite(&iten, sizeof(Produto), 1, ptFp);
    fflush(ptFp);
}

void cadastro_Categorias(FILE *ptFct)
{
    Categoria catego;

    printf("\n%45s ==================================", "");
    printf("\n%53s - CADATRAR CATEGORIAS -", "");
    printf("\n%45s ==================================\n", "");
    printf("\n%50s - INFORMACOES -", "");

    printf("\n%50s Nome:", "");
    limpa_buff();
    fgets(catego.nome, sizeof(catego.nome), stdin);
    tira_espaco(catego.nome);

    catego.num_Id = gera_id(CATEGORIA, sizeof(Categoria));

    fseek(ptFct, 0, SEEK_END);  // Posiciona no fim
    fwrite(&catego, sizeof(Categoria), 1, ptFct);
    fflush(ptFct);
}

void menu_pagamento(Carrinho **lista)
{
    int op;

    do
    {
        limpar_Terminal();
        printf("\n%45s ==================================", "");
        printf("\n%53s - MENU VENDAS -", "");
        printf("\n%45s ==================================", "");
        printf("\n%50s [1] Nova vendas", "");
        printf("\n%50s [2] Retirada de caixa", "");
        printf("\n%50s [3] Pagamento", "");
        printf("\n%50s [4] Voltar ao menu principal", "");
        printf("\n%50s Opcao:", "");
        scanf("%d", &op);

        switch(op)
        {
            case 1:
                novas_Vendas(lista);
            break;

            case 2:
                sanguia(lista);
            break;

            case 3:
                pagamento(lista);
            break;

            case 4:

            break;

            default:
            break;
        }
    } while (op != 4);

}

void produtos_Disponivies()
{
    Produto itens;

    FILE *ptFp = fopen(PRODUTO, "rb");
    valida_Abertura_Arq(ptFp);

    printf("\n%45s ==================================", "");
    printf("\n%53s - PRODUTOS -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - %4s %16s %6s %6s -\n", "", "ID", "DESCRICAO", "ESTOQUE", "VALOR");

    rewind(ptFp);
    while(fread(&itens, sizeof(Produto), 1, ptFp))
    {
        printf("\n%50s - %4d%16s%6d%6.2f -", "", itens.id, itens.desclicao, itens.quant_Estoque, itens.preco_Venda);
    }

    fclose(ptFp);
}

void produtos_No_Carrinho(Carrinho *lista)
{

    printf("\n%45s ==================================", "");
    printf("\n%53s - CARRINHO -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s- %4s %16s %6s %6s -\n","", "ID", "DESCRICAO", "QTD", "VALOR");

    Itens *aux;
    for (Carrinho *c = lista; c != NULL; c = c->prox) {
        for (aux = c->iten; aux != NULL; aux = aux->prox) {
            printf("\n%50s- %4d %16s %6d %6.2f -\n","", aux->id, aux->desclicao, aux->quantidade_levada, aux->preco_Venda);
        }
    }
    printf("\n%45s ==================================", "");
}

void novas_Vendas(Carrinho **lista)
{
    int codigo, quantidade, produtos_encontrados=0;
    char op, op_Quant, op_Excluir;
    Produto temp;
    Itens tempor, *iten_Do_Carr;

    //cria um novo carrinho para esse cliente
    Carrinho *carrinho = cadastrar_Carr();

    do
    {
        limpar_Terminal();
        produtos_Disponivies();
        produtos_No_Carrinho(carrinho);

        FILE *ptFp = fopen(PRODUTO, "rb");
        valida_Abertura_Arq(ptFp);

        do
        {
            printf("\n%50s Informe o codigo: ", "");
            scanf("%d", &codigo);
        } while(codigo < 0);

        rewind(ptFp);
        while(fread(&temp, sizeof(Produto), 1, ptFp) == 1)
        {
            if(codigo == temp.id)
            {
                produtos_encontrados = 1;
                if(temp.quant_Estoque == 0)
                    printf("\n%50s O estoque desse produto está vazio!!!", "");
                else
                {
                    printf("\n%50s Informe a quantidade: ", "");
                    scanf("%d", &quantidade);

                    if(quantidade > temp.quant_Estoque)
                    {
                        printf("\n%50s Quantidade de itens e superior ao estoque! Deseja contiar(s/n): ", "");
                        limpa_buff();
                        scanf("%c", &op_Quant);
                        if(op_Quant == 'n')
                            break;
                    }

                    temp.quant_Estoque -= quantidade;
                    if(temp.estoque_Minimo == temp.quant_Estoque)
                        printf("\n%50s Quantidade de estoque minimo atingida!!!\n", "");

                    //passa as informações do produto para a lista itens
                    tempor.id=temp.id;strcpy(tempor.desclicao, temp.desclicao);tempor.preco_Venda=temp.preco_Venda;
                    iten_Do_Carr = salvar_Prod_Carr(tempor, quantidade);

                    //adiciona o item no carrinho
                    adicionar_Item(&(carrinho->iten), iten_Do_Carr);

                    atualizar_Estoque(codigo, quantidade);

                    printf("\n%50s Deseja adicionar mais algun produto ao carrinho? (s/n): ", "");
                    limpa_buff();
                    scanf("%c", &op);
                }
            }
        }

        if(produtos_encontrados == 0)
        printf("\n%50s Codigo informado invalido ou nao existe!!!", "");

        fclose(ptFp);
    } while(op != 'n');

    //resulmo da compra
    do
    {
        limpar_Terminal();
        produtos_No_Carrinho(carrinho);
        printf("\n%50s - Deseja excluir algum produto (s/n): ", "");
        limpa_buff();
        scanf("%c", &op_Excluir);
        excluir_Prod_Carr(op_Excluir, &(carrinho->iten));
    }while(op_Excluir != 'n');

    //trás o resulmo da compra e faz o desconto se nessesário
    resulmo_Da_Venda(carrinho->iten, carrinho);
    printf("\n%d\n", carrinho->id_Cliente);
    //adiciona carrinho a lista
    adicionar_Carrinho(lista, carrinho);
}

void atualizar_Estoque(int idProduto, int quantidadeComprada)
{
    FILE *ptFp = fopen(PRODUTO, "rb+");
    if (!ptFp) {
        printf("\nERRO: Arquivo de estoque inacessível!\n");
        return;
    }

    Produto produto;
    int encontrado = 0;

    while (fread(&produto, sizeof(Produto), 1, ptFp)) {
        if (produto.id == idProduto) {
            produto.quant_Estoque -= quantidadeComprada;

            // Proteção contra estoque negativo
            if (produto.quant_Estoque < 0) {
                produto.quant_Estoque = 0;
            }

            // Volta o ponteiro para reescrever o produto atualizado
            //FSEEK - posicionando o ponteiro de posição no arquivo onde achou o ID
            //FWRITE - grava - o tamanho da struct, no exato ligar onde paraou o ponteiro
            fseek(ptFp, -sizeof(Produto), SEEK_CUR);
            fwrite(&produto, sizeof(Produto), 1, ptFp);
            encontrado = 1;
            break;
        }
    }

    fclose(ptFp);

    if (!encontrado) {
        printf("\nAVISO: Produto ID %d não encontrado!\n", idProduto);
    }
}

Carrinho *cadastrar_Carr()
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    int id;
    Cliente temp;

    FILE *ptFc = fopen(CLIENTE, "rb");
    valida_Abertura_Arq(ptFc);

    Carrinho *carr = (Carrinho *)  malloc(sizeof(Carrinho));

    //cadastrar o numero da vendo, que vai ser o numero do nosso carrinho
    carr->num_Da_Venda = gera_id(VENDAS, sizeof(Carrinho));

    //cadastra o id do cliente, como nosso id é gerado automaticamente esse menu vai mostrar os disponiveis
    printf("\n%45s ==================================", "");
    printf("\n%53s - CLIENTES -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - %4s%16s -\n", "", "ID", "NOME");
    rewind(ptFc);
    while(fread(&temp, sizeof(Cliente), 1, ptFc) == 1)
    {
        printf("\n%50s %4d%16s", "", temp.id, temp.nome_completo);
    }
    printf("\n%45s ==================================", "");
    printf("\n%50s Informe o id do cliente: ", "");
    scanf("%d", &id);
    carr->id_Cliente = id;

    //cadastra a data na horada compra
    carr->data_Di = tm_info->tm_mday;
    carr->data_Mes = tm_info->tm_mon + 1;
    carr->data_Ano = tm_info->tm_year + 1900;

    //valor total das vendas desse carrinho
    carr->total_Venda = 0;

    //situação do pagamento começa aberta
    carr->pagamento = 'a';

    //a lista de produtos do carrinho
    carr->iten = NULL;

    //como inicializamos a lista carrinho o proximo vai ser null
    carr->prox = NULL;

    fclose(ptFc);
    return carr;
}

Itens *salvar_Prod_Carr(Itens produto, int quantidade)
{
    Itens *novo = (Itens*) malloc(sizeof(Itens));

    novo->id = produto.id;
    strcpy(novo->desclicao, produto.desclicao);
    novo->preco_Venda = produto.preco_Venda;
    novo->quantidade_levada = quantidade;
    novo->valor_total = (quantidade * produto.preco_Venda);
    novo->prox = NULL;

    return novo;
}

void adicionar_Item(Itens **lista, Itens *novoItem) {
    if (*lista == NULL) {
        *lista = novoItem;
    } else {
        Itens *aux = *lista;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novoItem;
    }
}

void adicionar_Carrinho(Carrinho **lista, Carrinho *novoCarrinho) {
    if (*lista == NULL) {
        *lista = novoCarrinho;
    } else {
        Carrinho *aux = *lista;
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
        aux->prox = novoCarrinho;
        //*lista = aux;
    }
}

void excluir_Prod_Carr(char op, Itens **lista)
{
    int id;
    Itens *atual = *lista;
    Itens *anterior = NULL;

    if(op == 's')
    {
        printf("\n%45s ==================================", "");
        printf("\n%50s Informe o codigo do produto a ser excluido: ", "");
        scanf("%d", &id);

        while(atual)
        {
            if(atual->id == id)
            {
                if(anterior == NULL)
                {
                    *lista = atual->prox;
                }else
                {
                    anterior->prox  = atual->prox;
                }

                free(atual);
                printf("\n%50s Produto removido do carrinho!!!", "");
                return;
            }
            anterior = atual;
            atual  = atual->prox;
        }
        printf("\n%50s Produto não encontrado no carrinho!", "");
    }
}

void resulmo_Da_Venda(Itens *lista, Carrinho *carrinho)
{
    float total_A_Pagar=0, desconto;
    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - CARRINHO DO %s -", "", "");//nome do cliente
    printf("\n%45s ==================================", "");
    printf("\n%50s- %4s %16s %6s %6s %6s -\n","", "ID", "DESCRICAO", "QTD", "VALOR", "TOTAL");

    Itens *aux, *aux_S;

    for(aux = lista; aux != NULL; aux = aux->prox)
    {
        float total = aux->preco_Venda * aux->quantidade_levada;
        printf("\n%50s- %4d %16s %6d %6.2f %6.2f -\n","", aux->id, aux->desclicao, aux->quantidade_levada, aux->preco_Venda, total);
    }
    printf("\n%45s ==================================", "");

    for(aux_S = lista; aux_S != NULL; aux_S = aux_S->prox)
        total_A_Pagar += aux_S->valor_total;
    printf("\n%50s \t\tTOTAL A PAGAR: %.2f", "", total_A_Pagar);

    printf("\n%50s Ha desconto nessa venda (0-Nao ha / informe a porcentagem): ", "");
    scanf("%f", &desconto);
    if(desconto > 0 && desconto < 100)
    {
        desconto = desconto/100;
        total_A_Pagar = total_A_Pagar * (1-desconto);
    }

    printf("\n%50s \t\tTOTAL FINAL: %.2f", "", total_A_Pagar);

    carrinho->total_Venda = total_A_Pagar;
}

void sanguia(Carrinho **lista)
{
    float valor_Caixa = 0, quantia_Retirada;
    Pagamento conta;
    Carrinho *aux;

    if(user_Atual.tipo_Usuario == 1) {
        FILE *ptFpg = fopen(PAGAMENTO, "rb");
        valida_Abertura_Arq(ptFpg);

        // Soma apenas pagamentos em dinheiro ('d' ou 'md')
        for(aux = *lista; aux != NULL; aux = aux->prox)
        {
            if(strcmp(aux->tipo_Pagamento, "d") == 0)
                valor_Caixa += conta.valor_Pago_Dinhe;
            valor_Caixa += valor_Na_Aber;
        }

        printf("\n%50s O caixa tem R$ %.2f em notas", "", valor_No_Caixa);
        printf("\n%50s Deseja retirar qual quantia? (valor mínimo de caixa R$50,00): ", "");
        scanf("%f", &quantia_Retirada);

        if((valor_Caixa - quantia_Retirada) >= 50) {
            valor_No_Caixa -= quantia_Retirada;
            printf("\n%50s Retirada realizada! Novo saldo: R$%.2f", "", valor_No_Caixa);
        } else {
            printf("\n%50s Valor retirado deixaria o caixa com menos de R$50,00!", "");
        }

        fclose(ptFpg);
    } else {
        printf("\n%50s Apenas administradores podem fazer retiradas!", "");
    }
    delay(1000);
}

void pagamento(Carrinho **lista)
{
    int op, valida_Maquininha, id_Carr;
    float valor_Venda, valor_Recebido, valor_Troco;
    char valida_Dinheiro;
    Cliente cliente;
    Pagamento conta;
    Carrinho *c, *aux;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - CARRINHOS EM ABERTO -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s- %4s %16s %2s %6s -\n","", "ID", "NOME", "SITUACAO", "TOTAL");

    FILE *ptFc = fopen(CLIENTE, "rb");
    valida_Abertura_Arq(ptFc);
    for(c = *lista; c != NULL; c = c->prox)
    {
        rewind(ptFc);
        while(fread(&cliente, sizeof(Cliente), 1, ptFc))
        {
            if(c->id_Cliente == cliente.id)
            {
                printf("\n%50s- %4d %16s %2c %6.2f -", "", c->id_Cliente, cliente.nome_completo, c->pagamento, c->total_Venda);
            }
        }
        //fseek(ptFc, (c->id_Cliente - 1)*sizeof(Cliente), SEEK_SET );
        //fread(&cliente, sizeof(Cliente), 1, ptFc);

    }
    printf("\n%45s ==================================", "");
    printf("\n%50s Informe o ID do carrinho a ser pago: ", "");
    scanf("%d", &id_Carr);
    for(aux = *lista; aux != NULL; aux = aux->prox)
    {
        if(id_Carr  == aux->id_Cliente)
        {
            if(aux->pagamento == 'a')
            {
                conta.num_Venda  = id_Carr;
                conta.dia_Venda = aux->data_Di;
                fseek(ptFc, (aux->id_Cliente - 1)*sizeof(Cliente), SEEK_SET );
                fread(&cliente, sizeof(Cliente), 1, ptFc);
                strcpy(conta.nome_Cliente, cliente.nome_completo);
                conta.total_Pago = aux->total_Venda;
                valor_Venda = aux->total_Venda;

                do
                {
                    limpar_Terminal();
                    printf("\n%45s ==================================", "");
                    printf("\n%53s - PAGAMENTO -", "");
                    printf("\n%45s ==================================", "");
                    printf("\n%50s [1] Pagar com cartao", "");
                    printf("\n%50s [2] Pagar com dinheiro", "");
                    printf("\n%50s [3] Voltar ao menu anterior", "");
                    printf("\n%50s Opcao:", "");
                    scanf("%d", &op);

                    switch(op)
                    {
                        case 1:
                            printf("\n%45s ==================================", "");
                            printf("\n%53s - CARTAO -", "");
                            printf("\n%45s ==================================", "");
                            printf("\n%50s Aproximando cartao.....", "");
                            delay(1000);
                            printf("\n%50s Total da conta: R$%.2f", "", aux->total_Venda);
                            printf("\n%50s [1] Cartao passado com sucesso...", "");
                            printf("\n%50s [0] Erro ao passar o cartao", "");
                            do
                            {
                                printf("\n%50s Opcao:", "");
                                scanf("%d", &valida_Maquininha);
                            } while(valida_Maquininha !=  1  && valida_Maquininha != 0);

                            if(valida_Maquininha == 1)
                            {
                                strcpy(conta.tipo_De_Pagamento, "c");
                                aux->pagamento = 'f';
                                conta.valor_Pago_Cat = valor_Venda;
                                total_C += valor_Venda;
                                strcpy(aux->tipo_Pagamento, 'c');
                                salva_Pagamento(conta);
                            }
                        break;

                        case 2:
                            printf("\n%45s ==================================", "");
                            printf("\n%53s - DINHEIRO -", "");
                            printf("\n%45s ==================================", "");
                            printf("\n%50s - Total da conta: R$%.2f", "", aux->total_Venda);
                            printf("\n%50s - Quantidade recebida: ", "");
                            do
                            {
                                scanf("%f", &valor_Recebido);
                            }while(valor_Recebido < 0);

                            if(valor_Recebido < valor_Venda)
                            {
                                do
                                {
                                    printf("\n%50s Quantia recebida inferior ao valor do carrinho!!!", "");
                                    printf("\n%50s Deseja pagar o restante no cartao (s/n): ", "");
                                    limpa_buff();
                                    scanf("%c", &valida_Dinheiro);
                                    if(valida_Dinheiro == 's')
                                    {
                                        printf("\n%50s Restante pago no cartao...", "");
                                        strcpy(conta.tipo_De_Pagamento, "md");
                                        aux->pagamento = 'f';
                                        valor_No_Caixa += valor_Recebido;
                                        conta.valor_Pago_Dinhe =  valor_Recebido;
                                        total_D += valor_Recebido;
                                        conta.valor_Pago_Cat = valor_Venda - valor_Recebido;
                                        total_C += valor_Venda - valor_Recebido;
                                        strcpy(aux->tipo_Pagamento, 'md');
                                        salva_Pagamento(conta);
                                    }
                                } while(valida_Dinheiro == 's');
                            }else if(valor_Recebido > valor_Venda)
                            {
                                printf("\n%50s Quantia recebida maior do que o valor do carrinho!!!", "");
                                valor_Troco = valor_Venda - valor_Recebido;
                                valor_No_Caixa += valor_Recebido;
                                valor_No_Caixa -= valor_Troco;
                                printf("\n%50s Toma aqui seu troco: R$%.2f", "", valor_Troco);

                                strcpy(conta.tipo_De_Pagamento, "d");
                                aux->pagamento = 'f';
                                conta.valor_Pago_Dinhe = valor_Recebido;
                                total_D += valor_Recebido;
                                strcpy(aux->tipo_Pagamento, 'd');
                                salva_Pagamento(conta);
                            }else
                            {
                                printf("\n%50s Carrinho pago com sucesso...", "");
                                strcpy(conta.tipo_De_Pagamento, "d");
                                aux->pagamento = 'f';
                                valor_No_Caixa += valor_Venda;
                                conta.valor_Pago_Dinhe = valor_Venda;
                                total_D += valor_Venda;
                                strcpy(aux->tipo_Pagamento, 'd');
                                salva_Pagamento(conta);
                            }

                        break;

                        case 3:
                            printf("\n%50s Voltando ao menu anterior....", "");
                            delay(1000);
                        break;

                        default:
                        break;
                    }
                } while (op != 3);
            }else
            {
                printf("\n%50s Venda ja esta fechada...", "");
                delay(1000);
            }
        }
    }
fclose(ptFc);
}

void salva_Pagamento(Pagamento conta)
{
    Pagamento temp;
    FILE *ptFpg = fopen(PAGAMENTO, "ab");
    valida_Abertura_Arq(ptFpg);

    temp.dia_Venda = conta.dia_Venda;
    temp.num_Venda = conta.num_Venda;
    temp.total_Pago = conta.total_Pago;
    strcpy(temp.nome_Cliente, conta.nome_Cliente);
    strcpy(temp.tipo_De_Pagamento, conta.tipo_De_Pagamento);
    temp.valor_Pago_Cat = conta.valor_Pago_Cat;
    temp.valor_Pago_Dinhe = conta.valor_Pago_Dinhe;

    fseek(ptFpg, 0, SEEK_END);  // Posiciona no fim
    fwrite(&temp, sizeof(Pagamento), 1, ptFpg);
    fflush(ptFpg);

    fclose(ptFpg);
}

void registar_Usuario_Atual()
{
    int valida=0;
    Usuario user;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - Login -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - Informe seu login de usuario: ", "");
    limpa_buff();
    fgets(user_Atual.login, sizeof(user_Atual.login), stdin);
    tira_espaco(user_Atual.login);

    FILE *ptFu = fopen(USUARIO, "rb");
    valida_Abertura_Arq(ptFu);
    rewind(ptFu);
    while(fread(&user, sizeof(Usuario), 1, ptFu) == 1)
    {
        if(strcmp(user.login, user_Atual.login) == 0)
        {
            valida = 1;
            printf("\n%50s - Informe sua senha:", "");
            limpa_buff();
            fgets(user_Atual.senha, sizeof(user_Atual.senha), stdin);
            tira_espaco(user_Atual.senha);
            if(strcmp(user.senha, user_Atual.senha) == 0)
            {
                user_Atual.tipo_Usuario = user.tipo_Usuario;
                user_Atual.id = user.id;
            }else
            {
                printf("\n%50s Senha incorreta!!!","");
                delay(3000);
            }

        }
    }

    if(valida == 0)
    {
        printf("\n%50s Usuario nao encontrado!!!","");
        delay(3000);
        strcpy(user_Atual.login, "0");
    }

    fclose(ptFu);
}

void abretura_Caixa()
{
    int op;

    if(user_Atual.tipo_Usuario == 1)
    {
        do
        {
            limpar_Terminal();
            printf("\n%45s ==================================", "");
            printf("\n%53s - ABRETURA DE CAIXA -", "");
            printf("\n%45s ==================================", "");
            printf("\n%50s [1] Abretura de caixa", "");
            printf("\n%50s [2] Voltar ao menu anterior", "");
            printf("\n%50s Opcao:", "");
            scanf("%d", &op);
            printf("\n%45s ==================================", "");

            switch(op)
            {
                case 1:
                    printf("\n%50s - Informe o valor de abretura de caixa: ", "");
                    do
                    {
                        scanf("%f", &valor_No_Caixa);
                    } while(valor_No_Caixa < 0);
                    valor_Na_Aber = valor_No_Caixa;
                    aber_Fecha = 1;
                    abre_Menu = 1;
                break;

                case 2:
                    printf("\n%50s Voltando....", "");
                    delay(3000);
                break;

                default:
                    printf("\n%50s Opcao invalida....", "");
                    delay(3000);
                break;
            }
        } while (op != 2);
    }else
    {
        printf("\n%50s Usuario logado não é o ADM...", "");
    }

}

void fechamento_Caixa(Carrinho **lista)
{
    int op, quantidade_Vendas=0;
    float total_Faturado=0;
    char x;
    Carrinho *aux;

    if(aber_Fecha == 1)
    {
        if(user_Atual.tipo_Usuario == 1)
        {
            do
            {
                limpar_Terminal();
                printf("\n%45s ==================================", "");
                printf("\n%53s - FECHAMENTO DE CAIXA -", "");
                printf("\n%45s ==================================", "");
                printf("\n%50s [1] Fechamento de caixa", "");
                printf("\n%50s [2] Voltar ao menu anterior", "");
                printf("\n%50s Opcao:", "");
                scanf("%d", &op);
                printf("\n%45s ==================================", "");

                switch(op)
                {
                    case 1:
                            for(aux = *lista; aux != NULL; aux =aux->prox)
                            {
                                quantidade_Vendas++;
                                total_Faturado += aux->total_Venda;
                            }

                            printf("\n%58s - RESUMO -", "");
                            printf("\n%53s - Quantidade de vendas realizadas no dia: %d", "", quantidade_Vendas);
                            printf("\n%53s - Total bruto faturado: R$%.2f", "", total_Faturado);
                            printf("\n%53s - Valor de abretura de caixa: R$%.2f", "", valor_Na_Aber);
                            printf("\n%53s - Valor total pago em dinheiro: R$%.2f", "", total_D);
                            printf("\n%53s - Valor total pago em cartao: R$%.2f", "", total_C);
                            printf("\n%45s ==================================", "");
                            limpa_buff();
                            printf("\n%50s Precione qualquer tecla para sair: ", "");
                            scanf("%c", &x);
                            aber_Fecha = 0;
                            liberarMemoriaCarrinho(lista);

                    break;

                    case 2:
                        printf("\n%50s Voltando....", "");
                    break;

                    default:
                        printf("\n%50s Opcao invalida....", "");
                    break;
                }
            } while (op != 2);
        }else
        {
            printf("\n%50s Usuario logado não é o ADM...", "");
        }
    }else
    {
        printf("\n%50s O caixa ainda não foi abreto!!!", "");
    }


}

void relatorio()
{
    int op;

    do
    {
        limpar_Terminal();
        printf("\n%45s ==================================", "");
        printf("\n%53s - RELATORIO -", "");
        printf("\n%45s ==================================", "");
        printf("\n%50s [01] Listagem dos clientes", "");
        printf("\n%50s [02] Listagem dos clientes por ordem alfabetica", "");
        printf("\n%50s [03] Listagem dos clientes que compraram", "");
        printf("\n%50s [04] Listagem dos produtos", "");
        printf("\n%50s [05] Listagem dos produtos por ordem alfabetica", "");
        printf("\n%50s [06] Listagem dos produtos com estoque zerado ou minimo", "");
        printf("\n%50s [07] Listagem dos produtos mais vendidos", "");
        printf("\n%50s [08] Listagem das vendas", "");
        printf("\n%50s [09] Listagem das vendas de um periodo", "");
        printf("\n%50s [10] Faturamento consolidado", "");
        printf("\n%50s [11] Voltar ao menu principal", "");
        printf("\n%50s Opcao:", "");
        scanf("%d", &op);
        printf("\n%45s ==================================", "");

        switch(op)
        {
            case 1:
                listar_Clientes();
            break;

            case 2:
                listar_Clientes_Ordenadamente();
            break;

            case 3:
                lista_Clientes_Compraram();
            break;

            case 4:
                lista_Produtos();
            break;

            case 5:
                lista_Produtos_Orden();
            break;

            case 6:
                lista_Produtos_Zerados();
            break;

            case 7:
                lista_Produtos_Mais_Vendidos();
            break;

            case 8:
                lista_Vendas();
            break;

            case 9:
                lista_Vendas_Periodo();
            break;

            case 10:
                faturamento_Consolidado();
            break;

            case 11:
                printf("\n%50s Voltando....","");
            break;

            default:
            break;
        }
    } while (op != 11);

}

void listar_Clientes()
{
    char x;
    Cliente clientes;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - LISTA DOS CLIENTES -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - %6s %16s %16s %12s %12s", "", "ID", "NOME", "NOME SOCIAL", "CPF", "TELEFONE");

    FILE *ptFc = fopen(CLIENTE, "rb");
    valida_Abertura_Arq(ptFc);
    rewind(ptFc);
    while(fread(&clientes, sizeof(clientes), 1, ptFc) == 1)
    {
        printf("\n%50s - %6d %16s %16s %12d %12d", "", clientes.id,
                                                        clientes.nome_completo,
                                                        clientes.nome_social,
                                                        clientes.cpf,
                                                        clientes.telefone);
    }
    printf("\n%45s ==================================", "");
    printf("\n%45s Precione qualquer tecla para voltar: ", "");
    limpa_buff();
    scanf("%c", &x);
    fclose(ptFc);
}

void listar_Clientes_Ordenadamente()
{
    char x;
    Cliente clientes;
    cliOrdenado *aux, *lista = NULL;
    FILE *ptFc = fopen(CLIENTE, "rb");
    valida_Abertura_Arq(ptFc);

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - LISTA DOS CLIENTES POR ORDEM ALFABETICA -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - %6s %16s %16s %12s %12s", "", "ID", "NOME", "NOME SOCIAL", "CPF", "TELEFONE");

    rewind(ptFc);
    while(fread(&clientes, sizeof(clientes), 1, ptFc) == 1)
    {
        ordena(&lista, clientes);
    }

    for(aux = lista; aux != NULL; aux = aux->prox)
    {
        printf("\n%50s - %6d %16s %16s %12d %12d", "", aux->id,
                                                        aux->nome_completo,
                                                        aux->nome_social,
                                                        aux->cpf,
                                                        aux->telefone);
    }
    printf("\n%45s ==================================", "");
    printf("\n%45s Precione qualquer tecla para voltar: ", "");
    limpa_buff();
    scanf("%c", &x);
    fclose(ptFc);
}

void ordena(cliOrdenado **lista, Cliente cliente)
{
    cliOrdenado *novo = (cliOrdenado*)malloc(sizeof(cliOrdenado));

    strcpy(novo->nome_completo, cliente.nome_completo);
    strcpy(novo->nome_social, cliente.nome_social);
    novo->cpf  = cliente.cpf;
    novo->id = cliente.id;
    novo->telefone = cliente.telefone;
    novo->prox = NULL;

    // Caso a lista esteja vazia ou o novo nome seja menor que o primeiro
    if (*lista == NULL || strcmp(novo->nome_completo, (*lista)->nome_completo) < 0) {
        novo->prox = *lista;
        *lista = novo;
    } else {
        cliOrdenado *atual = *lista;
        // Percorre até achar a posição correta
        while (atual->prox != NULL && strcmp(novo->nome_completo, atual->prox->nome_completo) > 0) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }

}

void lista_Clientes_Compraram()
{
    int data;
    char x;
    Pagamento clientes;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - LISTA DOS CLIENTES QUE COMPRARAM -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s informe a partir de qual dia desse mes: ", "");
    scanf("%d", &data);
    printf("\n%45s ==================================", "");
    printf("\n%50s - %6s %6s %16s %4s %6s", "", "DIA", "ID", "NOME", "TIPO PAG.", "TOTAL");

    FILE *ptFpg = fopen(PAGAMENTO, "rb");
    valida_Abertura_Arq(ptFpg);
    rewind(ptFpg);
    while(fread(&clientes, sizeof(Pagamento), 1, ptFpg) == 1)
    {
        if(clientes.dia_Venda >= data)
        {
            printf("\n%50s - %6d %6d %16s %4s %6.2f", "", clientes.dia_Venda,
                                                        clientes.num_Venda,
                                                        clientes.nome_Cliente,
                                                        clientes.tipo_De_Pagamento,
                                                        clientes.total_Pago);
        }

    }
    printf("\n%45s ==================================", "");
    printf("\n%50s Precione qualquer tecla para voltar: ", "");
    limpa_buff();
    scanf("%c", &x);
    fclose(ptFpg);
}

void lista_Produtos()
{
    char x;
    Produto produto;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - LISTA DOS PRODUTOS -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - %6s %16s %6s %6s %6s", "", "ID", "NOME", "ESTOQUE", "V.COMPRA", "V.VENDA");

    FILE *ptFp = fopen(PRODUTO, "rb");
    valida_Abertura_Arq(ptFp);
    rewind(ptFp);
    while(fread(&produto, sizeof(Produto), 1, ptFp) == 1)
    {
        printf("\n%50s - %6d %16s %6d %6.2f %6.2f", "", produto.id,
                                                        produto.desclicao,
                                                        produto.quant_Estoque,
                                                        produto.preco_Compra,
                                                        produto.preco_Venda);
    }
    printf("\n%45s ==================================", "");
    printf("\n%45s Precione qualquer tecla para voltar: ", "");
    limpa_buff();
    scanf("%c", &x);
    fclose(ptFp);
}

void lista_Produtos_Orden()
{
    char x;
    Produto produto;
    OrdenaProd *aux, *lista = NULL;
    FILE *ptFp = fopen(PRODUTO, "rb");
    valida_Abertura_Arq(ptFp);

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - LISTA DOS CLIENTES POR ORDEM ALFABETICA -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - %6s %16s %16s %12s %12s", "", "ID", "NOME", "NOME SOCIAL", "CPF", "TELEFONE");

    rewind(ptFp);
    while(fread(&produto, sizeof(Produto), 1, ptFp) == 1)
    {
        ordena_Produtos(&lista, produto);
    }

    for(aux = lista; aux != NULL; aux = aux->prox)
    {
        printf("\n%50s - %6d %16s %6d %6.2f %6.2f", "", aux->id,
                                                        aux->descricao,
                                                        aux->estoque,
                                                        aux->compra,
                                                        aux->venda);
    }
    printf("\n%45s ==================================", "");
    printf("\n%45s Precione qualquer tecla para voltar: ", "");
    limpa_buff();
    scanf("%c", &x);
    fclose(ptFp);
}

void ordena_Produtos(OrdenaProd **lista, Produto produto)
{
    OrdenaProd *novo = (OrdenaProd*)malloc(sizeof(OrdenaProd));

    strcpy(novo->descricao, produto.desclicao);
    novo->id  = produto.id;
    novo->estoque = produto.quant_Estoque;
    novo->compra = produto.preco_Compra;
    novo->venda = produto.preco_Venda;
    novo->prox = NULL;

    // Caso a lista esteja vazia ou o novo nome seja menor que o primeiro
    if (*lista == NULL || strcmp(novo->descricao, (*lista)->descricao) < 0) {
        novo->prox = *lista;
        *lista = novo;
    } else {
        OrdenaProd *atual = *lista;
        // Percorre até achar a posição correta
        while (atual->prox != NULL && strcmp(novo->descricao, atual->prox->descricao) > 0) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
}

void lista_Produtos_Zerados()
{
    char x;
    Produto produto;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - LISTA DOS PRODUTOS ZERADOS -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - %6s %16s %6s %6s %6s", "", "ID", "NOME", "ESTOQUE", "V.COMPRA", "V.VENDA");

    FILE *ptFp = fopen(PRODUTO, "rb");
    valida_Abertura_Arq(ptFp);
    rewind(ptFp);
    while(fread(&produto, sizeof(Produto), 1, ptFp) == 1)
    {
        if(produto.quant_Estoque == 0 || produto.quant_Estoque <= produto.estoque_Minimo)
            printf("\n%50s - %6d %16s %6d %6.2f %6.2f", "", produto.id,
                                                        produto.desclicao,
                                                        produto.quant_Estoque,
                                                        produto.preco_Compra,
                                                        produto.preco_Venda);
    }
    printf("\n%45s ==================================", "");
    printf("\n%45s Precione qualquer tecla para voltar: ", "");
    limpa_buff();
    scanf("%c", &x);
    fclose(ptFp);
}

void lista_Produtos_Mais_Vendidos()
{
    char x;
    Produto produto;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - LISTA DOS PRODUTOS MAIS VENDIDOS -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - %6s %16s %6s %6s", "", "ID", "NOME", "V.COMPRA", "V.VENDA");

    FILE *ptFp = fopen(PRODUTO, "rb");
    valida_Abertura_Arq(ptFp);
    rewind(ptFp);
    while(fread(&produto, sizeof(Produto), 1, ptFp) == 1)
    {
        if(produto.quant_Estoque == 0 || produto.quant_Estoque <= produto.estoque_Minimo)
            printf("\n%50s - %6d %16s %6.2f %6.2f", "", produto.id,
                                                        produto.desclicao,
                                                        produto.preco_Compra,
                                                        produto.preco_Venda);
    }
    printf("\n%45s ==================================", "");
    printf("\n%45s Precione qualquer tecla para voltar: ", "");
    limpa_buff();
    scanf("%c", &x);
    fclose(ptFp);
}

void lista_Vendas()
{
    char x;
    Pagamento conta;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - LISTA DAS VENDAS -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s - %6s %16s %6s %6s %6s", "", "ID", "NOME", "NUM.VENDA", "PAGADO", "TOTAL");

    FILE *ptFpg = fopen(PAGAMENTO, "rb");
    valida_Abertura_Arq(ptFpg);
    rewind(ptFpg);
    while(fread(&conta, sizeof(Pagamento), 1, ptFpg) == 1)
    {
        printf("\n%50s - %6d %16s %6d %s %6.2f", "", conta.num_Venda,
                                                        conta.nome_Cliente,
                                                        conta.num_Venda,
                                                        conta.tipo_De_Pagamento,
                                                        conta.total_Pago);
    }
    printf("\n%45s ==================================", "");
    printf("\n%45s Precione qualquer tecla para voltar: ", "");
    limpa_buff();
    scanf("%c", &x);
    fclose(ptFpg);
}

void lista_Vendas_Periodo()
{
    int data;
    char x;
    Pagamento conta;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - LISTA DAS VENDAS NUM PERIODO -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s informe a partir de qual dia desse mes: ", "");
    scanf("%d", &data);
    printf("\n%45s ==================================", "");
    printf("\n%50s - %6s %16s %6s %6s %6s", "", "ID", "NOME", "NUM.VENDA", "PAGADO", "TOTAL");

    FILE *ptFpg = fopen(PAGAMENTO, "rb");
    valida_Abertura_Arq(ptFpg);
    rewind(ptFpg);
    while(fread(&conta, sizeof(Pagamento), 1, ptFpg) == 1)
    {
        if(conta.dia_Venda >= data)
            printf("\n%50s - %6d %16s %6d %s %6.2f", "", conta.num_Venda,
                                                        conta.nome_Cliente,
                                                        conta.num_Venda,
                                                        conta.tipo_De_Pagamento,
                                                        conta.total_Pago);
    }
    printf("\n%45s ==================================", "");
    printf("\n%45s Precione qualquer tecla para voltar: ", "");
    limpa_buff();
    scanf("%c", &x);
    fclose(ptFpg);
}

void faturamento_Consolidado()
{
    int op;
    float fatu_Dinheiro=0, fatu_Cartao=0;
    Pagamento conta;

    FILE *ptFpg =fopen(PAGAMENTO, "rb");
    valida_Abertura_Arq(ptFpg);
    rewind(ptFpg);
    while(fread(&conta, sizeof(Pagamento), 1, ptFpg)  == 1)
    {
        fatu_Cartao += conta.valor_Pago_Cat;
        fatu_Dinheiro += conta.valor_Pago_Dinhe;
    }

    do
    {
        limpar_Terminal();
        printf("\n%45s ==================================", "");
        printf("\n%53s - FATURAMENTO -", "");
        printf("\n%45s ==================================", "");
        printf("\n%50s [01] Consolidados em dinheiro", "");
        printf("\n%50s [02] Consolidados em cartao", "");
        printf("\n%50s [03] Voltar ao menu anterio", "");
        printf("\n%45s ==================================", "");
        printf("\n%50s Opcao: ", "");
        scanf("%d", &op);

        switch(op)
        {
            case 1:
                printf("\n%50s - Faturamento em dinheiro foi de R$%.2f", "", fatu_Dinheiro);
            break;

            case 2:
                printf("\n%50s - Faturamento em cartao foi de R$%.2f", "", fatu_Cartao);
            break;

            case 3:
                printf("\n%50s Voltando....", "");
            break;

            default:
            break;
        }
    } while (op != 3);


}

void liberarMemoriaCarrinho(Carrinho **inicio) {
    Carrinho *atualCarrinho, *proxCarrinho;
    Itens *atualItem, *proxItem;

    atualCarrinho = *inicio;

    while (atualCarrinho != NULL) {
        proxCarrinho = atualCarrinho->prox;

        // Libera os itens do carrinho
        atualItem = atualCarrinho->iten;
        while (atualItem != NULL) {
            proxItem = atualItem->prox;
            free(atualItem);
            atualItem = proxItem;
        }

        // Libera o carrinho
        free(atualCarrinho);
        atualCarrinho = proxCarrinho;
    }

    *inicio = NULL; // Garante que o ponteiro original aponte para NULL
}



