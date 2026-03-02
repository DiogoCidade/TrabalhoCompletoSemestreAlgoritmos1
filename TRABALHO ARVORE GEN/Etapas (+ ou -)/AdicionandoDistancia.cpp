#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <queue>
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
	for(auto pessoa : pessoas) {					//PARA CADA PESSOA DENTRO DO VECTOR DE PESSOAS:
		if(pessoa->name == p->name) { 				//SE O NOME DA PESSOA NO VECTOR FOR IGUAL AO NOME DA PESSOA INFORMADO PELO USUARIO,
			cout << "Pessoa ja inserida!" << "\n";	//ESTA PESSOA JA FOI INSERIDA ANTERIORMENTE. IMPEDE REPETICAO. CANCELA FUNCAO.
			return nullptr;
		}
	}
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

void infoPessoa(shared_ptr<Pessoa> pessoa, bool impressao, ofstream &arquivo) {
	if(impressao == true) {
		arquivo << "\n" << pessoa->name << ", " << pessoa->nasc << ", " << pessoa->genero << "\n\n";
	} else {
		cout << "\n" << pessoa->name << ", " << pessoa->nasc << ", " << pessoa->genero << "\n\n";
	}
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

//BUSCA LINEAR
void buscaPessoa(vector<shared_ptr<Pessoa>>& pessoas, string chave, ofstream& arquivo) {	
	int i;
	for (i = 0; i < pessoas.size(); i++) {
        if (chave == pessoas[i]->name) {
            break; //ENCERRA ANTECIPADAMENTE O FOR
        }
	}
    if (i < pessoas.size()) {
        infoPessoa(pessoas[i], false, arquivo);
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
	
	//AQUI TEM QUE TER VERIFICACAO SE RELACAO CRIADA JA NAO EXISTE; LE-SE:
	for(auto filho : pessoas[paiI]->descendentes) {	//PARA CADA FILHO DENTRO DO VECTOR DE FILHOS DO PAI INFORMADO PELO USUARIO:
		if(filho->name == pessoas[filhoI]->name) { 	//SE O NOME DO FILHO NO VECTOR FOR IGUAL AO NOME DO FILHO INFORMADO PELO USUARIO,
			cout << "Relacao ja inserida!" << "\n";	//ESTA RELACAO JA FOI INSERIDA ANTERIORMENTE. IMPEDE REPETICAO.
			return;
		}
	}
	
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

	for (auto& pai : pessoa->ascendentes) {
		// se o nome do ascendente ainda nao foi visitado, conta e explora seus ascendentes
		if (visit.find(pai->name) == visit.end()) {
			visit.insert(pai->name);           // marca como visitado			
			total++;                        // conta este ascendente
			total =+ contarAscendentes(pai, visit, total); // conta ascendentes recursivamente
		}
	}
	return total;
}

int distanciaEntre(shared_ptr<Pessoa> origem, shared_ptr<Pessoa> destino) {
    if (!origem || !destino) return -1;
    if (origem == destino) return 0;

    // Fila para a busca em largura
    queue<pair<shared_ptr<Pessoa>, int>> fila;
    unordered_set<shared_ptr<Pessoa>> visitados;

    fila.push({origem, 0});
    visitados.insert(origem);

    while (!fila.empty()) {
        auto atual = fila.front();
        fila.pop();

        shared_ptr<Pessoa> pessoaAtual = atual.first;
        int distancia = atual.second;

        // Se encontramos o destino, retorna a distancia
        if (pessoaAtual == destino)
            return distancia;

        // Adiciona pai se ainda nao visitado
        for (auto pai : pessoaAtual->ascendentes) {
        	if (!visitados.count(pai)) {
            	fila.push({pai, distancia + 1});
            	visitados.insert(pai);
        	}
    	}

        // Adiciona filho se ainda nao visitado
        for (auto filho : pessoaAtual->descendentes) {
            if (!visitados.count(filho)) {
                fila.push({filho, distancia + 1});
                visitados.insert(filho);
            }
        }
    }

    // Caso nao tenha ligacao
    return -1;
}

void calcularDistanciaParentesco(vector<shared_ptr<Pessoa>>& pessoas, string pessoa1, string pessoa2, ofstream& arquivo) {
    auto p1 = buscaPessoaIndice(pessoas, pessoa1);
    auto p2 = buscaPessoaIndice(pessoas, pessoa2);

    // Se uma das pessoas nao existir
    if (!p1 || !p2) {
        cout << "Pessoa(s) nao encontradas" << endl;
        return;
    }

    // Chama a funcao auxiliar que usa o BFS
    int distancia = distanciaEntre(pessoas[p1], pessoas[p2]);

    if (distancia == -1)
        arquivo << "\n" << "Nao existe relacao direta entre " << pessoa1 << " e " << pessoa2 << "." << "\n";
    else
        arquivo << "\n" << pessoa1 << " e " << pessoa2 << " estao a " << distancia << " grau(s) de parentesco." << "\n";
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
				cout << "Buscar pessoa: ";
    			string chave;
    			cin >> chave;
    			if(chave == "9"){
					break;
				}
				buscaPessoa(pessoas, chave, arquivo);
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
					infoPessoa(pessoas[ancestralI], true, arquivo);
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
					infoPessoa(pessoas[centroI], true, arquivo);
					arquivo << "Ascendentes: " << contarAscendentes(pessoas[centroI], visitDesc, 0) << "\n";
					arquivo << "Descendentes: " << contarDescendentes(pessoas[centroI], visitDesc, 0) << "\n";
					arquivo << "Arvore:" << "\n";
					imprimirArvoreAsc(pessoas[centroI], 0, arquivo);
					imprimirArvoreDesc(pessoas[centroI], 0, arquivo);
				} else {
					cout << "Pessoa nao encontrada." << "\n";
				}
				break;
			}
			case 6: {
    			string pessoa1, pessoa2;
    			cout << "Informe a primeira pessoa: ";
    			cin >> pessoa1;
    			cout << "Informe a segunda pessoa: ";
    			cin >> pessoa2;
    			calcularDistanciaParentesco(pessoas, pessoa1, pessoa2, arquivo); // Chama a funcao criada antes
    			break;
			}	
		}
	}

	arquivo.close();
	
	return 0;
}