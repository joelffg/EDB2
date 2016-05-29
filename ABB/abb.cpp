#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

template <typename Tipo>
struct No{												//Estrutura que representa um nó de uma fila ou de uma pilha
    Tipo dado;
    No *proximo;
};

typedef struct no_arvore{								//Estrutura que representa um nó de uma árvore
    struct no_arvore *esq;
	int nos_direita;
	int chave;
	int altura;
	int nos_esquerda;
    struct no_arvore *dir;
}No_arvore;	

//PILHA
template <typename Tipo>
struct Pilha{											//Estrutura que representa uma pilha
    No<Tipo> *inicio;
    int tamanho;
};

template <typename Tipo>
Pilha<Tipo>* criar_pilha(){								//Função que cria uma nova pilha
	Pilha<Tipo> * nova  = new Pilha<Tipo>;
	nova->tamanho = 0;
	nova->inicio = NULL;
	return nova;
}

template <typename Tipo>
void empilhar(Pilha<Tipo> *pilha, Tipo no){				//Função que empilha um nó em uma pilha
	No<Tipo> *novo =  new No<Tipo>;
	novo->dado = no;
	novo->proximo = pilha->inicio;
	pilha->inicio = novo;
	pilha->tamanho++;
}

template <typename Tipo>
int desempilhar(Pilha<Tipo> *pilha, Tipo *retorno){		//Função que desempilha um nó de uma pilha
	if(pilha->inicio){
		*retorno = pilha->inicio->dado;
		No<Tipo> *aux = pilha->inicio;
		pilha->inicio = aux->proximo;
		delete aux;
		return 1;
	}
	return 0;
}

//FILA
template <typename Tipo>
struct Fila{											//Estrutura que representa uma fila
    No<Tipo> *inicio;
    No<Tipo> *fim;
    int tamanho;
};

template <typename Tipo>
Fila<Tipo>* criar_fila(){								//Função que cria uma nova fila
	Fila<Tipo> *nova = new Fila<Tipo>;
	nova->tamanho = 0;
	nova->inicio = NULL;
	nova->fim = NULL;
	return nova;
}

template <typename Tipo>
void enfileirar(Fila<Tipo> *fila, Tipo num){			//Função que enfileira um nó em uma fila
	No<Tipo> *novo =  new No<Tipo>;
	novo->dado = num;
	novo->proximo = NULL;
	if(fila->fim == NULL)
		fila->inicio = novo;
	else
		fila->fim->proximo = novo;
	fila->fim = novo;
}

template <typename Tipo>
int desenfileirar(Fila<Tipo> *fila, Tipo *num){			//Função que desenfileira um nó de uma fila
	if(fila->inicio){
		*num = fila->inicio->dado;
		No<Tipo> *aux = fila->inicio;
		fila->inicio = aux->proximo;
		delete aux;
		return 1;
	}
	return 0;
}

//ÁRVORE
void insercao(int x, No_arvore *&pt_raiz){				//Função que insere um elemento em uma árvore
	Pilha<No_arvore*> *pilha_altura  = criar_pilha<No_arvore*>();
	int f;
	No_arvore *pt = pt_raiz;
	while(pt){
		if(pt->chave == x){
			f = 1;
			break;
		}
		else{
			empilhar<No_arvore*>(pilha_altura, pt);
			if(x < pt->chave){
				pt->nos_esquerda += 1;
				if(!pt->esq)
					f = 2;
				else{
					pt = pt->esq;
				}
			}
			else{
				pt->nos_direita += 1;
				if(!pt->dir)
					f = 3;
				else{
					pt = pt->dir;
				}
			}
			if(f > 1)
				break;
		}
	}
	if(f == 1){
		cout << "Elemento já existe" << endl;
		pt = pt_raiz;
		while(pt){
			if(pt->chave == x){
				f = 1;
				break;
			}
			else{
				if(x < pt->chave){
					pt->nos_esquerda -= 1;
					if(!pt->esq)
						f = 2;
					else{
						pt = pt->esq;
					}
				}
				else{
					pt->nos_direita -= 1;
					if(!pt->dir)
						f = 3;
					else{
						pt = pt->dir;
					}
				}
				if(f > 1)
					break;
			}
		}
	}
	else{
		No_arvore *no_atual;
		while(desempilhar(pilha_altura, &no_atual)){
			if(no_atual->dir){
				if(no_atual->esq){
					if(no_atual->dir->altura > no_atual->esq->altura)
						no_atual->altura = no_atual->dir->altura + 1;
					else
						no_atual->altura = no_atual->esq->altura + 1;
				}
				else
					no_atual->altura = no_atual->dir->altura + 1;
			}
			else{
				if(no_atual->esq)
					no_atual->altura = no_atual->esq->altura + 1;
				else
					no_atual->altura += 1;
			}
		}
		No_arvore *pt1 = new No_arvore;
		pt1->chave = x;
		pt1->dir = NULL;
		pt1->esq = NULL;
		pt1->nos_direita = 0;
		pt1->nos_esquerda = 0;
		pt1->altura = 1;
		if(f == 0)
			pt_raiz = pt1;
		else if(f == 2)
			pt->esq = pt1;
		else
			pt->dir = pt1;
	}
}

