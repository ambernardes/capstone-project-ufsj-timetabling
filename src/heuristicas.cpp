/*
 * 
 * heuristicas.cpp
 * Implesmentação das funções descritas em heuristicas.h
 * 
 * Antônio Marcos Machado <amm.bernardes@gmail.com>
 * 
 */

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
#include "heuristicas.h"
#include "Rooms.h"
#include <time.h>

using namespace std;

/* Constroi uma solução aleatória. Não se preocupa em respeitar nenhuma restrição. */
void constroi_solucao_aleatoria(tabela *table, vector<int> *aulas_numeradas, int dias, int ppd, int sal, int **AA, vector<Constraints> *cons, vector<Courses> *c, int **AI, int **DCxD, int **DCxS, int ***CxDxP, int **CM){
	
	srand( time( NULL));
	
	int d, s, p, rf;
	/* vetor auxiliar. */
	vector<int> aulas_aux;
	/* Copia o vetor de aulas. */
	copiar_aulas(aulas_numeradas, &aulas_aux);
	
	/* 
		Percorre toda a lista de aulas.
		A cada iteração, uma aula é sorteada dentro da lista.
		Um dia, uma sala e um periodo são sorteados aleatoriamente.
		A aula é alocada nessa posição.
	*/	
	
	//while((int)aulas_aux.size() > 0){
		/* Sorteia uma aula aleatoriamente. */
	//	x = rand() % aulas_aux.size();
	//	cout << x << endl; 
	for (int x = 0; x < (int)aulas_numeradas->size(); x++){
		
		/* Verificador das restrições. */
		rf = 1;
		
		/* Antes de fazer a inserção, é preciso verificar se as restrições fortes vão ser obedecidas. */
		while(rf){
			
			/* Sorteia o dia. */
			d = rand() % dias;
			/* Sorteia a sala. */
			/* Sorteia o periodo. */
			s = rand() % sal;
			p = rand() % ppd;
			
			/* Verifica  RF3. Sala vazia. */
			rf = verifica_RF03(table,d,p,s);
			
			/* Verifica RF2 somente se a restricão anterior foi obecedida. */
			if(rf == 0) rf = verifica_RF02(table,x,d,p,sal,AA);
			
			/* Verifica RF4 somente se a restriçao anterior foi obedecida. */
			if(rf == 0) rf = verifica_RF04(ppd, d, p, x, AI);
			
		}	
		
		
		/* Aula cadastrada com sucesso. */
		table[d].dia[s][p] = x+1;
		
		/* Atualiza matriz DCxD. */
		DCxD[aulas_numeradas->at(x)-1][d]++;
		
		/* Atualiza matriz DCxS */
		DCxS[aulas_numeradas->at(x)-1][s]++;
		
		/* Atualiza matriz CxDxP */
		CxDxP[c->at(aulas_numeradas->at(x)-1).curricula][d][p] = 1;
		
		/* Remove aula da lista. */
	//	aulas_aux.erase(aulas_aux.begin() + x-1);
	}
	/* Verifica RF1. */
	
	//	imprime_tabela_horarios(dias,sal,ppd,table);
}

void constroi_solucao_gulosa(){
	
}

/* Criação da solução inicial do GRASP. */
void constroi_solucao_parcialmente_aleatoria(float alfa, tabela *table, vector<int> *aulas_numeradas, int dias, int ppd, int sal, int **AA, vector<Constraints> *cons, vector<Courses> *c, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<Curricula> *cu, int **AH, vector<Rooms> *r){
	
	/* Guardam os valores dos custos */
	int rf, cmax, cmin, indice = 0;
	
	int qtd_horarios_viaveis;
	
	/* Gera lista de aulas não alocadas. A principio, todas as aulas são não alocadas. */
	vector<int> aulas_nao_alocadas;
	
	/* Vetor que armazena aulas que ja foram armazenadas. */
	vector<int> aulas_ja_alocadas;
	
	/* Possivel que esteja errada. */
	copiar_aulas(aulas_numeradas, &aulas_nao_alocadas);
	
	/* Aula que será alocada. Será atribuida aqui a primeira aula no vetor de aulas não alocadas. */
	int aula;
	
	/* Guarda os horarios. É preenchido inicialmente com todos os horarios, em seguida, são marcados os horarios inviáveis. */
	int *total_horarios, *H, p, d, s;
	aulaSalaCusto *custo_insercao;
	
	vector<aulaSalaCusto> LRC;
	
	/* Ordena de forma decrescente lista de aulas não alocadas. As aulas são numeradas de acordo com sua dificuldade de alocação. */
	ordena_vetor_aulas_por_conflito(aulas_numeradas, &aulas_ja_alocadas, &aulas_nao_alocadas, dias, ppd, sal, AA, AI, DCxD, c, cu, CxDxP);
	
	/* Para todas as aulas. */
	while(aulas_nao_alocadas.size() > 0){
	
		rf = 1;
		//srand(time(NULL));
		
		/* Pega a primeira aula do vetor de aulas não alocadas. */
		aula = aulas_nao_alocadas.at(0);
		
		/* 
			Encontra um horario válido para essa aula. A principio, todos os horarios são viáveis.
			Preenche o vetor marcando os horarios que são inviáveis para a aula.
			Horários viáveis são marcados como 0. Inviaveis marcados como 1 nesse vetor.
		*/
		
		total_horarios = alocar_vetor(dias * ppd);
		
		/* Encontra periodos inviaveis. */
		marcar_horarios_iniviaveis(total_horarios, aula, &aulas_nao_alocadas, dias, ppd, sal, DCxD, c, AI, CxDxP, AH, aulas_numeradas, cu, table, AA);   
		
		/* Recebe a quantidade de horarios viaveis. */
		qtd_horarios_viaveis = possui_horarios_viaveis(total_horarios, dias, ppd);
		
		/* Verifica necessidade de explosão. */
		if(qtd_horarios_viaveis == 0){
			
			/* Remove uma aula ja alocada. */
			rf = explosao(aula, table, aulas_numeradas, &aulas_ja_alocadas, &aulas_nao_alocadas, total_horarios, DCxD, DCxS, CxDxP, AH, dias, ppd, sal, c, cu, AA, AI);	
			/* Apos remover uma aula que causava inviabilidade, existe UM horario viavel. */
			qtd_horarios_viaveis++;
		}
			
		if(rf == 1){	
			
			/* Define H - que é o vetor de horarios viáveis. */
			H = alocar_vetor(qtd_horarios_viaveis);
			
			/* Guarda em vetor, somente os horarios que são viaveis. */
			preencher_horarios_viaveis(H,total_horarios,dias,ppd);
			
			/* Aloca o vetor de custos. Para cada horário todas as salas. */
			custo_insercao = alocar_vetor_custo(qtd_horarios_viaveis*sal);
			
			/* Para cada horário viável, preenche o vetor de custos de inserção. */
			preencher_vetor_custos(aula, H, qtd_horarios_viaveis, r, table, custo_insercao, DCxD, DCxS, CxDxP, aulas_numeradas, ppd, c, cu);
			
			/* Encontra o custo maximo e custo minimo. Na primeira execução, qualquer horário (com excessão dos indisponiveis) pode ser escolhido. */	
			atribui_custos_min_max(&cmin, &cmax, custo_insercao, qtd_horarios_viaveis*sal);
			
			/* Monta a LRC. */
			monta_LRC(&LRC, cmin, cmax, alfa, custo_insercao, qtd_horarios_viaveis*sal);
			
			/* Escolher aleatoriamente uma posição na lista. */
			indice = rand() % (int)LRC.size();
			
			/* Atribui sala/dia/periodo. */
			atribuir_horario_sala_dia(&LRC, &d, &s, &p, indice, ppd);
			
			/* Verifica restrições fortes. */
			rf = verifica_RF02(table,aula,d,p,s,AA);
				
			if (rf == 0) rf = verifica_RF04(ppd, d, p, aula, AI);
				
			if(rf == 0)	{
				
				//cout << "Materia " << aulas_numeradas->at(aula) -1 << endl;
				
				/* Atualiza as estruturas. */
				atualiza_estruturas_para_calculo(aulas_numeradas->at(aula), DCxD, DCxS, CxDxP, d, s, p, aulas_numeradas, 1, c, cu);
				
				/*for(int x = 0; x < (int)cu->size(); x++){
					for(int y = 0; y < 5; y++){
						for(int z = 0; z < 4; z++){
							if(CxDxP[x][y][z] == 1) cout << "Achou em " << x << endl;
							
						}		
					}
				}*/
				
				//getchar();
				
				
				/* Atualiza Matriz de horarios */
				AH[aula][LRC[indice].horario] = 1;
				
				/* Atualiza tabela horario. */
				table[d].dia[s][p] = aula + 1;
				
				/* Aula alocada com sucesso. Aula deve ser 'marcarda' como ja alocada. */
				aulas_ja_alocadas.push_back(aula);
				
				/* Remove a aula cadastrada da lista de aulas não alocadas. A primeira aula da lista. */
				aulas_nao_alocadas.erase(aulas_nao_alocadas.begin());
				
				/* Ordena de forma decrescente lista de aulas não alocadas. As aulas são numeradas de acordo com sua dificuldade de alocação. */
				ordena_vetor_aulas_por_conflito(aulas_numeradas, &aulas_ja_alocadas, &aulas_nao_alocadas, dias, ppd, sal, AA, AI, DCxD, c, cu, CxDxP);
				
				/* Desaloca os vetores */
				desalocar_estruturas_grasp(total_horarios, H, custo_insercao, &LRC);
			}	
		}	
	}
		
	/* Apaga as estruturas. */
	aulas_nao_alocadas.clear();
	aulas_ja_alocadas.clear();	
}

