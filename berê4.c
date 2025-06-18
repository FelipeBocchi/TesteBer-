//              CADASTROS
//  ----- INCLUDES/DEFINE ------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <time.h>

#define USUARIO "usuarios.bin"
#define CLIENTE "clientes.bin"
#define PRODUTO "produtos.bin"
#define CATEGORIA "categorias.bin"
#define VENDAS "vendas.bin"
#define Max_cpf 99999999999
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
    struct Carrinho *prox;
}Carrinho;

typedef Itens *No;

//  ---- VARIÁVEIS GLOBAIS -------

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
void adicionar_Item(Itens **, Itens *);
void adicionar_Carrinho(Carrinho **, Carrinho *);
void resulmo_Da_Venda(Itens *, Carrinho *);
void excluir_Prod_Carr(char, Itens **);
void sanguia(Carrinho *);
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

    do
    {
        switch(opcao = menu_Principal())
        {
            case 1:
                menu_Cadastros(ptFu, ptFc, ptFp, ptFct);
            break;

            case 2:
                menu_pagamento(&lista);
            break;

            case 3:
            break;

            case 4:
            break;

            case 5:
            break;

            case 6:
            break;

            default:
            break;
        }
    } while(opcao != 6);

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

    printf("\n%45s ==================================", "");
    printf("\n%50s - MENU PRINCIPAL -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s [1] Cadatro", "");
    printf("\n%50s [2] Vendas", "");
    printf("\n%50s [3] Abreturaa de  caixa", "");
    printf("\n%50s [4] Fechamento de caixa", "");
    printf("\n%50s [5] Relatorio", "");
    printf("\n%50s [6] Sair", "");
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
    int login_len;
    int senha_len;
    Usuario user;

    printf("\n%45s ==================================", "");
    printf("\n%53s - CADASTRAR USUARIO -", "");
    printf("\n%45s ==================================\n", "");
    printf("\n%50s - INFORMACOES -", "");

    do {
        printf("\n%50s Login (8-12): ", "");
        limpa_buff();
        fgets(user.login, sizeof(user.login), stdin);
        tira_espaco(user.login);
        login_len = strlen(user.login);
    } while(login_len < 8 || login_len > 12);

    do {
        printf("\n%50s Senha (6-8): ", "");
        limpa_buff();
        fgets(user.senha, sizeof(user.senha), stdin);
        tira_espaco(user.senha);
        senha_len = strlen(user.senha);
    } while(senha_len < 6 || senha_len > 8);

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

    do
    {
        printf("\n%50s Cpf: ", "");
        scanf("%d", &person.cpf);
    } while(person.cpf <= 0 || person.cpf > Max_cpf);


    printf("\n%50s Rua: ", "");
    limpa_buff();
    fgets(person.rua, sizeof(person.rua), stdin);
    tira_espaco(person.rua);

    do
    {
        printf("\n%50s Numero da casa: ", "");
        scanf("%d", &person.num);
    } while (person.num < 0);

    printf("\n%50s Bairro: ", "");
    limpa_buff();
    fgets(person.bairro, sizeof(person.bairro), stdin);
    tira_espaco(person.bairro);

    do
    {
       printf("\n%50s Telefone: ", "");
       scanf("%d", &person.telefone);
    } while (person.telefone < 0 && person.telefone > 9);

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
    printf("\n%50s - %4s%16s%s6%6s -\n", "", "ID", "DESCRICAO", "ESTOQUE", "VALOR");

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

    //adiciona carrinho a lista
    adicionar_Carrinho(lista, carrinho);
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
    printf("\n%45s ==================================", "");
    printf("\n%53s - CARRINHO DO %s -", "", "");//nome do cliente
    printf("\n%45s ==================================", "");
    printf("\n%50s- %4s %16s %6s %6s -\n","", "ID", "DESCRICAO", "QTD", "VALOR");

    Itens *aux, *aux_S;
    //for (Carrinho *c = lista; c != NULL; c = c->prox) {
       // for (aux = c->iten; aux != NULL; aux = aux->prox) {
           // printf("\n%50s- %4d %16s %6d %6.2f -\n","", aux->id, aux->desclicao, aux->quantidade_levada, aux->preco_Venda);
        //}
    //}

    for(aux = lista; aux != NULL; aux = aux->prox)
        printf("\n%50s- %4d %16s %6d %6.2f -\n","", aux->id, aux->desclicao, aux->quantidade_levada, aux->preco_Venda);
    printf("\n%45s ==================================", "");

    for(aux_S = lista; aux_S != NULL; aux_S = aux_S->prox)
        total_A_Pagar += aux_S->valor_total;
    printf("\n%50s \t\tTOTAL A PAGAR: %.2f", "", total_A_Pagar);

    printf("\n%50s Há desconto nessa venda (0-Não há / informe a porcentagem): ", "");
    scanf("%f", &desconto);
    if(desconto > 0 && desconto < 100)
    {
        desconto = desconto/100;
        total_A_Pagar = total_A_Pagar * (1-desconto);
    }

    printf("\n%50s \t\tTOTAL FINAL: %.2f", "", total_A_Pagar);

    carrinho->total_Venda = total_A_Pagar;
}

