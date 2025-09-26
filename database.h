#ifndef DATABASE_H
#define DATABASE_H

#include <sqlite3.h>

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

void salvarFuncionariosSQL(sqlite3 *db, funcionario *lista, int num_usuarios);
int carregarFuncionariosSQL(sqlite3 *db, funcionario **lista);
void salvarCarrosSQL(sqlite3 *db, carro *lista, int num_carros);
int carregarCarrosSQL(sqlite3 *db, carro **lista);
void inicializarBanco(sqlite3 **db);

#endif