/* 
	Constroi solução inicial. Dado o alfa, a construção pode ser de 3 tipos: 
	0 - Aleatória.
	0 < alfa < 1 - Gulosa e parcialmente aleatória. 
	1 - Gulosa.
*/

void constroi_solucao_inicial(float alfa, tabela *table, vector<int> *aulas_numeradas, int dias, int ppd, int sal, int **AA, vector<Constraints> *cons, vector<Courses> *c, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<Curricula> *cu, int **AH, vector<Rooms> *r, int **CM){
	
	/* Verifica o alfa e decide qual tipo de solução inicial será usado. */
	if(alfa == 0.0){
		
		/* Solução puramente aleatória. */
		//constroi_solucao_aleatoria(table,aulas_numeradas, dias, ppd, sal, AA, cons, c, AI, DCxD, DCxS, CxDxP, CM);
		exit(1);
	} else 	if((alfa > 0) && (alfa < 1)){
		
		/* Solução inicial do GRASP. Parcialmente aleatória. */
		constroi_solucao_parcialmente_aleatoria(alfa, table, aulas_numeradas, dias, ppd, sal, AA, cons, c, AI, DCxD, DCxS, CxDxP, cu, AH, r);
	
	}else {
		
		/* Solução puramente gulosa. Não será implementada. */
		exit(1);
	}
}

/* MOVE. Move uma aula para uma posição vazia. */
void move(int d, int s, int p, int d1, int s1, int p1, int **DCxD, int **DCxS, int ***CxDxP, int aula, vector<int> *aulas_numeradas, vector<Courses> *c, tabela *table, vector<Curricula> *cu){
	
	atualiza_estruturas_para_calculo(aulas_numeradas->at(aula-1), DCxD, DCxS, CxDxP, d, s, p, aulas_numeradas, 0, c, cu);
	atualiza_estruturas_para_calculo(aulas_numeradas->at(aula-1), DCxD, DCxS, CxDxP, d1, s1, p1, aulas_numeradas, 1, c, cu);
	
	table[d1].dia[s1][p1] = aula;
	table[d].dia[s][p] = -1;
}

/* SWAP. Troca duas aulas de lugar. */
void swap(int d, int s, int p, int d1, int s1, int p1, int **DCxD, int **DCxS, int ***CxDxP, int aula, int aula2, vector<int> *aulas_numeradas, vector<Courses> *c, tabela *table, vector<Curricula> *cu){
	
	/* Move a aula 1 para o lugar ocupado pela aula 2. */
	atualiza_estruturas_para_calculo(aulas_numeradas->at(aula-1), DCxD, DCxS, CxDxP, d, s, p, aulas_numeradas, 0, c, cu);
	atualiza_estruturas_para_calculo(aulas_numeradas->at(aula-1), DCxD, DCxS, CxDxP, d1, s1, p1, aulas_numeradas, 1, c, cu);
	
	/* Move a aula 2 para o lugar ocupado pela aula 1. */
	atualiza_estruturas_para_calculo(aulas_numeradas->at(aula2-1), DCxD, DCxS, CxDxP, d1, s1, p1, aulas_numeradas, 0, c, cu);
	atualiza_estruturas_para_calculo(aulas_numeradas->at(aula2-1), DCxD, DCxS, CxDxP, d, s, p, aulas_numeradas, 1, c, cu);
	
	/* Troca a aula na tabela de horarios auxiliar. */	
	table[d1].dia[s1][p1] = aula;
	table[d].dia[s][p] = aula2;	
}

/* 
	Se uma aula não pode ser adiciona, realoca uma aula ja alocada, para que a nova entre em seu lugar. 
	**** Fução um tanto acoplada. Se houver tempo, modularizar melhor esse trecho de código. ****
*/
int explosao(int aula_a_ser_alocada, tabela *table, vector<int> *aulas_numeradas , vector<int> *aulas_ja_alocadas, vector<int> *aulas_nao_alocadas, int *total_horarios, int **DCxD, int **DCxS, int ***CxDxP, int **AH, int dias, int ppd, int sal, vector<Courses> *c, vector<Curricula> *cu, int **AA, int **AI){
	
	//srand(time(NULL));
	
	/* Variavel usada para receber o horario. */
	int horario_para_explosao;
	
	/* Aula a ser removida. */
	int aula;
	
	/* Variaveis auxiliares usadas para fazer a troca. */
	int d, p, s;
	
	/* Escolhe aleatóriamente, um horário para a substituição. Não permite que seja um horário marcado como inviável. */
	horario_para_explosao = get_horario_aleatorio(ppd,dias,total_horarios);
	
	/* Tendo escolhido um horário, atualiza a tabela de horarios, retirando a aula que ocupava a posição escolhida. */
	get_dia_periodo_explosao(ppd, horario_para_explosao, &d, &p);
	
	/* Verifica qual o tipo da inviábilidade. Em ambos os casos, será necessaio descobrir a sala que a aula está alocada. */
	switch(total_horarios[horario_para_explosao]){
		
		/* Aula do mesmo curriculo acontecendo nesse horario. Deve-se encontrar qual a aula e removê-la. */
		case -2:
			
			/* Encontra a aula de mesmo curriculo que acontece nesse horário e a sala onde ela acontece. */
			for(int x = 0; x < sal; x++){
				
				/* Verifica se a aula pertence ao mesmo curriculo.  */
				if(table[d].dia[x][p] != -1){
					
					/* Verifica na tabela que guarda as incompatibilidades se as aulas possuem conflitos. */
					if(AA[aula_a_ser_alocada][table[d].dia[x][p]-1] > 0 ){
						
						aula = table[d].dia[x][p]-1;	
						s = x;
						//break;
					}
				}
			}
			
			break;
		
		/* Salas vazias. */
		case -3:
			
			/* Escolhe a aula que esteja acontecendo em uma sala aleatória. */
			s = rand() % sal;
			aula = table[d].dia[s][p]-1;
			
			break;
	}
	
	/* Antes de fazer as atribuições, verifica se a aula pode ser inserida nessa posição */
	int rf = verifica_RF02(table,aula_a_ser_alocada,d,p,s,AA);
				
	if (rf == 0) rf = verifica_RF04(ppd, d, p, aula_a_ser_alocada, AI);
	
	if(rf == 0){
	
		/* Atualiza tabelas auxiliares dinamicas e tabela de horários */
		atualiza_estruturas_para_calculo(aulas_numeradas->at(aula),DCxD,DCxS,CxDxP,d,s,p,aulas_numeradas,0,c,cu);
		
		/* Atualiza a tabela de horarios. */
		table[d].dia[s][p] = -1;
		
		/* Atualiza a matriz de periodos */
		AH[aula][horario_para_explosao] = 0;
		
		/* Adiciona a aula novamente à lista de aulas não alocadas. */
		aulas_nao_alocadas->push_back(aula);
		
		/* Retira a aula da lista de aulas ja alocadas */
		for(int x = 0; x < (int)aulas_ja_alocadas->size(); x++){
			
			/* Remove a aula do vetor. */
			if(aulas_ja_alocadas->at(x) == aula){
				
				aulas_ja_alocadas->erase(aulas_ja_alocadas->begin() + x);
				break;
			}
			
		}
		
		/* Marca horario como viável. */
		total_horarios[horario_para_explosao] = 0;
		
		return 1;
	}else {
		return 0;	
	}
}

