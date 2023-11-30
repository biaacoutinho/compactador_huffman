#pragma once
#include "NoArvore.h"

class NoLista
{
private:
	NoArvore* dado;
	NoLista* prox;
	NoLista* ant;

public:
	NoLista();
	NoLista(NoArvore*);
	NoArvore* getDado(); 
	NoLista* getProx();
	NoLista* getAnt();
	void setDado(NoArvore);
	void setProx(NoLista*);
	void setAnt(NoLista*);
};