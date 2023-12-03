#include <iostream>
#include <string.h>
#include <fstream>
#include <string.h>
#include <filesystem>
#include "Arvore.h"
#include "Lista.h"

using namespace std;

Arvore* arvore;
Arvore* novaArvore;
Lista* filaPrioridade;
char abriuArquivo = 0;
char* nomeArquivo = new char[50];

static void CriarFilaDePrioridade()
{
    ifstream arq(nomeArquivo, ios::binary);

    if (arq.is_open())
    {
        abriuArquivo = 1;
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

static void EscreverArvore(NoArvore* no, ofstream& arq)
{
    char EhFolha;
    if (no != nullptr)
    {
        EhFolha = 0;
        arq.write(&EhFolha, 1);
        const char a = no->getCaracter();
        arq.write(&a, 1);
        EscreverArvore(no->getEsq(), arq);
        EscreverArvore(no->getDir(), arq);
    }
    else
    {
        EhFolha = 1;
        arq.write(&EhFolha, 1);
    }
}

static NoArvore* LerArvore(NoArvore* no, ifstream& arq)
{
    char c;
    arq.get(c); //0 se tiver alguma coisa, 1 se nao tiver nada
    if (c == 0)
    {
        arq.get(c);
        no = new NoArvore(c, 0);
        no->setEsq(LerArvore(no->getEsq(), arq));
        no->setDir(LerArvore(no->getDir(), arq));
    }
    return no;
}

static void MostrarArvore(NoArvore* raiz) //metodo pra teste, não será usado. Provavelmente será apagado
{
    if (raiz != nullptr)
    {
        MostrarArvore(raiz->getEsq());
        cout << raiz->getCaracter() << endl;
        MostrarArvore(raiz->getDir());
    }
}

static void Compilar()
{
    cout << "Digite o caminho do arquivo que deseja compactar: ";
    cin >> nomeArquivo;

    CriarFilaDePrioridade();


    if (abriuArquivo)
    {
        arvore = CriarArvore();
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
        }

        char bitsNaoUsados = 8 - bitsEscritos;

        arq.seekp(0, ios::beg); //ajusta o ponteiro do arquivo pra primeira posicao
        arq.write(&bitsNaoUsados, 1); //escreve o numero de bits a ser desconsiderado

        EscreverArvore(arvore->getRaiz(), arq); //Escrever a arvore no arquivo

        arqLeitura.close();
        arq.close();
    }
    else 
    {
        cout << "\nErro ao abrir arquivo! \n";
        system("pause");
    }
}

static void Descompilar()
{
    cout << "Não tá pronto kkkkkk! somente chore com seu arquivo compactado que vc nao vai poder descompactar" << endl;
    system("pause");
}

int main()
{
    char opcao;
    do
    {
        system("cls || clear");
        cout << "Compactador e Descompactador mania!!! Venha compactar seus arquivos aqui <3" << endl;
        cout << "---------------------------------------------------------------------------" << endl;
        cout << "1 - Compactar arquivo" << endl;
        cout << "2 - Descompactar arquivo" << endl;
        cout << "0 - Sair do programa" << endl;
        cout << "\nSelecione a opção desejada: ";
        cin >> opcao;

        switch (opcao)
        {
            case '1': Compilar(); break;
            case '2': Descompilar(); break;
            case '0': break;
            default:
            {
                break;
            } 
        }
    } 
    while (opcao != 0);

    ////////// TESTE PRA VER SE TA COMPACTANDO BONITINHO /////////////////////////////
    //testado e aprovado
    // 
    //"B:\\ESCOLA\\MALIGNO\\compactador_huffman/compactado.dat"
    /*
    ifstream teste("C:\\Users\\Hugo\\Documents\\Cotuca\\TPOO/compactado.dat", ios::binary);

    char c;
    teste.get(c); //pega o primeiro byte, deveria ser o numero de bites para ignorar no ultimo byte
    novaArvore = new Arvore(new NoArvore());
    novaArvore->setRaiz(LerArvore(novaArvore->getRaiz(), teste));
    teste.close();

    MostrarArvore(arvore->getRaiz());

    cout << "-------------------" << endl;

    MostrarArvore(novaArvore->getRaiz());*/
}