/* Duas aulas não podem ser alocadas em uma mesma sala/periodo. */
int verifica_RF03(tabela *table,int dia, int ppd, int sal){
	
	/* Verifica se posição é vazia. */
	if(table[dia].dia[sal][ppd] == -1){
		return 0;	
	}else return 1;
				
}

/* Duas aulas do mesmo curriculo ou ministradas pelo mesmo professor, não pode acontecer no mesmo período. */
int verifica_RF02(tabela *table, int aula, int d, int p, int sal, int **AA){
	
	int aula2;
	
	/* Percorre todas as salas em um dado periodo de um dia. */
	for(int x = 0; x < sal; x++){
		
		aula2 = table[d].dia[x][p]-1;
		
		/* Só verifica se acontece conflito, para o caso de existir uma aula alocada nessa dia/sala/periodo. */
		if(table[d].dia[x][p] > 0){
			
			/* Se houver um conflito, seja por curriculo ou professor, encerra o processo. */
			if(AA[aula][aula2] > 0) return 1;
		}
	}
	
	/* Se chegou até esse ponto, é porque não ha conflitos. */
	return 0;
}

/* Avalia a lista de restrições e não permite que sejam desreipeitadas. */
int verifica_RF04(int ppd, int dia, int p, int aula, int **AI){
	
	if(AI[aula][(dia*ppd) + p] == 0) return 0;			
	else return 1;
}

/* Calcula o valor da função objetivo. */
int calcula_fo(vector<Courses> *c, vector<Rooms> *r, int curr, int dia, int ppd, int sal, int **DCxD, int **DCxS, int ***CxDxP){
	
	int fo1, fo2, fo3, fo4;
	
	/* Primeira restrição fraca : Dias minimos de aula. */	
	fo1 = contabiliza_RFc1(c, dia, DCxD);
	
	/* Segunda restrição fraca  : Aulas isoladas. */
	fo2 = contabiliza_RFc2(CxDxP,curr,dia,ppd);
	
	/* Terceira restrição fraca : Lotação das salas de aula. */
	fo3 = contabiliza_RFc3(c, r, DCxS);
	
	/* Quarta restrição fraca   : Estabilidade de sala.  */
	fo4 = contabiliza_RFc4(c, r, DCxS);
	
	//cout << "Dias Minimos: " << fo1  << endl << " Aulas isoladas: " << fo2 << endl << " Lotação das salas: " << fo3 << endl << " Estabilidade sala: " << fo4 << endl; 
	
	return (fo1+fo2+fo3+fo4);
}

/* Verifica o valor relacionado à primeira restrição fraca. Dias minimos de trabalho. */
int contabiliza_RFc1(vector<Courses> *c, int dia, int **DCxD){
	int fo = 0, qtd;
	
	/* Percorre todos os cursos. */
	for(int x = 0; x < (int)c->size(); x++){
		
		qtd =0;
		
		/* Percorre todos os dias. */
		for(int y = 0; y < dia; y++){
			if(DCxD[x][y] != 0) qtd++;
		}
		
		/* Fo recebe a quantidade de dias minimos de aula - a quantidade de dias que a aula foi dada. */
		if (qtd < c->at(x).dias_minimos) fo += c->at(x).dias_minimos - qtd;
	
	}
	
	/* Retorna o valor, multiplicado pelo peso. */
	return fo*rfc1;
}


/* Verifica o valor relacionado à terceira restrição fraca. Lotação das salas. */
int contabiliza_RFc3(vector<Courses> *c, vector<Rooms> *r, int **DCxS){
	int fo = 0;
	
	for(int x = 0; x < (int)c->size(); x++){
		for(int y = 0; y < (int)r->size(); y++){
			
			/* Se existirem aulas nessa turma. */
			if(DCxS[x][y] > 0){
				if (c->at(x).quantidade_alunos > r->at(y).lotacao){
					fo += (c->at(x).quantidade_alunos - r->at(y).lotacao) * DCxS[x][y];
				}	
			} 
		}
	}
	
	/* Retorna o valor, multiplicado pelo peso. */
	if(fo < 0) return 0;
	else return fo*rfc3;
}

/* Verifica o valor relacionado à quarta restrição fraca. Estabilidade da sala. */
int contabiliza_RFc4(vector<Courses> *c, vector<Rooms> *r, int **DCxS){
	int fo = 0, qtd = 0;
	
	/* Percorre todas as disciplinas. */
	for(int x = 0; x < (int)c->size(); x++){
		/* Percorre as salas de aula. */
		for(int y = 0; y < (int)r->size(); y++){
			
			/* 
				Se existirem aulas dessa disciplina nessa sala, ela é contabilizada.
				A ideia é aparecer em no minimo uma.
			*/
			if(DCxS[x][y] != 0){
				qtd++;	
			} 
		}
		
		/* Para cada sala distinta, soma-se uma violaçào. Se houver apenas uma sala, não há violação. */
		if(qtd > 1) fo += qtd-1;
		
		qtd = 0;
	
	}
	
	/* Retorna o valor, multiplicado pelo peso. */
	return fo*rfc4;
}

/* Verifica o valor relacionado à segunda restrição fraca. Aulas isoladas. */
int contabiliza_RFc2(int ***CxDxP, int curr, int dia, int ppd){
	
	int fo=0;
	
	/* Percorre os curriculos. */
	for(int x = 0; x < curr; x++){
		/* Percorre os dias. */
		for(int y = 0; y < dia; y++){
			/* Percorre os periodos. */
			for(int z = 0; z < ppd; z++){
				
				/* Verificação se acontece uma aula nesse periodo. */
				if(CxDxP[x][y][z] > 0){
					
					/* Primeira posição do vetor de periodos. */
					if(z == 0){
						if(CxDxP[x][y][z+1] != 1) fo++;
					}	
					
					/* Última posição do vetor de periodos. */
					else if(z == ppd-1){
						if(CxDxP[x][y][z-1] != 1) fo++;
					}
					
					/* Outras posições do vetor. */	
					else {
						if((CxDxP[x][y][z+1] != 1) && (CxDxP[x][y][z-1] != 1)) fo++;
					}	
				}
			}
		}	
	}
	
	return fo*rfc2;
}

/* Calcula o custo de inserção de uma aula na tabela. */
int calcula_custos_insercao(vector<Courses> *c, vector<Rooms> *r,int aula, int dia, int ppd, int sal, int **DCxD, int **DCxS, int ***CxDxP, vector<Curricula> *cu){
	
	int fo = 0;
	
	/* Primeira restrição fraca : Dias minimos de aula. */	
	fo += contabiliza_custo_insercao_RFc1(c, dia, DCxD, aula);
	
	/* Segunda restrição fraca  : Aulas isoladas. */
	fo += contabiliza_custo_insercao_RFc2(CxDxP,aula,dia,ppd, cu);
	
	/* Terceira restrição fraca : Lotação das salas de aula. */
	fo += contabiliza_custo_insercao_RFc3(c, r, DCxS, aula, sal);
	
	/* Quarta restrição fraca   : Estabilidade de sala.  */
	fo += contabiliza_custo_insercao_RFc4(aula, r, DCxS);
	
	return fo;	
		
}


/* Contabiliza qual o custo de inserir a aula na tabela horario.  */
int contabiliza_custo_insercao_RFc1(vector<Courses> *c, int dia, int **DCxD, int aula){
	int fo = 0, qtd = 0;
	
	/* Percorre todos os dias. */
	for(int y = 0; y < dia; y++){
		if(DCxD[aula][y] != 0) qtd++;
	}
	
	/* Fo recebe a quantidade de dias minimos de aula - a quantidade de dias que a aula foi dada. */
	if (qtd < c->at(aula).dias_minimos) fo += c->at(aula).dias_minimos - qtd;
	
	/* Retorna o valor, multiplicado pelo peso. */
	return fo*rfc1;
}

/* Contabiliza qual o custo de inserir a aula na tabela horario.  */
int contabiliza_custo_insercao_RFc2(int ***CxDxP, int aula, int dia, int ppd, vector<Curricula> *cu){
	
	int fo=0;
	/* Atualiza a tabela horario. Atualiza tanto na remoção quanto na adição. */
	int curr;
	vector<int> curriculos;
	
	/* Retorna os curriculos que a materia aparece. */
	get_curriculos_materia(aula, cu, &curriculos);
	
	/* Ataualizar todos os curriculos que a materia possa pertencer. */
	for(int x = 0; x < (int)curriculos.size(); x++){
		
		curr = curriculos.at(x);
		
		for(int z = 0; z < ppd; z++){
			
			/* Verificação se acontece uma aula nesse periodo. */
			if(CxDxP[curr][dia][z] == 1){
				
				/* Primeira posição do vetor de periodos. */
				if(z == 0){
					if(CxDxP[curr][dia][z+1] != 1) fo++;
				}	
				/* Última posição do vetor de periodos. */
				else if(z == ppd-1){
					if(CxDxP[curr][dia][z-1] != 1) fo++;
				}
				/* Outras posições do vetor. */	
				else {
					if((CxDxP[curr][dia][z+1] != 1) && (CxDxP[curr][dia][z-1] != 1)) fo++;
				}	
			}
		}
	}
	
	return fo*rfc2;
	
}

