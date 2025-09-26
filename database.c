#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "database.h"
#include <sqlite3.h>

void inicializarBanco(sqlite3 **db) {
    char *err_msg = 0;
    int rc;

    rc = sqlite3_open("empresa.db", db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(*db));
        exit(1);
    }

    const char *sql_funcionarios = "CREATE TABLE IF NOT EXISTS Funcionarios("
                                   "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                   "Nome TEXT NOT NULL,"
                                   "Senha INTEGER NOT NULL,"
                                   "Idade INTEGER,"
                                   "Cargo TEXT,"
                                   "Email TEXT NOT NULL UNIQUE);";
    rc = sqlite3_exec(*db, sql_funcionarios, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar tabela Funcionarios: %s\n", err_msg);
        sqlite3_free(err_msg);
    }

    const char *sql_carros = "CREATE TABLE IF NOT EXISTS Carros("
                             "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                             "Modelo TEXT NOT NULL,"
                             "Ano INTEGER,"
                             "Cor TEXT,"
                             "Problema TEXT);";
    rc = sqlite3_exec(*db, sql_carros, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar tabela Carros: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
}


void salvarFuncionariosSQL(sqlite3 *db, funcionario *lista, int num_usuarios) {
    char *err_msg = 0;
    int rc;

    const char *sql_delete = "DELETE FROM Funcionarios;";
    rc = sqlite3_exec(db, sql_delete, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao limpar a tabela Funcionarios: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }
    const char *sql_insert = "INSERT INTO Funcionarios (Nome, Senha, Idade, Cargo, Email) VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a instrução: %s\n", sqlite3_errmsg(db));
        return;
    }

    for (int i = 0; i < num_usuarios; i++) {
        sqlite3_bind_text(stmt, 1, lista[i].nome, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, lista[i].senha);
        sqlite3_bind_int(stmt, 3, lista[i].idade);
        sqlite3_bind_text(stmt, 4, lista[i].cargo, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 5, lista[i].email, -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Erro ao inserir funcionário %d: %s\n", i, sqlite3_errmsg(db));
        }
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);
    printf("Dados de funcionários salvos no banco de dados com sucesso!\n");
}

int carregarFuncionariosSQL(sqlite3 *db, funcionario **lista) {
    sqlite3_stmt *stmt;
    const char *sql_select = "SELECT ID, Nome, Senha, Idade, Cargo, Email FROM Funcionarios;";
    int rc;
    int count = 0;
    funcionario *temp_lista = NULL;

    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Falha ao preparar a instrução de carregamento: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        count++;
        temp_lista = (funcionario *)realloc(temp_lista, count * sizeof(funcionario));
        if (temp_lista == NULL) {
            fprintf(stderr, "Erro de alocação de memória ao carregar funcionários.\n");
            sqlite3_finalize(stmt);
            return 0;
        }

        strcpy(temp_lista[count - 1].nome, (const char *)sqlite3_column_text(stmt, 1));
        temp_lista[count - 1].senha = sqlite3_column_int(stmt, 2);
        temp_lista[count - 1].idade = sqlite3_column_int(stmt, 3);
        strcpy(temp_lista[count - 1].cargo, (const char *)sqlite3_column_text(stmt, 4));
        strcpy(temp_lista[count - 1].email, (const char *)sqlite3_column_text(stmt, 5));
    }

    sqlite3_finalize(stmt);
    *lista = temp_lista;
    return count;
}

void salvarCarrosSQL(sqlite3 *db, carro *lista, int num_carros) {
    char *err_msg = 0;
    int rc;

    const char *sql_delete = "DELETE FROM Carros;";
    rc = sqlite3_exec(db, sql_delete, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao limpar a tabela Carros: %s\n", err_msg);
        sqlite3_free(err_msg);
        return;
    }

    const char *sql_insert = "INSERT INTO Carros (Modelo, Ano, Cor, Problema) VALUES (?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql_insert, -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a instrução: %s\n", sqlite3_errmsg(db));
        return;
    }

    for (int i = 0; i < num_carros; i++) {
        sqlite3_bind_text(stmt, 1, lista[i].Modelo, -1, SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, lista[i].ano);
        sqlite3_bind_text(stmt, 3, lista[i].cor, -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, 4, lista[i].problema, -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            fprintf(stderr, "Erro ao inserir carro %d: %s\n", i, sqlite3_errmsg(db));
        }
        sqlite3_reset(stmt);
    }
    sqlite3_finalize(stmt);
    printf("Dados de carros salvos no banco de dados com sucesso!\n");
}

int carregarCarrosSQL(sqlite3 *db, carro **lista) {
    sqlite3_stmt *stmt;
    const char *sql_select = "SELECT ID, Modelo, Ano, Cor, Problema FROM Carros;";
    int rc;
    int count = 0;
    carro *temp_lista = NULL;

    rc = sqlite3_prepare_v2(db, sql_select, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Falha ao preparar a instrução de carregamento: %s\n", sqlite3_errmsg(db));
        return 0;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        count++;
        temp_lista = (carro *)realloc(temp_lista, count * sizeof(carro));
        if (temp_lista == NULL) {
            fprintf(stderr, "Erro de alocação de memória ao carregar carros.\n");
            sqlite3_finalize(stmt);
            return 0;
        }

        strcpy(temp_lista[count - 1].Modelo, (const char *)sqlite3_column_text(stmt, 1));
        temp_lista[count - 1].ano = sqlite3_column_int(stmt, 2);
        strcpy(temp_lista[count - 1].cor, (const char *)sqlite3_column_text(stmt, 3));
        strcpy(temp_lista[count - 1].problema, (const char *)sqlite3_column_text(stmt, 4));
    }

    sqlite3_finalize(stmt);
    *lista = temp_lista;
    return count;
}