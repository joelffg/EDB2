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
	int nos_pretos_direita;
	int chave;
	char cor;
	int altura_nos_pretos;
	int nos_pretos_esquerda;
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
	fila->tamanho++;
}

template <typename Tipo>
int desenfileirar(Fila<Tipo> *fila, Tipo *num){			//Função que desenfileira um nó de uma fila
	if(fila->inicio){
		*num = fila->inicio->dado;
		No<Tipo> *aux = fila->inicio;
		fila->inicio = aux->proximo;
		delete aux;
		fila->tamanho--;
		return 1;
	}
	return 0;
}

//ÁRVORE

void rotacao_direita(No_arvore *&no){
	if(no){
		if(no->esq){
			No_arvore *aux = new No_arvore;
			aux = no->esq;
			no->esq = aux->dir;
			aux->dir = no;
			no = aux;
		}
	}
}

void rotacao_esquerda(No_arvore *&no){
}

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
//				pt->nos_esquerda += 1;
				if(!pt->esq)
					f = 2;
				else{
					pt = pt->esq;
				}
			}
			else{
//				pt->nos_direita += 1;
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
//					pt->nos_esquerda -= 1;
					if(!pt->esq)
						f = 2;
					else{
						pt = pt->esq;
					}
				}
				else{
//					pt->nos_direita -= 1;
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
/*		while(desempilhar(pilha_altura, &no_atual)){
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
*/		No_arvore *pt1 = new No_arvore;
		pt1->chave = x;
		pt1->dir = NULL;
		pt1->esq = NULL;
		pt1->nos_pretos_direita = 0;
		pt1->nos_pretos_esquerda = 0;
		pt1->altura_nos_pretos = 1;
		pt1->cor = 'v';
		if(f == 0)
			pt_raiz = pt1;
		else if(f == 2)
			pt->esq = pt1;
		else
			pt->dir = pt1;
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

int main(){
	No_arvore *pt_raiz = NULL;
	construir_arvore(pt_raiz);
	//executar_operacores(pt_raiz);
	cout << toString(pt_raiz) << endl;
	rotacao_direita(pt_raiz);
	cout << toString(pt_raiz) << endl;
	return 0;
}