void remocao(int x, No_arvore *&pt_raiz){				//Função que remove um elemento de uma árvore
	Pilha<No_arvore*> *pilha_altura  = criar_pilha<No_arvore*>();
	int f = 0;
	No_arvore *pt = pt_raiz;
	No_arvore *pai = pt_raiz;
	while(pt){
		if(pt->chave == x){
			f = 1;
			break;
		}
		else{
			if(x < pt->chave){
				pt->nos_esquerda -= 1;
				if(!pt->esq)
					f = 2;
				else{
					pt = pt->esq;
				}
			}
			else{
				pt->nos_direita -= 1;
				if(!pt->dir)
					f = 3;
				else{
					pt = pt->dir;
				}
			}
			if(f > 1)
				break;
		}
	}
	
	if(f != 1){
		cout << "Elemento não existe" << endl;
		pt = pt_raiz;
		while(pt){
			if(pt->chave == x){
				f = 1;
				break;
			}
			else{
				if(x < pt->chave){
					pt->nos_esquerda += 1;
					if(!pt->esq)
						f = 2;
					else{
						pt = pt->esq;
					}
				}
				else{
					pt->nos_direita += 1;
					if(!pt->dir)
						f = 3;
					else{
						pt = pt->dir;
					}
				}
				if(f > 1)
					break;
			}
		}
	}
	else{
		f = 0;
		if(pai->chave == x)
			pai = NULL;
		else{
			while(pai){
				empilhar<No_arvore*>(pilha_altura, pai);
				if(x < pai->chave){
					if(pai->esq){
						if(pai->esq->chave == x){
							f = 2;
							break;
						}
						else
							pai = pai->esq;
					}
				}
				else{
					if(pai->dir){
						if(pai->dir->chave == x){
							f = 3;
							break;
						}
						else
							pai = pai->dir;
					}
				}
			}
		}
		if(!pt->esq){
			if(!pt->dir){
				if(f == 0)
					pt_raiz = NULL;
				else if(f == 2)
					pai->esq = NULL;
				else
					pai->dir = NULL;
			}
			else{
				if(f == 0)
					pt_raiz = pt_raiz->dir;
				else if(f == 2)
					pai->esq = pt->dir;
				else
					pai->dir = pt->dir;
			}
			
			No_arvore *no_atual;
			while(desempilhar(pilha_altura, &no_atual)){
				if(no_atual->dir){
					if(no_atual->esq){
						if(no_atual->dir->altura > no_atual->esq->altura)
							no_atual->altura = no_atual->dir->altura + 1;
						else
							no_atual->altura = no_atual->esq->altura + 1;
					}
					else
						no_atual->altura = no_atual->dir->altura;
				}
				else{
					if(no_atual->esq)
						no_atual->altura = no_atual->esq->altura;
					else
						no_atual->altura -= 1;
				}
			}
		}
		else{
			if(!pt->dir){
				if(f == 0)
					pt_raiz = pt_raiz->esq;
				else if(f == 2)
					pai->esq = pt->esq;
				else
					pai->dir = pt->esq;
					
				No_arvore *no_atual;
				while(desempilhar(pilha_altura, &no_atual)){
					if(no_atual->dir){
						if(no_atual->esq){
							if(no_atual->dir->altura > no_atual->esq->altura)
								no_atual->altura = no_atual->dir->altura + 1;
							else
								no_atual->altura = no_atual->esq->altura + 1;
						}
						else
							no_atual->altura = no_atual->dir->altura;
					}
					else{
						if(no_atual->esq)
							no_atual->altura = no_atual->esq->altura;
						else
							no_atual->altura -= 1;
					}
				}
			}
			else{
				No_arvore *pt1 = pt->esq;
				if(pt1->dir){
					while(pt1->dir){
						empilhar<No_arvore*>(pilha_altura, pt1);
						pt1->nos_direita -= 1;
						pt1 = pt1->dir; 
					}
					pt->esq->dir = pt1->esq;
					pt1->dir = pt->dir;
					pt1->esq = pt->esq;
					pt1->nos_direita = pt->nos_direita;
					pt1->nos_esquerda = pt->nos_esquerda - 1;
				}
				else{
					pt1->dir = pt->dir;
					pt1->nos_direita = pt->nos_direita;
					empilhar<No_arvore*>(pilha_altura, pt1);
				}
				if(f == 0)
					pt_raiz = pt1;
				else if(f == 2)
					pai->esq = pt1;
				else
					pai->dir = pt1;
					
				No_arvore *no_atual;
				while(desempilhar(pilha_altura, &no_atual)){
					if(no_atual->dir){
						if(no_atual->esq){
							if(no_atual->dir->altura > no_atual->esq->altura)
								no_atual->altura = no_atual->dir->altura + 1;
							else
								no_atual->altura = no_atual->esq->altura + 1;
						}
						else
							no_atual->altura = no_atual->dir->altura + 1;
					}
					else{
						if(no_atual->esq)
							no_atual->altura = no_atual->esq->altura + 1;
						else
							no_atual->altura -= 1;
					}
				}
				if(pt1->dir){
					if(pt1->esq){
						if(pt1->dir->altura > pt1->esq->altura)
							pt1->altura = pt1->dir->altura + 1;
						else
							pt1->altura = pt1->esq->altura + 1;
					}
					else
						pt1->altura = pt1->dir->altura + 1;
				}
				else{
					if(pt1->esq)
						pt1->altura = pt1->esq->altura + 1;
					else
						pt1->altura -= 1;
				}
			}
		}
	}
}