/* Contabiliza qual o custo de inserir a aula na tabela horario.  */
int contabiliza_custo_insercao_RFc3(vector<Courses> *c, vector<Rooms> *r, int **DCxS, int materia, int sal){
	
	int fo = 0;
	
	fo = (c->at(materia).quantidade_alunos - r->at(sal).lotacao) * DCxS[materia][sal];
	
	/* Retorna o valor, multiplicado pelo peso. */
	if(fo < 0) return 0;
	else return fo*rfc3;
}

/* Contabiliza qual o custo de inserir a aula na tabela horario.  */
int contabiliza_custo_insercao_RFc4(int materia, vector<Rooms> *r, int **DCxS){
	
	int fo = 0, qtd = 0;
	
	/* Percorre as salas de aula. */
	for(int y = 0; y < (int)r->size(); y++){
		
		/* 
			Se existirem aulas dessa disciplina nessa sala, ela é contabilizada.
			A ideia é aparecer em no minimo uma.
		*/
		if(DCxS[materia][y] != 0){
			qtd++;	
		} 
	}
	
	/* Para cada sala distinta, soma-se uma violaçào. Se houver apenas uma sala, não há violação. */
	if(qtd > 1) fo += qtd;
	
	
	/* Retorna o valor, multiplicado pelo peso. */
	return fo*rfc4;
}

/* Monta a lista restrica de canditados. */
void monta_LRC(vector<aulaSalaCusto> *LRC, int cmin, int cmax, float alfa, aulaSalaCusto *custos, int tamanho){
	
	int max;
	
	/* Variavel auxiliar */
	aulaSalaCusto asc;
	
	/* Atribui o valor maximo do intervalo. */
	max = (cmin + (alfa*(cmax - cmin)));
	
	/* Para todos os horaios/sala, verifica qual tem o custo no intervalo definido, adicionando-o à LRC. */
	for(int x = 0; x < tamanho; x++){
		
		/* Se o custo encontra-se no intervalo. */
		if(custos[x].custo >= cmin && custos[x].custo <= max && cmin > 0){
			
			asc.horario = custos[x].horario;
			asc.sala = custos[x].sala;
			asc.custo = custos[x].custo;
			
			LRC->push_back(asc);
		}
	
	}
}

/* Faz a busca local */
void busca_local(tabela *table, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu){
	
	int p, d, s;
	
	int tamanho_vizinhaca = (int)(aulas_numeradas->size() * vizinhanca);
	tabela *table_linha = NULL;
	/* Inicializa a talela horario auxiliar */
	table_linha = criar_tabela(dias, ppd, sal);
	
	for(int x = 0; x < 50; x++){
		
		/* A partir dessa escolha, encontra k vizinhos dessa solução. Fica com o melhor. */
		gera_melhor_vizinho(table, table_linha, d, s, p, dias, sal, ppd, AA, AI, DCxD, DCxS, CxDxP, aulas_numeradas, c, r, cu);
		
	}
	
	//cout << "Melhor fo = " << calcula_fo(c, r, cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP) << endl;
	
	/* Remove da memoria as estruturas criadas. */
	encerrar(table_linha,dias,sal,ppd);
}

/* Faz a busca local. Percorre toda a tabela horario, vasculhando possiveis vizinhanças. */
void hill_climbing(tabela *table, tabela *table_linha, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, int *fo){
	
	int rf, fo_linha = 0;
	int p, d, s;
	int p1, d1, s1;
	int movimento, iter = 0;	
	int tentativas_movimento;
		
	 /* Percorre todas as aulas alocadas. Somente aulas validas. */
	 while(iter < iterHillClimbing){
		
		//srand( time( NULL));
		
		/* Seleciona aula para troca. Seja por move ou swap. */
		seleciona_aula_troca(table_linha, &d, &s, &p, dias, sal, ppd, 0, -1, -1, -1);
		
		rf = 1;
		
		/* Escolhe o movimento a ser usado */
		movimento = rand() % 100;
		tentativas_movimento = 0;
			
		/* MOVE. */
		if(movimento % 2 == 0){
				
			/* Verifica a possibilidade de movimentação da aula. */
			while(rf) {
				
				/* Para que não fique perdido tentando encontrar troca viavel, delimita-se um numero maximo de tentativas. */
				if(tentativas_movimento == tentativaMaxima) break;
				
				/* Seleciona posição vazia para a troca. */
				seleciona_aula_troca(table_linha, &d1, &s1, &p1, dias, sal, ppd, 1, d, s, p);
				
				rf = verifica_RF02(table_linha,table_linha[d].dia[s][p]-1,d1,p1,s1,AA);
				
				/* Só verifica essa restrição, caso tenha atendido a restrição anterior. */
				if (rf == 0) rf = verifica_RF04(ppd, d1, p1, table_linha[d].dia[s][p]-1, AI);
				
				tentativas_movimento++;
			}
			if(rf == 0){
				
				/* Faz o movimento. */
				move(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table_linha[d].dia[s][p], aulas_numeradas, c,table_linha,cu);
				
				/* Calcular a função objetivo. */	
				fo_linha = calcula_fo(c, r, curr, dias, ppd, sal, DCxD, DCxS, CxDxP);
				
				//cout << "Antiga " << *fo << " nova " << fo_linha << endl; getchar();
				
				/* Comparar com a função corrente. */
				if(fo_linha < *fo){
					
					/* Faz a troca. */	
					*fo = fo_linha;
					
					/* Altera a aula na tabela. */
					copiar_tabela_horario(table_linha,table,dias,sal, ppd);
					
				}else {
					
					/* Desfaz o movimento. */
					move(d1, s1, p1, d, s, p, DCxD, DCxS, CxDxP, table_linha[d1].dia[s1][p1], aulas_numeradas, c,table_linha, cu);
				}
				
				/* Incrementa a iteração. */
				iter++;
			}
		
		}else {
		/* SWAP. */	
			
			/* Verifica a possibilidade de movimentação da aula. */
			while(rf) {
				
				/* Para que não fique perdido tentando encontrar troca viavel, delimita-se um numero maximo de tentativas. */
				if(tentativas_movimento == tentativaMaxima) break;
				
				/* Seleciona aula para troca. */
				seleciona_aula_troca(table_linha, &d1, &s1, &p1, dias, sal, ppd, 0, d, s, p);
					
				/* Verifica se a aula 1, pode ser alocada no lugar da aula 2 */
				rf = verifica_RF02(table_linha,table_linha[d].dia[s][p]-1,d1,p1,s1,AA);
				if (rf == 0) rf = verifica_RF04(ppd, d1, p1, table_linha[d].dia[s][p]-1, AI);
				
				/* Verifica se a aula 2, pode ser alocada no lugar da aula 1 */
				if (rf == 0) rf = verifica_RF02(table_linha,table_linha[d1].dia[s1][p1]-1,d,p,s,AA);
				if (rf == 0) rf = verifica_RF04(ppd, d, p, table_linha[d1].dia[s1][p1]-1, AI);
				
				tentativas_movimento++;
			}
			if(rf == 0){
				
				/* Faz a troca das aulas. */
				swap(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table_linha[d].dia[s][p],table_linha[d1].dia[s1][p1] , aulas_numeradas, c,table_linha, cu);
				
				/* Calcular a função objetivo. */	
				fo_linha = calcula_fo(c, r, curr, dias, ppd, sal, DCxD, DCxS, CxDxP);
				
				/* Comparar com a função corrente. */
				if(fo_linha < *fo){
					
					/* Faz a troca. */	
					*fo = fo_linha;
					
					/* Altera a aula na tabela. */
					copiar_tabela_horario(table_linha,table,dias,sal, ppd);
					
				}else {
					
					/* Desfaz o movimento. */
					swap(d1, s1, p1, d, s, p, DCxD, DCxS, CxDxP, table_linha[d1].dia[s1][p1],table_linha[d].dia[s][p] , aulas_numeradas, c,table_linha, cu);
					
				}
				
				/* Incrementa a iteração. */
				iter++;
			
			}	
		}
	}
}		

