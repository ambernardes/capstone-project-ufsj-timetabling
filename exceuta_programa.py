#Executa instancias

import os

#Main
def main():
	
	limite_superior = 2
	
	os.system("make clean; make")
	
	#Percorre a lista de instancias. 20 vezes para cada instancia. 
	for i in range(2,limite_superior + 1):
		
		for j in range(1, 2):
			
			print "Comp" + str(i) + " Iteracao " + str(j)
			
			if(i < 10):
				instancia = "instancia/comp0"+str(i)+".ctt"
			else:
				instancia = "instancia/comp"+str(i)+".ctt"
			
			#instancia = "instancia/toy.ctt"
			
			saida = "saidas/saidaComp"+str(i)+"_"+str(j)
			
			#os.system("./timetable " + str(instancia) + " " + str(0.08) + " " + str(saida) + " >> Saidas.txt")
			os.system("./timetable " + str(instancia) + " " + str(0.08) + " " + str(saida))
			
	return


if __name__ == '__main__':
	main()