string toString(No_arvore *pt_raiz){					//Função que retorna uma String que contém a sequência de visitação (percorrimento) da ABB por nível
	No_arvore *pt = pt_raiz;
	string resultado = "";
	Fila<No_arvore*> *fila_nivel = criar_fila<No_arvore*>();
	while(pt){
		resultado = resultado + to_string(pt->chave) + " ";
		No_arvore *no_atual;
		if(pt->esq) enfileirar<No_arvore*>(fila_nivel, pt->esq);
		if(pt->dir) enfileirar<No_arvore*>(fila_nivel, pt->dir);
		if(desenfileirar<No_arvore*>(fila_nivel, &no_atual))
			pt = no_atual;
		else
			break;
	}
	return resultado;
}

int enesimo(No_arvore *pt_raiz, int n){					//Função que retorna o n-ésimo elemento (contando a partir de 1) do percurso em ordem (ordem simétrica) da ABB.
	if(pt_raiz){
		if(n <= pt_raiz->nos_esquerda){
			if(pt_raiz->esq)
				enesimo(pt_raiz->esq, n);
			else
				cout << "Não existe nenhum elemento nessa posição" << endl;
		}
		else if(pt_raiz->nos_esquerda + 1 == n)
			return pt_raiz->chave;
		else{
			if(pt_raiz->dir)
				enesimo(pt_raiz->dir,  n - 1 - pt_raiz->nos_esquerda);
			else
				cout << "Não existe nenhum elemento nessa posição" << endl;
		}
	}
	else
		cout <<  "A árvore está vazia" << endl;
}

int posicao(No_arvore *pt_raiz, int n){					//Função que retorna a posição ocupada pelo elemento x em um percurso em ordem simétrica na ABB (contando a partir de 1)
	No_arvore *pt = pt_raiz;
	if(!pt)
		cout << "A árvore está vazia" << endl;
	else if(pt->chave == n)
		return pt->nos_esquerda + 1;
	else if(pt->chave > n)
		return posicao(pt->esq, n);
	else
		return pt->nos_esquerda + 1 + posicao(pt->dir, n);
}