/* Faz a busca local. Percorre toda a tabela horario, vasculhando possiveis vizinhanças. */
void hill_climbing_com_melhoria(tabela *table, tabela *table_linha, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, int *fo){
	
	int rf, fo_linha = 0;
	int p, d, s;
	int p1, d1, s1;
	int movimento, melhoria = 0;	
	int tentativas_movimento;	
	
	/* Percorre todas as aulas alocadas. Somente aulas validas. */
	while(melhoria < iterSemMelhora){
				
		//srand( time( NULL));
		
		/* Seleciona aula para troca. Seja por move ou swap. */
		seleciona_aula_troca(table_linha, &d, &s, &p, dias, sal, ppd, 0, -1, -1, -1);
		
		rf = 1;
		
		/* Escolhe o movimento a ser usado */
		movimento = rand() % 100;
		tentativas_movimento = 0;
					
		/* MOVE. */
		if(movimento%2 == 0){
				
			/* Verifica a possibilidade de movimentação da aula. */
			while(rf) {
				
				/* Para que não fique perdido tentando encontrar troca viavel, delimita-se um numero maximo de tentativas. */
				if(tentativas_movimento == tentativaMaxima) break;
				
				/* Seleciona posição vazia para a troca. */
				seleciona_aula_troca(table_linha, &d1, &s1, &p1, dias, sal, ppd, 1, d, s, p);
				
				rf = verifica_RF02(table_linha,table_linha[d].dia[s][p]-1,d1,p1,s1,AA);
				
				/* Só verifica essa restrição, caso tenha atendido a restrição anterior. */
				if (rf == 0) rf = verifica_RF04(ppd, d1, p1, table_linha[d].dia[s][p]-1, AI);
				
				tentativas_movimento++;
			}
			
			if(rf == 0){
				
				/* Faz o movimento. */
				move(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table_linha[d].dia[s][p], aulas_numeradas, c,table_linha,cu);
				
				/* Calcular a função objetivo. */	
				fo_linha = calcula_fo(c, r, curr, dias, ppd, sal, DCxD, DCxS, CxDxP);
				
				/* Comparar com a função corrente. */
				if(fo_linha < *fo){
					
					/* Faz a troca. */	
					*fo = fo_linha;
					
					/* Altera a aula na tabela. */
					copiar_tabela_horario(table_linha,table,dias,sal, ppd);
					
					/* Como houve melhoria, reseta o contador. */
					melhoria = 0;
					
				}else {
					
					/* Desfaz o movimento. */
					move(d1, s1, p1, d, s, p, DCxD, DCxS, CxDxP, table_linha[d1].dia[s1][p1], aulas_numeradas, c,table_linha, cu);
					
					melhoria++;
				}
			}
			
		}else {
		/* SWAP. */	
			
			/* Verifica a possibilidade de movimentação da aula. */
			while(rf) {
				
				/* Para que não fique perdido tentando encontrar troca viavel, delimita-se um numero maximo de tentativas. */
				if(tentativas_movimento == tentativaMaxima) break;
				
				/* Seleciona aula para troca. */
				seleciona_aula_troca(table_linha, &d1, &s1, &p1, dias, sal, ppd, 0, d, s, p);
					
				/* Verifica se a aula 1, pode ser alocada no lugar da aula 2 */
				rf = verifica_RF02(table_linha,table_linha[d].dia[s][p]-1,d1,p1,s1,AA);
				if (rf == 0) rf = verifica_RF04(ppd, d1, p1, table_linha[d].dia[s][p]-1, AI);
				
				/* Verifica se a aula 2, pode ser alocada no lugar da aula 1 */
				if (rf == 0) rf = verifica_RF02(table_linha,table_linha[d1].dia[s1][p1]-1,d,p,s,AA);
				if (rf == 0) rf = verifica_RF04(ppd, d, p, table_linha[d1].dia[s1][p1]-1, AI);
				
				tentativas_movimento++;
			}
			
			if(rf == 0){
				
				/* Faz a troca das aulas. */
				swap(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table_linha[d].dia[s][p],table_linha[d1].dia[s1][p1] , aulas_numeradas, c,table_linha, cu);
				
				/* Calcular a função objetivo. */	
				fo_linha = calcula_fo(c, r, curr, dias, ppd, sal, DCxD, DCxS, CxDxP);
				
				/* Comparar com a função corrente. */
				if(fo_linha < *fo){
					
					/* Faz a troca. */	
					*fo = fo_linha;
					
					/* Altera a aula na tabela. */
					copiar_tabela_horario(table_linha,table,dias,sal, ppd);
					
					melhoria = 0;
					
				}else {
					
					/* Desfaz o movimento. */
					swap(d1, s1, p1, d, s, p, DCxD, DCxS, CxDxP, table_linha[d1].dia[s1][p1],table_linha[d].dia[s][p] , aulas_numeradas, c,table_linha, cu);
					
					melhoria++;
				}
			}
		}
	}
}		

