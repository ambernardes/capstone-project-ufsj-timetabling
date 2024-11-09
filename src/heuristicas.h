#ifndef HEURISTICAS
#define HEURISTICAS
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "util.h"
#include <vector>
#include "Courses.h"
#include "Constraints.h"
#include "Curricula.h"
#include "Rooms.h"
#include <map>


using namespace std;

/* Heuristicas construtivas. */

void constroi_solucao_aleatoria(tabela *table, vector<int> *aulas_numeradas, int dias, int ppd, int sal, int **AA, vector<Constraints> *cons, vector<Courses> *c, int **AI, int **DCxD, int **DCxS, int ***CxDxP, int **CM);

void constroi_solucao_gulosa();

void constroi_solucao_parcialmente_aleatoria(float alfa, tabela *table, vector<int> *aulas_numeradas, int dias, int ppd, int sal, int **AA, vector<Constraints> *cons, vector<Courses> *c, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<Curricula> *cu, int **AH, vector<Rooms> *r);

void constroi_solucao_inicial(float alfa, tabela *table, vector<int> *aulas_numeradas, int dias, int ppd, int sal, int **AA, vector<Constraints> *cons, vector<Courses> *c, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<Curricula> *cu, int **AH, vector<Rooms> *r, int **CM);

int explosao(int aula_a_ser_alocada, tabela *table, vector<int> *aulas_numeradas , vector<int> *aulas_ja_alocadas, vector<int> *aulas_nao_alocadas, int *total_horarios, int **DCxD, int **DCxS, int ***CxDxP, int **AH, int dias, int ppd, int sal, vector<Courses> *c, vector<Curricula> *cu, int **AA, int **AI);

void GRASP();

void monta_LRC(vector<aulaSalaCusto> *LRC, int cmin, int cmax, float alfa, aulaSalaCusto *custos, int tamanho);


/* -------- Fim Heuristicas construtivas ----------- */

/* Busca local e vizinhança. */


void ILS(tabela *table, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu);

int busca_tabu(tabela *table, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, int fo_estrela);

void busca_local(tabela *table, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu);

void hill_climbing(tabela *table, tabela *table_linha, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, int *fo);

void hill_climbing_com_melhoria(tabela *table, tabela *table_linha, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, int *fo);

void move(int d, int s, int p, int d1, int s1, int p1, int **DCxD, int **DCxS, int ***CxDxP, int aula, vector<int> *aulas_numeradas, vector<Courses> *c, tabela *table, vector<Curricula> *cu);

void swap(int d, int s, int p, int d1, int s1, int p1, int **DCxD, int **DCxS, int ***CxDxP, int aula, int aula2, vector<int> *aulas_numeradas, vector<Courses> *c, tabela *table, vector<Curricula> *cu);

void gera_melhor_vizinho(tabela *table, tabela *table_linha, int d, int s, int p, int dias, int sal, int ppd, int **AA, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu);

int gera_melhor_vizinho_tabu(tabela *table, int dias, int sal, int ppd, int **AA, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, map<movimentos, bool> *mt, int fo_star, movimentos *mv);

/* -------- Fim Busca local e vizinhança ----------- */

/* Restrições fortes */

int verifica_RF01(tabela *table, int dia, int ppd, int sal);

int verifica_RF02(tabela *table, int aula, int d, int p, int s, int **AA);

int verifica_RF03(tabela *table,int dia, int ppd, int sal);

int verifica_RF04(int ppd, int dia, int p, int aula, int **AI);

void perturbacao (tabela *table, int dias, int sal, int ppd, int **AA, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, vector<movimentos> *moves, int nivel);

/* -------- Fim - Restriçõs fortes----------*/

/* Restrições fracas */

int contabiliza_RFc1(vector<Courses> *c, int dia, int **DCxD);

int contabiliza_RFc2(int ***CxDxP, int curr, int dia, int ppd);

int contabiliza_RFc3(vector<Courses> *c, vector<Rooms> *r, int **DCxS);

int contabiliza_RFc4(vector<Courses> *c, vector<Rooms> *r, int **DCxS);

int contabiliza_custo_insercao_RFc1(vector<Courses> *c, int dia, int **DCxD, int aula);

int contabiliza_custo_insercao_RFc2(int ***CxDxP, int aula, int dia, int ppd, vector<Curricula> *cu);

int contabiliza_custo_insercao_RFc3(vector<Courses> *c, vector<Rooms> *r, int **DCxS, int materia, int sal);

int contabiliza_custo_insercao_RFc4(int materia, vector<Rooms> *r, int **DCxS);


/* -------- Fim - Restrições fracas */

int calcula_fo(vector<Courses> *c, vector<Rooms> *r, int curr, int dia, int ppd, int sal, int **DCxD, int **DCxS, int ***CxDxP);

int calcula_custos_insercao(vector<Courses> *c, vector<Rooms> *r,int aula, int dia, int ppd, int sal, int **DCxD, int **DCxS, int ***CxDxP, vector<Curricula> *cu);

#endif


