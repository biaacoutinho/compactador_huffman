#include "NoLista.h"

NoLista::NoLista()
{
	this->dado = nullptr;
	this->ant = nullptr;
	this->prox = nullptr;
}

NoLista::NoLista(NoArvore* info)
{
	this->dado = info;
	this->prox = nullptr;
	this->ant = nullptr;
}

NoArvore* NoLista::getDado()
{
	return this->dado;
}

NoLista* NoLista::getProx()
{
	return this->prox;
}

NoLista* NoLista::getAnt()
{
	return this->ant;
}

void NoLista::setDado(NoArvore info)
{
	*this->dado = info;
}

void NoLista::setProx(NoLista* n)
{
	this->prox = n;
}

void NoLista::setAnt(NoLista* n)
{
	this->ant = n;
}