/* Procedimento responsavel pela etapa de refinamento. */
void ILS(tabela *table, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu){
	
	tabela *table_linha = criar_tabela(dias, ppd, sal);
	tabela *table_duas_linhas = criar_tabela(dias, ppd, sal);
	int sem_melhoria = 0;
	int nivel = 1;
	int fo, fo_duas_linhas;
	int **DCxD_linha = alocar_dia(c->size(), dias);
	int **DCxS_linha = alocar_dia(c->size(), sal);
	int ***CxDxP_linha = alocar_vetor_3d(curr,dias,ppd);
	vector<movimentos> moves;
	
	/* Procedimento de busca local. */
	busca_local(table, dias, sal, ppd, curr, DCxD, DCxS, CxDxP, aulas_numeradas, AA, AI, c, r, cu);
	fo = calcula_fo(c, r, curr, dias, ppd, sal, DCxD, DCxS, CxDxP); 
	
	/* Copia tabela para uma tabela auxiliar. */
	copiar_tabela_horario(table,table_linha,dias,sal, ppd);
	//copiar_tabela_horario(table,table_duas_linhas,dias,sal, ppd);
	
	/* 
	    Copia matrizes auxiliares. Todas as operaçoes realizadas nesse trecho de codigo, 
		são feitas sobre as matrizes auxiliares criadas nessa função. 
		Isso se deve ao fato das matrizes auxiliares normais, refletirem sempre o melhor resultado no momento,
		e devido aos contantes movimentos de perturbação isso pode não acontecer, perdendo a referencia da melhor solução.
		
	*/
	
	//copia_matriz(DCxD, DCxD_linha, (int)c->size(), dias);
	//copia_matriz(DCxS, DCxS_linha, (int)c->size(), sal);
	//copia_matriz3d(CxDxP, CxDxP_linha, curr, dias, ppd);
	
	/* Perturbação e nova busca local. */
	while(sem_melhoria < iterSemMelhora){
		
		/* Perturba solução */
		perturbacao (table_linha, dias, sal, ppd, AA, AI, DCxD, DCxS, CxDxP, aulas_numeradas, c, r, cu, &moves, nivel);
		
		/* Copia tabela para uma tabela auxiliar. Refaz os movimentos da perturbação nas tabelas auxiliares.*/
		copiar_tabela_horario(table_linha, table_duas_linhas, dias, sal, ppd);
		copia_matriz(DCxD, DCxD_linha, (int)c->size(), dias);
		copia_matriz(DCxS, DCxS_linha, (int)c->size(), sal);
		copia_matriz3d(CxDxP, CxDxP_linha, curr, dias, ppd);
		
		//for(int x = 0; x < moves.size(); x++){
			
			/* Desfaz os movimentos feitos pela perturbação. */
		/*	if(moves.at(x).tipo_movimento == 0){
					
				move(moves.at(x).dia_origem, 
					 moves.at(x).sala_origem, 
					 moves.at(x).periodo_origem, 
					 moves.at(x).dia_destino,
					 moves.at(x).sala_destino, 
					 moves.at(x).periodo_destino,
					 DCxD_linha, DCxS_linha, CxDxP_linha,
					 table_duas_linhas[moves.at(x).dia_origem].dia[moves.at(x).sala_origem][moves.at(x).periodo_origem], 
					 aulas_numeradas, c, table_linha, cu);
					 
			}else {
				
				swap(moves.at(x).dia_origem, 
					 moves.at(x).sala_origem, 
					 moves.at(x).periodo_origem, 
					 moves.at(x).dia_destino,
					 moves.at(x).sala_destino, 
					 moves.at(x).periodo_destino,
					 DCxD_linha, DCxS_linha, CxDxP_linha,
					 table_duas_linhas[moves.at(x).dia_origem].dia[moves.at(x).sala_origem][moves.at(x).periodo_origem],
					 table_duas_linhas[moves.at(x).dia_destino].dia[moves.at(x).sala_destino][moves.at(x).periodo_destino],	
					 aulas_numeradas, c,table_linha, cu);
			}
		}*/
		
		/* Efetua Busca Local */
		busca_local(table_duas_linhas, dias, sal, ppd, curr, DCxD_linha, DCxS_linha, CxDxP_linha, aulas_numeradas, AA, AI, c, r, cu);
		fo_duas_linhas = calcula_fo(c, r, curr, dias, ppd, sal, DCxD_linha, DCxS_linha, CxDxP_linha); 
		
		
		
		/* Verifica se o ótimo local pós perturbação é melhor que o outro ótimo local ja encontrado anteriormente. */
		//if(fo_duas_linhas < (fo + ((iterSemMelhora - sem_melhoria) * vizinhanca))){
		if(fo_duas_linhas < fo){
			
			cout << "Mudou de "<< fo << " para " << fo_duas_linhas << endl; //getchar();
			
			/* Nova função obijetivo. */
			fo = fo_duas_linhas;
			
			/* Faz da solução corrente a nova solução. */
			copiar_tabela_horario(table_duas_linhas, table_linha, dias, sal , ppd);
			copia_matriz(DCxD_linha, DCxD, (int)c->size(), dias);
			copia_matriz(DCxS_linha, DCxS, (int)c->size(), sal);
			copia_matriz3d(CxDxP_linha, CxDxP, curr, dias, ppd);
			
			/* Volta para o primeiro nivel de busca */
			nivel = 1;
			
			//sem_melhoria = 0;
			
		}else{
			
			/* Incrementa o numero de iterações sem melhoria. */
			//sem_melhoria ++;
			
			/* Inverte o nivel de perturbação. */	
			nivel = nivel * -1;
			
			/* Como não melhorou, desfaz os movimentos. */
			for(int x = moves.size() -1; x >= 0; x--){
				
				/* Desfaz os movimentos feitos pela perturbação. */
				if(moves.at(x).tipo_movimento == 0){
						
					move(moves.at(x).dia_destino, 
					     moves.at(x).sala_destino, 
					     moves.at(x).periodo_destino, 
					     moves.at(x).dia_origem,
						 moves.at(x).sala_origem, 
						 moves.at(x).periodo_origem,
						 DCxD, DCxS, CxDxP,
						 table_linha[moves.at(x).dia_destino].dia[moves.at(x).sala_destino][moves.at(x).periodo_destino], 
						 aulas_numeradas, c, table_linha, cu);
						 
				}else {
					
					swap(moves.at(x).dia_origem,
						 moves.at(x).sala_origem, 
						 moves.at(x).periodo_origem, 
						 moves.at(x).dia_destino,
						 moves.at(x).sala_destino, 
						 moves.at(x).periodo_destino,
						 DCxD, DCxS, CxDxP,
						 table_linha[moves.at(x).dia_origem].dia[moves.at(x).sala_origem][moves.at(x).periodo_origem],
						 table_linha[moves.at(x).dia_destino].dia[moves.at(x).sala_destino][moves.at(x).periodo_destino],	
						 aulas_numeradas, c,table_linha, cu);
				}
			}
			
			/* Matrizes auxiliares, alem da perturbação, tem a possivel busca local. */
			//copiar_tabela_horario(table_linha,table_duas_linhas,dias,sal, ppd);
		}
		
		/* Apos ter desfeito os movimentos, apaga o vetor. */
		moves.clear();
		sem_melhoria++;
	}
	
	/* Atribui a melhor solução a table */
	copiar_tabela_horario(table_linha, table, dias, sal, ppd);
		
	/* Apaga matrizes auxiliares. */
	DCxD_linha = liberar_dia((int)c->size(), dias, DCxD_linha);
	DCxS_linha = liberar_dia((int)c->size(), sal, DCxS_linha);
	CxDxP_linha = liberar_vetor_3d(curr, dias, ppd, CxDxP_linha);
	encerrar(table_linha,dias,sal,ppd);
	encerrar(table_duas_linhas,dias,sal,ppd);
}

/* Gera k vizinhanças para uma aula. Retorna a melhor dessas vizinhanças. */
void gera_melhor_vizinho(tabela *table, tabela *table_linha, int d, int s, int p, int dias, int sal, int ppd, int **AA, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu){
	
	//srand( time( NULL));
	int k = 0, rf = 1;
	int d1, p1, s1, fo_linha;
	int fo_estrela = calcula_fo(c, r, (int)cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP); 
	
	int mv;
	int tentativas_movimento;
	
	/* Estrutura referente aos movimentos. */
	vector<movimentos> moves;
	movimentos movimento;
	
	copiar_tabela_horario(table, table_linha, dias, sal, ppd);
	
	//cout << "Entrou com fo = " << calcula_fo(c, r, cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP) << endl;
	
	/* Para cada vizinho, faz um movimento com a aula escolhida. Armazena a melhor solução. */	
	while(k < K){
		
		/* Seleciona aula para troca. Seja por move ou swap. */
		seleciona_aula_troca(table, &d, &s, &p, dias, sal, ppd, 0, -1, -1, -1);	
		
		/* Armazena parte do movimento. */
		movimento.dia_origem = d; 
		movimento.sala_origem = s; 
		movimento.periodo_origem = p;
		
		rf = 1;
		
		/* Sorteia qual movimento será usado. */
		mv = rand() % 10;
		tentativas_movimento = 0;
		
		/* MOVE */
		if(mv % 2 == 0){
			
			while(rf){
				
				/* Para que não fique perdido tentando encontrar troca viavel, delimita-se um numero maximo de tentativas. */
				if(tentativas_movimento == tentativaMaxima) break;
				
				/* Seleciona aula para troca. */
				seleciona_aula_troca(table_linha, &d1, &s1, &p1, dias, sal, ppd, 1, d, s, p);
					
				rf = verifica_RF02(table_linha,table_linha[d].dia[s][p]-1,d1,p1,sal,AA);
				
				/* Só verifica essa restrição, caso tenha atendido a restrição anterior. */
				if (rf == 0) rf = verifica_RF04(ppd, d1, p1, table_linha[d].dia[s][p]-1, AI);
				
				/* Incrementa a tentativa de movimentos. */
				tentativas_movimento++;
			}
			
			if(rf == 0){
				
				/* Faz o movimento. */
				move(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table_linha[d].dia[s][p], aulas_numeradas, c,table_linha,cu);
				
				/* Calcular a função objetivo. */	
				fo_linha = calcula_fo(c, r, (int)cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP);
				
				/* Comparar com a função corrente. */
				if(fo_linha < fo_estrela){
					
					cout << fo_linha << endl;
					/* Faz a troca. */	
					fo_estrela = fo_linha;
					
					/* Guarda o movimento. */
					movimento.tipo_movimento = 0; 
					movimento.dia_destino = d1; 
					movimento.sala_destino = s1;
					movimento.periodo_destino = p1; 
					movimento.fo = fo_estrela; 
					
					moves.push_back(movimento);
					
					/* Desfaz o movimento. */
					move(d1, s1, p1, d, s, p, DCxD, DCxS, CxDxP, table_linha[d1].dia[s1][p1], aulas_numeradas, c,table_linha, cu);
					k++;
				}else {
					
					/* Desfaz o movimento. */
					move(d1, s1, p1, d, s, p, DCxD, DCxS, CxDxP, table_linha[d1].dia[s1][p1], aulas_numeradas, c,table_linha, cu);
					k++;
				}
			}
		
		/* SWAP */	
		}else {
			
			while(rf) {
				
				/* Para que não fique perdido tentando encontrar troca viavel, delimita-se um numero maximo de tentativas. */
				if(tentativas_movimento == tentativaMaxima) break;
				
				/* Seleciona aula para troca. */
				seleciona_aula_troca(table_linha, &d1, &s1, &p1, dias, sal, ppd, 0, d, s, p);
					
				/* Verifica se a aula 1, pode ser alocada no lugar da aula 2 */
				rf = verifica_RF02(table_linha ,table_linha[d].dia[s][p]-1,d1,p1,sal,AA);
				if (rf == 0) rf = verifica_RF04(ppd, d1, p1, table_linha[d].dia[s][p]-1, AI);
				
				/* Verifica se a aula 2, pode ser alocada no lugar da aula 1 */
				if (rf == 0) rf = verifica_RF02(table_linha,table_linha[d1].dia[s1][p1]-1,d,p,sal,AA);
				if (rf == 0) rf = verifica_RF04(ppd, d, p, table_linha[d1].dia[s1][p1]-1, AI);
				
				/* Incrementa a tentativa de movimentos. */
				tentativas_movimento++;
			}
			
			if(rf == 0){
				
				/* Faz a troca das aulas. */
				swap(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table_linha[d].dia[s][p],table_linha[d1].dia[s1][p1], aulas_numeradas, c, table_linha, cu);
				
				/* Calcular a função objetivo. */	
				fo_linha = calcula_fo(c, r, (int)cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP);
				
				/* Comparar com a função corrente. */
				if(fo_linha < fo_estrela){
					
					cout << fo_linha << endl;
					
					/* Faz a troca. */	
					fo_estrela = fo_linha;
					
					movimento.tipo_movimento = 1;
					movimento.dia_destino = d1; 
					movimento.sala_destino = s1;
					movimento.periodo_destino = p1; 
					movimento.fo = fo_estrela; 
					
					moves.push_back(movimento);
					
					/* Desfaz o movimento. */
					//swap(d1, s1, p1, d, s, p, DCxD, DCxS, CxDxP, table_linha[d1].dia[s1][p1], table_linha[d].dia[s][p] , aulas_numeradas, c,table_linha, cu);
					swap(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table_linha[d].dia[s][p], table_linha[d1].dia[s1][p1] , aulas_numeradas, c,table_linha, cu);
					k++;
				}else {
					
					/* Desfaz o movimento. */
					//swap(d1, s1, p1, d, s, p, DCxD, DCxS, CxDxP, table_linha[d1].dia[s1][p1],table_linha[d].dia[s][p] , aulas_numeradas, c,table_linha, cu);
					swap(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table_linha[d].dia[s][p], table_linha[d1].dia[s1][p1] , aulas_numeradas, c,table_linha, cu);
					k++;
				}
			}	
		}
	}
	
	/* Refaz o movimento que gerou o melhor vizinho. */
	if((int ) moves.size() > 0)	{
		/* Ordena vetor de movimentos. */
		qsort(&moves[0], moves.size(), sizeof(movimentos), comparar_fo);
		
		/* Faz o melhor movimento encontrado. */
		if(moves.at(0).tipo_movimento == 0){
			cout << "Mudou com move " << endl;	
			move(moves.at(0).dia_origem,
				 moves.at(0).sala_origem, 
				 moves.at(0).periodo_origem, 
				 moves.at(0).dia_destino,
				 moves.at(0).sala_destino, 
				 moves.at(0).periodo_destino,
				 DCxD, DCxS, CxDxP,
				 table[moves.at(0).dia_origem].dia[moves.at(0).sala_origem][moves.at(0).periodo_origem], 
				 aulas_numeradas, c, table, cu);
		}else {
			
			cout << "Mudou com swap " << endl;
			
			swap(moves.at(0).dia_origem,
				 moves.at(0).sala_origem, 
				 moves.at(0).periodo_origem, 
				 moves.at(0).dia_destino,
				 moves.at(0).sala_destino, 
				 moves.at(0).periodo_destino,
				 DCxD, DCxS, CxDxP,
				 table[moves.at(0).dia_origem].dia[moves.at(0).sala_origem][moves.at(0).periodo_origem],
				 table[moves.at(0).dia_destino].dia[moves.at(0).sala_destino][moves.at(0).periodo_destino],	
				 aulas_numeradas, c, table, cu);
		}
	}
	
	//cout << "Saiu com fo = " << calcula_fo(c, r, cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP) << endl;
	
	//getchar();
	
	moves.clear();
}



