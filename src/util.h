#ifndef UTIL
#define UTIL

#include <iostream>
#include "Courses.h"
#include "Constraints.h"
#include "Curricula.h"
#include "Rooms.h"
#include <vector>
#include <map>

#define rfc1 5
#define rfc2 2
#define rfc3 1
#define rfc4 1
#define poolElite 10
#define maxIter 500
#define bloco 10
#define btmax 2000
#define btsize 100
#define K 35

#define vizinhanca 0.01
#define iterSemMelhora 100
#define iterHillClimbing 10
#define MIN -9999
#define tentativaMaxima 100
#define n_alfa 6
#define DEBUG cout << "E la vamos nós." << endl

using namespace std;

typedef struct{
	int **dia;	
} tabela;

/* Guarda a aula e o valor de conflito dela. */
typedef struct{
	int aula;	
	int conflito;
} aulaConflito;

/* Guarda a relação horário/sala e o valor referente ao custo de inserção da aula nesse horário/sala. */
typedef struct{
	int horario;
	int sala;	
	int custo;
} aulaSalaCusto;

/* Estrutura que guarda os vetores estaticos. */
typedef struct{
	
	/* Uma vez setados, só serão usados para consulta. */
	
	int dias;
	int sal;
	int ppd;
	int mat;
	int curr;
	int restr;
	
	/* Vetores. */
	vector<Rooms> r;
	vector<Courses> c;
	vector<Curricula> cu;
	vector<Constraints> con;	
	
	/* Estruturas estaticas. */
	int **AA;
	int **AI;
	
}Problema;


/* Estrutura que salva os movimentos tentados. */
struct movimentos {
	
	/* Tipo do movimento. 0 -> MOVE; 1-> SWAP. */
	int tipo_movimento;
	
	/* Aulas a serem novidas */
	int aula_origem;
	int aula_destino; 
	
	/* Dados de destino. */
	int dia_origem;
	int sala_origem;
	int periodo_origem;
	
	/* Dados de origem. */
	int dia_destino;
	int sala_destino;
	int periodo_destino;
	
	/* Função objetivo. */
	int fo;
	
	bool operator<(const movimentos &other) const { return aula_origem < other.aula_origem; }
	bool operator==(const movimentos &other) const { 
					return  aula_origem== other.aula_origem 
							&& aula_destino== other.aula_destino
							&& tipo_movimento== other.tipo_movimento
							&& dia_origem == other.dia_origem
							&& sala_origem == other.sala_origem
							&& periodo_origem == other.periodo_origem
							&& dia_destino == other.dia_destino
							&& sala_destino == other.sala_destino
							&& periodo_destino == other.periodo_destino; }
};
typedef struct movimentos movimentos;

/* Estrutura que guarda as tabelas dinamicas. */
typedef struct{
	
	int **DCxD;
	int **DCxS;
	int ***CxDxP;
	tabela *table;
	int fo;
	
}Solucao;

/* Arquivos */

void le_cabecalho(int *mat, int *dias, int *ppd, int *sal, int *curr, int *restr, char *nome_arquivo);

void preenche_estruturas(vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, vector<Constraints> *con, int nmat, int ndias, int nppd, int nsal, int ncurr, int nrestr, char *nome_arquivo);

void escreve_solucao(vector<Courses> *c, vector<Rooms> *r, vector<int> *aulas_numeradas, tabela *v, int dias, int sal, int ppd, char *nome_arquivo);

/* Matrizes */

tabela *criar_tabela(int dias, int ppd, int sal);

int **iniciar_matriz_dias(int ppd, int sal);

tabela *alocar_semana (int n);

tabela *liberar_semana (tabela *v);

int **alocar_dia (int m, int n);

int ***alocar_vetor_3d (int m, int n, int o);

int **liberar_dia (int m, int n, int **v);

int ***liberar_vetor_3d (int m, int n, int o, int ***v);

void copiar_tabela_horario(tabela *table, tabela *table_linha, int d, int s, int p);

