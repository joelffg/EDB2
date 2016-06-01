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
	int chave;
	char cor;
    struct no_arvore *dir;
    struct no_arvore *pai;
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
		if(fila->tamanho == 0)
			fila->fim = NULL;
		return 1;
	}
	return 0;
}

//ÁRVORE
void rotacao_direita(No_arvore *no, No_arvore *&pt_raiz){
	if(no){
		if(no->esq){
			No_arvore *aux = no->esq;
			no->esq = aux->dir;
			if(aux->dir)
				aux->dir->pai = no;
			aux->pai = no->pai;
			if(!no->pai)
				pt_raiz = aux;
			else if(no->pai->esq == no)
				no->pai->esq = aux;
			else
				no->pai->dir = aux;
			aux->dir = no;
			no->pai = aux;
		}
	}
}

void rotacao_esquerda(No_arvore *no, No_arvore *&pt_raiz){
	if(no){
		if(no->dir){
			No_arvore *aux = no->dir;
			no->dir = aux->esq;
			if(aux->esq)
				aux->esq->pai = no;
			aux->pai = no->pai;
			if(!no->pai)
				pt_raiz = aux;
			else if(no->pai->esq == no)
				no->pai->esq = aux;
			else
				no->pai->dir = aux;
			aux->esq = no;
			no->pai = aux;
		}
	}
}

void reparacao_insercao(No_arvore *&no, No_arvore *&pt_raiz){
	No_arvore *x = no;
	if(x->pai){
		if(x->pai->pai){
			while(x->pai->cor == 'V'){
				if(x->pai == x->pai->pai->esq){			//Pai a esquerda do avô
					if(x->pai->pai->dir && x->pai->pai->dir->cor == 'V'){	//Caso 1
						x->pai->pai->dir->cor = 'P';
						x->pai->cor = 'P';
						x->pai->pai->cor = 'V';
						x = x->pai->pai;
					}
					else{
						if(x == x->pai->dir){			//Caso 2
							rotacao_esquerda(x->pai, pt_raiz);
							x = x->esq;
						}
						else{							//Caso 3
							rotacao_direita(x->pai->pai, pt_raiz);
							x->pai->cor = 'P';
							x->pai->dir->cor = 'V';
						}
					}
				}
				else{									//Pai a direita do avô
					if(x->pai->pai->esq && x->pai->pai->esq->cor == 'V'){	//Caso 1
						x->pai->pai->esq->cor = 'P';
						x->pai->cor = 'P';
						x->pai->pai->cor = 'V';
						x = x->pai->pai;
					}
					else{
						if(x == x->pai->dir){			//Caso 2
							rotacao_esquerda(x->pai->pai, pt_raiz);
							x->pai->cor = 'P';
							x->pai->esq->cor = 'V';
						}
						else{							//Caso 3
							rotacao_direita(x->pai, pt_raiz);
							x = x->dir;
						}
					}
				}
				if(!x->pai){
					x->cor = 'P';
					break;
				}
			}
		}
	}
}

void reparacao_remocao(No_arvore *&no, No_arvore *&pt_raiz){
	No_arvore *z = no;
	while(z != pt_raiz && z->cor == 'P'){
		if(z->chave < z->pai->chave){
			No_arvore *w = z->pai->dir;
			if(w->cor == 'V'){														//Caso 1
				w->cor = 'P';
				z->pai->cor = 'V';
				rotacao_esquerda(z->pai, pt_raiz);
				w = z->pai->dir;
			}
			if((!w->esq || w->esq->cor == 'P') && (!w->dir || w->dir->cor == 'P')){	//Caso 2
				w->cor = 'V';
				z = z->pai;
			}
			else{
				if(!w->dir || w->dir->cor == 'P'){									//Caso 3
					if(w->esq)
						w->esq->cor = 'P';
					w->cor = 'V';
					rotacao_direita(w, pt_raiz);
					w = z->pai->dir;
				}
				w->cor = z->pai->cor;												//Caso 4
				z->pai->cor = 'P';
				if(w->dir)
					w->dir->cor = 'P';
				rotacao_esquerda(z->pai, pt_raiz);
				z = pt_raiz;
			}
		}
		else{
			No_arvore *w = z->pai->esq;
			if(w->cor == 'V'){														//Caso 1
				w->cor = 'P';
				z->pai->cor = 'V';
				rotacao_esquerda(z->pai, pt_raiz);
				w = z->pai->esq;
			}
			if((!w->dir || w->dir->cor == 'P') && (!w->esq || w->esq->cor == 'P')){	//Caso 2
				w->cor = 'V';
				z = z->pai;
			}
			else{
				if(!w->esq || w->esq->cor == 'P'){									//Caso 3
					if(w->dir)
						w->dir->cor = 'P';
					w->cor = 'V';
					rotacao_direita(w, pt_raiz);
					w = z->pai->esq;
				}
				w->cor = z->pai->cor;												//Caso 4
				z->pai->cor = 'P';
				if(w->esq)
					w->esq->cor = 'P';
				rotacao_esquerda(z->pai, pt_raiz);
				z = pt_raiz;
			}
		}
	}
	z->cor = 'P';
}

