#include <stdlib.h>
#include <iostream>
#include <string.h>
#include "Arvore.h"

Arvore::Arvore()
{
    this->raiz = nullptr;
    this->antecessor = nullptr;
    this->atual = nullptr;
}

Arvore::Arvore(NoArvore* r)
{
    this->raiz = r;
    this->antecessor = nullptr;
    this->atual = nullptr;
}

void Arvore::IncluirRaiz(NoArvore* dado)
{
    this->raiz = dado;
}

void Arvore::IncluirAEsq(NoArvore* dado)
{
    if (this->raiz != nullptr)
    {
        this->raiz->setEsq(dado);
    }
}

void Arvore::IncluirADir(NoArvore* dado)
{
    if (this->raiz != nullptr)
    {
        this->raiz->setDir(dado);
    }
}

void Arvore::Codificar(NoArvore* no, char* codigo)
{
    if (no != nullptr)
    {
        char a[11];
        strcpy_s(a, codigo);
        strcat_s(a, 11, "0");
        Codificar(no->getEsq(), a);
        no->setCodigo(codigo);
        char b[11];
        strcpy_s(b, codigo);
        strcat_s(b, 11, "1");
        Codificar(no->getDir(), b);
    }
}

char* Arvore::BuscarCodigo(NoArvore* no, char procurado)
{
    if (no == nullptr)
        return nullptr;

    if (procurado == no->getCaracter())
        return no->getCodigo();

    char* codigoEsq = BuscarCodigo(no->getEsq(), procurado);
    if (codigoEsq != nullptr)
        return codigoEsq;

    char* codigoDir = BuscarCodigo(no->getDir(), procurado);
    return codigoDir;
}

NoArvore* Arvore::getRaiz()
{
    return this->raiz;
}

NoArvore* Arvore::getAtual()
{
    return this->atual;
}

NoArvore* Arvore::getAntecessor()
{
    return this->antecessor;
}

void Arvore::setRaiz(NoArvore* r)
{
    this->raiz = r;
}

void Arvore::setAtual(NoArvore* n)
{
    this->atual = n;
}

void Arvore::setAntecessor(NoArvore* n)
{
    this->antecessor = n;
}