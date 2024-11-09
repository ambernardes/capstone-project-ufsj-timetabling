#ifndef CURSOS
#define CURSOS

#include <iostream>

class Courses {
	public:
		/* Construtor. */
		Courses();
		
		/* Variaveis. Tudo publico mesmo. */
		char curso[500];        //Armazena o nome do curso/materia.
		char professor[500]; 	 //Armazena o nome do professor. 
		int quantidade_aulas;   //Quantidade de aulas que deve ter na semana.
		int dias_minimos;       //Quantidade minima de dias que essa materia deve estar.
		int quantidade_alunos;  //Quantidade de alunos que assistem essa materia.
		int curricula;          //Id do curriculo que a disciplina pertence.
	void imprimir();
};

#endif