No_arvore* busca_arn(No_arvore *&pt, int x, int *f){
	if(pt){
		if(pt->chave == x){
			*f = 1;
			return pt;
		}
		else{
			if(x < pt->chave){
				if(!pt->esq)
					*f = 2;
				else
					pt = pt->esq;
			}
			else{
				if(!pt->dir)
					*f = 3;
				else
					pt = pt->dir;
			}
			if(*f < 1)
				busca_arn(pt, x, f);
			else
				return pt;
		}
	}
}

void insercao(int x, No_arvore *&pt_raiz){				//Função que insere um elemento em uma árvore
	int f;
	No_arvore *pt = pt_raiz;
	while(pt){
		if(pt->chave == x){
			f = 1;
			break;
		}
		else{
			if(x < pt->chave){
				if(!pt->esq)
					f = 2;
				else{
					pt = pt->esq;
				}
			}
			else{
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
					if(!pt->esq)
						f = 2;
					else{
						pt = pt->esq;
					}
				}
				else{
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
		No_arvore *pt1 = new No_arvore;
		pt1->chave = x;
		pt1->dir = NULL;
		pt1->esq = NULL;
		pt1->pai = NULL;
		pt1->cor = 'V';
		if(f == 0){
			pt1->cor = 'P';
			pt_raiz = pt1;
		}
		else if(f == 2){
			pt1->pai = pt;
			pt->esq = pt1;
		}
		else{
			pt1->pai = pt;
			pt->dir = pt1;
		}
		reparacao_insercao(pt1, pt_raiz);
	}
}

void remocao(int x, No_arvore *&pt_raiz){				//Função que remove um elemento de uma árvore
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
				if(!pt->esq)
					f = 2;
				else{
					pt = pt->esq;
				}
			}
			else{
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
					if(!pt->esq)
						f = 2;
					else{
						pt = pt->esq;
					}
				}
				else{
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
				pt->dir->pai = pai;
			}
			if(pt->cor == 'P'){
				reparacao_remocao(pt, pt_raiz);
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
				pt->esq->pai = pai;	
				if(pt->cor == 'P'){
					reparacao_remocao(pt, pt_raiz);
				}
			}
			else{
				No_arvore *pt1 = pt->esq;
				if(pt1->dir){
					while(pt1->dir){
						pt1 = pt1->dir; 
					}
					pt1->pai->dir = pt1->esq;
					pt1->dir = pt->dir;
					pt1->esq = pt->esq;
				}
				else{
					pt1->dir = pt->dir;
					pt->dir->pai = pt1;
				}
				if(f == 0){
					pt_raiz->chave = pt1->chave;
					pt_raiz->dir = pt1->dir;
					pt_raiz->esq = pt1->esq;
				}
				else if(f == 2){
					pai->esq = pt1;
					pt1->pai = pt->pai;
				}
				else{
					pai->dir = pt1;
					pt1->pai = pt->pai;
				}
				pt1->dir->pai = pt1;
				if(pt1->cor == 'P')
					reparacao_remocao(pt1, pt_raiz);
				pt1->cor = pt->cor;
			}
		}
	}
}

string toString(No_arvore *pt_raiz){					//Função que retorna uma String que contém a sequência de visitação (percorrimento) da ABB por nível
	No_arvore *pt = pt_raiz;
	string resultado = "";
	Fila<No_arvore*> *fila_nivel = criar_fila<No_arvore*>();
	while(pt){
		resultado = resultado + to_string(pt->chave) + " " + pt->cor + " - ";
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
	while(desenfileirar(fila_insercao, &num)){
		insercao(num, pt_raiz);
	}
}

void executar_operacores(No_arvore *&pt_raiz){			//Função que executa as operações que estão no arquivo contendo-as
	Fila<string> *fila_operacoes;
	fila_operacoes = ler_operacoes();
	string operacao;
	while(desenfileirar(fila_operacoes, &operacao)){
		cout << "-----------" << operacao << "-------------" << endl;
		if(operacao == "IMPRIMA"){
			cout << toString(pt_raiz) << endl;
		}
		else{
			int num, found, f = 0;
    		found = operacao.find_first_of(" ");
    		string comando = operacao.substr(0, found), str_num = operacao.substr(found+1);
			num = stoi(str_num, nullptr, 10);
			if(comando == "BUSCA"){
				int busca = busca_arn(pt_raiz, num, &f)->chave;
				if(f != 1)
					cout << "O elemento buscado não existe na árvore" << endl;
				else
					cout << busca << endl;		
			}
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
