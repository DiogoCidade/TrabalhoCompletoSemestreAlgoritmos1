#include <iostream>													//ENTRADA E SAIDA DE DADOS (COUT, CIN)
#include <fstream>													//MANIPULACAO DE ARQUIVOS (OFSTREAM)
#include <string>													//USO DE MANIPULACAO DE STRINGS (NOME, DATAS, ETC.)
#include <vector>													//ARMAZENAR LISTAS DE PESSOAS E RELACOES
#include <memory>													//PONTEIROS INTELIGENTES (SHARED_PTR)
#include <unordered_set>											//CONTROLE DE ELEMENTOS VISITADOS (NA BUSCA DE PARENTESCO)
#include <queue>													//ESTRUTURA DE FILA USADA NA BUSCA POR GRAU DE PARENTESCO

using namespace std;

//NOMEAR O ARQUIVO DE SAIDA
void nomearArq(string& nomeArq){									//STRING DO NOME DO ARQUIVO COMO ARGUMENTO
	cout << "De um nome ao arquivo: ";
	getline(cin >> ws, nomeArq);									//INPUT DO NOME DO ARQUIVO
	nomeArq += ".txt";												//DEFINICAO DE DOMINIO
}
//MENU DE OPCOES
void menuOp() {
	cout << "\n" << "1 - Inserir nova pessoa" << "\n";
	cout << "2 - Adicionar relacao de parentesco" << "\n";
	cout << "3 - Buscar pessoa" << "\n";
	cout << "4 - Imprimir arvore a partir de uma pessoa" << "\n";
	cout << "5 - Imprimir toda a arvore de uma pessoa (anscendentes e descendentes)" << "\n";
	cout << "6 - Imprimir grau de parentesco entre duas pessoas" << "\n";
	cout << "0 - Sair / Encerrar" << "\n\n";
}

//MENU DE RETORNO
void menuRetornar() {
	cout << "\n" << "9 - Retornar ao menu anterior" << "\n\n";
}

//PESSOA CONTENDO INFORMACOES DE NOME, DATA DE NASCIMENTO E GENERO
//CADA PESSOA TEM SEU PROPRIO VECTOR PARA ARMAZENAR SEU DESCENDENTES E ASCENDENTES
//SHARED_PTR EH UM SMART POINTER (FAZ ALOCACAO E LIBERACAO DE MEMORIA AUTOMATICAMENTE) 
struct Pessoa{
	string name;
	string nasc;
	char genero;
	vector<shared_ptr<Pessoa>> ascendentes; 
	vector<shared_ptr<Pessoa>> descendentes; 
};

//LEITURA DE PESSOAS
shared_ptr<Pessoa> criaPessoa(vector<shared_ptr<Pessoa>>& pessoas) {
	auto p = make_shared<Pessoa>();									//CRIACAO DE UMA PESSOA "P"
											
	cout << "Informe o nome da pessoa: ";
	getline(cin >> ws, p->name); 									//INPUT DO NOME
	for(auto pessoa : pessoas) {									//PARA CADA PESSOA DENTRO DO VECTOR DE PESSOAS:
		if(pessoa->name == p->name) { 								//SE O NOME DA PESSOA NO VECTOR FOR IGUAL AO NOME DA PESSOA INFORMADO PELO USUARIO,
			cout << "Pessoa ja inserida anteriormente!" << "\n";	//ESTA PESSOA JA FOI INSERIDA ANTERIORMENTE. IMPEDE REPETICAO. CANCELA FUNCAO.
			return nullptr;
		}
	}
	if(p->name == "9"){												//CASO O USUARIO DIGITE "9"
		cout << "Funcao cancelada!" << "\n";						//CANCELA A FUNCAO
		return nullptr;												//E RETORNA AO MENU
	}
	
	cout << "Informe a data de nascimento da pessoa (XX/XX/XXXX): ";
	cin >> p->nasc;													//INPUT DA DATA DE NASCIMENTO
	if(p->nasc == "9"){
		cout << "Funcao cancelada!" << "\n";
		return nullptr;
	}
	
	cout << "Informe o genero da pessoa (M/F): ";
	cin >> p->genero;												//INPUT DO SEXO, MASCULINO OU FEMININO
	if(p->genero == '9'){
		cout << "Funcao cancelada!" << "\n";
		return nullptr;
	}
	
	pessoas.push_back(p);											//POR FIM, INSERE A PESSOA "P" CRIADA NO VECTOR DE PESSOAS

	cout << "\n" << "Pessoa inserida com sucesso!" << "\n";

	return p;														//RETORNA O SMART POINTER DA PESSOA "P" AO INVES DO VECTOR INTEIRO
}

