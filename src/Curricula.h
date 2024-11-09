#ifndef CURRICULA
#define CURRICULA
#include<vector>



class Curricula {
	public:
		/* Construtor. */
		Curricula();
		
		/* Variaveis. Tudo publico mesmo. */
		char nome[500];							//Armazena o nome da sala de aula.
		int quantidade_materias;				//Armazena a quantidade de materias desse curriculo.
		std::vector<int> lista_cursos;   		//Armazena a lista de disciplinas de um curriculo.
 
};

#endif
