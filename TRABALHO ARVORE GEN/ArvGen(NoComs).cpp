#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <queue>

using namespace std;

void nomearArq(string& nomeArq){
	cout << "De um nome ao arquivo: ";
	getline(cin >> ws, nomeArq);
	nomeArq += ".txt";
}

void menuOp() {
	cout << "\n" << "1 - Inserir nova pessoa" << "\n";
	cout << "2 - Adicionar relacao de parentesco" << "\n";
	cout << "3 - Buscar pessoa" << "\n";
	cout << "4 - Imprimir arvore a partir de uma pessoa" << "\n";
	cout << "5 - Imprimir toda a arvore de uma pessoa (anscendentes e descendentes)" << "\n";
	cout << "6 - Imprimir grau de parentesco entre duas pessoas" << "\n";
	cout << "0 - Sair / Encerrar" << "\n\n";
}

void menuRetornar() {
	cout << "\n" << "9 - Retornar ao menu anterior" << "\n\n";
}

struct Pessoa{
	string name;
	string nasc;
	char genero;
	vector<shared_ptr<Pessoa>> ascendentes; 
	vector<shared_ptr<Pessoa>> descendentes; 
};

shared_ptr<Pessoa> criaPessoa(vector<shared_ptr<Pessoa>>& pessoas) {
	auto p = make_shared<Pessoa>();
											
	cout << "Informe o nome da pessoa: ";
	getline(cin >> ws, p->name); 
	for(auto pessoa : pessoas) {
		if(pessoa->name == p->name) { 
			cout << "Pessoa ja inserida anteriormente!" << "\n";
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
	
	pessoas.push_back(p);

	cout << "\n" << "Pessoa inserida com sucesso!" << "\n";

	return p;	
}

void infoPessoa(shared_ptr<Pessoa> pessoa, bool impressao, ofstream &arquivo) {	
	if(impressao == true) {	
		arquivo << pessoa->name << ", " << pessoa->nasc << ", " << pessoa->genero << "\n\n";
	} else {
		cout << pessoa->name << ", " << pessoa->nasc << ", " << pessoa->genero << "\n\n";
	}
}

int buscaPessoaIndice(vector<shared_ptr<Pessoa>>& pessoas, string chave) {	
	int i;
	for (i = 0; i < pessoas.size(); i++) {
        if (pessoas[i]->name == chave) {
            return i; 
        }
	}
    return -1;	
}

void buscaPessoa(vector<shared_ptr<Pessoa>>& pessoas, ofstream& arquivo) {
	menuRetornar();	
	
	cout << "Buscar pessoa: ";	
    string chave;
    cin >> chave;
    if(chave == "9"){
		return;
	}	
	
	int i;
	
	for (i = 0; i < pessoas.size(); i++) {
        if (chave == pessoas[i]->name) {
            break; 	
        }
	}
	
    if (i < pessoas.size()) {
        infoPessoa(pessoas[i], false, arquivo);
    } else {
        cout << "\n" << "Pessoa nao encontrada" << "\n";
    }
}

auto adicionaRelacao(vector<shared_ptr<Pessoa>>& pessoas) {
	cout << "Informe o pai/mae: ";
	string pai;											
	getline(cin >> ws, pai);
	
	if(pai == "9"){	
		cout << "Funcao cancelada!" << "\n";
		return;
	}
	
	int paiI = buscaPessoaIndice(pessoas, pai);
	
	if (paiI == -1) { 	
		cout << "Pessoa nao encontrada." << "\n";
		adicionaRelacao(pessoas);
		return; 
	}
	
	cout << "Informe o filho: ";
	string filho;
	getline(cin >> ws, filho);
	
	if(filho == "9"){
		cout << "Funcao cancelada!" << "\n";
		return;
	}
	
	int filhoI = buscaPessoaIndice(pessoas, filho);	
	
	if (filhoI == -1) { 
		cout << "Pessoa nao encontrada." << "\n"; 
		adicionaRelacao(pessoas);
		return; 
	}

	for(auto filho : pessoas[paiI]->descendentes) {	
		if(filho->name == pessoas[filhoI]->name) { 	
			cout << "Relacao ja inserida anteriormente!" << "\n";
			return;
		}
	}
	
	pessoas[paiI]->descendentes.push_back(pessoas[filhoI]); 
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

void imprimirArvore(shared_ptr<Pessoa> pessoa, int geracao, bool ascendentes, ofstream& arquivo) {
	if(!pessoa) return;
	
	NivelGeracao nivelGer = nivelGeracoes(geracao);
	
	if(ascendentes == true) {	
		for(auto& pai : pessoa->ascendentes) {	
			imprimirArvore(pai, geracao + 1, true, arquivo);
		}
		if(geracao != 0) {
			arquivo << "G" << geracao + 1 << " " << nivelGer.espaco << nivelGer.nivel << pessoa->name << "\n";		
		}
	} else {	
		arquivo << "G" << geracao + 1 << ":" << nivelGer.espaco << nivelGer.nivel << pessoa->name << "\n";
		for(auto& filho : pessoa->descendentes) {	
			imprimirArvore(filho, geracao + 1, false, arquivo);	
		}
	}
}

int contarRelacionados(shared_ptr<Pessoa> pessoa, unordered_set<string>& visit, int total, bool ascendentes) { 
	if(!pessoa) return 0;
	
	int totalAsc = total;
	int totalDesc = total;
	
	if(ascendentes == true) {
		for (auto& pai : pessoa->ascendentes) {	
			if (visit.find(pai->name) == visit.end()) {	
				visit.insert(pai->name); 	
				totalAsc++;  
				totalAsc =+ contarRelacionados(pai, visit, totalAsc, true);	
			}
		}
		visit.clear();	
		return totalAsc;
	} else {
		for (auto& filho : pessoa->descendentes) {	
			if (visit.find(filho->name) == visit.end()) {	
				visit.insert(filho->name); 
				totalDesc++;   
				totalDesc =+ contarRelacionados(filho, visit, totalDesc, false);
			}
		}
		visit.clear();
		return totalDesc;
	}
}

int contarRelacionadosDiretos(shared_ptr<Pessoa> pessoa, unordered_set<string>& visit, int total, bool ascendentes) {
	if(!pessoa) return 0;
	
	int totalAsc = total;
	int totalDesc = total;
	
	if(ascendentes == true) {	
		for (auto& pai : pessoa->ascendentes) {	
			if (visit.find(pai->name) == visit.end()) {
				visit.insert(pai->name); 		
				totalAsc++;   
			}
		}
		visit.clear();
		return totalAsc;
	} else {	
		for (auto& filho : pessoa->descendentes) {	
			if (visit.find(filho->name) == visit.end()) {
				visit.insert(filho->name);   
				totalDesc++; 
			}
		}
		visit.clear();	
		return totalDesc;
	}
}

int distanciaEntre(shared_ptr<Pessoa> origem, shared_ptr<Pessoa> destino) {
    if ((origem || destino) < 0) return -1;		
    if (origem == destino) return 0;

    queue<pair<shared_ptr<Pessoa>, int>> fila;	
	unordered_set<shared_ptr<Pessoa>> visitados;

    fila.push({origem, 0});	
    visitados.insert(origem);

    while (!fila.empty()) {	
        auto atual = fila.front();
        fila.pop();	

        shared_ptr<Pessoa> pessoaAtual = atual.first;
        int distancia = atual.second;

        if (pessoaAtual == destino) {
        	return distancia;
		}

        for (auto pai : pessoaAtual->ascendentes) {	
        	if (!visitados.count(pai)) {
            	fila.push({pai, distancia + 1});
            	visitados.insert(pai);	
        	}
    	}

        for (auto filho : pessoaAtual->descendentes) {	
            if (!visitados.count(filho)) {	
                fila.push({filho, distancia + 1});	
                visitados.insert(filho);
            }
        }
    }

    return -1;
}

void calcularDistanciaParentesco(vector<shared_ptr<Pessoa>>& pessoas, string pessoa1, string pessoa2, ofstream& arquivo) {
    int p1 = buscaPessoaIndice(pessoas, pessoa1);
    int p2 = buscaPessoaIndice(pessoas, pessoa2);

    if ((p1 || p2) < 0) {
        cout << "\n" << "Pessoa(s) nao encontradas" << endl;
        return;
    }
    
    int distancia = distanciaEntre(pessoas[p1], pessoas[p2]);
    
    cout << "Distancia de parentesco impressa com sucesso!" << "\n";

    if (distancia == -1) {
        arquivo << "Nao existe relacao entre " << pessoa1 << " e " << pessoa2 << "." << "\n\n";
    } else {
        arquivo << pessoa1 << " e " << pessoa2 << " sao parentes de " << distancia << "o grau." << "\n\n";
	}
}

int main() {
	vector<shared_ptr<Pessoa>> pessoas;
	
	string nomeArq = "";
	nomearArq(nomeArq);	
	ofstream arquivo(nomeArq);	
	
	int comand = 1;	
	
	cout << "Insira uma pessoa inicial:" << "\n\n";
	criaPessoa(pessoas);
	
	while(comand != 0){	
		menuOp(); 
		cin >> comand;
		switch(comand) {
			case 0: {	
				break;
			}
			case 1: {
				menuRetornar();	
				criaPessoa(pessoas);
				adicionaRelacao(pessoas);	
				break;	
			}
			case 2: {
				menuRetornar();	
				adicionaRelacao(pessoas);	
				break;
			}
			case 3: {
				buscaPessoa(pessoas, arquivo);	
				break;
			}
			case 4: {	
				menuRetornar();	
				cout << "Informe o anscestral: ";		
				string ancestral;	
				getline(cin >> ws, ancestral);
				if(ancestral == "9"){
					break;
				}
				int ancestralI = buscaPessoaIndice(pessoas, ancestral);	
				unordered_set<string> visitDesc;
				if (ancestralI != -1) {	
					infoPessoa(pessoas[ancestralI], true, arquivo);	
					arquivo << "Descendentes diretos: " << contarRelacionadosDiretos(pessoas[ancestralI], visitDesc, 0, false) << "\n";	
					arquivo << "Descendentes: " << contarRelacionados(pessoas[ancestralI], visitDesc, 0, false) << "\n\n";
					arquivo << "Arvore:" << "\n";						
					imprimirArvore(pessoas[ancestralI], 0, false, arquivo);
					arquivo << "\n";
					cout << "\n" << "Arvore impressa com sucesso!" << "\n";
				} else {
					cout << "\n" << "Pessoa nao encontrada." << "\n";
				}
				break;
			}
			case 5: {
				menuRetornar();
				cout << "Informe a pessoa central: ";
				string centro;	
				getline(cin >> ws, centro);
				if(centro == "9"){
					break;
				}
				int centroI = buscaPessoaIndice(pessoas, centro);
				unordered_set<string> visitDesc;
				unordered_set<string> visitAsc;
				if (centroI != -1) {								
					infoPessoa(pessoas[centroI], true, arquivo);
					arquivo << "Ascendentes diretos: " << contarRelacionadosDiretos(pessoas[centroI], visitDesc, 0, true) << "\n";
					arquivo << "Ascendentes: " << contarRelacionados(pessoas[centroI], visitDesc, 0, true) << "\n\n";
					arquivo << "Descendentes diretos: " << contarRelacionadosDiretos(pessoas[centroI], visitDesc, 0, false) << "\n";
					arquivo << "Descendentes: " << contarRelacionados(pessoas[centroI], visitDesc, 0, false) << "\n\n";	
					arquivo << "Arvore:" << "\n";
					imprimirArvore(pessoas[centroI], 0, true, arquivo);
					imprimirArvore(pessoas[centroI], 0, false, arquivo);
					arquivo << "\n";
					cout << "\n" << "Arvore impressa com sucesso!" << "\n";
				} else {
					cout << "\n" << "Pessoa nao encontrada." << "\n";
				}
				break;
			}
			case 6: {
    			string pessoa1, pessoa2;
    			cout << "Informe a primeira pessoa: ";
    			getline(cin >> ws, pessoa1);
    			cout << "Informe a segunda pessoa: ";
    			getline(cin >> ws, pessoa2);
    			calcularDistanciaParentesco(pessoas, pessoa1, pessoa2, arquivo);
    			break;
			}	
		}
	}

	arquivo.close();
	
	return 0;
}