void copiar_tabela_horario_intermediaria(tabela *dest, int d, int s, int p, int d1, int s1, int p1);

void preencher_matriz_AA(vector<Curricula> *curr,vector<Courses> *c, vector<int> *aulas_numeradas, int N, int **AA, int **CM);

void preencher_matriz_AI(tabela *table, vector<Constraints> *con, vector<Courses> *c, int **AI, vector<int> * aulas_numeradas, int h);

void preencher_matriz_CM(int mat, vector<Curricula> *cu, vector<int> *aulas_numeradas, int **CM);

void inicializar_estruturas_dinamicas(int **AH, int **DCxD, int **DCxS, int ***CxDxP, tabela *table, tabela *table_linha, int sal, int mat, int dias, int N, int H, int curr, int ppd );

void inicializar_estruturas_estaticas(tabela *table, int **AA, int **AI, int **CM, int N, int curr, int mat, int H, int ppd,  vector<Courses> *c, vector<Curricula> *cu, vector<Constraints> *con, vector<int> *aulas_numeradas);

void liberar_estruturas_dinamicas(tabela *table, tabela *table_linha, int **DCxD, int **DCxS, int **AH, int ***CxDxP, int mat, int dias, int sal, int curr, int ppd, int N, int H);

void zera_matriz_CxDxP(int curr, int dias, int ppd, int ***CxDxP);

void zera_matriz(int linhas, int col, int **matriz);

void copia_matriz(int **src,  int **dest, int l, int c);

void copia_matriz3d(int ***src,  int ***dest, int l, int c, int p);

/* Vetores */

int *alocar_vetor (int n);

int *liberar_vetor (int *v);

aulaSalaCusto *alocar_vetor_custo(int n);

aulaSalaCusto *liberar_vetor_custo (aulaSalaCusto *v);

void numera_aulas(vector<Courses> c, vector<int> *aulas_numeradas);

void imprime_tabela_horarios(int dias, int sal, int ppd, tabela *table);

void copiar_aulas(vector<int> *a1, vector<int> *a2);

void ordena_vetor_aulas_por_conflito(vector<int> *aulas_numeradas, vector<int> *aulas_ja_alocadas, vector<int> *aulas_nao_alocadas, int dias, int ppd, int sal, int **AA, int **AI, int **DCxD, vector<Courses> *c, vector<Curricula> *cu, int ***CxDxP);

void preencher_vetor_conflitos(vector<aulaConflito> *quantidade_conflitos, vector<int> *aulas_ja_alocadas, vector<int> *aulas_numeradas, vector<int> *aulas_nao_alocadas, int dias, int ppd, int sal, int **AA, int **AI, int **DCxD, vector<Courses> *c, vector<Curricula> *cu, int ***CxDxP);

void preencher_horarios_viaveis(int *H, int *total_horarios, int dias, int ppd);

void preencher_vetor_custos(int aula, int *H, int qtd_horarios_viaveis, vector<Rooms> *r, tabela *table, aulaSalaCusto *asc, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int ppd, vector<Courses> *c, vector<Curricula> *cu);

/* Auxiliares */

void reseta_estruturas(int ***CxDxP, int **AH, int **DCxD, int **DCxS,  tabela *table, int dias, int sal, int curr, int mat, int ppd, int H, int N);

void marcar_horarios_iniviaveis(int *total_horarios, int aula, vector<int> *aulas_nao_alocadas, int dias, int ppd, int sal, int **DCxD, vector<Courses> *c, int **AI, int ***CxDxP, int **AH, vector<int> *aulas_numeradas, vector<Curricula> *cu, tabela *table, int **AA);

int possui_horarios_viaveis(int *total_horarios, int dias, int ppd);

void encerrar(tabela *v, int dias, int sal, int ppd);

void seleciona_aula_troca(tabela *table,int *d,int *s,int *p, int dias, int sal, int ppd, int tipo, int d1, int s1, int p1);

