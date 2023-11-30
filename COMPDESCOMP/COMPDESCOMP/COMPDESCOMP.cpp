// COMPDESCOMP.cpp : Este arquivo contém a função 'main'. A execução do programa começa e termina ali.
//

#include <iostream>
#include <string.h>
#include "Arvore.h"
#include "Lista.h"

using namespace std;

Lista* filaPrioridade;

char teste[] = "Eu amo programar, principalmente em c++! S2";

static void CriarFilaDePrioridade()
{
    filaPrioridade = new Lista();

    //Separa caracteres e define suas frequencias
    for (int i = 0; teste[i] != '\0'; i++) 

    {
        char cRepetido = 0;
        for (filaPrioridade->setAtual(filaPrioridade->getPrimeiro());
            filaPrioridade->getAtual() != nullptr;
            filaPrioridade->setAtual(filaPrioridade->getAtual()->getProx()))
        {
            if (teste[i] == filaPrioridade->getAtual()->getDado()->getCaracter())
            {
                filaPrioridade->getAtual()->getDado()->setFrequencia(filaPrioridade->getAtual()->getDado()->getFrequencia() + 1);
                cRepetido = 1;
                break;
            }
        }
        if (!cRepetido)
        {
            filaPrioridade->Inserir(new NoLista(new NoArvore(teste[i], 1)));
        }
    }
    //Ordenar lista
    filaPrioridade->Ordenar();
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
    arvore->Codificar(arvore->getRaiz(), codigo);
}


// Executar programa: Ctrl + F5 ou Menu Depurar > Iniciar Sem Depuração
// Depurar programa: F5 ou menu Depurar > Iniciar Depuração

// Dicas para Começar: 
//   1. Use a janela do Gerenciador de Soluções para adicionar/gerenciar arquivos
//   2. Use a janela do Team Explorer para conectar-se ao controle do código-fonte
//   3. Use a janela de Saída para ver mensagens de saída do build e outras mensagens
//   4. Use a janela Lista de Erros para exibir erros
//   5. Ir Para o Projeto > Adicionar Novo Item para criar novos arquivos de código, ou Projeto > Adicionar Item Existente para adicionar arquivos de código existentes ao projeto
//   6. No futuro, para abrir este projeto novamente, vá para Arquivo > Abrir > Projeto e selecione o arquivo. sln
