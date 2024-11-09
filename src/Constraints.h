#ifndef RESTRICAO
#define RESTRICAO


class Constraints {
	public:
		/* Construtor. */
		Constraints();
		
		/* Variaveis. Tudo publico mesmo. */
		char curso[500];				//Armazena o nome do curso.
		int dia;					//Armazena o dia em que essa aula não pode acontecer.
		int periodo;				//Armazena o periodo do dia em que essa aula não pode acontecer.
 
};

#endif
