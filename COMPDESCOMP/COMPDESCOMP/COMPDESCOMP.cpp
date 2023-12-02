#include <iostream>
#include <string.h>
#include <fstream>
#include <string.h>
#include "Arvore.h"
#include "Lista.h"

using namespace std;

Lista* filaPrioridade;

char* nomeArquivo = new char[50];

static void CriarFilaDePrioridade()
{
    ifstream arq(nomeArquivo, ios::binary);
    filaPrioridade = new Lista();

    //Separa caracteres e define suas frequencias
    for (char c; arq.get(c); )
    {
        char cRepetido = 0;
        for (filaPrioridade->setAtual(filaPrioridade->getPrimeiro());
            filaPrioridade->getAtual() != nullptr;
            filaPrioridade->setAtual(filaPrioridade->getAtual()->getProx()))
        {
            if (c == filaPrioridade->getAtual()->getDado()->getCaracter())
            {
                filaPrioridade->getAtual()->getDado()->setFrequencia(filaPrioridade->getAtual()->getDado()->getFrequencia() + 1);
                cRepetido = 1;
                break;
            }
        }
        if (!cRepetido)
        {
            filaPrioridade->Inserir(new NoLista(new NoArvore(c, 1)));
        }
    }
    //Ordenar lista
    filaPrioridade->Ordenar();
    arq.close();
}

static Arvore* CriarArvore()
{
    while (filaPrioridade->Tamanho() >= 2)
    {
        NoArvore* nodo = new NoArvore();
        nodo->setEsq(filaPrioridade->Remover());
        nodo->setDir(filaPrioridade->Remover());
        nodo->setFrequencia(nodo->getEsq()->getFrequencia() + nodo->getDir()->getFrequencia());
        filaPrioridade->Inserir(new NoLista(nodo));
    }

    return new Arvore(filaPrioridade->Remover());
}

int main()
{
    cout << "Digite o caminho do arquivo que deseja compactar: ";
    cin >> nomeArquivo;

    CriarFilaDePrioridade();

    for (filaPrioridade->setAtual(filaPrioridade->getPrimeiro());
        filaPrioridade->getAtual() != nullptr;
        filaPrioridade->setAtual(filaPrioridade->getAtual()->getProx()))
    {
        NoArvore a = *filaPrioridade->getAtual()->getDado();
        cout << a.getCaracter() << ',' << a.getFrequencia() << '\n';
    }

    cout << filaPrioridade->Tamanho() << '\n';

    Arvore* arvore = CriarArvore();
    char codigo[11];
    strcpy_s(codigo, "");
    arvore->Codificar(arvore->getRaiz(), 0);

    //ofstream arq("B:\\ESCOLA\\MALIGNO\\compactador_huffman/compactado.dat", ios::binary);
    ofstream arq("D:\\ARMAG/compactado.dat", ios::binary);
    ifstream arqLeitura(nomeArquivo, ios::binary);

    for (char c; arqLeitura.get(c); )
    {
        unsigned int codigo = arvore->BuscarCodigo(arvore->getRaiz(), c);
        arq.write(codigo, sizeof(codigo));
    }
    
    arqLeitura.close();
    arq.close();
}