int mediana(No_arvore *pt_raiz){						//Função que retorna o elemento que contém a mediana da ABB. Se a ABB possuir um número par de elementos, retorne o menor dentre os dois elementos medianos
	No_arvore *pt = pt_raiz;
	if(pt){
		int m = pt->nos_direita + pt->nos_esquerda + 1;
		if(m%2 != 0 && m != 0)
			return enesimo(pt_raiz, (m/2) + 1);
		else if(m%2 == 0 && m != 0)
			return ((enesimo(pt_raiz, (m/2)) + enesimo(pt_raiz, (m/2) + 1))/2);
	}
	else
		cout << "A árvore está vazia" << endl;
}

bool ehCheia(No_arvore *pt_raiz){						//Função que retorna verdadeiro se a ABB for uma árvore binária cheia e falso, caso contrário
	if(pt_raiz && ((pow(2,pt_raiz->altura) - 1) == pt_raiz->nos_direita + pt_raiz->nos_esquerda + 1))
		return true;
	return false;
}

bool ehCompleta(No_arvore *pt_raiz){					//Fumção que retorna verdadeiro se a ABB for uma árvore binária completa
	No_arvore *pt = pt_raiz;
	Fila<No_arvore*> *fila_nivel = criar_fila<No_arvore*>();
	if(!pt)
		return false;
	while(pt){
		No_arvore *no_atual;
		if(pt->esq)
			enfileirar<No_arvore*>(fila_nivel, pt->esq);
		else if(pt->altura > 2)
			return false;
		if(pt->dir)
			enfileirar<No_arvore*>(fila_nivel, pt->dir);
		else if(pt->altura > 2)
			return false;
		if(desenfileirar<No_arvore*>(fila_nivel, &no_atual))
			pt = no_atual;
		else
			return true;
	}
}

//ARQUIVOS
Fila<int>* ler_arvore(){								//Função que lê o arquivo contendo os nós das árvores e retorna uma fila com eles
	Fila<int> *fila = criar_fila<int>();
	int line;
	ifstream myfile("arvore.txt");
	if(myfile.is_open()){
		while (myfile >> line)
	        enfileirar(fila, line);
	}
	else cout << "Unable to open file";
	return fila;
}

Fila<string>* ler_operacoes(){							//Função que lê o aruivo contendo as operações a serem feitas pelo programa e retorna a fila com eles
	Fila<string> *fila = criar_fila<string>();
	string line;
	ifstream myfile ("operacoes.txt");
	if(myfile.is_open()){
		while (!myfile.eof()){
			getline (myfile,line);
			enfileirar(fila, line);
		}
		myfile.close();
	}
	else cout << "Unable to open file";
	return fila;
}

void construir_arvore(No_arvore *&pt_raiz){				//Função que constrói a ABB a portir do arquivo contendo os elementos dela
	Fila<int> *fila_insercao;
	fila_insercao = ler_arvore();
	int num;
	while(desenfileirar(fila_insercao, &num))
		insercao(num, pt_raiz);
}

void executar_operacores(No_arvore *&pt_raiz){			//Função que executa as operações que estão no arquivo contendo-as
	Fila<string> *fila_operacoes;
	fila_operacoes = ler_operacoes();
	string operacao;
	while(desenfileirar(fila_operacoes, &operacao)){
		cout << "-----------" << operacao << "-------------" << endl;
		if(operacao == "MEDIANA")
			cout << mediana(pt_raiz) << endl;
		else if(operacao == "CHEIA"){
			if(ehCheia(pt_raiz))
				cout << "A árvore é cheia" << endl;
			else
				cout << "A árvore não é cheia" << endl;
		}
		else if(operacao == "COMPLETA"){
			if(ehCompleta(pt_raiz))
				cout << "A árvore é completa" << endl;
			else
				cout << "A árvore não é completa" << endl;
		}
		else if(operacao == "IMPRIMA"){
			cout << toString(pt_raiz) << endl;
		}
		else{
			int num, found;
    		found = operacao.find_first_of(" ");
    		string comando = operacao.substr(0, found), str_num = operacao.substr(found+1);
			num = stoi(str_num, nullptr, 10);
			if(comando == "ENESIMO")
				cout << enesimo(pt_raiz, num) << endl;
			else if(comando == "POSICAO")
				cout << posicao(pt_raiz, num) << endl;
			else if(comando == "REMOVA")
				remocao(num, pt_raiz);
			else
				cout << "Comando: '" << comando << "' inválido" << endl;
		}
		cout << "--------------------------------" << endl;
	}
}

int main(){
	No_arvore *pt_raiz = NULL;
	construir_arvore(pt_raiz);
	executar_operacores(pt_raiz);
	return 0;
}
