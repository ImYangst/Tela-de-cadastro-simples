#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Funcionarios
{
    char nome[50];
    int senha;
    int idade;
    char cargo[50];
    char email[50];

} funcionario;

void limparBufferEntrada();
void lerFormulario(funcionario *f);
void mostrarFuncionarios(funcionario *f2);
int validarLogin(funcionario *lista, int usuarios);
void salvarFuncionarios(funcionario *lista, int num_usuarios, const char *funcionarios);
int carregarfuncionarios(funcionario **lista, const char *funcionarios);
void pausarTerminal();

void limparBufferEntrada()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void lerFormulario(funcionario *f)
{
    char temp_idade[10];
    char temp_senha[5];

    printf("Informe seu nome:\n>>> ");
    fgets(f->nome, sizeof(f->nome), stdin);
    f->nome[strcspn(f->nome, "\n")] = '\0';

    printf("Informe sua pin: (maximo 4 digitos)\n>>> ");
    fgets(temp_senha, sizeof(temp_senha), stdin);
    temp_senha[strcspn(temp_senha, "\n")] = '\0';
    f->senha = atoi(temp_senha);

    printf("Informe sua idade:\n>>> ");
    fgets(temp_idade, sizeof(temp_idade), stdin);
    temp_idade[strcspn(temp_idade, "\n")] = '\0';
    f->idade = atoi(temp_idade);

    printf("Informe seu cargo:\n>>> ");
    fgets(f->cargo, sizeof(f->cargo), stdin);
    f->cargo[strcspn(f->cargo, "\n")] = '\0';

    printf("Informe seu email: (Maximo de 40 caracteres)\n>>> ");
    fgets(f->email, sizeof(f->email), stdin);
    f->email[strcspn(f->email, "\n")] = '\0';
}

void mostrarFuncionarios(funcionario *f2)
{
    printf("\n============= Dados =============\n");
    printf("Nome: %s\n", f2->nome);
    printf("Cargo: %s\n", f2->cargo);
    printf("Idade: %d\n", f2->idade);
    printf("Email: %s\n", f2->email);
    printf("Pin: %d\n", f2->senha);
}

int validarLogin(funcionario *lista, int usuarios)
{
    char login_email[50];
    char login_senha[10];
    int login_pin;
    int sucess = 0;

    limparBufferEntrada();

    printf("\n============= Login =============\n");
    printf("Informe o email de usuario:\n>>>");
    fgets(login_email, sizeof(login_email), stdin);
    login_email[strcspn(login_email, "\n")] = '\0';

    printf("Informe o pin de usuario:\n>>>");
    fgets(login_senha, sizeof(login_senha), stdin);
    login_senha[strcspn(login_senha, "\n")] = '\0';
    login_pin = atoi(login_senha);

    for (int i = 0; i < usuarios; i++)
    {
        if (strcmp(login_email, lista[i].email) == 0)
        {
            if (login_pin == lista[i].senha)
            {
                sucess = 1;
                break;
            }
        }
    }
    return sucess;
}

void salvarFuncionarios(funcionario *lista, int num_usuarios, const char *funcionarios)
{
    FILE *arquivo;
    int i;
    arquivo = fopen(funcionarios, "w");

    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo para salvar");
        return;
    }

    fprintf(arquivo, "%d\n", num_usuarios);
    for (i = 0; i < num_usuarios; i++)
    {
        fprintf(arquivo, "%s,%d,%d,%s,%s\n", lista[i].nome, lista[i].senha, lista[i].idade, lista[i].cargo, lista[i].email);
    }
    fclose(arquivo);
}

int carregarfuncionarios(funcionario **lista, const char *funcionarios)
{
    FILE *arquivo;
    int FuncionariosArquivados = 0;

    arquivo = fopen(funcionarios, "r");
    if (arquivo == NULL)
    {
        return 0;
    }

    if (fscanf(arquivo, "%d\n", &FuncionariosArquivados) != 1)
    {
        fclose(arquivo);
        return 0;
    }

    if (FuncionariosArquivados <= 0)
    {
        fclose(arquivo);
        return 0;
    }

    *lista = (funcionario *)malloc(FuncionariosArquivados * sizeof(funcionario));
    if (*lista == NULL)
    {
        fclose(arquivo);
        return 0;
    }

    for (int i = 0; i < FuncionariosArquivados; i++)
    {
        if (fscanf(arquivo, "%[^,],%d,%d,%[^,],%[^\n]\n", (*lista)[i].nome, &(*lista)[i].senha, &(*lista)[i].idade, (*lista)[i].cargo, (*lista)[i].email) != 5)
        {
            FuncionariosArquivados = i;
            break;
        }
    }
    fclose(arquivo);
    return FuncionariosArquivados;
}