//EXIBICAO DAS INFORMACOES DE UMA PESSOA
void infoPessoa(shared_ptr<Pessoa> pessoa, bool impressao, ofstream &arquivo) {					//IMPRESSAO DAS INFORMACOES DE UMA PESSOA CASO SOLICITADO
	if(impressao == true) {																		//BOOLEANO PARA ESCOLHER ENTRE: 
		arquivo << pessoa->name << ", " << pessoa->nasc << ", " << pessoa->genero << "\n\n";	//IMPRIMIR NO ARQUIVO TXT (JUNTO DA ARVORE),
	} else {																					//OU
		cout << pessoa->name << ", " << pessoa->nasc << ", " << pessoa->genero << "\n\n";		//PARA EXIBIR NO PROMPT, PARA VERIFICACAO DE EXISTENCIA
	}
}

//BUSCA LINEAR QUE RETORNA O INDICE DA PESSOA BUSCADA NO VECTOR
int buscaPessoaIndice(vector<shared_ptr<Pessoa>>& pessoas, string chave) {	
	int i;
	for (i = 0; i < pessoas.size(); i++) {
        if (pessoas[i]->name == chave) {							//SE ACHAR A PESSOA PROCURADA
            return i; 												//RETORNA O INDICE QUE O PONTEIRO APONTA
        }
	}
    return -1;														//SE NAO ENCONTRAR RETORNA -1
}

//BUSCA LINEAR QUE INFORMA SE A PESSOA EXISTE OU NAO
void buscaPessoa(vector<shared_ptr<Pessoa>>& pessoas, ofstream& arquivo) {
	menuRetornar();													//MENU DE RETORNO
	
	cout << "Buscar pessoa: ";										//INPUT DA PESSOA BUSCADA
    string chave;
    cin >> chave;
    
    if(chave == "9"){												//OPCAO DE RETORNO
		return;
	}	
	
	int i;
	
	for (i = 0; i < pessoas.size(); i++) {
        if (chave == pessoas[i]->name) {							//SE ACHAR A PESSOA PROCURADA
            break; 													//ENCERRA ANTECIPADAMENTE O FOR
        }
	}
	
    if (i < pessoas.size()) {										//SE A PESSOA ESTA NO VECTOR
        infoPessoa(pessoas[i], false, arquivo);						//EXIBE AS INFORMACOES DA PESSOA NO PROMPT (BOOL FALSE)
    } else {
        cout << "\n" << "Pessoa nao encontrada" << "\n";
    }
}

