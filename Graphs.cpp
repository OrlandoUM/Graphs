#include<stdlib.h>
#include<stdio.h>
#include<conio.h>
#include<ctype.h>

//numero maximo de vertices
#define  MAX_NUM_VERT  50

//entrada da lista de adjacencias
struct no {
	int vertice;
	no* prox;
};

// variaveis globais que representam o grafo
int numVertices = 0;
no* lista_adj[MAX_NUM_VERT];

// prototipos das funcoes auxiliares
void inicializa_grafo(no* adj[]);
void insere_adjacente(no* adj[], int v, int sucessor);
void desaloca_grafo(no* adj[], int n);
void ler_grafo(char caminho[100]);
void imprime_grafo(no* adj[], int n);
void ConverterListaParaMatriz(no* adj[], int x);
void exibeMat(int mat[][7]);
void DeterminarGrau(no* adj[]);
void ExibeGrau(int vet[]);
void GrauMinMax(int vet[]);
void ConverterMatrizParaLista(int mat[][7], no* aux[]);
void DeterminaConexaoLista(no* aux[]);
void DeterminaConexaoMatriz(int matriz[][7]);
void RemoveAresta(no* aux[]);
int VerificaVertice(no* aux[], int v1, int v2);
void Unstack(no* aux[], int v1, int v2);

// Funcao principal (ponto de entrada do programa).
int main() {
	//inicializa o grafo
	int x;
	inicializa_grafo(lista_adj);

	//le grafo a partir do arquivo
	ler_grafo("graph.txt");
	x = numVertices;

	//imprime as listas de adjacencia do grafo
	imprime_grafo(lista_adj, numVertices);

	ConverterListaParaMatriz(lista_adj, x);

	DeterminarGrau(lista_adj);

	DeterminaConexaoLista(lista_adj);

	RemoveAresta(lista_adj);

	//libera memoria
	desaloca_grafo(lista_adj, numVertices);

	//espera digitar um caracter
	getch();

	return 0;
}

int VerificaVertice(no* aux[], int v1, int v2) {
	int i;
	no* a;
	a = aux[v1];
	while (a != NULL) {
		if (a->vertice == v2) {
			return 1; //TRUE
		}
		a = a->prox;
	}
	return 0; //FALSE
}

void Unstack(no* aux[], int v1, int v2) {
	no* aux2;
	no* aux3;
	if (aux[v1]->prox == NULL && aux[v1]->vertice == v2) {
		delete aux[v1];
		aux[v1] = NULL;
	}
	else {
		aux2 = aux[v1]->prox;
		aux3 = aux[v1];
		if (aux[v1]->prox != NULL && aux[v1]->vertice == v2) {
			delete aux3;
			aux[v1] = aux2;
			aux3 = NULL;
		}
		else {
			while (aux2 != NULL) {
				if (aux2->vertice == v2) {
					aux3->prox = aux2->prox;
					delete aux2;
					aux2 = NULL;
					return;
				}
				aux2 = aux2->prox;
				aux3 = aux3->prox;
			}
		}
	}
}

void RemoveAresta(no* aux[]) {
	int v1, v2, retorno;
	printf("\n\n-----------REMOCAO DA ARESTA----------\n");
	printf("Informe o primeiro vertice:");
	scanf("%d", &v1);
	if (v1 < 0 || v1 >= numVertices) {
		printf("\n\nPRIMEIRO VERTICE NÃO ENCONTRADO");
		return;
	}
	printf("Informe o segundo vertice:");
	scanf("%d", &v2);
	if (v1 == v2) {
		printf("\n\nVERTICES COM MESMO VALOR");
		return;
	}
	retorno = VerificaVertice(aux, v1, v2);
	if (retorno == 0) {
		printf("\n\nSEGUNDO VERTICE NAO ENCONTRADO");
		return;
	}
	Unstack(aux, v1, v2);
	printf("\n");
	imprime_grafo(aux, numVertices);
}

void DeterminaConexaoLista(no* aux[]) {
	int i;
	for (i = 0; i < numVertices; i++) {
		if (aux[i] == NULL) {
			printf("\n\nGRAFO NAO CONEXO - Via Lista");
			return;
		}
	}
	printf("\n\nGRAFO CONEXO - Via Lista");
}

void DeterminaConexaoMatriz(int matriz[][7]) {
	int i, j, soma = 0;
	for (i = 0; i < numVertices; i++) {
		for (j = 0; j < numVertices; j++) {
			soma = soma + matriz[i][j];
		}
		if (soma == 0) {
			printf("\n\nGRAFO CONEXO - Via Matriz");
			return;
		}
		soma = 0;
	}
	printf("\n\nGRAFO NÃO CONEXO - Via Matriz");
}