/* Perturba a solução. */
void perturbacao (tabela *table, int dias, int sal, int ppd, int **AA, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, vector<movimentos> *moves, int nivel){
	
	//srand( time( NULL));
	
	int d, s, p;
	int d1, s1, p1;
	int movimento;
	int limite, rf;
	int tentativas_movimento;
	movimentos movimentos_feitos;
	
	nivel > 0 ? limite = 2 : limite = 3;
		
	for(int x = 0; x < limite; x++){
		
		rf = 1;
		
		/* Seleciona aula para troca. */
		seleciona_aula_troca(table, &d, &s, &p, dias, sal, ppd, 0, -1, -1, -1);	
		
		/* Seleciona o movimento. */
		movimento = rand() % 10;
		tentativas_movimento = 0;
		
		/* -MOVE- */
		if(movimento % 2 == 0){
			
			while(rf){
				
				/* Para que não fique perdido tentando encontrar troca viavel, delimita-se um numero maximo de tentativas. */
				if(tentativas_movimento == tentativaMaxima) break;
				
				/* Seleciona aula para troca. */
				seleciona_aula_troca(table, &d1, &s1, &p1, dias, sal, ppd, 1, d, s, p);
					
				rf = verifica_RF02(table,table[d].dia[s][p]-1,d1,p1,sal,AA);
				/* Só verifica essa restrição, caso tenha atendido a restrição anterior. */
				if (rf == 0) rf = verifica_RF04(ppd, d1, p1, table[d].dia[s][p]-1, AI);
				
				/* Incrementa a tentativa de movimentos. */
				tentativas_movimento++;
			}
	
			/* Faz o movimento. */
			if(rf == 0){ 
				
				movimentos_feitos.tipo_movimento = 0;
				movimentos_feitos.dia_origem = d;
				movimentos_feitos.sala_origem = s;
				movimentos_feitos.periodo_origem = p;
				movimentos_feitos.dia_destino = d1;
				movimentos_feitos.sala_destino = s1;
				movimentos_feitos.periodo_destino = p1;
				
				moves->push_back(movimentos_feitos);
				
				move(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table[d].dia[s][p], aulas_numeradas, c,table,cu);
			}	
			else x--;
		}
		
		/* -SWAP- */
		else{
			
			while(rf) {
				
				/* Para que não fique perdido tentando encontrar troca viavel, delimita-se um numero maximo de tentativas. */
				if(tentativas_movimento == tentativaMaxima) break;
				
				/* Seleciona aula para troca. */
				seleciona_aula_troca(table, &d1, &s1, &p1, dias, sal, ppd, 0, d, s, p);
					
				/* Verifica se a aula 1, pode ser alocada no lugar da aula 2 */
				rf = verifica_RF02(table,table[d].dia[s][p]-1,d1,p1,sal,AA);
				if (rf == 0) rf = verifica_RF04(ppd, d1, p1, table[d].dia[s][p]-1, AI);
				
				/* Verifica se a aula 2, pode ser alocada no lugar da aula 1 */
				if (rf == 0) rf = verifica_RF02(table,table[d1].dia[s1][p1]-1,d,p,sal,AA);
				if (rf == 0) rf = verifica_RF04(ppd, d, p, table[d1].dia[s1][p1]-1, AI);
				
				/* Incrementa a tentativa de movimentos. */
				tentativas_movimento++;
			}
			
			/* Faz a troca das aulas. */
			if (rf == 0){ 
				
				movimentos_feitos.tipo_movimento = 1;
				movimentos_feitos.dia_origem = d;
				movimentos_feitos.sala_origem = s;
				movimentos_feitos.periodo_origem = p;
				movimentos_feitos.dia_destino = d1;
				movimentos_feitos.sala_destino = s1;
				movimentos_feitos.periodo_destino = p1;
				
				moves->push_back(movimentos_feitos);
				
				swap(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table[d].dia[s][p],table[d1].dia[s1][p1] , aulas_numeradas, c,table, cu);
			}
			else x--;
		}
	}
}


