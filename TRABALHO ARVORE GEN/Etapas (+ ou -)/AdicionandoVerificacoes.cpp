#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
using namespace std;

//ADICIONAR PESSOA E SEUS DADOS 		//FEITO
//ADICIONAR RELACAO ENTRE PESSOAS		//FEITO
//EXIBIR ASCENDENTES E ANTESCEDENTES	//FEITO
//EXIBIR ARVORE A PARTIR DE UMA PESSOA	//FEITO
//EXIBIR GRAU DE PARENTESCO ENTRE DOIS CABOCLO
//EXIBIR CONTAGEM DE DESCENDENTES		//FEITO

//AGORA SERIA BEM BACANA FAZER AS COISA BONITO

//NOMEAR O ARQUIVO DE SAIDA
void nomearArq(string& nomeArq){ 	//PASSAGEM POR REFERENCIA DO ENDERECO &
	cout << "De um nome ao arquivo: ";
	getline(cin >> ws, nomeArq);
	nomeArq += ".txt";
}
//MENU DE OPCOES
void menuOp() {
	cout << "\n" << "1 - Inserir nova pessoa" << "\n";
	cout << "2 - Adicionar relacao de parentesco" << "\n"; //ACHO QUE ISSO VAI SER INSTANTANEAMENTE APOS INSERIR PESSOA NOVA
	cout << "3 - Buscar pessoa" << "\n";
	cout << "4 - Imprimir arvore a partir de uma pessoa" << "\n";
	cout << "5 - Imprimir toda a arvore de uma pessoa (anscendentes e descendentes)" << "\n";
	cout << "6 - Imprimir grau de parentesco entre duas pessoas" << "\n";
	cout << "0 - Sair / Encerrar" << "\n\n";
}

void menuRetornar() {
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
											
	cout << "Informe o nome da pessoa: ";
	getline(cin >> ws, p->name); 
	if(p->name == "9"){
		cout << "Funcao cancelada!" << "\n";
		return nullptr;
	}
	
	cout << "Informe a data de nascimento da pessoa (XX/XX/XXXX): ";
	cin >> p->nasc;
	if(p->nasc == "9"){
		cout << "Funcao cancelada!" << "\n";
		return nullptr;
	}
	
	cout << "Informe o genero da pessoa (M/F): ";
	cin >> p->genero;
	if(p->genero == '9'){
		cout << "Funcao cancelada!" << "\n";
		return nullptr;
	}
	
	//AQUI TEM QUE TER VERIFICACAO SE PESSOA INSERIDA NAO EH REPETIDA
	pessoas.push_back(p);

	cout << "\n" << "Pessoa inserida com sucesso!" << "\n";

	return p;					//RETORNA O SMART POINTER AO INVES DO VECTOR INTEIRO
	//PRECISA FAZER VERIFICACAO PRA NAO INSERIR REPETIDO
}

void infoPessoa(shared_ptr<Pessoa> pessoa, ofstream &arquivo) {
	arquivo << "\n" << pessoa->name << ", " << pessoa->nasc << ", " << pessoa->genero << "\n\n";
}

//BUSCA LINEAR QUE RETORNA O INDICE DA PESSOA BUSCADA NO VECTOR
int buscaPessoaIndice(vector<shared_ptr<Pessoa>>& pessoas, string chave) {	
	int i;
	for (i = 0; i < pessoas.size(); i++) {
        if (pessoas[i]->name == chave) {		//SE ACHAR A PESSOA PROCURADA
            return i; 							//RETORNA O INDICE QUE O PONTEIRO APONTA
        }
	}
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
        cout << "\n" << pessoas[i]->name << ", " << pessoas[i]->nasc << ", " << pessoas[i]->genero << "\n"; 
    } else {
        cout << "\n" << "Pessoa nao encontrada" << "\n";
    }
}

//SISTEMA DE ADICAO DE RELACIONAMENTO, ONDE CADA PESSOA TEM UM VECTOR DE ASCENDENTES E DESCENDENTES
//O USER INFORMA O PAI E O FILHO, E O PONTEIRO APONTA A PESSOA FILHO PRA DENTRO DO VECTOR DESCENDENTES DO PAI
auto adicionaRelacao(vector<shared_ptr<Pessoa>>& pessoas) {	
	cout << "Informe o pai/mae: ";				
	string pai;											
	cin >> pai;
	if(pai == "9"){
		cout << "Funcao cancelada!" << "\n";
		return;
	}
	int paiI = buscaPessoaIndice(pessoas, pai);
	if (paiI == -1) { 
		cout << "Pessoa nao encontrada." << "\n";
		menuRetornar();
		adicionaRelacao(pessoas);
		return; 
	}
	
	cout << "Informe o filho: ";
	string filho;
	cin >> filho;
	if(filho == "9"){
		cout << "Funcao cancelada!" << "\n";
		return;
	}
	int filhoI = buscaPessoaIndice(pessoas, filho);
	if (filhoI == -1) { 
		cout << "Pessoa nao encontrada." << "\n"; 
		menuRetornar();
		adicionaRelacao(pessoas);
		return; 
	}
	
	
	//AQUI TEM QUE TER VERIFICACAO SE RELACAO CRIADA JA NAO EXISTE
	pessoas[paiI]->descendentes.push_back(pessoas[filhoI]);
	
	//COM O MESMO PAI E O FILHO INFORMADO PELO USER, O PONTEIRO APONTA A PESSOA PAI PRA DENTRO DO VECTOR ASCENDENTES DO FILHO
	pessoas[filhoI]->ascendentes.push_back(pessoas[paiI]);
	
	cout << "\n" << "Relacao adicionada com sucesso!" << "\n";
}

struct NivelGeracao {
    string espaco;
    string nivel;
};

