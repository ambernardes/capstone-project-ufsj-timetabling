#include"Courses.h"

using namespace std;

Courses::Courses(){

}

void Courses::imprimir() {
	cout << "Curso: " << curso << " ----> ";
	cout << "Professor: " << professor << " - ";
	cout << "Qtd aulas: " << quantidade_aulas << " - diasMin.: " << dias_minimos << " - qtdAlunos: " << quantidade_alunos << endl;
}


