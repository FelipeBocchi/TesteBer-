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
