#include <iostream>
#include <string.h>
#include <fstream>
#include <string.h>
#include "Arvore.h"
#include "Lista.h"

using namespace std;

Arvore* arvore;
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

static void Encerrar()
{
    delete arvore;
    delete filaPrioridade;
    delete nomeArquivo;
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

    arvore = CriarArvore();
    char codigo[11];
    strcpy_s(codigo, "");
    arvore->Codificar(arvore->getRaiz(), 0, 0);

    ofstream arq("B:\\ESCOLA\\MALIGNO\\compactador_huffman/compactado.dat", ios::binary);
    //ofstream arq("D:\\ARMAG/compactado.dat", ios::binary);
    ifstream arqLeitura(nomeArquivo, ios::binary);

    char byte = 0;
    int bitsEscritos = 0;
    for (char c; arqLeitura.get(c); )
    {
        unsigned int codigo = arvore->BuscarCodigo(arvore->getRaiz(), c);
        int tamCodigo = arvore->getAtual()->getTamanho();

        if (tamCodigo < (8 - bitsEscritos)) //verifica se tem espaço no byte para o codigo inteiro
        {
            byte <<= tamCodigo;
            byte |= codigo;
            bitsEscritos += tamCodigo;
            if (bitsEscritos == 8)
                arq.write(&byte, 1);
        }
        else
        {
            byte <<= (8 - bitsEscritos); //disponibiliza os bits restantes
            int bitsNGravados = (tamCodigo - (8 - bitsEscritos));
            unsigned int codAux = codigo >> bitsNGravados; //parte o codigo em dois,
            //a parte que cabe no bit atual(codAux),
            //e a parte que nao cabe
            byte |= codAux; // armazena a parte que cabe no byte atual

            arq.write(&byte, 1);//escreve o byte prontinho

            byte = 0; //zera o byteAtual denovo (00000000)

            unsigned __int8 mascara = ~(0b11111111 << bitsNGravados); //cria uma mascara para conseguir gravar
            //o numero certo de bits do codigo
            byte = codigo & mascara; //grava os bits restantes

            bitsEscritos = bitsNGravados; //atualiza o numero de bits gravados
        }

        char bitsNaoUsados = 8 - bitsEscritos;

        arq.seekp(0, ios::beg); //ajusta o ponteiro do arquivo pra primeira posicao
        arq.write(&bitsNaoUsados, 1); //escreve o numero de bits a ser desconsiderado

        //Escrever a arvore no arquivo
    }

    arqLeitura.close();
    arq.close();

    Encerrar(); //escrever destrutores nas classes, é preciso liberar a memoria de todos os ponteiros
}