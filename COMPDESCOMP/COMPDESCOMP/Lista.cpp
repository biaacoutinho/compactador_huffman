#include <iostream>
#include "Lista.h"

Lista::Lista()
{
	this->atual = nullptr;
	this->primeiro = nullptr;
	this->ultimo = nullptr;
}

void Lista::Inserir(NoLista* dado) 
{
	if (this->primeiro == nullptr)
	{
		this->primeiro = dado;
        this->ultimo = primeiro;
	}
    else
    {
        this->ultimo->setProx(dado);
        dado->setAnt(this->ultimo);
        this->ultimo = dado;
    }
    tamanho++;
}

NoArvore* Lista::Remover()
{
    if (primeiro != nullptr)
    {
        NoArvore* aRemover = this->primeiro->getDado();
        this->primeiro = primeiro->getProx();
        if (primeiro != nullptr)
            this->primeiro->setAnt(nullptr);

        this->tamanho--;

        return aRemover;
    }
    return nullptr;
}

void Lista::Ordenar()
{
    for (this->atual = this->primeiro; this->atual != nullptr; this->atual = this->atual->getProx())
    {
        NoArvore* menor = new NoArvore(0, INT16_MAX);
        for (NoLista* p = this->atual->getProx(); p != nullptr; p = p->getProx())
        {
            if (p->getDado()->getFrequencia() < menor->getFrequencia())
            {
                menor = p->getDado();
            }
        }
        if (menor->getFrequencia() < this->atual->getDado()->getFrequencia())
        {
            NoArvore temp = *this->atual->getDado();
            this->atual->setDado(*menor);
            *menor = temp;
        }
    }
}

int Lista::Tamanho()
{
    return this->tamanho;
}

NoLista* Lista::getAtual()
{
	return this->atual;
}

NoLista* Lista::getPrimeiro()
{
	return this->primeiro;
}

NoLista* Lista::getUltimo()
{
	return this->ultimo;
}

void Lista::setAtual(NoLista* n)
{
	this->atual = n;
}

void Lista::setPrimeiro(NoLista* n)
{
	this->primeiro = n;
}

void Lista::setUltimo(NoLista* n)
{
	this->ultimo = n;
}