void sanguia(Carrinho *lista)
{
    int usuario_L, usuario_S, valida=0;
    float valor_No_Caixa=0, quantia_Retirada;
    Itens *aux, *aux_S;
    Usuario user;

    printf("%50s Informe seu login de usuario", "");
    scanf("%d", &usuario_L);

    FILE *ptFu = fopen(USUARIO, "rb");
    valida_Abertura_Arq(ptFu);
    rewind(ptFu);
    while(fread(&user, sizeof(Usuario), 1, ptFu) == 1)
    {
        if(user.login == usuario_L)
        {
            valida = 1;
            if(user.login == 1)
            {
                printf("%50s Informe sua senha:", "");
                scanf("%d", &usuario_S);
                if(user.senha == usuario_S)
                {
                    for (Carrinho *c = lista; c != NULL; c = c->prox) {
                        for (aux = c->iten; aux != NULL; aux = aux->prox) {
                            valor_No_Caixa += aux->valor_total;
                        }
                    }

                    printf("%50s O caixa tem R$ %.2f em notas", "", valor_No_Caixa);
                    printf("%50s deseja retirar qual quantia (valor minimo de caixa R$50,00)", "");
                    printf("%50s Valor:)", "");
                    scanf("%f", &quantia_Retirada);
                    valor_No_Caixa-=quantia_Retirada;
                    if(valor_No_Caixa <  50)
                        printf("%50s Valar retirado deixou o caixa com menos de R$50, devolva!", "");
                }
            }

        }
    }
    if(valida == 0)
        printf("\n%50s Id invalida ou nao existe!!!", "");
    fclose(ptFu);
}

void pagamento(Carrinho **lista)
{
    int op, id_Carr;
    Cliente cliente;
    Carrinho *c;
    Itens *aux;

    limpar_Terminal();
    printf("\n%45s ==================================", "");
    printf("\n%53s - CARRINHOS EM ABERTO -", "");
    printf("\n%45s ==================================", "");
    printf("\n%50s- %4s %16s %2s %6s -\n","", "ID", "NOME", "SITUACAO", "TOTAL");
    for(c = *lista; c != NULL; c =  c->prox)
    {
        FILE *ptFc = fopen(CLIENTE, "rb");
        valida_Abertura_Arq(ptFc);
        fseek(ptFc, -(c->id_Cliente)*sizeof(Cliente), SEEK_SET );
        fread(&cliente, sizeof(Cliente), 1, ptFc);

        printf("\n%50s- %4d %16s %2c %6.2f -", "", c->num_Da_Venda, cliente.nome_completo, c->pagamento, c->total_Venda);
    }
    printf("\n%45s ==================================", "");
    printf("\n%50s Informe o ID do carrinho a ser pago: ", "");
    scanf("%d", &id_Carr);
    //if() se a situacao do carrinho é aberto ou fechada

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
                    //catão
            break;

            case 2:
                    //dinheiro
            break;

            case 3:
                printf("\n%50s Voltando ao menu anterior....", "");
            break;

            default:
            break;
        }
    } while (op != 3);
}

//          NOTAS DO QUE FALTA
/*
    SANGUIA CRIADA, PORÉM FALTA CRIAR O CAIXA E VALIDAR SE O CARRINHO FOI PAGO EM DINHEIRO
    PAGAMENTO
    ajustar os conteirdo de valores  - ver com o professor
*/