//SISTEMA DE ADICAO DE RELACIONAMENTO, ONDE CADA PESSOA TEM UM VECTOR DE ASCENDENTES E DESCENDENTES
//O USER INFORMA O PAI E O FILHO, E O PONTEIRO APONTA A PESSOA FILHO PRA DENTRO DO VECTOR DESCENDENTES DO PAI
auto adicionaRelacao(vector<shared_ptr<Pessoa>>& pessoas) {
	cout << "Informe o pai/mae: ";									//INPUT DO PAI
	string pai;											
	getline(cin >> ws, pai);
	
	if(pai == "9"){													//OPCAO DE RETORNO
		cout << "Funcao cancelada!" << "\n";
		return;
	}
	
	int paiI = buscaPessoaIndice(pessoas, pai);						//BUSCA O INDICE DO PAI NO VECTOR PESSOAS
	
	if (paiI == -1) { 												//CASO NAO ENCONTRADO (ERRO DE DIGITACAO, POR EXEMPLO)
		cout << "Pessoa nao encontrada." << "\n";
		adicionaRelacao(pessoas);									//AGE RECURSIVAMENTE ATE ENCONTRAR
		return; 
	}
	
	cout << "Informe o filho: ";									//INPUT DO FILHO
	string filho;
	getline(cin >> ws, filho);
	
	if(filho == "9"){												//OPCAO DE RETORNO
		cout << "Funcao cancelada!" << "\n";
		return;
	}
	
	int filhoI = buscaPessoaIndice(pessoas, filho);					//BUSCA O INDICE DO PAI NO VECTOR PESSOAS
	
	if (filhoI == -1) { 											//CASO NAO ENCONTRADO (ERRO DE DIGITACAO, POR EXEMPLO)
		cout << "Pessoa nao encontrada." << "\n"; 
		adicionaRelacao(pessoas);									//AGE RECURSIVAMENTE ATE ENCONTRAR
		return; 
	}
	
	//VERIFICACAO SE RELACAO CRIADA JA NAO EXISTE; LE-SE:
	for(auto filho : pessoas[paiI]->descendentes) {					//PARA CADA FILHO DENTRO DO VECTOR DE FILHOS DO PAI INFORMADO PELO USUARIO:
		if(filho->name == pessoas[filhoI]->name) { 					//SE O NOME DO FILHO NO VECTOR FOR IGUAL AO NOME DO FILHO INFORMADO PELO USUARIO,
			cout << "Relacao ja inserida anteriormente!" << "\n";	//ESTA RELACAO JA FOI INSERIDA ANTERIORMENTE. IMPEDE REPETICAO.
			return;
		}
	}
	
	pessoas[paiI]->descendentes.push_back(pessoas[filhoI]); 		//INSERE FILHO INFORMADO PELO USUARIO DENTRO DO VECTOR DE DESCENDENTES DO PAI INFORMADO
																	//COM O MESMO PAI E O MESMO FILHO INFORMADO PELO USUARIO
	pessoas[filhoI]->ascendentes.push_back(pessoas[paiI]);			//INSERE PAI INFORMADO PELO USUARIO DENTRO DO VECTOR DE ASCENDENTES DO FILHO INFORMADO
	
	cout << "\n" << "Relacao adicionada com sucesso!" << "\n";
}

//FORMATACAO DE IMPRESSAO DA ARVORE
struct NivelGeracao {												//CRIACAO DE STRUCT PRA PODER CHAMAR TANTO O ESPACO QUANTO O NIVEL
    string espaco;		
    string nivel;
};

NivelGeracao nivelGeracoes(int geracao) {							//GERACAO COMO ARGUMENTO PARA RECURSIVIDADE
    NivelGeracao n;													//CRIACAO DE UMA GERACAO "N"
    n.espaco = " ";													//ESPACO INICIAL
    n.nivel = "+-";													//NIVEL INICIAL
    
    for (int i = 0; i < geracao; i++) {								//PARA CADA GERACAO
        n.espaco += "  ";											//AUMENTA O ESPACO
        n.nivel += "-";												//AUMENTA O NIVEL(GALHO)
    }
    return n;														//RETORNA O "N", N.ESPACO OU N.NIVEL
}

//IMPRESSAO DE UMA PESSOA E TODOS SEUS ASCENDENTES E DESCENDENTES EM FORMATO DE ARVORE:
	//DIFERENCA CONSISTE NA ORDEM: 
		//ASCENDENTES: VAI PASSANDO PARA A PROXIMA PESSOA NO SEU VECTOR ASCENDENTES (LACO FOR)
			//FUNCAO RECURSIVA, CHAMA A SI MESMO, CRIANDO UM CICLO, NA ULTIMA PESSOA, IMPRIME E COMECA A RETORNAR
			//IMPRIMINDO TODOS OS ANTERIORES, E ENTAO ENCERRA 
		//DESCENDENTES: IMPRIME A PESSOA E PASSA PARA A PROXIMA PESSOA NO SEU VECTOR DESCENDENTES (LACO FOR)
			//FUNCAO RECURSIVA, CHAMA A SI MESMO, CRIANDO UM CICLO, AO FINAL, RETORNA, RETORNA, RETORNA, E ENCERRA A FUNCAO
