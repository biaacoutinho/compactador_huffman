#pragma once
typedef struct NoArvore {
	char caracter;
	unsigned int frequencia;
	NoArvore* dir;
	NoArvore* esq;
}NoArvore;

class Arvore
{
	private:
		NoArvore raiz, atual, antecessor;
	public:
		void incluirNo(char, unsigned int);
};