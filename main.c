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

typedef struct Carros
{
    char Modelo[50];
    int ano;
    char cor[20];
    char problema[100];

} carro;

void pausarTerminal();
void limparBufferEntrada();
void exibirCarros(carro *cs2);
void registrarCarro(carro *cs);
void salvarCarros(carro *lista, int num_carros, const char *carros);
int carregarCarros(carro **lista, const char *carros);
void lerFormulario(funcionario *f);
void mostrarFuncionarios(funcionario *f2);
int validarLogin(funcionario *lista, int usuarios);
int carregarfuncionarios(funcionario **lista, const char *funcionarios);
void salvarFuncionarios(funcionario *lista, int num_usuarios, const char *funcionarios);

void registrarCarro(carro *cs)
{
    char temp_ano[10];

    printf("Informe o modelo de seu veiculo:\n>>> ");
    fgets(cs->Modelo, sizeof(cs->Modelo), stdin);
    cs->Modelo[strcspn(cs->Modelo, "\n")] = '\0';

    printf("Informe o ano de seu veiculo:\n>>> ");
    fgets(temp_ano, sizeof(temp_ano), stdin);
    temp_ano[strcspn(temp_ano, "\n")] = '\0';
    cs->ano = atoi(temp_ano);
    limparBufferEntrada();

    printf("Informe a cor do seu veículo:\n>>> ");
    fgets(cs->cor, sizeof(cs->cor), stdin);
    cs->cor[strcspn(cs->cor, "\n")] = '\0';

    printf("Informe o problema do seu veículo:\n>>> ");
    fgets(cs->problema, sizeof(cs->problema), stdin);
    cs->problema[strcspn(cs->problema, "\n")] = '\0';
}

void exibirCarros(carro *cs2)
{
    printf("\n=========== informacoes ===========\n");
    printf("Modelo:%s\n", cs2->Modelo);
    printf("Ano: %d\n", cs2->ano);
    printf("Cor: %s\n", cs2->cor);
    printf("Problema: %s\n", cs2->problema);
}

void salvarCarros(carro *lista, int num_carros, const char *carros)
{
    FILE *arquivo;
    int i;
    arquivo = fopen(carros, "w");

    if (arquivo == NULL)
    {
        perror("Erro ao abrir o arquivo");
        return;
    }

    fprintf(arquivo, "%d\n", num_carros);
    for (i = 0; i < num_carros; i++)
    {
        fprintf(arquivo, "%s,%d,%s,%s", lista[i].Modelo, lista[i].ano, lista[i].cor, lista[i].problema);
    }
    fclose(arquivo);
}

int carregarCarros(carro **lista, const char *carros)
{
    FILE *arquivo;
    int CarrosArquivados = 0;
    arquivo = fopen(carros, "r");

    if (arquivo == NULL)
    {
        return 0;
    }

    if (fscanf(arquivo, "%d\n", &CarrosArquivados) != 1)
    {
        fclose(arquivo);
        return 0;
    }

    if (CarrosArquivados <= 0)
    {
        fclose(arquivo);
        return 0;
    }

    *lista = (carro *)malloc(CarrosArquivados * sizeof(carro));
    if (*lista == NULL)
    {
        fclose(arquivo);
        return 0;
    }

    for (int i = 0; i < CarrosArquivados; i++)
    {
        if (fscanf(arquivo, "%[^,],%d,%[^,],%[^\n]\n", (*lista)[i].Modelo, &(*lista)[i].ano, (*lista)[i].cor, (*lista)[i].problema) != 4)
        {
            CarrosArquivados = i;
            break;
        }
    }
    fclose(arquivo);
    return CarrosArquivados;
}

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
    limparBufferEntrada();

    printf("Informe sua idade:\n>>> ");
    fgets(temp_idade, sizeof(temp_idade), stdin);
    temp_idade[strcspn(temp_idade, "\n")] = '\0';
    f->idade = atoi(temp_idade);
    limparBufferEntrada();

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
    printf("Informe o email de usuario:\n>>> ");
    fgets(login_email, sizeof(login_email), stdin);
    login_email[strcspn(login_email, "\n")] = '\0';

    printf("Informe o pin de usuario:\n>>> ");
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
    limparBufferEntrada();
}