void imprimirArvore(shared_ptr<Pessoa> pessoa, int geracao, bool ascendentes, ofstream& arquivo) {	//BOOL PARA DEFINIR ASCENDENTES OU DESCENDENTES
	if(!pessoa) return;
	
	NivelGeracao nivelGer = nivelGeracoes(geracao);
	
	if(ascendentes == true) {										//SE BOOLEANO TRUE: IMPRIME ASCENDENTES
		for(auto& pai : pessoa->ascendentes) {						//PARA CADA "PAI" NO VECTOR ASCENDENTES DA PESSOA INFORMADA
			imprimirArvore(pai, geracao + 1, true, arquivo);		//CHAMA A SI MESMO, AUMENTANDO A GERACAO E SUBINDO OS GALHOS
		}
		if(geracao != 0) {											//GERACAO 0 SIGNIFICA A PESSOA INICIAL, IF IMPEDE QUE SEJA IMPRESSA DUAS VEZES QUANDO CHAMADO JUNTO DOS DESCENDENTES
			arquivo << "G" << geracao + 1 << " " << nivelGer.espaco << nivelGer.nivel << pessoa->name << "\n";		
		}
	} else {														//SE BOOLEANO FALSE: CONTA DESCENDENTES
		arquivo << "G" << geracao + 1 << ":" << nivelGer.espaco << nivelGer.nivel << pessoa->name << "\n";
		for(auto& filho : pessoa->descendentes) {					//PARA CADA "FILHO" NO VECTOR DESCENDENTES DA PESSOA INFORMADA
			imprimirArvore(filho, geracao + 1, false, arquivo);		//CHAMA A SI MESMO, AUMENTANDO A GERACAO E SUBINDO OS GALHOS
		}
	}
}

//CONTAGEM DE TODOS OS RELACIONADOS DA PESSOA INFORMADA (RECURSIVAMENTE)
int contarRelacionados(shared_ptr<Pessoa> pessoa, unordered_set<string>& visit, int total, bool ascendentes) { //BOOL PARA DEFINIR ASCENDENTES OU DESCENDENTES
	if(!pessoa) return 0;
	
	int totalAsc = total;											//TOTAL DE ASCENDENTES = TOTAL INFORMADO NO ARGUMENTO
	int totalDesc = total;											//TOTAL DE DESCENDENTES = TOTAL INFORMADO NO ARGUMENTO
	
	if(ascendentes == true) {										//SE BOOLEANO TRUE: CONTA ASCENDENTES
		for (auto& pai : pessoa->ascendentes) {						//PARA CADA "PAI" NO VECTOR ASCENDENTES DA PESSOA INFORMADA
			if (visit.find(pai->name) == visit.end()) {				//SE O NOME DO ASCENDENTE AINDA NAO FOI VISITADO
				visit.insert(pai->name);           					//MARCA COMO VISITADO		
				totalAsc++;                        					//SOMA A CONTAGEM
				totalAsc =+ contarRelacionados(pai, visit, totalAsc, true);	//SOMA VALOR RETORNADO PELA CHAMADA RECURSIVA COM O PAI (ASCENDENTE) COMO ARGUMENTO
			}
		}
		visit.clear();												//APOS TODA A CONTAGEM, LIMPA A LISTA DE VISITADOS
		return totalAsc;											//RETORNA O NUMERO DE ASCENDENTES
	} else {														//SE BOOLEANO FALSE: CONTA DESCENDENTES
		for (auto& filho : pessoa->descendentes) {					//PARA CADA "FILHO" NO VECTOR DESCENDENTES DA PESSOA INFORMADA
			if (visit.find(filho->name) == visit.end()) {			//SE O NOME DO DESCENDENTE AINDA NAO FOI VISITADO
				visit.insert(filho->name);         					//MARCA COMO VISITADO
				totalDesc++;                        				//SOMA A CONTAGEM
				totalDesc =+ contarRelacionados(filho, visit, totalDesc, false); //SOMA VALOR RETORNADO PELA CHAMADA RECURSIVA COM O FILHO (DESCENDENTE) COMO ARGUMENTO
			}
		}
		visit.clear();												//APOS TODA A CONTAGEM, LIMPA A LISTA DE VISITADOS
		return totalDesc;											//RETORNA O NUMERO DE DESCENDENTES
	}
}

