#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

//ADICIONAR PESSOA E SEUS DADOS 		//FEITO
//ADICIONAR RELACAO ENTRE PESSOAS		//AQUI FUDEO
//EXIBIR ASCENDENTES E ANTESCEDENTES
//EXIBIR ARVORE A PARTIR DE UMA PESSOA

struct Pessoa{
	string name;
	string nasc;
	char genero;
};
/*
vector<Pessoa> criaPessoa(vector<Pessoa> pessoas) {
	Pessoa p;
	cout << "Informe o nome da pessoa: ";
	getline(cin >> ws, p.name); //GETLINE() SUBSTITUI O CIN >> PQ ESSA MERDA NAO ACEITA SPACEBAR; CIN >> WS PRA CONSERTAR UM OUTRO PROBLEMA LA FODASE
	cout << "Informe a data de nascimento da pessoa (XX/XX/XXXX): ";
	cin >> p.nasc;
	cout << "Informe o genero da pessoa (M/F): ";
	cin >> p.genero;
	pessoas.push_back(p);

	return pessoas;
}
*/

vector<Pessoa> criaPessoa(vector<Pessoa>& pessoas) {	//FORMA MAIS EFICIENTE
	Pessoa p;											//PASSAR POR REFERENCIA
	cout << "Informe o nome da pessoa: ";
	cin >> p.name;
	cout << "Informe a data de nascimento da pessoa: ";
	cin >> p.nasc;
	cout << "Informe o genero da pessoa: ";
	cin >> p.genero;
	pessoas.push_back(p);

	return pessoas;
}
/*
//BUSCA BINARIA REALMENTE NAO EH O MAIS BACANA AQUI, PRA NAO DIZER QUE NEM SEQUER FUNCIONA

void swap(int * a, int * b) {
	int temp; 				//VARIAVEL TEMPORARIA
	temp = (*a);			//TEMPORARIO SEGURA O VALOR ORIGINAL 
	(*a) = (*b);			//VALOR ORIGINAL VIRA O VALOR FINAL
	(*b) = temp;			//VALOR FINAL PEGA O VALOR ORIGINAL DO TEMPORARIO
}

void quickSort(const vector<Pessoa>& pessoas, int inicio, int fim) {
        if(inicio > fim) {
            return;
        }
        string pivo = pessoas[fim];
        int pontEsq = inicio;
        int pontDir = fim;
        
        while(pontEsq < pontDir) {
            while(pessoas[pontEsq] <= pivo) {
                pontEsq++;
            }
            while(pessoas[pontDir] >= pivo) {
                pontDir--;
            }
            swap(pessoas[pontEsq], pessoas[pontDir]);  
        }
        swap(pessoas[pontEsq], fim);
        
        quickSort(nomes, inicio, pontEsq - 1);
        quickSort(nomes, pontEsq + 1, fim);    
    }

//BUSCA BINARIA QUE RETORNA O INDICE DA PESSOA BUSCADA
int buscaBinaria(const vector<Pessoa>& pessoas, int inicio, int fim, const string& chave) {
	if(inicio > fim) {
		return -1; 										//ELEMENTO NAO ENCONTRADO
	} else {
		int pivo = (inicio + fim) / 2;
		if(pessoas[pivo] == chave) {
			return pivo;								//ELEMENTO ENCONTRADO
		} else {
			if(pessoas[pivo] < chave) {
				return buscaBinaria(pessoas, pivo + 1, fim, chave);		//ESQUERDA
			} else {
				return buscaBinaria(pessoas, inicio, pivo - 1, chave);	//DIREITA
			}
		}
	}
}

//BUSCA BINARIA QUE RETORNA TRUE OR FALSE, PRA FAZER A BUSCA AQUELA
bool buscaBinariaBool(const vector<Pessoa>& pessoas, int inicio, int fim, const string& chave) {
	if(inicio > fim) {
		return false; 										//ELEMENTO NAO ENCONTRADO
	} else {
		int pivo = (inicio + fim) / 2;
		if(pessoas[pivo] == chave) {
			return true;									//ELEMENTO ENCONTRADO
		} else {
			if(pessoas[pivo] < chave) {
				return buscaBinaria(pessoas, pivo + 1, fim, chave);		//ESQUERDA
			} else {
				return buscaBinaria(pessoas, inicio, pivo - 1, chave);	//DIREITA
			}
		}
	}
}
*/
int main() {
	/*int comand;
	cin >> comand;
	
	while(comand != 0){
		//AQUI DENTRO VAI TODO O EXECUTAVEL
		//O SWITCH E TUDO MAIS
		//UMA VEZ QUE O INPUT DE 0
		//ENCERRA O PROGRAMA
	}
	*/
	vector<Pessoa> pessoas;
	/*
	pessoas = criaPessoa(pessoas);
	pessoas = criaPessoa(pessoas);
	//	criaPessoa(pessoas);		//PASSAGEM POR REFERENCIA FUNCIONA AQUI
	*/
	
	
	criaPessoa(pessoas);
	criaPessoa(pessoas);
	criaPessoa(pessoas);
	
	ofstream arquivo("teste2.txt");
	arquivo << "Ordem desordenada: " << "\n";
	for (const auto& pessoa : pessoas) {	//NAO SEI DIZER; SO PRA TESTE
		arquivo << pessoa.name << ", " << pessoa.nasc << ", " << pessoa.genero << "\n";
	}
	
	quickSort(pessoas, 0, pessoas.size() - 1);
	
	arquivo << "Ordem ordenada: " << "\n";
	for (const auto& pessoa : pessoas) {	//NAO SEI DIZER; SO PRA TESTE
		arquivo << pessoa.name << ", " << pessoa.nasc << ", " << pessoa.genero << "\n";
	}
	/*
	string chave = pessoas.name
	buscaBinaria(pessoas, 0, pessoas.size() - 1, )
	*/
	arquivo.close();
	
	return 0;
}