void ConverterMatrizParaLista(int mat[][7], no* aux[]) {
	int i, j;
	printf("\n\n-------------CONVERCAO PARA LISTA--------------\n");

	desaloca_grafo(aux, numVertices);

	for (i = 0; i < numVertices; i++) {
		for (j = 0; j < numVertices; j++) {
			if (mat[i][j] == 1) {
				insere_adjacente(aux, i, j);
			}
		}
	}
	imprime_grafo(aux, i);
}

void DeterminarGrau(no* adj[]) {
	int vet[7];
	int i, cont = 0;
	no* aux;
	for (i = 0; i < numVertices; i++) {
		aux = adj[i];
		while (aux != NULL) {
			cont++;
			aux = aux->prox;
		}
		vet[i] = cont;
		cont = 0;
	}
	ExibeGrau(vet);
	GrauMinMax(vet);
}

void GrauMinMax(int vet[]) {
	int i, min, max;
	min = vet[0];
	max = vet[0];
	for (i = 1; i < numVertices; i++) {
		if (vet[i] < min) {
			min = vet[i];
		}
		if (vet[i] > max) {
			max = vet[i];
		}
	}
	printf("\n\nGRAU MAXIMO: %d", max);
	printf("\nGRAU MINIMO: %d", min);
}

void ExibeGrau(int vet[]) {
	int i;
	printf("\n\n -------------DETERMINAR GRAU----------\n");
	for (i = 0; i < numVertices; i++) {
		printf("VET[%d] = %d\n", i, vet[i]);
	}
}

void ConverterListaParaMatriz(no* adj[], int x) {
	int i, j;
	no* aux;
	no* aux2[7];
	int matriz[7][7];
	for (i = 0; i < x; i++) {
		aux = adj[i];
		while (aux != NULL) {
			matriz[i][aux->vertice] = 1;
			aux = aux->prox;
		}
	}
	for (i = 0; i < numVertices; i++) {
		for (j = 0; j < numVertices; j++) {
			if (matriz[i][j] != 1) {
				matriz[i][j] = 0;
			}
		}
	}
	exibeMat(matriz);
	DeterminaConexaoMatriz(matriz);
	ConverterMatrizParaLista(matriz, adj);
}

void exibeMat(int mat[][7]) {
	int i, j;
	printf("\n\n------------CONVERCAO PARA MATRIZ-------------\n");
	for (i = 0; i < numVertices; i++) {
		for (j = 0; j < numVertices; j++) {
			printf("%d  ", mat[i][j]);
		}
		printf("\n");
	}
	printf("\n\n");
}

// Inicializa as listas de adjacencias.
void inicializa_grafo(no* adj[]) {
	for (int i = 0; i < MAX_NUM_VERT; i++) {
		adj[i] = NULL;
	}
}

// Insere o vertice "sucessor" no final da lista de adjacencias 
// de "v", ou seja, insere o arco (v -> sucessor)
void insere_adjacente(no* adj[], int v, int sucessor) {
	no* aux;
	no* novo;

	novo = new no;
	novo->vertice = sucessor;
	novo->prox = NULL;

	if (adj[v] == NULL) {
		// se for o primeiro elemento inserido na lista, 
		// a cabeça da lista passa a apontar para ele
		adj[v] = novo;

	}
	else {
		// se nao for o primeiro, percorre a lista inteira
		// e insere o elemento após a ultima posição
		aux = adj[v];

		while (aux->prox != NULL)
			aux = aux->prox;

		aux->prox = novo;
	}
}

// Le um grafo a partir do arquivo passado por parametro
void ler_grafo(char caminho[100]) {
	FILE* arquivo;
	int v, sucessor;

	arquivo = fopen(caminho, "r");

	if (arquivo == NULL) {
		printf("Erro: nao foi possivel abrir o arquivo.\n");

	}
	else {
		//le a primeira linha		
		fscanf(arquivo, "%d", &numVertices);

		//le cada uma das demais linhas
		for (v = 0; v < numVertices; v++) {
			fscanf(arquivo, "%d", &sucessor);

			while (sucessor != -1) {
				insere_adjacente(lista_adj, v, sucessor);
				fscanf(arquivo, "%d", &sucessor);
			}
		}

		fclose(arquivo);
	}

}

// Imprime as listas de adjacencias dadas
void imprime_grafo(no* adj[], int n) {
	int i;
	no* aux;

	for (i = 0; i < n; i++) {
		//imprime o vertice
		printf("Adj[%d] - ", i);

		//imprime os sucessores do vertice
		aux = adj[i];
		while (aux != NULL) {
			printf("%d->", aux->vertice);
			aux = aux->prox;
		}

		printf(".\n");
	}
}

// Percorre as listas desalocando memoria
void desaloca_grafo(no* adj[], int n) {
	int i;
	no* anterior;
	no* atual;

	for (i = 0; i < n; i++) {
		atual = adj[i];
		while (atual != NULL) {
			anterior = atual;
			atual = atual->prox;
			delete anterior;
			anterior = NULL;
		}
		adj[i] = atual;
	}
}