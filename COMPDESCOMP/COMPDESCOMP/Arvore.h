#pragma once
#include "NoArvore.h"

class Arvore
{
	private:
		NoArvore *raiz, *atual, *antecessor;
	public:
		Arvore();
		Arvore(NoArvore*);
		void IncluirRaiz(NoArvore*);
		void IncluirAEsq(NoArvore*);
		void IncluirADir(NoArvore*);
		void Codificar(NoArvore*, unsigned int, unsigned int);
		unsigned int BuscarCodigo(NoArvore*, unsigned int);
		NoArvore* getRaiz();
		NoArvore* getAtual();
		NoArvore* getAntecessor();
		void setRaiz(NoArvore*);
		void setAtual(NoArvore*);
		void setAntecessor(NoArvore*);
};