NivelGeracao nivelGeracoes(int geracao) {
    NivelGeracao n;
    n.espaco = " ";
    n.nivel = "+-";
    for (int i = 0; i < geracao; i++) {
        n.espaco += "  ";
        n.nivel += "-";
    }
    return n;
}

//IMPRESSAO DE UMA PESSOA E TODOS SEUS DESCENDENTES EM FORMATO DE ARVORE
void imprimirArvoreDesc(shared_ptr<Pessoa> pessoa, int geracao, ofstream& arquivo) {
	if(!pessoa) return;
	//IMPRIME A PESSOA E PASSA PARA A PROXIMA PESSOA NO SEU VECTOR DESCENDENTES (LACO FOR)
	//FUNCAO RECURSIVA, CHAMA A SI MESMO, CRIANDO UM CICLO, AO FINAL, RETORNA, RETORNA, RETORNA, E ENCERRA A FUNCAO
	NivelGeracao nivelGer = nivelGeracoes(geracao);
	arquivo << "G" << geracao + 1 << ":" << nivelGer.espaco << nivelGer.nivel << pessoa->name << "\n";
	for(auto& filho : pessoa->descendentes) {
		imprimirArvoreDesc(filho, geracao + 1, arquivo);
	}
}

//IMPRESSAO DE UMA PESSOA E TODOS SEUS ASCENDENTES EM FORMATO DE ARVORE, FUNCIONAMENTO IGUAL A ANTERIOR
void imprimirArvoreAsc(shared_ptr<Pessoa> pessoa, int geracao, ofstream& arquivo) {
	if(!pessoa) return;
	//DIFERENCA CONSISTE NA ORDEM: VAI PASSANDO PARA A PROXIMA PESSOA NO SEU VECTOR ASCENDENTES (LACO FOR)
	//FUNCAO RECURSIVA, CHAMA A SI MESMO, CRIANDO UM CICLO, NA ULTIMA PESSOA, IMPRIME E COMECA A RETORNAR
	//IMPRIMINDO TODOS OS ANTERIORES, E ENTAO ENCERRA 
	for(auto& filho : pessoa->ascendentes) {
		imprimirArvoreAsc(filho, geracao + 1, arquivo);
	}
	if(geracao != 0) {
		arquivo << "G" << geracao + 1 << " " << nivelGeracoes(geracao).espaco << nivelGeracoes(geracao).nivel << pessoa->name << "\n";		
	}
}

// conta todos os descendentes da pessoa passada (recursivo, com controle de visitados)
int contarDescendentes(shared_ptr<Pessoa> pessoa, unordered_set<string>& visit, int total) {
	if(!pessoa) return 0;
	for (auto& filho : pessoa->descendentes) {
		// se o nome do filho ainda nao foi visitado, conta e explora seus descendentes
		if (visit.find(filho->name) == visit.end()) {
			visit.insert(filho->name);         // marca como visitado
			total++;                        // conta este filho
			total =+ contarDescendentes(filho, visit, total); // conta descendentes recursivamente
		}
	}
	return total;
}

// conta todos os ascendentes da pessoa passada (recursivo, com controle de visitados)
int contarAscendentes(shared_ptr<Pessoa> pessoa, unordered_set<string>& visit, int total) {
	if(!pessoa) return 0;

	for (auto& avo : pessoa->ascendentes) {
		// se o nome do ascendente ainda nao foi visitado, conta e explora seus ascendentes
		if (visit.find(avo->name) == visit.end()) {
			visit.insert(avo->name);           // marca como visitado			
			total++;                        // conta este ascendente
			total =+ contarAscendentes(avo, visit, total); // conta ascendentes recursivamente
		}
	}
	return total;
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
	cout << "Insira uma pessoa inicial:" << "\n\n";
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
				menuRetornar();
				criaPessoa(pessoas);
				break;	//PASSAGEM POR REFERENCIA FUNCIONA AQUI
			}
			case 2: {
				menuRetornar();
				adicionaRelacao(pessoas);
				break;
			}
			case 3: {
				menuRetornar();
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
				menuRetornar();
				
				cout << "Informe o anscestral: ";		
				string ancestral;						
				cin >> ancestral;
				
				if(ancestral == "9"){
					break;
				}
				int ancestralI = buscaPessoaIndice(pessoas, ancestral);
				
				unordered_set<string> visitDesc;
				
				if (ancestralI != -1) {
					infoPessoa(pessoas[ancestralI], arquivo);
					arquivo << "Descendentes: " << contarDescendentes(pessoas[ancestralI], visitDesc, 0) << "\n";
					arquivo << "Arvore:" << "\n";
					imprimirArvoreDesc(pessoas[ancestralI], 0, arquivo);
				} else {
					cout << "Pessoa nao encontrada." << "\n";
				}
				break;
			}
			case 5: {
				menuRetornar();
				cout << "Informe a pessoa central: ";		
				string centro;						
				cin >> centro;
				if(centro == "9"){
					break;
				}
				int centroI = buscaPessoaIndice(pessoas, centro);
				
				unordered_set<string> visitDesc;
				unordered_set<string> visitAsc;
				
				if (centroI != -1) {
					infoPessoa(pessoas[centroI], arquivo);
					arquivo << "Descendentes: " << contarDescendentes(pessoas[centroI], visitDesc, 0) << "\n";
					arquivo << "Ascendentes: " << contarAscendentes(pessoas[centroI], visitDesc, 0) << "\n";
					arquivo << "Arvore:" << "\n";
					imprimirArvoreAsc(pessoas[centroI], 0, arquivo);
					imprimirArvoreDesc(pessoas[centroI], 0, arquivo);
				} else {
					cout << "Pessoa nao encontrada." << "\n";
				}
				break;
			}	
		}
	}

	arquivo.close();
	
	return 0;
}