/* Faz a busca local */
int busca_tabu(tabela *table, int dias, int sal, int ppd, int curr, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, int **AA, int **AI, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, int fo_star){
	
	/* Estrutura utilizada para agilizar a busca de um movimento na lista tabu. */
	vector<movimentos> lista_tabu;
	map<movimentos, bool> map_tabu;
	
	int fo, x = 0, melhor_iter = 0, d = 0, s = 0, p = 0;
	movimentos movimento;
	
	tabela *table_estrela_local = criar_tabela(dias, ppd, sal);
	
	copiar_tabela_horario(table, table_estrela_local, dias, sal, ppd);
	
	int fo_estrela_local = calcula_fo(c, r, (int)cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP); 
	
	/* Realiza a busca local. */
	while(x - melhor_iter < btmax){
	//for(int x = 0; x < btmax; x++){
		
		// A partir dessa escolha, encontra k vizinhos dessa solução. Fica com o melhor. 
		fo = gera_melhor_vizinho_tabu(table, dias, sal, ppd, AA, AI, DCxD, DCxS, CxDxP, aulas_numeradas, c, r, cu, &map_tabu, fo_star, &movimento);
		
		// Verifica se o melhor vizinho encontrado é melhor que f*
		if(fo < fo_estrela_local){
			
			cout << "-> "<< fo << endl;// getchar();
			
			copiar_tabela_horario(table, table_estrela_local, dias, sal, ppd);
			
			fo_estrela_local = fo;
			
			melhor_iter = x;
			
			// Adiciona o movimento que gerou a solução na lista, pois não é possivel voltar para ele.
			movimentos mov_aux;
			
			mov_aux.tipo_movimento = movimento.tipo_movimento;
			mov_aux.dia_origem = movimento.dia_destino;
			mov_aux.sala_origem = movimento.sala_destino;
			mov_aux.periodo_origem = movimento.periodo_destino;
			mov_aux.dia_destino = movimento.dia_origem;
			mov_aux.sala_destino = movimento.sala_origem;
			mov_aux.periodo_destino = movimento.periodo_origem;
			mov_aux.aula_origem = movimento.aula_destino;
			mov_aux.aula_destino = movimento.aula_origem;
			
			/* Movimento de piora, é adicionado na lista tabu. */
			if((int)lista_tabu.size() == btsize){
				
				/* Remove elemento da lista e do map. */
				remove_elemento_lista_tabu(&lista_tabu, &map_tabu);
				
				/* Adiciona o item no inicio da fila. Adiciona o elemento no map. */
				adiciona_elemento_lista_tabu(&lista_tabu, &map_tabu, mov_aux);
				
			}else {
				
				/* Adiciona movimento de piora na lista tabu e no mapa auxiliar. Por se tratar de uma fila, insere no inicio. */
				adiciona_elemento_lista_tabu(&lista_tabu, &map_tabu, mov_aux);
			
			}
			
		// Se não, adiciona o movimento na lista tabu.
		}else {
			
			/* Movimento de piora, é adicionado na lista tabu. */
			if((int)lista_tabu.size() == btsize){
				
				/* Remove elemento da lista e do map. */
				remove_elemento_lista_tabu(&lista_tabu, &map_tabu);
				
				/* Adiciona o item no inicio da fila. Adiciona o elemento no map. */
				adiciona_elemento_lista_tabu(&lista_tabu, &map_tabu, movimento);
				
			}else {
				
				/* Adiciona movimento de piora na lista tabu e no mapa auxiliar. Por se tratar de uma fila, insere no inicio. */
				adiciona_elemento_lista_tabu(&lista_tabu, &map_tabu, movimento);
			
			}
		}
		
		x++;
	}
	
	copiar_tabela_horario(table_estrela_local, table, dias, sal, ppd);
	encerrar(table_estrela_local, dias, sal, ppd);
	lista_tabu.clear();
	map_tabu.clear();
	
	return fo_estrela_local;
}

/* Escolhe o melhor vizinho. Os vizinhos que piorem a solução são mandados para a lista tabu */
	int gera_melhor_vizinho_tabu(tabela *table, int dias, int sal, int ppd, int **AA, int **AI, int **DCxD, int **DCxS, int ***CxDxP, vector<int> *aulas_numeradas, vector<Courses> *c, vector<Rooms> *r, vector<Curricula> *cu, map<movimentos, bool> *mt, int fo_star, movimentos *mvt){
	
	int k = 0, rf = 1, fo_estrela = calcula_fo(c, r, (int)cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP); 
	int d, p, s, d1, p1, s1, fo_linha, mv;
	
	vector<movimentos> moves;
	movimentos movimento;
	
	/* Para cada vizinho, faz um movimento com a aula escolhida. Armazena a melhor solução. */	
	while(k < K){
		
		/* Seleciona aula para troca. Seja por move ou swap. */
		seleciona_aula_troca(table, &d, &s, &p, dias, sal, ppd, 0, -1, -1, -1);	
		
		/* Armazena parte do movimento. */
		movimento.dia_origem = d; 
		movimento.sala_origem = s; 
		movimento.periodo_origem = p;
		/* Armazena parte do movimento. */
		
		/* Sorteia qual movimento será usado. */
		mv = rand() % 10;
		
		rf = 1;
		
		/* MOVE */
		if(mv % 2 == 0){
			
			/* Seleciona aula para troca. */
			rf = seleciona_aula_troca_tabu(table, &d1, &s1, &p1, 
										   dias, sal, ppd, 1, d, s, p, 
										   0, mt, AA, AI, DCxD, DCxS, 
										   CxDxP, cu, c, aulas_numeradas, 
										   r, fo_star, fo_estrela);
			
			//cout << "d " << d << " s" << s << " p" << p << " d1" << d1 << " s1" << s1 << " p1" << p1 << endl;
					
			if(rf == 0){
				
				movimento.dia_destino = d1; 
				movimento.sala_destino = s1;
				movimento.periodo_destino = p1; 
					
				/* Calcular a função objetivo. */	
				fo_linha = calcula_fo(c, r, (int)cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP);
				
				movimento.aula_origem = table[d].dia[s][p];
				movimento.aula_destino = table[d1].dia[s1][p1]; 
				movimento.tipo_movimento = 0; 
				movimento.fo = fo_linha; 
				
				moves.push_back(movimento);
				
				/* Desfaz o movimento. */
				move(d1, s1, p1, d, s, p, DCxD, DCxS, CxDxP, table[d1].dia[s1][p1], aulas_numeradas, c,table, cu);
				k++;
			}
		
		/* SWAP */	
		}else {
			
			/* Seleciona aula para troca e faz a troca caso seja possivel. */
			rf = seleciona_aula_troca_tabu(table, &d1, &s1, &p1, 
										   dias, sal, ppd, 0, d, s, p, 
										   1, mt, AA, AI, DCxD, DCxS, CxDxP, cu, c, 
										   aulas_numeradas, r, fo_star, fo_estrela);
										   
			//cout << "d " << d << " s" << s << " p" << p << " d1" << d1 << " s1" << s1 << " p1" << p1 << endl;							   
			
			if(rf == 0){
				
				movimento.dia_destino = d1; 
				movimento.sala_destino = s1;
				movimento.periodo_destino = p1; 
					
				/* Calcular a função objetivo. */	
				fo_linha = calcula_fo(c, r, (int)cu->size(), dias, ppd, sal, DCxD, DCxS, CxDxP);
				
				
				movimento.tipo_movimento = 1;
				movimento.aula_origem = table[d].dia[s][p];
				movimento.aula_destino = table[d1].dia[s1][p1]; 
				movimento.fo = fo_linha; 
				
				moves.push_back(movimento);
					
				/* Desfaz o movimento. */
				swap(d, s, p, d1, s1, p1, DCxD, DCxS, CxDxP, table[d].dia[s][p], table[d1].dia[s1][p1] , aulas_numeradas, c,table, cu);
				k++;
			}	
		}
		
		//getchar();
	}
	
	//getchar();
	
	/* Refaz o movimento que gerou o melhor vizinho. */
	if((int ) moves.size() > 0)	{
		/* Ordena vetor de movimentos. */
		qsort(&moves[0], moves.size(), sizeof(movimentos), comparar_fo);
		
		/* Faz o melhor movimento encontrado. */
		if(moves.at(0).tipo_movimento == 0){
			
			move(moves.at(0).dia_origem,
				 moves.at(0).sala_origem, 
				 moves.at(0).periodo_origem, 
				 moves.at(0).dia_destino,
				 moves.at(0).sala_destino, 
				 moves.at(0).periodo_destino,
				 DCxD, DCxS, CxDxP,
				 table[moves.at(0).dia_origem].dia[moves.at(0).sala_origem][moves.at(0).periodo_origem], 
				 aulas_numeradas, c, table, cu);
		}else {
			
			swap(moves.at(0).dia_origem,
				 moves.at(0).sala_origem, 
				 moves.at(0).periodo_origem, 
				 moves.at(0).dia_destino,
				 moves.at(0).sala_destino, 
				 moves.at(0).periodo_destino,
				 DCxD, DCxS, CxDxP,
				 table[moves.at(0).dia_origem].dia[moves.at(0).sala_origem][moves.at(0).periodo_origem],
				 table[moves.at(0).dia_destino].dia[moves.at(0).sala_destino][moves.at(0).periodo_destino],	
				 aulas_numeradas, c, table, cu);
		}
		
		fo_estrela = moves.at(0).fo;
		*mvt = moves.at(0);
	}
	
	moves.clear();
	
	return fo_estrela;	
}	

/* --------- FIM heuristicas.cpp ------------- */
