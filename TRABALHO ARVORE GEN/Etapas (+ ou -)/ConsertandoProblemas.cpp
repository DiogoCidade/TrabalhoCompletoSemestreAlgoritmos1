#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

//ADICIONAR PESSOA E SEUS DADOS 		//FEITO
//ADICIONAR RELACAO ENTRE PESSOAS		//FEITO
//EXIBIR ASCENDENTES E ANTESCEDENTES
//EXIBIR ARVORE A PARTIR DE UMA PESSOA	//MEIO FEITO, PRECISA IR PRO DOC


//NOMEAR O ARQUIVO DE SAIDA
void nomearArq(string& nomeArq){ 	//PASSAGEM POR REFERENCIA DO ENDERECO &
	cout << "De um nome ao arquivo: ";
	cin >> nomeArq;
	nomeArq += ".txt";
}
//MENU DE OPCOES
void menuOp() {
	cout << "\n" << "1 - Inserir nova pessoa" << "\n";
	cout << "2 - Adicionar relacao de parentesco" << "\n"; //ACHO QUE ISSO VAI SER INSTANTANEAMENTE APOS INSERIR PESSOA NOVA
	cout << "3 - Buscar pessoa" << "\n";
	cout << "4 - Imprimir arvore a partir de uma pessoa" << "\n";
	cout << "5 - Imprimir toda a arvore de uma pessoa (anscendentes e descendentes)" << "\n";
	cout << "0 - Sair / Encerrar" << "\n\n";
}

void retornar() {
	cout << "\n" << "9 - Retornar ao menu anterior" << "\n\n";
}

//PESSOA CONTENDO INFORMACOES DE NOME, DATA DE NASCIMENTO E GENERO
struct Pessoa{
	string name;
	string nasc;
	char genero;
	//CADA PESSOA DENTRO DO VECTOR TAMBEM TEM 
	//SEU PROPRIO VECTOR PARA ARMAZENAR DUPLICADAMENTE SEU FILHOS
	//SHARED_PTR EH UM PONTEIRO INTELIGENTE, 
	vector<shared_ptr<Pessoa>> ascendentes; 
	vector<shared_ptr<Pessoa>> descendentes; 
};

//LEITURA DE PESSOAS
shared_ptr<Pessoa> criaPessoa(vector<shared_ptr<Pessoa>>& pessoas) {	//PASSAGEM POR REFERENCIA DO ENDERECO &
	auto p = make_shared<Pessoa>();											
	cout << "\n" << "Informe o nome da pessoa: ";
	getline(cin >> ws, p->name); //GETLINE() SUBSTITUI O CIN >> PQ ESSA MERDA NAO ACEITA SPACEBAR; CIN >> WS PRA CONSERTAR UM OUTRO PROBLEMA LA FODASE
	cout << "Informe a data de nascimento da pessoa (XX/XX/XXXX): ";
	cin >> p->nasc;
	cout << "Informe o genero da pessoa (M/F): ";
	cin >> p->genero;
	pessoas.push_back(p);

	cout << "\n" << "Pessoa inserida com sucesso!" << "\n\n";

	return p;					//RETORNA O SMART POINTER AO INVES DO VECTOR INTEIRO
	//PRECISA FAZER VERIFICACAO PRA NAO INSERIR REPETIDO
}

//BUSCA LINEAR QUE RETORNA O INDICE DA PESSOA BUSCADA NO VECTOR
int buscaPessoaIndice(vector<shared_ptr<Pessoa>>& pessoas, string chave) {	
	int i;
	for (i = 0; i < pessoas.size(); i++) {
        if (pessoas[i]->name == chave) {		//SE ACHAR A PESSOA PROCURADA
            return i; 							//RETORNA O INDICE QUE O PONTEIRO APONTA
        }
	}/*
    if (i < pessoas.size()) {
        cout << "\n" << "Pessoa encontrada no indice: " << i << "\n\n";
    } else {
        cout << "\n" << "Pessoa nao encontrada" << "\n\n";
    }*/
    return -1;									//SE NAO ENCONTRAR RETORNA -1
}

//BUSCA LINEAR QUE RETORNA TRUE OR FALSE
void buscaPessoa(vector<shared_ptr<Pessoa>>& pessoas, string chave) {	
	int i;
	for (i = 0; i < pessoas.size(); i++) {
        if (chave == pessoas[i]->name) {
            break; //ENCERRA ANTECIPADAMENTE O FOR
        }
	}
    if (i < pessoas.size()) {
        cout << "\n" << "Pessoa ja cadastrada" << "\n\n";
    } else {
        cout << "\n" << "Pessoa nao cadastrada" << "\n\n";
    }
}

