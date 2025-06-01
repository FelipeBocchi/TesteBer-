//              CADASTROS
//  ----- INCLUDES/DEFINE ------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define USUARIO "usuarios.bin"
#define CLIENTE "clientes.bin"
#define PRODUTO "produtos.bin"
#define CATEGORIA "categorias.bin"
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



//  ---- VARIÁVEIS GLOBAIS -------

//  ---- DECLARAÇÃO DE FUNÇÕES -----
void menu_Cadastros(FILE *, FILE *ptFc, FILE *ptFp, FILE *ptFct);
void cadatro_Usuario(FILE *);
void cadastro_Cliente(FILE *);
void cadastro_Produtos(FILE *, FILE *);
void cadastro_Categorias(FILE *);
void valida_Abertura_Arq(FILE *);
void limpa_buff(void);
void tira_espaco(char[]);
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

    do
    {
        switch(opcao = menu_Principal())
        {
            case 1:
                menu_Cadastros(ptFu, ptFc, ptFp, ptFct);
            break;

            case 2:
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