int seleciona_aula_troca_tabu(tabela *table,int *d,int *s,int *p, int dias, int sal, int ppd, int tipo, 
							  int d1, int s1, int p1, int tipo_movimento, map<movimentos, bool> *map_tabu, int **AA, int **AI, 
							  int **DCxD, int **DCxS, int ***CxDxP, vector<Curricula> *cu, 
							  vector<Courses> *, vector<int> *aulas_numeradas, vector<Rooms> *r, int fo, int fo_estrela);

int verifica_aula_ja_usada(int aula, vector<int> *aulas_utilizadas, vector<int> *aulas_numeradas);

int verificar_movimento_tabu(map<movimentos, bool> *map_tabu, movimentos move);

char *get_professor(int materia, vector<Courses> *c);

int get_horarios_professor(int materia, vector<Courses> *c, int **DCxD, int dias);

int get_total_indisponibilidades(int materia, int dias, int ppd, int **AI);

int get_total_periodos_curriculo(int materia, vector<Curricula> *cu, int ***CxDxP, int dias, int ppd);

int verifica_aula_ja_usada(int aula, vector<int> *aulas_utilizadas);

//void get_horarios_inviaveis_professor(int materia, vector<Courses> *c, int **DCxD, int dias, int ppd, int *total_horarios, int **AH, vector<int> *aulas_numeradas);

void get_horarios_inviaveis_professor(int materia, vector<Courses> *c, int ***CxDxP, int dias, int ppd, int *total_horarios, vector<Curricula> *cu, tabela *table, int **AA, int sal);

void get_horarios_todas_salas_ocupadas(tabela *table, int *total_horarios, int tamanho, int ppd, int sal);

void get_total_inviaveis_indisponibilidades(int materia, int dias, int ppd, int **AI, int *total_horarios);

void get_total_periodos_inviaveis_curriculo(int materia, vector<Courses> *c, int ***CxDxP, int dias, int ppd, int *total_horarios, vector<Curricula> *cu, tabela *table, int **AA, int sal);

void get_aulas_mesmo_curriculo(int materia, vector<int> *materias_mesmo_curriculo, vector<Courses> *c);

int get_id_aula_por_nome(vector<Courses> *c, char *nome_curso);

int get_horario_aleatorio(int ppd, int dias, int *total_horarios);

void get_dia_periodo_explosao(int ppd, int horario_para_explosao, int *d, int *p);

void get_curriculos_materia(int materia, vector<Curricula> *cu, vector<int> *curriculos_materia);

int comparar_conflito (const void * a, const void * b);

int comparar_custos (const void * a, const void * b);

int comparar_fo (const void * a, const void * b);

int esta_na_lista(int aula, vector<int> *vetor);

void atualiza_estruturas_para_calculo(int materia, int **DCxD, int **DCxS, int ***CxDxP, int d, int s, int p, vector<int> *aulas_numeradas, int situacao, vector<Courses> *c, vector<Curricula> *cu);

void atribui_custos_min_max(int *cmin, int *cmax, aulaSalaCusto *custos, int tamanho_vetor);

void atribuir_horario_sala_dia(vector <aulaSalaCusto> *LRC, int *d, int *s,int *p, int x, int ppd);

void desalocar_estruturas_grasp(int *total_horarios, int *H, aulaSalaCusto *custo_insercao, vector<aulaSalaCusto> *LRC);

void zera_tabela(int dias, int ppd, int sal, tabela *table);

int verifica_mesmos_parametros(int *d,int *s,int *p, int d1, int s1,int p1);

void remove_elemento_lista_tabu(vector<movimentos> *lista_tabu, map<movimentos, bool> *map_tabu);

void adiciona_elemento_lista_tabu(vector<movimentos> *lista_tabu, map<movimentos, bool> *map_tabu, movimentos m);

int verifica_restricoes_fortes(movimentos m, tabela *table, int **AA, int **AI, int sal, int ppd);

void preenche_lista_alpha(float *lista);

int obter_posicao(float *alphas, int parTam);

#endif