//CONTAGEM DE RELACIONADOS DIRETOS DA PESSOA INFORMADA (SEM RECURSIVIDADE)
int contarRelacionadosDiretos(shared_ptr<Pessoa> pessoa, unordered_set<string>& visit, int total, bool ascendentes) { //BOOL PARA DEFINIR ASCENDENTES OU DESCENDENTES
	if(!pessoa) return 0;
	
	int totalAsc = total;											//TOTAL DE ASCENDENTES = TOTAL INFORMADO NO ARGUMENTO
	int totalDesc = total;											//TOTAL DE DESCENDENTES = TOTAL INFORMADO NO ARGUMENTO
	
	if(ascendentes == true) {										//SE BOOLEANO TRUE: CONTA ASCENDENTES
		for (auto& pai : pessoa->ascendentes) {						//PARA CADA "PAI" NO VECTOR ASCENDENTES DA PESSOA INFORMADA
			if (visit.find(pai->name) == visit.end()) {				//SE O NOME DO ASCENDENTE AINDA NAO FOI VISITADO
				visit.insert(pai->name);           					//MARCA COMO VISITADO			
				totalAsc++;                       					//SOMA A CONTAGEM
			}
		}
		visit.clear();												//APOS TODA A CONTAGEM, LIMPA A LISTA DE VISITADOS
		return totalAsc;											//RETORNA O NUMERO DE ASCENDENTES DIRETOS
	} else {														//SE BOOLEANO FALSE: CONTA DESCENDENTES
		for (auto& filho : pessoa->descendentes) {					//PARA CADA "FILHO" NO VECTOR DESCENDENTES DA PESSOA INFORMADA
			if (visit.find(filho->name) == visit.end()) {			//SE O NOME DO DESCENDENTE AINDA NAO FOI VISITADO
				visit.insert(filho->name);         					//MARCA COMO VISITADO
				totalDesc++;                        				//SOMA A CONTAGEM
			}
		}
		visit.clear();												//APOS TODA A CONTAGEM, LIMPA A LISTA DE VISITADOS
		return totalDesc;											//RETORNA O NUMERO DE DESCENDENTES DIRETOS
	}
}

//CALCULADORA DE DISTANCIA DE PARENTESCO ENTRE DUAS PESSOAS
int distanciaEntre(shared_ptr<Pessoa> origem, shared_ptr<Pessoa> destino) {
    if ((origem || destino) < 0) return -1;							//SE ALGUMA DAS PESSOA NAO EXISTIR		
    if (origem == destino) return 0;								//SE AS DUAS PESSOA FOREM A MESMA

    queue<pair<shared_ptr<Pessoa>, int>> fila;						//FILA PARA BUSCA EM LARGURA (BFS)
	unordered_set<shared_ptr<Pessoa>> visitados;					//LISTA DE VISITADOS

    fila.push({origem, 0});											//ADICIONA A PESSOA ORIGEM A FILA E DISTANCIA 0
    visitados.insert(origem);										//ADICIONA A PESSOA ORIGEM A LISTA

    while (!fila.empty()) {											//ENQUANTO A FILA NAO ESTIVER VAZIA
        auto atual = fila.front();									//PEGA A PRIMEIRA PESSOA DA FILA (E SUA DISTANCIA)
        fila.pop();													//REMOVE A PESSOA DA FILA

        shared_ptr<Pessoa> pessoaAtual = atual.first;				//PESSOA ATUAL EH ANALISADA
        int distancia = atual.second;								//DISTANCIA (GRAU DE PARENTESCO) ATE A PESSOA INICIAL

        if (pessoaAtual == destino) {								//QUANDO ENCONTRAR A PESSOA DESTINO
        	return distancia;										//RETORNA A DISTANCIA
		}

        for (auto pai : pessoaAtual->ascendentes) {					//PARA CADA "PAI" NO VECTOR ASCENDENTES DA PESSOA ATUAL
        	if (!visitados.count(pai)) {							//SE PAI NAO VISITADO
            	fila.push({pai, distancia + 1});					//ADICIONA PAI A FILA E AUMENTA A DISTANCIA EM 1
            	visitados.insert(pai);								//ADICIONA PAI A LISTA DE VISITADOS
        	}
    	}

        for (auto filho : pessoaAtual->descendentes) {				//PARA CADA "FILHO" NO VECTOR DESCENDENTES DA PESSOA ATUAL
            if (!visitados.count(filho)) {							//SE FILHO NAO VISITADO
                fila.push({filho, distancia + 1});					//ADICIONA FILHO A FILA E AUMENTA A DISTANCIA EM 1
                visitados.insert(filho);							//ADICIONA DILHO A LISTA DE VISITADOS
            }
        }
    }

    return -1;
}