int main(void)
{
    int funcionarios = 0;
    int carros = 0;
    int opcao_func; 
    int opcao_carro; 
    int access;
    char access_str[10];

    funcionario *lista_funcionarios = NULL; 
    carro *lista_carros = NULL; 

    funcionarios = carregarfuncionarios(&lista_funcionarios, "funcionarios.txt");
    if (funcionarios > 0)
    {
        printf("%d funcionarios carregados com sucesso do arquivo.\n", funcionarios);
    }
    else
    {
        printf("Nenhum funcionario carregado. Iniciando com lista vazia.\n");
    }

    carros = carregarCarros(&lista_carros, "carros.txt");
    if (carros > 0)
    {
        printf("%d carros carregados com sucesso do arquivo.\n", carros);
    }
    else
    {
        printf("Nenhum carro carregado. Iniciando com lista vazia.\n");
    }

    do
    {
        printf("\n--------- MENU DE ENTRADA ---------\n");
        printf(" [1] Funcionario.\n");
        printf(" [2] Cliente.\n");
        printf(" [3] Sair.\n");
        printf("Escolha uma opcao: ");

        fgets(access_str, sizeof(access_str), stdin);
        access_str[strcspn(access_str, "\n")] = '\0';
        access = atoi(access_str);
        switch (access)
        {
        case 1:
            do
            {
                printf("\n--------- MENU DE OPCOES ---------\n");
                printf(" [1] Cadastrar novos funcionarios\n");
                printf(" [2] Salvar funcionarios\n");
                printf(" [3] Carregar funcionarios\n");
                printf(" [4] Exibir funcionarios\n");
                printf(" [5] Login\n");
                printf(" [6] Sair\n");
                printf("Escolha uma opcao: ");

                char opcao_str[10];
                fgets(opcao_str, sizeof(opcao_str), stdin);
                opcao_str[strcspn(opcao_str, "\n")] = '\0';
                opcao_func = atoi(opcao_str);

                switch (opcao_func)
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

                    funcionario *temp_func = (funcionario *)realloc(lista_funcionarios, (funcionarios + novos_funcionarios) * sizeof(funcionario));

                    if (temp_func == NULL)
                    {
                        printf("Erro ao realocar memoria. Nao foi possivel adicionar novos funcionarios.\n");
                    }
                    else
                    {
                        lista_funcionarios = temp_func;
                        limparBufferEntrada();
                        for (int i = 0; i < novos_funcionarios; i++)
                        {
                            printf("\nCadastrando funcionario %d:\n", funcionarios + i + 1);
                            lerFormulario(&lista_funcionarios[funcionarios + i]);
                        }
                        funcionarios += novos_funcionarios;
                        printf("Funcionarios cadastrados com sucesso!\n");
                    }
                    break;
                case 2:
                    if (lista_funcionarios != NULL && funcionarios > 0)
                    {
                        salvarFuncionarios(lista_funcionarios, funcionarios, "funcionarios.txt");
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
                    if (lista_funcionarios != NULL)
                    {
                        free(lista_funcionarios);
                        lista_funcionarios = NULL;
                    }
                    funcionarios = carregarfuncionarios(&lista_funcionarios, "funcionarios.txt");
                    if (funcionarios > 0)
                    {
                        printf("%d funcionarios carregados com sucesso!\n", funcionarios);
                        pausarTerminal();
                    }
                    else
                    {
                        printf("Erro ao carregar os dados ou arquivo nao encontrado.\n");
                        pausarTerminal();
                    }
                    break;
                case 4:
                    if (lista_funcionarios != NULL && funcionarios > 0)
                    {
                        for (int i = 0; i < funcionarios; i++)
                        {
                            mostrarFuncionarios(&lista_funcionarios[i]);
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
                    if (lista_funcionarios != NULL && funcionarios > 0)
                    {
                        int login_status = validarLogin(lista_funcionarios, funcionarios);
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
                    printf("Saindo do menu de funcionarios...\n");
                    break;
                default:
                    printf("Opcao invalida. Tente novamente!\n");
                    break;
                }
            } while (opcao_func != 6);
            break;
        case 2:
            do
            {
                printf("\n--------- MENU DE OPCOES ---------\n");
                printf(" [1] Registrar veiculos\n");
                printf(" [2] Salvar veiculos\n");
                printf(" [3] Carregar veiculos\n");
                printf(" [4] Exibir veiculos\n");
                printf(" [5] Sair\n");
                printf("Escolha uma opcao: ");

                char opcao_str[10];
                fgets(opcao_str, sizeof(opcao_str), stdin);
                opcao_str[strcspn(opcao_str, "\n")] = '\0';
                opcao_carro = atoi(opcao_str);

                switch (opcao_carro)
                {
                case 1:
                    printf("Informe o numero de veiculos que deseja registrar:\n>>> ");
                    char temp_carros[10];
                    fgets(temp_carros, sizeof(temp_carros), stdin);
                    temp_carros[strcspn(temp_carros, "\n")] = '\0';
                    int novos_carros = atoi(temp_carros);

                    if (novos_carros <= 0)
                    {
                        printf("Nenhuma carro para ser registrado.\n");
                        break;
                    }

                    carro *temp_carro = (carro *)realloc(lista_carros, (carros + novos_carros) * sizeof(carro));

                    if (temp_carro == NULL)
                    {
                        printf("Erro ao realocar memoria. Nao foi possivel registrar novos carros.\n");
                    }
                    else
                    {
                        lista_carros = temp_carro;
                        limparBufferEntrada();
                        for (int i = 0; i < novos_carros; i++)
                        {
                            printf("\nCadastrando carro %d:\n", carros + i + 1);
                            registrarCarro(&lista_carros[carros + i]);
                        }
                        carros += novos_carros;
                        printf("Carros registrados com sucesso!\n");
                    }
                    break;
                case 2:
                    if (lista_carros != NULL && carros > 0)
                    {
                        salvarCarros(lista_carros, carros, "carros.txt");
                        printf("Dados salvos com sucesso!\n");
                        pausarTerminal();
                    }
                    else
                    {
                        printf("Nenhum carros para salvar.\n");
                        pausarTerminal();
                    }
                    break;
                case 3:
                    if (lista_carros != NULL)
                    {
                        free(lista_carros);
                        lista_carros = NULL;
                    }
                    carros = carregarCarros(&lista_carros, "carros.txt");
                    if (carros > 0)
                    {
                        printf("%d carros carregados com sucesso!\n", carros);
                        pausarTerminal();
                    }
                    else
                    {
                        printf("Erro ao carregar dados ou arquivo nao encontrado.\n");
                        pausarTerminal();
                    }
                    break;
                case 4:
                    if (lista_carros != NULL && carros > 0)
                    {
                        for (int i = 0; i < carros; i++)
                        {
                            exibirCarros(&lista_carros[i]);
                        }
                        pausarTerminal();
                    }
                    else
                    {
                        printf("Nenhum carro registrado para ser exibido.\n");
                        pausarTerminal();
                    }
                    break;
                case 5:
                    printf("Saindo do menu de clientes...\n");
                    break;
                default:
                    printf("Opcao invalida. Tente novamente!\n");
                    break;
                }
            } while (opcao_carro != 5);
            break;
        case 3:
            printf("Saindo do programa...\n");
            break;
        default:
            printf("Opcao invalida. Tente novamente!\n");
            break;
        }
    } while (access != 3);

    if (lista_funcionarios != NULL)
    {
        free(lista_funcionarios);
    }
    if (lista_carros != NULL)
    {
        free(lista_carros);
    }

    return 0;
}
