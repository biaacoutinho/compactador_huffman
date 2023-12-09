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

void Arvore::Codificar(NoArvore* no, unsigned int codigo, unsigned int tamanho) //raiz, 0, 0
{
    if (no != nullptr)
    {
        /*char a[11];
        strcpy_s(a, codigo);
        strcat_s(a, 11, "0");
        Codificar(no->getEsq(), a);
        no->setCodigo(codigo);
        char b[11];
        strcpy_s(b, codigo);
        strcat_s(b, 11, "1");
        Codificar(no->getDir(), b);*/

        Codificar(no->getEsq(), (codigo << 1) | 0, tamanho + 1);
        no->setCodigo(codigo);
        no->setTamanho(tamanho);
        Codificar(no->getDir(), (codigo << 1) | 1, tamanho + 1);
    }
}

unsigned int Arvore::BuscarCodigo(NoArvore* no, char procurado)
{
    if (no == nullptr)
        return INT16_MAX;

    if (procurado == no->getCaracter())
    {
        setAtual(no);
        return no->getCodigo();
    }
    
    unsigned int codigoEsq = BuscarCodigo(no->getEsq(), procurado);
    if (codigoEsq != INT16_MAX)
        return codigoEsq;
    unsigned int codigoDir = BuscarCodigo(no->getDir(), procurado);
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