void calcularDistanciaParentesco(vector<shared_ptr<Pessoa>>& pessoas, string pessoa1, string pessoa2, ofstream& arquivo) {
    int p1 = buscaPessoaIndice(pessoas, pessoa1);					//CRIA E PROCURA INDICE DA PESSOA INFORMADA DENTRO DO VECTOR DE PESSOAS
    int p2 = buscaPessoaIndice(pessoas, pessoa2);					//CRIA E PROCURA INDICE DA PESSOA INFORMADA DENTRO DO VECTOR DE PESSOAS

    if ((p1 || p2) < 0) {											//SE ALGUMA DAS PESSOA NAO EXISTIR	
        cout << "\n" << "Pessoa(s) nao encontradas" << endl;
        return;
    }
    
    int distancia = distanciaEntre(pessoas[p1], pessoas[p2]);		//CHAMA A FUNCAO DE PROCURA E CALCULO DE PARENTESCO
    
    cout << "Distancia de parentesco impressa com sucesso!" << "\n";

    if (distancia == -1) {
        arquivo << "Nao existe relacao entre " << pessoa1 << " e " << pessoa2 << "." << "\n\n";
    } else {
        arquivo << pessoa1 << " e " << pessoa2 << " sao parentes de " << distancia << "o grau." << "\n\n";
	}
}