//SISTEMA DE ADICAO DE RELACIONAMENTO, ONDE CADA PESSOA TEM UM VECTOR DE ASCENDENTES E DESCENDENTES
//O USER INFORMA O PAI E O FILHO, E O PONTEIRO APONTA A PESSOA FILHO PRA DENTRO DO VECTOR DESCENDENTES DO PAI
auto adicionaRelacao(vector<shared_ptr<Pessoa>>& pessoas) {			//PRECISAMOS ADICIONAR VERIFICACAO DE EXISTENCIA DE PESSOAS
	cout << "\n" << "Informe o pai/mae: ";				//E TALVEZ UM ALERTA DE PESSOAS SEM RELACAO ADICIONADA
	string pai;											//MO TRETA
	cin >> pai;
	int paiI = buscaPessoaIndice(pessoas, pai);
	if (paiI == -1) { 
		cout << "Pessoa nao encontrada." << "\n"; 
		return; 
	}
	
	cout << "Informe o filho: ";
	string filho;
	cin >> filho;
	int filhoI = buscaPessoaIndice(pessoas, filho);
	if (filhoI == -1) { 
		cout << "Pessoa nao encontrada." << "\n"; 
		return; 
	}
	
	pessoas[paiI]->descendentes.push_back(pessoas[filhoI]);
	
//COM O MESMO PAI E O FILHO INFORMADO PELO USER, O PONTEIRO APONTA A PESSOA PAI PRA DENTRO DO VECTOR ASCENDENTES DO FILHO
	pessoas[filhoI]->ascendentes.push_back(pessoas[paiI]);
	/*
	for (const auto& pessoa : pessoas[paiI].filhos) {	//IMPRIME TABELA COM AS PESSOAS CADASTRADAS; SO PRA TESTE
		cout << pessoa.name << ", " << pessoa.nasc << ", " << pessoa.genero << "\n";
	}*/
	cout << "\n" << "Relacao adicionada com sucesso!" << "\n";
}

string nivelGeracoes(int geracao) {
	string nivel = "_";
	for(int i = 0; i < geracao; i++) {
		nivel += "__";
	}
	return nivel;
}

void imprimirArvore(shared_ptr<Pessoa> pessoa, int geracao, ofstream& arquivo) {
	if(!pessoa) return;
	
	arquivo << "G" << geracao + 1 << nivelGeracoes(geracao) << pessoa->name << "\n";
	for(auto& filho : pessoa->descendentes) {
		imprimirArvore(filho, geracao + 1, arquivo);
	}
}

int main() {
	vector<shared_ptr<Pessoa>> pessoas;
	
	//NOMEAR E DEFINIR O ARQUIVO DE SAIDA
	string nomeArq = "";
	nomearArq(nomeArq);
	ofstream arquivo(nomeArq);
	
	//VARIAVEL QUE O USUARIO VAI DEFINIR PARA NAVEGAR PELO MENU
	int comand = 1;
	
	//INSERIR PESSOA INICIAL
	cout << "Insira uma pessoa" << "\n";
	criaPessoa(pessoas);
	
	while(comand != 0){
		//AQUI DENTRO VAI TODO O EXECUTAVEL, O SWITCH E TUDO MAIS
		//UMA VEZ QUE O INPUT DE 0 ENCERRA O PROGRAMA
		
		menuOp(); //O MENU DE OPCOES
		cin >> comand;
		switch(comand) {
			case 0: {
				break;
			}
			case 1: {
				criaPessoa(pessoas);
				break;	//PASSAGEM POR REFERENCIA FUNCIONA AQUI
			}
			case 2: {
				adicionaRelacao(pessoas);
				break;
			}
			case 3: {
				retornar();
				cout << "\n" << "Buscar pessoa: ";
    			string chave;
    			cin >> chave;
    			if(chave == "9"){
					break;
				}
				buscaPessoa(pessoas, chave);
				break;
			}
			case 4: {
				retornar();
				cout << "Informe o anscestral: ";		
				string ancestral;						
				cin >> ancestral;
				if(ancestral == "9"){
					break;
				}
				int ancestralI = buscaPessoaIndice(pessoas, ancestral);
				if (ancestralI != -1) {
					imprimirArvore(pessoas[ancestralI], 0, arquivo);
				} else {
					cout << "Pessoa nao encontrada." << "\n";
				}
				break;
			}
		}
	}
	/*
	for (const auto& pessoa : pessoas) {	//IMPRIME TABELA COM AS PESSOAS CADASTRADAS; SO PRA TESTE
		arquivo << pessoa->name << ", " << pessoa->nasc << ", " << pessoa->genero << "\n";
	}
	*/
	arquivo.close();
	
	return 0;
}