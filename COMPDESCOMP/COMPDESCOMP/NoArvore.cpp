#include "NoArvore.h"

NoArvore::NoArvore()
{
	this->caracter = 0;
	this->frequencia = 0;
	this->codigo = nullptr;
	this->dir = nullptr;
	this->esq = nullptr;
}

NoArvore::NoArvore(char c, unsigned int f)
{
	this->caracter = c;
	this->frequencia = f;
	this->codigo = nullptr;
	this->dir = nullptr;
	this->esq = nullptr;
}

char NoArvore::getCaracter()
{
	return this->caracter;
}

unsigned int NoArvore::getFrequencia()
{
	return this->frequencia;
}

char* NoArvore::getCodigo()
{
	return this->codigo;
}

NoArvore* NoArvore::getDir()
{
	return this->dir;
}

NoArvore* NoArvore::getEsq()
{
	return this->esq;
}

void NoArvore::setCaracter(char c)
{
	this->caracter = c;
}

void NoArvore::setFrequencia(unsigned int f)
{
	this->frequencia = f;
}

void NoArvore::setEsq(NoArvore* n)
{
	this->esq = n;
}

void NoArvore::setDir(NoArvore* n)
{
	this->dir = n;
}

void NoArvore::setCodigo(char* c)
{
	this->codigo = c;
}