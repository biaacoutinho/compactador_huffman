#pragma once
#include "NoLista.h"

class Lista
{
	private:
		NoLista* atual;
		NoLista* primeiro;
		NoLista* ultimo;
		int tamanho;

	public:
		Lista();
		Lista(Lista*);
		int Tamanho();
		void Inserir(NoLista*);
		void InserirNoFim(NoLista*);
		NoArvore* Remover();
		void Ordenar();
		NoLista* getAtual();
		NoLista* getPrimeiro();
		NoLista* getUltimo();
		void setAtual(NoLista*);
		void setPrimeiro(NoLista*);
		void setUltimo(NoLista*);
};