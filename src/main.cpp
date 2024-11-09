#include <iostream>
#include "Courses.h"
#include "Constraints.h"
#include "Curricula.h"
#include "heuristicas.h"
#include "Rooms.h"
#include "util.h"
#include <vector>
#include <map>
#include <time.h>
#include <limits.h>

using namespace std;

int main(int argc, char *argv[]){
	
		srand( time( NULL));

	
	/* Verifica parametros de entrada. */
	if(argc != 4){
		cerr << "Correto:  " << argv[0] << " <input_file> <alpha> <output_file>" << endl;
		exit(1);
	}
	
	int N;
	int H;
	int **AA = NULL;
	int **AI = NULL;
	int **AH = NULL;
	int **CM = NULL;
	int **DCxD = NULL;
	int **DCxS = NULL;
	int ***CxDxP = NULL;
	int **DCxD_aux = NULL;
	int **DCxS_aux = NULL;
	int ***CxDxP_aux = NULL;
	int mat;
	int dias; 
	int ppd;
	int sal;
	int curr;
	int restr;
	int iter = 1;
	int fo, fo_estrela = INT_MAX;
	float alfa = atof(argv[2]);
	
	/* Tabelas. */
	tabela *table = NULL;
	tabela *table_aux = NULL;
	tabela *table_linha = NULL;
	tabela *table_estrela = NULL;
	
	/* listas */
	vector<Rooms> r;
	vector<Courses> c;
	vector<Curricula> cu;
	vector<Constraints> con;
	//vector<Solucao> p_elite;
	vector<int> aulas_numeradas;
	
	// alpha corrente
	double alpha;
	int flag = 0;

	double *alphas;
	int *cont;
	int *score;
	double *prob, *avg, *q;

	alphas = (double *) malloc(4 * sizeof(double));
	alphas[0] = 0.08; alphas[1] = 0.15; alphas[2] = 0.25; alphas[3] = 0.3; 

	cont = (int *) malloc(4 * sizeof(int));
	score = (int *) malloc(4 * sizeof(int));
	prob = (double *) malloc(4 * sizeof(double));
	avg = (double *) malloc(4 * sizeof(double));
	q = (double *) malloc(4 * sizeof(double));

	for(int i = 0; i < 4; i++){
		cont[i] = 0;
		score[i] = 0;
		prob[i] = 0.25 + prob[i-1];
		avg[i] = 0;
		q[i] = 0;
	}
	
	time_t inicio, fim;
	double seconds=408.0;

	/* Preenche a lista de parametros alpha */
	//preenche_lista_alpha(&alphas[0]);
	
	/* Le o cabeçalho do arquivo. */
	le_cabecalho(&mat,&dias,&ppd,&sal,&curr,&restr, argv[1]);
	
	/* Preenche as estruturas. */
	preenche_estruturas(&c, &r, &cu, &con, mat, dias, ppd, sal, curr, restr, argv[1]);
	
	/* Numera as aulas, de acordo com sua quantidade */
	numera_aulas(c,&aulas_numeradas);
	
	/* Seta o valor de N */
	N = aulas_numeradas.size();
	/* Seta o valor de H */
	H = dias * ppd;
	
	/* Aloca a matriz auxiliar CM e a preenche. */
	CM = alocar_dia(curr, mat);
	preencher_matriz_CM(mat, &cu, &aulas_numeradas, CM);
	
	/* Aloca Matriz AA e a preenche. */
	AA = alocar_dia(N,N);
	preencher_matriz_AA(&cu, &c, &aulas_numeradas, N, AA, CM);
	
	/* Aloca Matriz auxiliar AI e a preenche. */
	AI = alocar_dia(N,H);
	preencher_matriz_AI(table, &con, &c, AI, &aulas_numeradas, ppd);
	
	/* Aloca as estruturas dinamicas. */
	
	/* Aloca a Matriz auxiliar AH. */
	AH = alocar_dia(N,H);
	
	/* Aloca Matriz auxiliar DCxD. */
	DCxD = alocar_dia(mat, dias);
	DCxD_aux = alocar_dia(mat, dias);
	
	/* Aloca Matriz auxiliar DCxS. */
	DCxS = alocar_dia(mat, sal);
	DCxS_aux = alocar_dia(mat, sal);
	
	/* Aloca Matriz auxiliar CxDxP. */
	CxDxP = alocar_vetor_3d(curr,dias,ppd);
	CxDxP_aux = alocar_vetor_3d(curr,dias,ppd);
	
	/* Inicializa a talela horario */
	table = criar_tabela(dias, ppd, sal);
	table_aux = criar_tabela(dias, ppd, sal);
	table_linha = criar_tabela(dias, ppd, sal);
	table_estrela = criar_tabela(dias, ppd, sal);
	
	time(&inicio);
	time(&fim);
	
	/* loop de iterações. */
	while(/*iter < maxIter + 1  &&*/ difftime(fim,inicio) < seconds){
		
		cout << iter << endl;
		
		
		// Seleciona alpha
		float val = (float)rand()/RAND_MAX;
		//printf("val: %f\n", val);
		int pos = 0;
		while (prob[pos] <= val) pos++;
		//printf("pos: %d\n", pos);
		alpha = alphas[pos];
		//printf("alpha escolhido: %lf\n", alpha);
		
		alfa = alpha;
		
		// Constroi a solução inicial. 
		constroi_solucao_inicial(alfa, table, &aulas_numeradas, dias, ppd, sal, AA, &con, &c, AI, DCxD, DCxS, CxDxP, &cu, AH, &r, CM);
		fo = calcula_fo(&c, &r, curr, dias, ppd, sal, DCxD, DCxS, CxDxP); 
		
		cout << "Inicial " << fo << endl;
		
		// Reativo
		cont[pos] += 1; 
		score[pos] += fo;
		double sum_q = 0;
		
		//for (int k=0; k<4; k++) printf("count[%d] = %d\n", k, cont[k]);
		
		if (flag == 0){
			flag =1;
			for (int k=0; k<4; k++) 
				if(cont[k] == 0) flag = 0;
		}

		if (flag == 1 && iter % bloco == 0){

			for (int k=0; k<4; k++) {
				if (cont[k] > 0) avg[k] = (double)(score[k]/cont[k]);
				q[k] = (double)(avg[k]/fo_estrela);
				sum_q += q[k];
			
			}
			
			prob[0] = q[0]/sum_q;
			for (int k=1; k<4; k++) {
				prob[k] = q[k]/sum_q + prob[k-1];
			}
		}
		
		// Busca Local. 
		fo = busca_tabu(table, dias, sal, ppd, curr, DCxD, DCxS, CxDxP, &aulas_numeradas, AA, AI, &c, &r, &cu, fo_estrela);
		
		if(fo < fo_estrela){
			cout << "fo obtida " << fo << " fo_estrela " << fo_estrela << endl; 
			copiar_tabela_horario(table, table_estrela, dias, sal, ppd);
			fo_estrela = fo;
		}
		
		
		/* Zerar todas as estruturas dinamicas. */
		if(iter <= maxIter) reseta_estruturas(CxDxP, AH, DCxD, DCxS, table, dias, sal, curr, mat, ppd, H, N);
		
		/* Incrementa o numero de iterações. */
		iter++;
		
		// Atualiza o tempo
		time(&fim);
	}
	
	cout << argv[3] << ": " << fo_estrela << endl;
	
	/* Escreve a solução. */
	escreve_solucao(&c, &r, &aulas_numeradas, table_estrela, dias, sal, ppd, argv[3]);
	
	//imprime_tabela_horarios(dias, sal, ppd, table_estrela);
	
	/* Apaga as estruturas dinamicas. */
	liberar_estruturas_dinamicas(table, table_estrela, DCxD, DCxS, AH, CxDxP, mat, dias, sal, curr, ppd, N, H);
	//DCxD_aux = liberar_dia(mat, dias, DCxD_aux);
	//DCxS_aux = liberar_dia(mat, sal, DCxS_aux);
	//CxDxP_aux = liberar_vetor_3d(curr, dias, ppd, CxDxP_aux);
	//encerrar(table_aux, dias, sal, ppd);
	encerrar(table_linha, dias, sal, ppd);
	aulas_numeradas.clear();
	AA = liberar_dia(N,N,AA);
	CM = liberar_dia(curr, mat,CM);
	AI = liberar_dia(N,H,AI);
	
	c.clear();
	r.clear();
	con.clear();
	
	for(int x = 0; x < (int)cu.size(); x++){
		cu.at(x).lista_cursos.clear();	
	}
	
	cu.clear();
	
	return 0;	
}
