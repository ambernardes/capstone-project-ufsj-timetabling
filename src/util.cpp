#include <cstdio>
#include <cstdlib>
#include <string.h>
#include "util.h"
#include <iostream>
#include <vector>
#include <map>
#include "Courses.h"
#include "Constraints.h"
#include "Curricula.h"
#include "Rooms.h"
#include "heuristicas.h"
#include <time.h>

using namespace std;

/* Faz alocação do vetor de custo de inserção. */
aulaSalaCusto *alocar_vetor_custo(int n){
	aulaSalaCusto *v;
	/* ponteiro para o vetor */
	if (n < 1) { /* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return (NULL);
	}
	/* aloca o vetor */
	v = (aulaSalaCusto *) calloc (n, sizeof(aulaSalaCusto));
	if (v == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		return (NULL);
	}
	return (v);
	/* retorna o ponteiro para o vetor */
}

aulaSalaCusto *liberar_vetor_custo (aulaSalaCusto *v){
	if (v == NULL) return (NULL);
	/* libera o vetor */
	free(v);
	/* retorna o ponteiro */
	return (NULL); 
}

/* Faz a alocação do vetor na memória */
int *alocar_vetor (int n){
	int *v;
	/* ponteiro para o vetor */
	if (n < 1) { /* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return (NULL);
	}
	/* aloca o vetor */
	v = (int *) calloc (n, sizeof(int));
	if (v == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		return (NULL);
	}
	return (v);
	/* retorna o ponteiro para o vetor */
}

/* Libera o espaço do vetor na memória */
int *liberar_vetor (int *v){
	if (v == NULL) return (NULL);
	/* libera o vetor */
	free(v);
	/* retorna o ponteiro */
	return (NULL); 
}

/* Faz a alocação da tabela horario na memória */
tabela *alocar_semana (int n){
	tabela *v = NULL;
	/* ponteiro para o vetor */
	if (n < 1) { /* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return (NULL);
	}
	/* aloca o vetor */
	v = (tabela *) calloc (n, sizeof(tabela));
	if (v == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		return (NULL);
	}
	/* retorna o ponteiro para o vetor */
	return (v);
}

/* Libera o espaço da tabela horario na memória */
tabela *liberar_semana (tabela *v){
	if (v == NULL) return (NULL);
	/* libera o vetor */
	free(v);
	/* retorna o ponteiro */
	return (NULL); 
}

int **alocar_dia (int m, int n){
	/* ponteiro para a matriz */
	int **v = NULL; 
	/* variavel auxiliar */
	int	i;
	
	/* verifica parametros recebidos */
	if (m < 1 || n < 1) { 
		printf ("** Erro: Parametro invalido **\n");
		return (NULL);
	}
	/* aloca as linhas da matriz */
	v = (int **) calloc (m, sizeof(int *));
	/* Um vetor de m ponteiros para float */
	if (v == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		return (NULL);
	}
	/* aloca as colunas da matriz */
	for ( i = 0; i < m; i++ ) {
		v[i] = (int*) calloc (n, sizeof(int));
		
		/* m vetores de n floats */
		if (v[i] == NULL) {
			printf ("** Erro: Memoria Insuficiente **");
			return (NULL);
		}
	}
	/* retorna o ponteiro para a matriz */
	return (v); 
}

/* Aloca vetor 3D.*/
int ***alocar_vetor_3d (int m, int n, int o){
	/* ponteiro para a matriz */
	int ***v; 
	/* variavel auxiliar */
	int	i, j;
	
	/* verifica parametros recebidos */
	if (m < 1 || n < 1 || o < 1) { 
		printf ("** Erro: Parametro invalido **\n");
		return (NULL);
	}
	/* aloca as linhas da matriz */
	v = (int ***) calloc (m, sizeof(int **));
	/* Um vetor de m ponteiros para float */
	if (v == NULL) {
		printf ("** Erro: Memoria Insuficiente **");
		return (NULL);
	}
	/* aloca as colunas da matriz */
	for ( i = 0; i < m; i++ ) {
		v[i] = (int**) calloc (n, sizeof(int *));
		
		/* m vetores de n floats */
		if (v[i] == NULL) {
			printf ("** Erro: Memoria Insuficiente **");
			return (NULL);
		}
	}
	
	/* Aloca o terceiro vetor */
	for ( i = 0; i < m; i++ ) {
		for (j =0; j < n; j++){
			v[i][j] = (int*) calloc (o, sizeof(int));
		
			/* m vetores de n floats */
			if (v[i][j] == NULL) {
				printf ("** Erro: Memoria Insuficiente **");
				return (NULL);
			}	
		}
	}
	
	
	/* retorna o ponteiro para a matriz */
	return (v); 
}

/* Aloca vetor 3D.*/
int ***liberar_vetor_3d (int m, int n, int o, int ***v){
	
	/* variavel auxiliar */
	int	i, j;
	
	/* verifica parametros recebidos */
	if (m < 1 || n < 1 || o < 1) { 
		printf ("** Erro: Parametro invalido **\n");
		return (NULL);
	}
	
	/* Aloca o terceiro vetor */
	for ( i = 0; i < m; i++ ) {
		for (j =0; j < n; j++){
			free(v[i][j]);
		}
	}
	
	/* aloca as colunas da matriz */
	for ( i = 0; i < m; i++ ) {
		free(v[i]);
	}
	
	free(v);
	
	/* retorna o ponteiro para a matriz */
	return (v); 
}


int **liberar_dia (int m, int n, int **v){
	/* variavel auxiliar */
	int i; 
	if (v == NULL) return (NULL);
	
	if (m < 1 || n < 1) { /* verifica parametros recebidos */
		printf ("** Erro: Parametro invalido **\n");
		return (v);
	}
	/* libera as linhas da	matriz */
	for (i=0; i< m; i++) 
		free (v[i]); 
	
	/* libera a matriz (vetor de ponteiros) */
	free (v);
	
	/* retorna um ponteiro nulo */
	return (NULL); 
}

/* Libera a memoria dos dias */ 
void encerrar(tabela *v, int dias, int sal, int ppd){
	
	/* Libra as matrizes de dia. */
	for(int x = 0; x < dias; x++){
		v[x].dia = liberar_dia(sal, ppd, v[x].dia);
	}
	
	free(v);
} 

/* Le o cabeçalho do arquivo do dataset. */
void le_cabecalho(int *mat, int *dias, int *ppd, int *sal, int *curr, int *restr, char *nome_arquivo){
	
	FILE *arquivo;
	char linha[3000];
	
	arquivo = fopen(nome_arquivo, "r");
	
	if (arquivo == NULL){
		cerr << "Não foi possivel abrir o arquivo.\n " << endl;
		exit(1);
	}
	
	/* A primeira linha do arquivo não interessa. */
	
	fscanf(arquivo,"%s",linha);
	fscanf(arquivo,"%s",linha);
	
	/* Leitura dos cursos/materias */
	fscanf(arquivo,"%s",linha);
	fscanf(arquivo,"%d",mat);
	
	/* Leitura das salas de aula */
	fscanf(arquivo,"%s",linha);
	fscanf(arquivo,"%d",sal);
	
	/* Leitura dos dias. Geralmente 5, */
	fscanf(arquivo,"%s",linha);
	fscanf(arquivo,"%d",dias);
	
	/* Leitura dos periodos por dia */
	fscanf(arquivo,"%s",linha);
	fscanf(arquivo,"%d",ppd);
	
	/* Leitura dos curriculos */
	fscanf(arquivo,"%s",linha);
	fscanf(arquivo,"%d",curr);
	
	/* Leitura das retrições */
	fscanf(arquivo,"%s",linha);
	fscanf(arquivo,"%d",restr);
	
	/* Fecha o arquivo. */
	fclose(arquivo);
	return;
}

/* Inicializa as estruturas propostas. */
void preenche_estruturas(vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, vector<Constraints> *con, int nmat, int ndias, int nppd, int nsal, int ncurr, int nrestr, char *nome_arquivo){
	
	FILE *arquivo;
	char linha[3000];
	
	arquivo = fopen(nome_arquivo, "r");

	if (arquivo == NULL){
		cerr << "Não foi possivel abrir o arquivo.\n " <<endl;
		exit(1);
	}
	
	while (!feof(arquivo)){
		
		fscanf(arquivo,"%s",linha);
		
		/* Preenche o vetor de cursos */
		if(!strcmp(linha,"COURSES:")){
			
			for(int x = 0; x < nmat; x++){
				/* Intancia o objeto */
				Courses curso;
				
				/* Faz a leitura dos campos e atribuição no objeto */
				fscanf(arquivo,"%s",curso.curso);
				fscanf(arquivo,"%s",curso.professor);
				fscanf(arquivo,"%d",&curso.quantidade_aulas);
				fscanf(arquivo,"%d",&curso.dias_minimos);
				fscanf(arquivo,"%d",&curso.quantidade_alunos);
				/* Adiciona o objeto a lista*/
				c->push_back(curso);
			}
			
		}	
		
		/* Preenche o vetor de salas */
		if(!strcmp(linha,"ROOMS:")){
			for(int x = 0; x < nsal; x++){
				/* Intancia o objeto */
				Rooms rooms;
				
				/* Faz a leitura dos campos e atribuição no objeto */
				fscanf(arquivo,"%s",rooms.nome);
				fscanf(arquivo,"%d",&rooms.lotacao);
				/* Adiciona o objeto a lista*/
				r->push_back(rooms);
			}
		}	

		/* Preenche o vetor de curriculos */
		if(!strcmp(linha,"CURRICULA:")){
			
			for(int x = 0; x < ncurr; x++){
				/* Intancia o objeto */
				Curricula curricula;
				
				/* Faz a leitura dos campos e atribuição no objeto */
				fscanf(arquivo,"%s",curricula.nome);
				fscanf(arquivo,"%d",&curricula.quantidade_materias);
				
				/* Adiciona as disciplinas. */
				for(int y =0; y < curricula.quantidade_materias; y++){
					/* Le o curso. */
					fscanf(arquivo,"%s",linha);
					
					/* Procura pelo curso no vetor de cursos. */
					for(int z = 0; z < (int)c->size(); z++){
						if(!strcmp(linha, c->at(z).curso)){ 
							/* Adiciona o indice do curso ao vetor. */
							
							curricula.lista_cursos.push_back(z);
							
							/* Atribui a essa disciplina, qual é o curriculo que ela pertence. */
							c->at(z).curricula = x;
							break;
						}	
					}
				}
				
				/* Adiciona o objeto a lista*/
				cu->push_back(curricula);
			}
		}
		
		/* Preenche o vetor de restrições */
		if(!strcmp(linha,"UNAVAILABILITY_CONSTRAINTS:")){
			
			for(int x = 0; x < nrestr; x++){
				/* Intancia o objeto */
				Constraints constraints;
				
				/* Faz a leitura dos campos e atribuição no objeto */
				fscanf(arquivo,"%s",constraints.curso);
				fscanf(arquivo,"%d",&constraints.dia);
				fscanf(arquivo,"%d",&constraints.periodo);
				/* Adiciona o objeto a lista*/
				con->push_back(constraints);
			}
			
		}		
	}
	
	/* Fecha o arquivo. */
	fclose(arquivo);
}

/* Numera as aulas de acordo com a sua quantidade */
void numera_aulas(vector<Courses> c, vector<int> *aulas_numeradas){
	
	int indice =0;
	
	for(int x = 0; x < (int)c.size(); x++){
		
		indice = x+1;		
		
		for (int y = 0;  y < c[x].quantidade_aulas; y++){
			/* O numero de cada aula, será controlado pelo indice dessa aula */
			aulas_numeradas->push_back(indice);	
		}
	}
}

/* Cria e inicializa a tabela horário */
tabela *criar_tabela(int dias, int ppd, int sal){
	
	tabela *table = NULL;
	
	/* Faz a alocação das tabelas. */	
	table = alocar_semana (dias);
	
	/* 
		Tendo alocado as tabelas de acordo com os dias, 
	    aloca as matrizes sala x ppd e a inicializa com -1 
	    em todas as posições. 
	*/
	for(int x = 0; x < dias; x++){
		table[x].dia = iniciar_matriz_dias(ppd, sal);
	}
	
	return table;
}

/* Inicializa a matriz de dias. */
int **iniciar_matriz_dias(int ppd, int sal){
	
	int **v = alocar_dia(sal, ppd);
	for(int x = 0; x < sal; x++){
		for(int y = 0; y < ppd; y++){
			v[x][y] = -1;
		}
	}
	return v;
}

/* Imprime a tabela horarios. */
void imprime_tabela_horarios(int dias, int sal, int ppd, tabela *table){
	
	for(int x = 0; x < dias; x++){
		
		for(int y=0; y < sal; y++){
			for(int z=0; z < ppd; z++){
				cout << table[x].dia[y][z] << "  ";
			}
			cout << endl;
		}	
		cout << endl; 
	}
	
}

/* Escreve o arquivo de saida. Formato da saida: <CourseID> <RoomID> <Day> <Day_Period> */
void escreve_solucao(vector<Courses> *c, vector<Rooms> *r, vector<int> *aulas_numeradas, tabela *table, int dias, int sal, int ppd, char *nome_arquivo){
	
	int indice;
	FILE *arquivo;
	arquivo=fopen(nome_arquivo,"w");
	
	if(arquivo == NULL) {
		printf("Nao foi possivel abrir arquivo auxiliar");
		return;
	}
	
	/* Percorre todos os dias. */
	for (int x = 0; x < dias; x++){
		
		/* Percorre as salas. */
		for(int y = 0; y < sal; y++){
			
			/* Percorre os periodos. */
			for(int z = 0; z < ppd; z++){
				
				/* Verifica se o horário não está vago. */
				if(table[x].dia[y][z] != -1){
					
					/* Verifica qual o curso está alocado no horário*/
					indice = aulas_numeradas->at(table[x].dia[y][z]-1);	
					
					/* Escreve no arquivo*/
					fprintf(arquivo,"%s %s %d %d\n",c->at(indice-1).curso, r->at(y).nome,x,z);
					
				}
			}
		}	
	}
	
	fclose(arquivo);
}

/* Preenche a matriz auxiliar CM. */
void preencher_matriz_CM(int mat, vector<Curricula> *cu, vector<int> *aulas_numeradas, int **CM){
	
	/* Para cada curriculo na lista, marca todas as materias que pertencem a ele. */
	for(int x = 0; x < (int)cu->size(); x++){
		
		/* Percorre lista de materias do curriculo. */	
		for(int y = 0; y < (int)cu->at(x).lista_cursos.size(); y++){
					
			CM[x][cu->at(x).lista_cursos.at(y)] = 1;	
		
		}
	
	}
		
}

/* Preenche a matriz auxliar AA. */
void preencher_matriz_AA(vector<Curricula> *curr,vector<Courses> *c, vector<int> *aulas_numeradas, int N, int **AA, int **CM){
	
	int a1, a2;
	
	/*  
	 	Percorre toda a matriz, de forma a verificar todas as aulas em relação a todas as aulas. 
		Por padrão, não ha conflitos entre as aulas. 
	*/
	for(int x = 0; x < N; x++){
		for(int y = 0; y < N; y++){
			
			/* Verifica se a aula x e a y são a mesma aula. */
			if(aulas_numeradas->at(x) == aulas_numeradas->at(y)){
				if(AA[x][y] == 0 &&	AA[y][x] == 0){		
					AA[x][y] = 1;
					AA[y][x] = 1;
				}	
			
			}else {
				
				/* Verifica se ha conflito entre as duas aulas. LEMBRANDO QUE A POSICAO DA AULA NO VETOR, É O VALOR EM AULA_NUMERADA-1*/
				a1 = aulas_numeradas->at(x)-1;
				a2 = aulas_numeradas->at(y)-1;
				
				/* Verifica se as aulas estão no mesmo curriculo. */
				for(int i = 0; i < (int)curr->size(); i++){
					
					/* Se as duas aulas aparecem no mesmo curriculo. */
					if(CM[i][a1] == 1 && CM[i][a2] == 1){
						
						if(AA[x][y] == 0 &&	AA[y][x] == 0){
							
							AA[x][y] = 1;
							AA[y][x] = 1;
						
						}
					}	
				}
					
				/* Verifica se as aulas são lecionadas pelo mesmo professor. */
				if (!strcmp(c->at(a1).professor,c->at(a2).professor)) {
					
					if(AA[x][y] == 0 &&	AA[y][x] == 0){
						/* Existe um conflito. */
						AA[x][y] = 3;
						AA[y][x] = 3;
					}
				
				}
			}
		}	
	}
}

/* Preenche matriz auxiliar AI. */
void preencher_matriz_AI(tabela *table, vector<Constraints> *con, vector<Courses> *c, int **AI, vector<int> * aulas_numeradas, int h){
	
	int inicio, fim, aula;
	
	/* Percorre lista de restrições */
	for(int x = 0; x < (int)con->size(); x++){
		
		/* Descobre o id do curso da restrição. */	
		aula = get_id_aula_por_nome(c,con->at(x).curso);
		
		/* Pega-se o intervalo de aulas desse curso no vetor de aulas numeradas. */
		for(int y = 0; y < (int)aulas_numeradas->size(); y++){
			
			if(aulas_numeradas->at(y) == aula+1){
				inicio = y;
				break;
			} 
		}
		
		/* Descobre até onde vai a aula. */
		fim = inicio + c->at(aula).quantidade_aulas;
		
		/* Informa a restrição na matriz AI */	
		for(int z = inicio; z < fim; z++) AI[z][(con->at(x).dia * h) + con->at(x).periodo] = 1;
	}
}	


/* Copia o vetor de aulas. */
void copiar_aulas(vector<int> *a1, vector<int> *a2){
	
	for(int x = 0; x < (int)a1->size(); x++){
		a2->push_back(x);	
	}
}

/* Copia a tabela de horarios para uma tabela auxiliar. */
void copiar_tabela_horario(tabela *src, tabela *dest, int d, int s, int p){
	
	for(int x = 0; x < d; x++){
		for(int y = 0; y < s; y++){
			for(int z = 0; z < p; z++){
				dest[x].dia[y][z] = src[x].dia[y][z];
			}
		}
	}
}

/* Copia tabela intermediaria. Ou seja, somente os dias que houveram alterações. */
void copiar_tabela_horario_intermediaria(tabela *dest, int d, int s, int p, int d1, int s1, int p1){
	
	/* Copia somente o periodo onde houveram alerações. */
	int aux;
	
	aux = dest[d].dia[s][p];
	dest[d].dia[s][p] = dest[d1].dia[s1][p1];
	dest[d1].dia[s1][p1] = aux;
	
}


/* Seleciona aula para ser trocada na exploração da vizinhança. 
	0 -> escolhe posiçao ocupada.
	1 -> escolhe posicao vazia.
*/
void seleciona_aula_troca(tabela *table,int *d,int *s,int *p, int dias, int sal, int ppd, int tipo, int d1, int s1, int p1){
	
	//srand(time( NULL));
	
	int encontrado = 0;
	
	while(!encontrado) {
		/* Seleciona a aula aleatoria a ser movida. */
		*d = rand() % dias;
		*s = rand() % sal;
		*p = rand() % ppd;
		
		/* Verifica se o num eh o mesmo. */
		if (d1 == *d && s1 == *s && p1 == *p){
			continue;
		
		}else {
			/* Funcao de avaliacao. */
			int ret = verifica_RF03(table,*d,*p,*s);
			
			if ((ret && tipo==0) || (!ret && tipo==1) ) encontrado = 1;
			
			else encontrado = 0;
		}	
	}
}

/* Seleciona aula para ser trocada na exploração da vizinhança. 
	0 -> escolhe posiçao ocupada.
	1 -> escolhe posicao vazia.
*/
int seleciona_aula_troca_tabu(tabela *table,int *d,int *s,int *p, int dias, int sal, int ppd, int tipo, 
							  int d1, int s1, int p1, int tipo_movimento, map<movimentos, bool> *map_tabu, int **AA, int **AI, 
							  int **DCxD, int **DCxS, int ***CxDxP, vector<Curricula> *cu, 
							  vector<Courses> *c, vector<int> *aulas_numeradas, vector<Rooms> *r, int fo, int fo_estrela){
	
	//srand(time( NULL));
	int rf;
	int encontrado = 0;
	int tentativas = 0;
	movimentos m;
	
	/* Preenche dados do movimento. */
	m.tipo_movimento = tipo_movimento;
	m.aula_origem = table[d1].dia[s1][p1];
	m.dia_origem = d1;
	m.sala_origem = s1;
	m.periodo_origem = p1;
	
	while(!encontrado) {
		
		/* Verifica quantas tentativas foram feitas */
		if(tentativas >= tentativaMaxima) return 1;
		
		/* Seleciona a aula aleatoria a ser movida. */
		*d = rand() % dias;
		*s = rand() % sal;
		*p = rand() % ppd;
		
		/* Dados de destino do movimento. */
		m.aula_destino = table[*d].dia[*s][*p];
		m.dia_destino = *d;
		m.sala_destino = *s;
		m.periodo_destino = *p;
		
		/* Verifica se o num eh o mesmo. */
		if (d1 == *d && s1 == *s && p1 == *p){
			continue;
		
		/* Verifica se movimento encontra-se na lita tabu. */
		}else{ 
			
			/* Funcao de avaliacao. */
			int ret = verifica_RF03(table,*d,*p,*s);
		
			if ((ret && tipo==0) || (!ret && tipo==1) ){ 
				
				rf = verifica_restricoes_fortes(m, table, AA, AI, sal, ppd);
			
				if(rf == 0){
					
					/* Verifica se o movimento encontra-se na lista tabu. */
					int na_lista = verificar_movimento_tabu(map_tabu, m);
					
					/* Faz o movimento. */
					if(tipo_movimento == 0)
						move(m.dia_origem, 
							 m.sala_origem, 
							 m.periodo_origem, 
							 m.dia_destino, 
							 m.sala_destino, 
							 m.periodo_destino, 
							 DCxD, 
							 DCxS, 
							 CxDxP, 
							 table[m.dia_origem].dia[m.sala_origem][m.periodo_origem], 
							 aulas_numeradas, 
							 c,
							 table,
							 cu);
					else 
						swap(m.dia_origem, 
							 m.sala_origem, 
							 m.periodo_origem, 
							 m.dia_destino, 
							 m.sala_destino, 
							 m.periodo_destino,  
							 DCxD, 
							 DCxS, 
							 CxDxP, 
							 table[m.dia_origem].dia[m.sala_origem][m.periodo_origem], 
							 table[m.dia_destino].dia[m.sala_destino][m.periodo_destino], 
							 aulas_numeradas, 
							 c, 
							 table, 
							 cu);
				
					/* Se o movimento não encontra-se na lista tabu. */
					if(!na_lista){
						
						//encontrado = 1;
						return 0;
					/* Se o movimento é valido e não se encontra na lista tabu, simplesmente faz o movimento. Seja ele de piora ou melhora. */
					}else {
						
						/* Testa se o movimento traz melhora. */
						int fo_linha = calcula_fo(c, r, (int)cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP);
						
						/* Se o movimento trouxer melhoras, mantem o movimento */ 
						if(/*fo_linha < fo &&*/ fo_linha < fo_estrela){ 
							//cout << "Achou" << endl; 
							encontrado = 1;	
						
						/* Senão defaz o movimento. */
						}else {
							
							/* Faz o movimento. */
							if(tipo_movimento == 0)
								move(m.dia_destino, 
									 m.sala_destino, 
									 m.periodo_destino, 
									 m.dia_origem, 
									 m.sala_origem, 
									 m.periodo_origem, 
									 DCxD, 
									 DCxS, 
									 CxDxP, 
									 table[m.dia_destino].dia[m.sala_destino][m.periodo_destino], 
									 aulas_numeradas, 
									 c,
									 table,
									 cu);
							else 
								swap(m.dia_origem, 
									 m.sala_origem, 
									 m.periodo_origem, 
									 m.dia_destino, 
									 m.sala_destino, 
									 m.periodo_destino,  
									 DCxD, 
									 DCxS, 
									 CxDxP, 
									 table[m.dia_origem].dia[m.sala_origem][m.periodo_origem], 
									 table[m.dia_destino].dia[m.sala_destino][m.periodo_destino], 
									 aulas_numeradas, 
									 c, 
									 table, 
									 cu);
							
							encontrado = 0;	
						}
					}
				}
				
				else encontrado = 0;
			} 
			
			else encontrado = 0;
		}
		
		tentativas++;	
	}
	
	return 0;
}

/* 
	Ordena o vetor de aulas. 
	Ordenação é decrescente de acordo com o custo de inserção de uma aula.
*/
void ordena_vetor_aulas_por_conflito(vector<int> *aulas_numeradas,  vector<int> *aulas_ja_alocadas, vector<int> *aulas_nao_alocadas, int dias, int ppd, int sal, int **AA, int **AI, int **DCxD, vector<Courses> *c, vector<Curricula> *cu, int ***CxDxP){
	
	/* Vetor de conflitos. */
	vector<aulaConflito> quantidade_conflitos;
	
	/* Preenche vetor de conflitos. */
	preencher_vetor_conflitos(&quantidade_conflitos, aulas_ja_alocadas, aulas_numeradas, aulas_nao_alocadas, dias, ppd, sal, AA, AI, DCxD, c, cu, CxDxP);
	
	/* Faz ordenação. Ordena o vetor de conflitos e a partir dessa ordenação, refaz o vetor de aulas não alocadas. */
	qsort(&quantidade_conflitos[0], quantidade_conflitos.size(), sizeof(aulaConflito), comparar_conflito);
	
	/* Zera o vetor para que as aulas sejam posicionadas novamente de forma ordenada. */
	aulas_nao_alocadas->clear();
	
	/* Preenche de forma ordenada o vetor de aulas não alocadas, mantendo as aulas que ja foram alocadas sempre no final */
	for(int x = 0; x < (int) quantidade_conflitos.size(); x++){
		aulas_nao_alocadas->push_back(quantidade_conflitos.at(x).aula);
	}
	
	quantidade_conflitos.clear();
}

/* Preenche vetor de conflitos. */
void preencher_vetor_conflitos(vector<aulaConflito> *quantidade_conflitos, vector<int> *aulas_ja_alocadas, vector<int> *aulas_numeradas, vector<int> *aulas_nao_alocadas, int dias, int ppd, int sal, int **AA, int **AI, int **DCxD, vector<Courses> *c, vector<Curricula> *cu, int ***CxDxP){
	
	int custo = 0;
	
	/* Variável auxiliar */
	aulaConflito aula_conflito;
	
	/* Para todas as aulas não cadastradas. verificar quantidade de conflitos elas apresentam. */
	for(int x = 0; x < (int)aulas_nao_alocadas->size(); x++ ){
		
		custo = 0;
		
		// Na primeira iteração, não existem aulas alocadas ainda. 
		if((int) aulas_ja_alocadas->size() > 0){
			/* Contar os horários em o que o professor da disciplina já leciona alguma aula. */
			custo += get_horarios_professor(aulas_numeradas->at(aulas_nao_alocadas->at(x))-1, c, DCxD, dias);
		}else custo = custo;
		
		/* Horarios inviaveis. Percorre matriz AI para averiguar. */
		custo += get_total_indisponibilidades(x, dias, ppd, AI);
		
		if((int) aulas_ja_alocadas->size() > 0){
			/* Horários em que estão alocadas disciplinas do mesmo currículo. */
			custo += get_total_periodos_curriculo(aulas_numeradas->at(aulas_nao_alocadas->at(x)), cu, CxDxP, dias, ppd);
		}else custo = custo;
		
		/* Adiciona o valor total dos conflitos ao vetor. */
		aula_conflito.conflito = custo;
		aula_conflito.aula = aulas_nao_alocadas->at(x);
		
		quantidade_conflitos->push_back(aula_conflito);
	}
}

/* Verifica o custo de inserção de uma aula na tabela horario. */
int verifica_custo_insercao_aula(){

	return 0;	
}

/* Descobre quem é o professor que leciona uma disciplina. */
char *get_professor(int materia, vector<Courses> *c){
	
	return c->at(materia).professor;
}

/* Preenche vetor com aulas lecionadas pelo professor. */
void get_cursos_lecionados_pelo_professor(char *professor, vector<int> *cursos_lecionados_pelo_professor, vector<Courses> *c){
	
	/* Percorre o vetor de cursos, procurando pelos cursos lecionados pelo professor desejado. */
	for(int x = 0; x < (int) c->size(); x++){
		
		/* Se o professor leciona o curso, esse curso é adicionado a lista. */
		if(!strcmp(professor,c->at(x).professor)){
			cursos_lecionados_pelo_professor->push_back(x);
		}
	}    
}

/* Verifica quantidade de horarios que o professor leciona. */
int get_horarios_professor(int materia, vector<Courses> *c, int **DCxD, int dias){
	
	int custo = 0;
	char professor[500];
	vector<int> cursos_lecionados_pelo_professor;
	
	/* Descobre o nome do professor. */
	strcpy(professor, get_professor(materia, c));
	
	/* Descobre quais as materias lecionadas pelo professor. */
	get_cursos_lecionados_pelo_professor(professor, &cursos_lecionados_pelo_professor, c);
	
	/* Procura na matriz auxiliar CDxD a quantidade de vezes que as aulas encontradas são dadas. */
	for(int x = 0; x < (int)cursos_lecionados_pelo_professor.size(); x++){
		
		for(int y = 0; y < dias; y++){
			
			/* Custo recebe a quantidade de vezes que uma disciplina selecionada está acontecendo ao longo dos dias. */
			custo += DCxD[cursos_lecionados_pelo_professor.at(x)][y];
		
		}
	}
	
	cursos_lecionados_pelo_professor.clear();
	
	return custo;
}


/* Verifica a quantidade de indisponibilidades da aula. */
int get_total_indisponibilidades(int materia, int dias, int ppd, int **AI){
		
	int custo = 0;
	
	/* Percorre matriz AI, procurando indisponibilidades dos professores em dados periodos. */
	for(int x = 0; x < (dias*ppd); x++){
		
		custo += AI[materia][x];
	
	}
	
	return custo;
}

/* Verifica a quantidade de periodos onde aulas do mesmo curriculo estão sendo dadas. */
int get_total_periodos_curriculo(int materia, vector<Curricula> *cu, int ***CxDxP, int dias, int ppd){
	
	int custo = 0;
	
	/* Atualiza a tabela horario. Atualiza tanto na remoção quanto na adição. */
	int curricula;
	vector<int> curriculos;
	
	/* Retorna os curriculos que a materia aparece. */
	get_curriculos_materia(materia, cu, &curriculos);
	
	/* Contar quantidade de horários em que elas aparecem. Percorre matriz 3d, e*/
	for (int x = 0; x < dias; x++){
		
		for(int y = 0; y < ppd; y++){
			
			/* Ataualizar todos os curriculos que a materia possa pertencer. */
			for(int z = 0; z < (int)curriculos.size(); z++){
		
				curricula = curriculos.at(z);
				custo += CxDxP[curricula][x][y];	
			
			}	
		}
	}
	
	curriculos.clear();
	
	/* Retorna o custo. */
	return custo;
}

/* Guarda em um vetor, todas as aulas de um mesmo curriculo. */
void get_aulas_mesmo_curriculo(int materia, vector<int> *materias_mesmo_curriculo, vector<Courses> *c){
	
	/* Percorre o vetor de cursos, e guarda os cursos que pertencem ao mesmo curriculo. */
	for(int x = 0; x < (int) c->size(); x++){
		
		/* Se o curso pertence ao mesmo curriculo que o curso passado, adiciona-o no vetor. Adiciona inclusive o proprio curso. */
		if(c->at(x).curricula == c->at(materia).curricula){
			materias_mesmo_curriculo->push_back(x);
		}
			
	}
} 

/* Comparar conflito. Função auxiliar para a ordenação das aulas de acordo com os conflitos. http://www.cplusplus.com/reference/cstdlib/qsort/*/
int comparar_conflito (const void * a, const void * b){
  
	aulaConflito x = *(aulaConflito*) a;
	aulaConflito y = *(aulaConflito*) b;
   	
   	return ((int) y.conflito - (int) x.conflito);
  
  
  
}

/* Comparar custo. Função auxiliar para a ordenação das aulas de acordo com os custos de inserção. http://www.cplusplus.com/reference/cstdlib/qsort/*/
int comparar_custo (const void * a, const void * b){
  
	aulaSalaCusto x = *(aulaSalaCusto*) a;
	aulaSalaCusto y = *(aulaSalaCusto*) b;
   	
   	return ((int) x.custo - (int) y.custo);
  
  
  
}

/* Verifica se um numero existe num vetor. */
int esta_na_lista(int aula, vector<int> *vetor){
	
	/* Percorre o vetor a procura de um numero. Retorna 1 quando ele está na lista. 0 quando não está. */
	for(int x = 0; x < (int)vetor->size(); x++){
		
		if(vetor->at(x) == aula){
			
			return 1;	
		}
	}
	
	/* Número não presente na lista. */
	return 0;
}

/* 
	Marca os horarios indisponiveis para aquela disciplina. 
	-2 -> Horários inviáveis por aula do mesmo currículo.
	-1 -> Horários inviáveis por indisponibilidade.
	 0 -> Horários viáveis.
	 x -> Horário inviável. O valor que aparecerá é o numero da aula que causa a inviabilidade.
*/
void marcar_horarios_iniviaveis(int *total_horarios, int aula, vector<int> *aulas_nao_alocadas, int dias, int ppd, int sal, int **DCxD, vector<Courses> *c, int **AI, int ***CxDxP, int **AH, vector<int> *aulas_numeradas, vector<Curricula> *cu, tabela *table, int **AA){
	
	/* Indisponibilidade. */
	get_total_inviaveis_indisponibilidades(aula, dias, ppd, AI, total_horarios);
	
	if((int) aulas_nao_alocadas->size() != (int)aulas_numeradas->size()){
		/* Aulas conflitantes. */
		get_total_periodos_inviaveis_curriculo(aula, c, CxDxP, dias, ppd, total_horarios, cu, table, AA, sal);
		//get_total_periodos_inviaveis_curriculo(aulas_numeradas->at(aula) -1, c, CxDxP, dias, ppd, total_horarios, cu, table, AA, sal);
	}
	
	if((int) aulas_nao_alocadas->size() != (int)aulas_numeradas->size()){
		/* Horarios com todas as salas oculapos são inviaveis também. Podendo ser explodidos. */
		get_horarios_todas_salas_ocupadas(table, total_horarios, dias*ppd, ppd, sal);
	}
}

/* Marca os horarios que um professor não pode dar aula, por ja lecionar alguma outra materia. */
//void get_horarios_inviaveis_professor(int materia, vector<Courses> *c, int **DCxD, int dias, int ppd, int *total_horarios, int **AH, vector<int> *aulas_numeradas){
void get_horarios_inviaveis_professor(int materia, vector<Courses> *c, int ***CxDxP, int dias, int ppd, int *total_horarios, vector<Curricula> *cu, tabela *table, int **AA, int sal){
	
	/* Percorre os dias. */
	for(int x = 0; x < dias; x++){
		/* Percorre os periodos. */
		for(int y = 0; y < ppd; y++){
			/* Percorre as salas. */
			for (int s = 0; s < sal; s++){
				/* Se não for uma sala vazia */
				if(table[x].dia[s][y] != -1){
				
					if(AA[materia][table[x].dia[s][y]-1] == 2 || AA[materia][table[x].dia[s][y]-1] == 3){
						
						if(total_horarios[(ppd *x) +y] == 0) total_horarios[(ppd *x) +y] = table[x].dia[s][y]-1;		
						break;
					}	
				}
			}
		}
	}
}

/* Marca os horarios que uma materia não pode ser dada por questoes de inviabilidade. */
void get_total_inviaveis_indisponibilidades(int materia, int dias, int ppd, int **AI, int *total_horarios){
		
	/* Percorre matriz AI, procurando indisponibilidades dos professores em dados periodos. */
	for(int x = 0; x < (dias*ppd); x++){
		
		if(AI[materia][x] == 1){
			total_horarios[x] = -1;
		}
	}	
}

/* Marca os horarios em que existam aulas de um mesmo curriculo acontecendo. */
void get_total_periodos_inviaveis_curriculo(int materia, vector<Courses> *c, int ***CxDxP, int dias, int ppd, int *total_horarios, vector<Curricula> *cu, tabela *table, int **AA, int sal){

	
	/* Atualiza a tabela horario. Atualiza tanto na remoção quanto na adição. */
	/*int curricula;
	vector<int> curriculos;*/
	
	/* Retorna os curriculos que a materia aparece. */
	//get_curriculos_materia(materia, cu, &curriculos);
	
	/* Percorre os dias. */
	/*for(int x = 0; x < dias; x++){
		// Percorre os periodos. 
		for(int y = 0; y < ppd; y++){
			// Percorre as salas.
			for (int z = 0; z < (int)curriculos.size(); z++){
				
				curricula = curriculos.at(z);
				
				if(CxDxP[curricula][x][y] > 0){
					if(total_horarios[(ppd *x) +y] != -1) total_horarios[(ppd *x) +y] = -2;		
				}
			}
		}
	}*/
			
	/* Percorre os dias. */
	for(int x = 0; x < dias; x++){
		// Percorre os periodos. 
		for(int y = 0; y < ppd; y++){
			// Percorre as salas.
			for (int s = 0; s < sal; s++){
				// Se não for uma sala vazia 
				if(table[x].dia[s][y] != -1){
				
					if(AA[materia][table[x].dia[s][y]-1] > 0){
						
						if(total_horarios[(ppd *x) +y] != -1) total_horarios[(ppd *x) +y] = -2;		
						
						break;
					}	
				}
			}
		}
	}
	
	//curriculos.clear();
}

/* Verifica se existem horarios viaveis. Retorna 0 caso encontre algum.*/
int possui_horarios_viaveis(int *total_horarios, int dias, int ppd){
	
	int cont = 0;
	
	/* Percorre o vetor de horarios, até encontrar algum horario que não tenha sido marcado como invalido. */
	for(int x = 0; x < (dias * ppd); x++){
		
		/* Contabiliza o horario como válido. */
		if(total_horarios[x] == 0){
			cont++;;
		}
		
	}
		
	/* Retorna a quantidade de horarios viáveis. */	
	return cont;	
}

/* Armazena quais são os horários que possuem aulas viáveis. */
void preencher_horarios_viaveis(int *H, int *total_horarios, int dias, int ppd){
	
	int i = 0;
	
	/* Percorre vetor com todos os horarios, e armazena em H somente os horarios que são viaveis */
	for(int x = 0; x < (dias * ppd); x++){
		
		if(total_horarios[x] == 0){
			
			H[i] = x;
			i++;
		}
		
	}
}

/* Preenche o vetor com os custos das aulas nas salas. */
void preencher_vetor_custos(int aula, int *H, int qtd_horarios_viaveis, vector<Rooms> *r, tabela *table, aulaSalaCusto *asc, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int ppd, vector<Courses> *c, vector<Curricula> *cu){
	
	int indice_custo = 0;
	int d, p;
	
	/* Para cada horario no vetor de horarios disponiveis. */
	for	(int x = 0; x < qtd_horarios_viaveis; x++){ 
		
		/* Descobre o dia em que o horario em questão estã. */
		d = (int)(H[x]/ppd); p = H[x]%ppd;
		
		/* Para cada sala. */
		for (int y = 0; y < (int)r->size(); y++){
			
			/* Testa se sala está vazia. */
			if(table[d].dia[y][p] == -1){	
				
				/* Para que o calculo seja feito, antes é necessario atualizar as estruturas. */
				atualiza_estruturas_para_calculo(aulas_numeradas->at(aula), DCxD, DCxS, CxDxP, d, y, p, aulas_numeradas,1, c, cu);
				
				/* Se sala está vazia, verifica o custo de inserção. */
				asc[indice_custo].horario = H[x]; 
				asc[indice_custo].sala = y;
				asc[indice_custo].custo = calcula_custos_insercao(c, r, aulas_numeradas->at(aula)-1, d, p, y, DCxD, DCxS, CxDxP, cu);
				
				/* Volta as estruturas ao normal. */
				atualiza_estruturas_para_calculo(aulas_numeradas->at(aula), DCxD, DCxS, CxDxP, d, y, p, aulas_numeradas,0, c, cu);
				
				/* Incrementa o contador do vetor de custos. */
				indice_custo++;
				
			}else{	
				
				/* Se sala não está vazia, atribuise o valor de MIN. */
				asc[indice_custo].horario = H[x]; 
				asc[indice_custo].sala = y;
				asc[indice_custo].custo = MIN;
				
				/* Incrementa o contador do vetor de custos. */
				indice_custo++;
			
			}	
		}
	}
	
	/*cout << "---------------" << endl;
	
	for(int x = 0; x < (int)cu->size(); x++){
		for(int y = 0; y < 5; y++){
			for(int z = 0; z < 4; z++){
				if(CxDxP[x][y][z] == 1) cout << "Achou em " << x << endl;
				
			}		
		}
	}*/
	
	//getchar();
}


/* Atualiza as estruturas dinamicas para o calculo do custo de inserção. */
void atualiza_estruturas_para_calculo(int materia, int **DCxD, int **DCxS, int ***CxDxP, int d, int s, int p, vector<int> *aulas_numeradas, int situacao, vector<Courses> *c, vector<Curricula> *cu){
	
	/* Atualiza a tabela horario. Atualiza tanto na remoção quanto na adição. */
	int curricula;
	vector<int> curriculos;
	
	/* Retorna os curriculos que a materia aparece. */
	get_curriculos_materia(materia, cu, &curriculos);
	
	if(situacao > 0){
		
		/* Atualiza matriz DCxD. */
		DCxD[materia-1][d]++;
			
		/* Atualiza matriz DCxS */
		DCxS[materia-1][s]++;
		
		/* Ataualizar todos os curriculos que a materia possa pertencer. */
		for(int z = 0; z < (int)curriculos.size(); z++){
		
			curricula = curriculos.at(z);
			/* Atualiza matriz CxDxP */
			CxDxP[curricula][d][p]++;
		}	
		
	}else {
		
		/* Atualiza matriz DCxD. */
		DCxD[materia-1][d]--;
			
		/* Atualiza matriz DCxS */
		DCxS[materia-1][s]--;
		
		/* Ataualizar todos os curriculos que a materia possa pertencer. */
		for(int z = 0; z < (int)curriculos.size(); z++){
		
			curricula = curriculos.at(z);
			/* Atualiza matriz CxDxP */
			CxDxP[curricula][d][p]--;
		}	
	}
	
	curriculos.clear();
	
}

/* Encontra o maior e menor custo de se inserir uma aula na tabela. */
void atribui_custos_min_max(int *cmin, int *cmax, aulaSalaCusto *custos, int tamanho_vetor){
	
	/* Ordena vetor de custos. */
	qsort(custos,tamanho_vetor, sizeof(aulaSalaCusto),comparar_custo);
	
	/* cmin é primeira posição não nula. */
	for(int x = 0; x < tamanho_vetor; x++){
		
		/* Atribui o menor custo não negativo. */
		if(custos[x].custo > 0){
			*cmin = custos[x].custo; 
			break;
		}
	}
	
	/* cmax é a ultima posição. */
	*cmax = custos[tamanho_vetor-1].custo;
	
}

/* Atribui sala/dia/horario em que a aula será alocada. */
void atribuir_horario_sala_dia(vector <aulaSalaCusto> *LRC, int *d, int *s,int *p, int x, int ppd){
	
	/* Faz as conversões de indice necessarias. */
	*d = (int) (LRC->at(x).horario/ppd);
	*p = (LRC->at(x).horario%ppd);
	*s = LRC->at(x).sala;	
}

/* Apaga as estruturas usadas na construção do grasp. */
void desalocar_estruturas_grasp(int *total_horarios, int *H, aulaSalaCusto *custo_insercao, vector<aulaSalaCusto> *LRC){
	
	total_horarios = liberar_vetor(total_horarios);
	H = liberar_vetor(H);
	custo_insercao = liberar_vetor_custo(custo_insercao);
	LRC->clear();
}

/* Recebe o nome de um curso, e retorna o id desse curso no vetor de cursos. */
int get_id_aula_por_nome(vector<Courses> *c, char *nome_curso){
	
	for(int x = 0; x < (int)c->size(); x++){
		
		if(!strcmp(nome_curso, c->at(x).curso)){
			return x;	
		}
	}
	
	return -1;
	
}

/* Retorna uma lista contendo os curriculos em que a materia aparece. */
void get_curriculos_materia(int materia, vector<Curricula> *cu, vector<int> *curriculos_materia){
	
	/* 
		Para todos os curriculos, verifica em sua lista de materias, 
		se a matéria passada no parametro. Adiciona na lista os curriculos em que a materia apareça.
	*/
	for(int x = 0; x < (int)cu->size(); x++){
		
		for(int y = 0; y < (int) cu->at(x).lista_cursos.size(); y++){
			
			if(cu->at(x).lista_cursos.at(y) == materia-1){ 
				curriculos_materia->push_back(x);
				break;
			}	
		
		}	
	}
}

/* Retorna um horario aleatório. */
int get_horario_aleatorio(int ppd, int dias, int *total_horarios){
	
	//srand( time( NULL));
	
	int horario = rand() % (dias*ppd);
	
	/* Como não existe um horario viável, tem de se procurar 1. Horários marcados como inviaveis por indisponibilidade não podem ser "explodidos." */
	while(total_horarios[horario] == -1){
		
		horario = rand() % (dias*ppd);
	
	}
	
	return horario;
}

/* Atribui dia e periodo para realizar o procedimento de explosão */
void get_dia_periodo_explosao(int ppd, int horario_para_explosao, int *d, int *p){
	
	*d = (int)(horario_para_explosao/ppd); 
	/* Descobrindo periodo */
	*p = horario_para_explosao%ppd;
}

/* Marca como inviável horarios em que todas as salas de aula estejam ocupadas. */
void get_horarios_todas_salas_ocupadas(tabela *table, int *total_horarios, int tamanho, int ppd, int sal){
	
	int p, d, aux;
	
	/* Percorre todos os horarios, desde os validos quanto os ja marcados como invalidos. */
	for(int x = 0; x < tamanho; x++){
		
		aux = 0;
		
		/* Verifica somente horarios que até então estejam marcados como viáveis. */
		if(total_horarios[x] == 0){
			
			d = (int)(x/ppd); p =x%ppd;
			
			/* Percorre todas as salas de aula nesse periodo/dia */
			for(int s = 0; s < sal; s++){
				/* Se achar pelo menos uma sala vazia, ja sai do laço. */
				if(table[d].dia[s][p] == -1){
					aux = 1;
					break;
				}	
			}
			
			/* Se chegar até esse ponto, é porque todas as salas estão ocupadas.  */	
			if (aux == 0) total_horarios[x] = -3;
			
		
		}	
	}
}

/* Cria e inicializa as matrizes dinamicas */
void inicializar_estruturas_dinamicas(int **AH, int **DCxD, int **DCxS, int ***CxDxP, tabela *table, tabela *table_linha, int sal, int mat, int dias, int N, int H, int curr, int ppd ){
	
	/* Aloca a Matriz auxiliar AH. */
	AH = alocar_dia(N,H);
	
	/* Aloca Matriz auxiliar DC×D. */
	DCxD = alocar_dia(mat, dias);
	
	/* Aloca Matriz auxiliar DCxS. */
	DCxS = alocar_dia(mat, sal);
	
	/* Aloca Matriz auxiliar CxDxP. */
	CxDxP = alocar_vetor_3d(curr,dias,ppd);
	
	/* Inicializa a talela horario */
	table = criar_tabela(dias, ppd, sal);
	
	/* Inicializa a talela horario auxiliar */
	table_linha = criar_tabela(dias, ppd, sal);
}

/* Cria e inicializa as estruturas estaticas */
void inicializar_estruturas_estaticas(tabela *table, int **AA, int **AI, int **CM, int N, int curr, int mat, int H, int ppd,  vector<Courses> *c, vector<Curricula> *cu, vector<Constraints> *con, vector<int> *aulas_numeradas){
	
	/* Aloca a matriz auxiliar CM e a preenche. */
	CM = alocar_dia(curr, mat);
	preencher_matriz_CM(mat, cu, aulas_numeradas, CM);
	
	/* Aloca Matriz AA e a preenche. */
	AA = alocar_dia(N,N);
	preencher_matriz_AA(cu, c, aulas_numeradas, N, AA, CM);
	
	/* Aloca Matriz auxiliar AI e a preenche. */
	AI = alocar_dia(N,H);
	preencher_matriz_AI(table,con,c,AI, aulas_numeradas, ppd);
}

/* Libera todas as estruturas dinamicas. */
void liberar_estruturas_dinamicas(tabela *table, tabela *table_linha, int **DCxD, int **DCxS, int **AH, int ***CxDxP, int mat, int dias, int sal, int curr, int ppd, int N, int H){
	
	DCxD = liberar_dia(mat, dias, DCxD);
	DCxS = liberar_dia(mat, sal, DCxS);
	CxDxP = liberar_vetor_3d(curr, dias, ppd, CxDxP);
	AH = liberar_dia(N,H, AH);
	encerrar(table_linha, dias, sal, ppd);
	encerrar(table, dias, sal, ppd);
}

/* Zera todas as estruturas. */
void reseta_estruturas(int ***CxDxP, int **AH, int **DCxD, int **DCxS, tabela *table, int dias, int sal, int curr, int mat, int ppd, int H, int N){
	
	zera_matriz(N, H, AH);
	zera_matriz(mat, dias, DCxD);
	zera_matriz(mat, sal, DCxS);
	zera_matriz_CxDxP(curr, dias, ppd, CxDxP); 
	zera_tabela(dias, ppd, sal, table);

}

/* Reseta a matriz bidimensional passada. */
void zera_matriz(int linhas, int col, int **matriz){
	
	/* Todas as posições recebem 0. */
	for(int x = 0; x < linhas; x++){
		for(int y = 0; y < col; y++){
			matriz[x][y] = 0;	
		}
	}
}

/* Reseta matriz CxDxP */
void zera_matriz_CxDxP(int curr, int dias, int ppd, int ***CxDxP){
	
	/* Todas as posições recebem 0. */
	for(int x = 0; x < curr; x++){
		for(int y = 0; y < dias; y++){
			for(int z = 0; z < ppd; z++){
				CxDxP[x][y][z] = 0;
			}	
		}
	}
	
}

/* Reseta a tabela horário */
void zera_tabela(int dias, int ppd, int sal, tabela *table){
	
	for(int x = 0; x < dias; x++){
		table[x].dia = iniciar_matriz_dias(ppd, sal);
	}
}

/* Escolhe um horario valido para o MOVE */
void escolhe_horario_valido_MOVE(tabela *table_linha, int **AA, int **AI, int d, int s, int p, int *d1, int *s1, int *p1, int dias, int sal, int ppd){

}

/* Escolhe um horario valido para o MOVE */
void escolhe_aula_valida_SWAP(){
	
}

/* Copia matrizes. */
void copia_matriz(int **src,  int **dest, int l, int c){
	
	for(int x = 0; x < l; x++){
		for(int y = 0; y < c; y++){
			dest[x][y] = src[x][y];
		}
	}
}

/* Copia matrizes 3d. */
void copia_matriz3d(int ***src,  int ***dest, int l, int c, int p){
	
	for(int x = 0; x < l; x++){
		for(int y = 0; y < c; y++){
			for(int z = 0; z < p; z++){
				dest[x][y][z] = src[x][y][z];
			}
		}
	}
}

/* Verifica se a materia (ou aula) está no vetor. */
int verifica_aula_ja_usada(int aula, vector<int> *aulas_utilizadas, vector<int> *aulas_numeradas){
	
	if ((int)aulas_utilizadas->size() == 0) return 1;
	
	/* Percorre o vetor de aulas utilizadas a procura da aula. */
	for(int x = 0; x < (int)aulas_utilizadas->size(); x++){
		
		//if(aulas_utilizadas->at(x) == aulas_numeradas->at(aula)) return 0;
		if(aulas_utilizadas->at(x) == aula) return 0;
	}
	
	/* Se a aula não está no vetor, */
	return 1;
}

/* Verifica se o horario escolhido é igual ao horario ja ocupado. */
int verifica_mesmos_parametros(int d, int s,int p, int d1, int s1,int p1){
	
	/* Chamada para a primeira escolha. */
	if(d == -1){
		return 0;
	}
	
	/* Se o horario escolhido para troca é igual ao horario ja escolhido anteriormente. */
	if((d == d1) && (s == s1) && (p == p1)) return 0;
	
	return 1;
}

/* Compara fo, para ordenação. */
int comparar_fo (const void * a, const void * b){
  
	movimentos x = *(movimentos*) a;
	movimentos y = *(movimentos*) b;
   	
   	return ((int) x.fo - (int) y.fo);
}

/* Verifica se o movimento desejado encontra-se na lista tabu, ou seja, se o movimento está marcado como proibido. */
int verificar_movimento_tabu(map<movimentos, bool> *map_tabu, movimentos m){
	
	/* Se o movimento sendo tentado é um movimento probido, ele estará presente na lista. Retorna 1. */
	map<movimentos,bool>::iterator it = map_tabu->find(m);
	if( it != map_tabu->end() ) return 1;
	else return 0;
}

/* Remove o ultimo elemento da lista tabu. Remove tambem do map auxiliar. */
void remove_elemento_lista_tabu(vector<movimentos> *lista_tabu, map<movimentos, bool> *map_tabu){
	
	movimentos m;
	
	/* Guarda o ultimo elemento da lista tabu. */
	m = lista_tabu->at(lista_tabu->size()-1);
	
	/* Remove o elemento do map. */
	map<movimentos,bool>::iterator it = map_tabu->find(m);
	if( it != map_tabu->end() ) map_tabu->erase(it);
	
	/* Remove o ultimo elemento da lista.*/
	lista_tabu->erase(lista_tabu->begin() + lista_tabu->size()-1);
}

/* Adiciona elemento na lista tabu e no map auxiliar. */
void adiciona_elemento_lista_tabu(vector<movimentos> *lista_tabu, map<movimentos, bool> *map_tabu, movimentos m){
	
	/* Adiciona elementro a lista. */
	lista_tabu->insert (lista_tabu->begin() , m );
	
	/* Adiciona elementro ao map. */
	map_tabu->insert (std::make_pair(m, true) );
		
}

/* Verifica as restrições fortes. */
int verifica_restricoes_fortes(movimentos m, tabela *table, int **AA, int **AI, int sal, int ppd){
	
	int rf;
	
	/* Verifica restrição de conflitos. */
	rf = verifica_RF02(table, 
					   table[m.dia_origem].dia[m.sala_origem][m.periodo_origem]-1, 
					   m.dia_destino, 
					   m.periodo_destino, 
					   sal, 
					   AA);
				
	/* Só verifica essa restrição, caso tenha atendido a restrição anterior. */
	if (rf == 0) rf = verifica_RF04(ppd, 
									m.dia_destino, 
									m.periodo_destino, 
									table[m.dia_origem].dia[m.sala_origem][m.periodo_origem]-1, 
									AI);
	
	/* Caso se trate de um Swap. */
	if(m.tipo_movimento == 1 && rf == 0){
			
		/* Verifica restrição de conflitos. */
		rf = verifica_RF02(table, 
						   table[m.dia_destino].dia[m.sala_destino][m.periodo_destino]-1, 
						   m.dia_origem, 
						   m.periodo_origem, 
						   sal, 
						   AA);
					
		/* Só verifica essa restrição, caso tenha atendido a restrição anterior. */
		if (rf == 0) rf = verifica_RF04(ppd, 
									    m.dia_origem, 
									    m.periodo_origem, 
									    table[m.dia_destino].dia[m.sala_destino][m.periodo_destino]-1, 
									    AI);
	
	}
	return rf;
				
}

/* Preenche lista de alphas. */
void preenche_lista_alpha(float *lista){
	
	FILE *arquivo;
	float alpha;
	
	arquivo = fopen("alphas.txt", "r");

	if (arquivo == NULL){
		cerr << "Não foi possivel abrir o arquivo.\n " <<endl;
		exit(1);
	}
	
	for(int x = 0; x < n_alfa; x++){
		
		fscanf(arquivo,"%f",&alpha);
		lista[x] = alpha;
	
	}	
}

/* Escolhe o alfa. */
int obter_posicao(float *alphas, int parTam){
	if (!alphas || parTam == 0) return 0;
	
	float rand_prob = ((float)rand()/RAND_MAX);
	
	int pos = 0;
	
	for (int i = 1; i < parTam; i++)
		if (rand_prob <= alphas[i] && rand_prob > alphas[i-1])
			pos = i;
	
	return pos;
}

/* ---------------- Fim util.cpp -----------------*/
