#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

//ADICIONAR PESSOA E SEUS DADOS 		//FEITO
//ADICIONAR RELACAO ENTRE PESSOAS		//AQUI FUDEO
//EXIBIR ASCENDENTES E ANTESCEDENTES
//EXIBIR ARVORE A PARTIR DE UMA PESSOA


//NOMEAR O ARQUIVO DE SAIDA
void nomearArq(string& nomeArq){ 	//PASSAGEM POR REFERENCIA DO ENDERECO &
	cout << "De um nome ao arquivo: ";
	cin >> nomeArq;
	nomeArq += ".txt";
}
//MENU DE OPCOES
void menuOp() {
	cout << "1 - Inserir nova pessoa" << "\n";
	cout << "2 - Adicionar relacao de parentesco" << "\n"; //ACHO QUE ISSO VAI SER INSTANTANEAMENTE APOS INSERIR PESSOA NOVA
	cout << "3 - Buscar pessoa" << "\n";
	cout << "4 - Imprimir arvore a partir de uma pessoa" << "\n";
	cout << "5 - Imprimir toda a arvore de uma pessoa (anscendentes e descendentes)" << "\n";
	cout << "0 - Sair / Encerrar" << "\n\n";
}

//PESSOA CONTENDO INFORMACOES DE NOME, DATA DE NASCIMENTO E GENERO
struct Pessoa{
	string name;
	string nasc;
	char genero;
	//CADA PESSOA DENTRO DO VECTOR TAMBEM TEM 
	//SEU PROPRIO VECTOR PARA ARMAZENAR DUPLICADAMENTE SEU FILHOS
	//SHARED_PTR EH UM PONTEIRO INTELIGENTE, 
	vector<Pessoa> filhos; 
};

//LEITURA DE PESSOAS
auto criaPessoa(vector<Pessoa>& pessoas) {	//PASSAGEM POR REFERENCIA DO ENDERECO &
	Pessoa p;											
	cout << "\n" << "Informe o nome da pessoa: ";
	getline(cin >> ws, p.name); //GETLINE() SUBSTITUI O CIN >> PQ ESSA MERDA NAO ACEITA SPACEBAR; CIN >> WS PRA CONSERTAR UM OUTRO PROBLEMA LA FODASE
	cout << "Informe a data de nascimento da pessoa (XX/XX/XXXX): ";
	cin >> p.nasc;
	cout << "Informe o genero da pessoa (M/F): ";
	cin >> p.genero;
	pessoas.push_back(p);

	cout << "\n" << "Pessoa inserida com sucesso!" << "\n\n";

	return pessoas;
}

//BUSCA LINEAR QUE RETORNA O INDICE DA PESSOA BUSCADA NO VECTOR
int buscaPessoaIndice(vector<Pessoa> pessoas, string chave) {	
	int i;
	for (i = 0; i < pessoas.size(); i++) {
        if (chave == pessoas[i].name) {		//SE ACHAR A PESSOA PROCURADA
            break; 							//ENCERRA ANTECIPADAMENTE O FOR
        }
	}/*
    if (i < pessoas.size()) {
        cout << "\n" << "Pessoa encontrada no indice: " << i << "\n\n";
    } else {
        cout << "\n" << "Pessoa nao encontrada" << "\n\n";
    }*/
    return i;
}

//BUSCA LINEAR QUE RETORNA TRUE OR FALSE
void buscaPessoa(vector<Pessoa> pessoas, string chave) {	
	int i;
	for (i = 0; i < pessoas.size(); i++) {
        if (chave == pessoas[i].name) {
            break; //ENCERRA ANTECIPADAMENTE O FOR
        }
	}
    if (i < pessoas.size()) {
        cout << "\n" << "Pessoa ja cadastrada" << "\n\n";
    } else {
        cout << "\n" << "Pessoa nao cadastrada" << "\n\n";
    }
}

auto adicionaRelacao(vector<Pessoa> pessoas) {
	cout << "Informe o pai/mae: ";
	string pai;
	cin >> pai;
	int paiI = buscaPessoaIndice(pessoas, pai);
	
	cout << "Informe o filho: ";
	string filho;
	cin >> filho;
	int filhoI = buscaPessoaIndice(pessoas, filho);
	
	pessoas[paiI].filhos.push_back(pessoas[filhoI]);
	
	for (const auto& pessoa : pessoas[paiI].filhos) {	//IMPRIME TABELA COM AS PESSOAS CADASTRADAS; SO PRA TESTE
		cout << pessoa.name << ", " << pessoa.nasc << ", " << pessoa.genero << "\n";
	}
}

int main() {
	vector<Pessoa> pessoas;
	
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
				cout << "\n" << "Buscar pessoa: ";
    			string chave;
    			cin >> chave;
				buscaPessoa(pessoas, chave);
				break;
			}
		}
	}

	for (const auto& pessoa : pessoas) {	//IMPRIME TABELA COM AS PESSOAS CADASTRADAS; SO PRA TESTE
		arquivo << pessoa.name << ", " << pessoa.nasc << ", " << pessoa.genero << "\n";
	}
	
	arquivo.close();
	
	return 0;
}
/*
public class Person {
	public string Name = string.empty();
	public Person(string name) {
		Name = name;
	} 
	public List<Person> Childs = new List<Person>();
}

public static void ManagePersonTree() {
	Person p1 = new Person("p1");
	Person p1 = new Person("p2");
	Person p1 = new Person("p3");
	Person p1 = new Person("p4");
	Person p1 = new Person("p5");
	Person p1 = new Person("p6");
	Person p1 = new Person("p7");
	Person p1 = new Person("p8");
	Person p1 = new Person("p9");
	Person p1 = new Person("p10");
	Person p1 = new Person("p11");
	Person p1 = new Person("p12");
	
	p1.Childs.AddRange(new[] {p2, p8});
	p2.Childs.AddRange(new[] {p3, p4});
	p4.Childs.Add(p5);
	p5.Childs.AddRange(new[] {p6, p7});
	p8.Childs.AddRange(new[] {p9, p10, p11});
	p10.Childs.Add(p12);
}
*/