int main() {
	vector<shared_ptr<Pessoa>> pessoas;												//CRIACAO DO VECTOR DE PESSOAS
	
	//NOMEAR E DEFINIR O ARQUIVO DE SAIDA
	string nomeArq = "";															//CRIA NOMEARQ
	nomearArq(nomeArq);																//FUNCAO DE NOMEAR ARQUIVO
	ofstream arquivo(nomeArq);														//CRIA ARQUIVO COM NOME DEFINIDO PELO USUARIO
	
	//VARIAVEL QUE O USUARIO VAI DEFINIR PARA NAVEGAR PELO MENU
	int comand = 1;																	//VARIAVEL QUE O USUARIO VAI DEFINIR PARA NAVEGAR PELO MENU
	
	//INSERIR PESSOA INICIAL
	cout << "Insira uma pessoa inicial:" << "\n\n";
	criaPessoa(pessoas);															//INSERCAO DA PRIMEIRA PESSOA, UNICA SEM ADICAO DE RELACIONAMENTO OBRIGATORIO
	
	while(comand != 0){																//LOOP WHILE, ENQUANTO O USUARIO NAO DIGITAR "0", NAO ENCERRA
		menuOp(); 																	//O MENU DE OPCOES
		cin >> comand;																//PROMPT DE NAVEGACAO DO USUARIO
		switch(comand) {
			case 0: {																//0: ENCERRA
				break;
			}
			case 1: {																//1: CRIACAO DE NOVA PESSOA
				menuRetornar();														//MENU DE RETORNO
				criaPessoa(pessoas);												//FUNCAO DE CRIAR PESSOA
				adicionaRelacao(pessoas);											//FUNCAO DE ADICIONAR RELACIONAMENTO IMEDIATAMENTE APOS CRIAR PESSOA,
				break;																//PARA EVITAR PESSOAS "NO LIMBO"
			}
			case 2: {																//2: ADICIONAR RELACAO (SEPARADAMENTE DA OBRIGATORIA ACIMA)
				menuRetornar();														//MENU DE RETORNO
				adicionaRelacao(pessoas);											//FUNCAO DE ADIOCIONAR RELACAO
				break;
			}
			case 3: {																//3: BUSCA DE PESSOA (PARA VERIFICAR SE JA INSERIDA)
				buscaPessoa(pessoas, arquivo);										//FUNCAO DE BUSCA QUE RETORNA OS DADOS DA PESSOA BUSCADA (CASO EXISTA)
				break;
			}
			case 4: {																//4: IMPRESSAO DA ARVORE DE DESCENDENTES
				menuRetornar();														//MENU DE RETORNO
				cout << "Informe o anscestral: ";		
				string ancestral;													//INFORMA O ANCESTRAL DA ARVORE
				getline(cin >> ws, ancestral);
				if(ancestral == "9"){												//OPCAO DE RETORNO
					break;
				}
				int ancestralI = buscaPessoaIndice(pessoas, ancestral);				//BUSCA O INDICE DA PESSOA ANCESTRAL NO VECTOR PESSOAS
				unordered_set<string> visitDesc;									//CRIA A LISTA DE VISITADOS
				if (ancestralI != -1) {												//CASO ANCESTRAL EXISTA
					infoPessoa(pessoas[ancestralI], true, arquivo);					//IMPRIME AS INFORMACOES DO ANCESTRAL
					arquivo << "Descendentes diretos: " << contarRelacionadosDiretos(pessoas[ancestralI], visitDesc, 0, false) << "\n";	//IMPRIME A CONTAGEM DE DESCENDENTES DIRETOS
					arquivo << "Descendentes: " << contarRelacionados(pessoas[ancestralI], visitDesc, 0, false) << "\n\n";				//IMPRIME A CONTAGEM DE TODOS OS DESCENDENTES
					arquivo << "Arvore:" << "\n";						
					imprimirArvore(pessoas[ancestralI], 0, false, arquivo);			//IMPRIME A ARVORE DE DESCENDENTES (BOOL FALSE)
					arquivo << "\n";
					cout << "\n" << "Arvore impressa com sucesso!" << "\n";
				} else {
					cout << "\n" << "Pessoa nao encontrada." << "\n";
				}
				break;
			}
			case 5: {																//5: IMPRESSAO DA ARVORE DE ASCENDENTES E DESCENDENTES
				menuRetornar();														//MENU DE RETORNO
				cout << "Informe a pessoa central: ";
				string centro;														//INFORMA A PESSOA CENTRAL DA ARVORE
				getline(cin >> ws, centro);
				if(centro == "9"){													//OPCAO DE RETORNO
					break;
				}
				int centroI = buscaPessoaIndice(pessoas, centro);					//BUSCA O INDICE DA PESSOA CENTRAL NO VECTOR DE PESSOAS
				unordered_set<string> visitDesc;									//CRIA A LISTA DE DESCENDENTES VISITADOS
				unordered_set<string> visitAsc;										//CRIA A LISTA DE ASCENDENTES VISITADOS
				if (centroI != -1) {								
					infoPessoa(pessoas[centroI], true, arquivo);					//CASO PESSOA CENTRAL EXISTA
					arquivo << "Ascendentes diretos: " << contarRelacionadosDiretos(pessoas[centroI], visitDesc, 0, true) << "\n";		//IMPRIME A CONTAGEM DE ASCENDENTES DIRETOS
					arquivo << "Ascendentes: " << contarRelacionados(pessoas[centroI], visitDesc, 0, true) << "\n\n";					//IMPRIME A CONTAGEM DE TODOS OS ASCENDENTES
					arquivo << "Descendentes diretos: " << contarRelacionadosDiretos(pessoas[centroI], visitDesc, 0, false) << "\n";	//IMPRIME A CONTAGEM DE DESCENDENTES DIRETOS
					arquivo << "Descendentes: " << contarRelacionados(pessoas[centroI], visitDesc, 0, false) << "\n\n";					//IMPRIME A CONTAGEM DE TODOS OS DESCENDENTES
					arquivo << "Arvore:" << "\n";
					imprimirArvore(pessoas[centroI], 0, true, arquivo);				//IMPRIME A ARVORE DE ASCENDENTES (BOOL TRUE)
					imprimirArvore(pessoas[centroI], 0, false, arquivo);			//IMPRIME A ARVORE DE DESCENDENTES (BOOL FALSE)
					arquivo << "\n";
					cout << "\n" << "Arvore impressa com sucesso!" << "\n";
				} else {
					cout << "\n" << "Pessoa nao encontrada." << "\n";
				}
				break;
			}
			case 6: {																//6: CALCULA O GRAU DE PARENTESCO ENTRE DUAS PESSOAS
    			string pessoa1, pessoa2;											//INFORMA AS PESSOAS DESEJADAS
    			cout << "Informe a primeira pessoa: ";
    			getline(cin >> ws, pessoa1);
    			cout << "Informe a segunda pessoa: ";
    			getline(cin >> ws, pessoa2);
    			calcularDistanciaParentesco(pessoas, pessoa1, pessoa2, arquivo);	//FUNCAO DE CALCULO E IMPRESSAO DA DISTANCIA DE PARENTESCO
    			break;
			}	
		}
	}

	arquivo.close();																//QUANDO FORA DO SWITCH (USUARIO INFORMOU 0), ENCERRA O PROGRAMA E FECHA O ARQUIVO
	
	return 0;
}