void pausarTerminal()
{
    printf("\nPressione ENTER para continuar...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    getchar();
}

int main(void)
{
    int funcionarios = 0;
    int opcao;
    char opcao_str[10];
    funcionario *lista = NULL;

    funcionarios = carregarfuncionarios(&lista, "funcionarios.txt");
    if (funcionarios > 0)
    {
        printf("%d funcionarios carregados com sucesso do arquivo.\n", funcionarios);
    }
    else
    {
        printf("Nenhum funcionario carregado. Iniciando com lista vazia. \n");
    }

    do
    {
        printf("\n--- MENU DE OPÇÕES ---\n");
        printf("1. Cadastrar novos funcionarios\n");
        printf("2. Salvar funcionarios\n");
        printf("3. Carregar funcionarios\n");
        printf("4. Exibir funcionarios\n");
        printf("5. Login\n");
        printf("6. Sair\n");
        printf("Escolha uma opção: ");

        fgets(opcao_str, sizeof(opcao_str), stdin);
        opcao_str[strcspn(opcao_str, "\n")] = '\0';
        opcao = atoi(opcao_str);

        switch (opcao)
        {
        case 1:
            printf("Informe a quantidade de funcionarios a serem cadastrados:\n>>> ");
            char temp_funcionarios[10];
            fgets(temp_funcionarios, sizeof(temp_funcionarios), stdin);
            temp_funcionarios[strcspn(temp_funcionarios, "\n")] = '\0';
            int novos_funcionarios = atoi(temp_funcionarios);

            if (novos_funcionarios <= 0)
            {
                printf("Nenhum funcionario para adicionar.\n");
                break;
            }

            funcionario *temp = (funcionario *)realloc(lista, (funcionarios + novos_funcionarios) * sizeof(funcionario));

            if (temp == NULL)
            {
                printf("Erro ao realocar memoria. Nao foi possivel adicionar novos funcionarios.\n");
            }
            else
            {
                lista = temp;
                limparBufferEntrada();
                for (int i = 0; i < novos_funcionarios; i++)
                {
                    printf("\nCadastrando funcionario %d:\n", funcionarios + i + 1);
                    lerFormulario(&lista[funcionarios + i]);
                }
                funcionarios += novos_funcionarios;
                printf("Funcionarios cadastrados com sucesso!\n");
            }
            break;
        case 2:
            if (lista != NULL && funcionarios > 0)
            {
                salvarFuncionarios(lista, funcionarios, "funcionarios.txt");
                printf("Dados salvos com sucesso!\n");
                pausarTerminal();
            }
            else
            {
                printf("Nenhum funcionario para salvar.\n");
                pausarTerminal();
            }
            break;
        case 3:
            if (lista != NULL)
            {
                free(lista);
                lista = NULL;
            }
            funcionarios = carregarfuncionarios(&lista, "funcionarios.txt");
            if (funcionarios > 0)
            {
                printf("%d funcionarios carregados com sucesso!\n", funcionarios);
                pausarTerminal();
            }
            else
            {
                printf("Erro ao carregar os dados ou arquivo nao encontrada.\n");
                pausarTerminal();
            }
            break;
        case 4:
            if (lista != NULL && funcionarios > 0)
            {
                for (int i = 0; i < funcionarios; i++)
                {
                    mostrarFuncionarios(&lista[i]);
                }
                pausarTerminal();
            }
            else
            {
                printf("Nenhum usuario para exibir.\n");
                pausarTerminal();
            }
            break;
        case 5:
            if (lista != NULL && funcionarios > 0)
            {
                int login_status = validarLogin(lista, funcionarios);
                if (login_status == 1)
                {
                    printf("Login bem sucedido!\n");
                    pausarTerminal();
                }
                else
                {
                    printf("Login falhou. Email ou senha incorretos.\n");
                    pausarTerminal();
                }
            }
            else
            {
                printf("Nao ha funcionarios cadastrados para realizar o login.\n");
                pausarTerminal();
            }
            break;
        case 6:
            if (lista != NULL)
            {
                free(lista);
            }
            printf("Saindo do programa...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente!\n");
            break;
        }
    } while (opcao != 6);
    return 0;
}