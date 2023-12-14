#include <iostream>
#include <string.h>
#include <fstream>
#include <string.h>
#include <filesystem>
#include <bitset>
#include "Arvore.h"
#include "Lista.h"

using namespace std;

Arvore* arvore;
Arvore* novaArvore;
Lista* filaPrioridade;
Lista* novaFilaPrioridade;
char abriuArquivo = 0;
string nomeArquivo;

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
        arq.close();
        filaPrioridade->Ordenar();
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

static void EscreverFila(ofstream& arq)
{
    if (!arq.is_open())
    {
        return;
    }

    int tamanho = novaFilaPrioridade->Tamanho();
    arq.write(reinterpret_cast<const char*>(&tamanho), 4);

    auto noAtual = novaFilaPrioridade->getPrimeiro();
    while (noAtual != nullptr)
    {
        auto dado = noAtual->getDado();
        char c = dado->getCaracter();
        arq.write(&c, 1);
        unsigned int freq = dado->getFrequencia();
        arq.write(reinterpret_cast<const char*>(&freq), sizeof(unsigned int));

        noAtual = noAtual->getProx();
    }
}

static void LerFila(ifstream& arq, unsigned int tamFila)
{
    if (filaPrioridade)
        delete filaPrioridade;

    filaPrioridade = new Lista();

    for (int i = 0; i < tamFila; i++)
    {
        char c = arq.get();
        unsigned int freq;
        arq.read(reinterpret_cast<char*>(&freq), sizeof(unsigned int));
        filaPrioridade->Inserir(new NoLista(new NoArvore(c, freq)));
    }
}

static void EscreverArvore(NoArvore* no, ofstream& arq, char *byte, int *bits)
{
    if (no->getEsq() == nullptr && no->getDir() == nullptr)
    {
        *byte <<= 1;
        *byte |= 0b1;
        (*bits)++;
        char a = no->getCaracter();

        *byte <<= (8 - *bits); //disponibiliza os bits restantes
        int bitsNGravados = (8 - (8 - *bits));
        int codAux = a >> bitsNGravados; //parte o codigo em dois,
        //a parte que cabe no bit atual(codAux),
        //e a parte que nao cabe
        *byte |= codAux; // armazena a parte que cabe no byte atual

        arq.write(byte, 1);//escreve o byte prontinho

        *byte = 0b0; //zera o byteAtual denovo (00000000)

        int mascara = ~(0b11111111 << bitsNGravados); //cria uma mascara para conseguir gravar
        //o numero certo de bits do codigo
        *byte = a & mascara; //grava os bits restantes

        *bits = bitsNGravados; //atualiza o numero de bits gravados

        if (*bits == 8)
        {
            arq.write(byte, 1);
            *bits = 0;
        }

        return;
    }
    else
    {
        *byte <<= 1;
        *byte |= 0;
        (*bits)++;
    }

    if (*bits == 8)
    {
        arq.write(byte, 1);
        *bits = 0;
    }

    EscreverArvore(no->getEsq(), arq, byte, bits);
    EscreverArvore(no->getDir(), arq, byte, bits);
}

static NoArvore* LerArvore(NoArvore* no, ifstream& arq, char* byte, int* bitsLidos) //alterar para ler um unico bit
{
    char bit = 0;

	if (*bitsLidos == 8)
	{
		arq.get(*byte);
		*bitsLidos = 0;
	}

	bit = *byte;
	bit <<= *bitsLidos;
	bit >>= 7;
	(*bitsLidos)++;
	if (bit) //se o bit lido for 1
	{
		char c = 0;
		c = (*byte << *bitsLidos); //primeira parte do char
		arq.get(*byte); //inicia um novo byte
        uint8_t b = *byte;
		c |= (b >> (8 - *bitsLidos));
		no = new NoArvore(c, 0);
        cout << c;
		return no;
	}
	else //se o bit lido for 0
	{
		no = new NoArvore('\0', 0);
		no->setEsq(LerArvore(no->getEsq(), arq, byte, bitsLidos));
		no->setDir(LerArvore(no->getDir(), arq, byte, bitsLidos));
		return no;
	}
}

static void MostrarArvore(NoArvore* raiz) //metodo pra teste, não será usado. Provavelmente será apagado
{
    if (raiz != nullptr)
    {
        MostrarArvore(raiz->getEsq());
        cout << raiz->getCaracter() << " : " << raiz->getCodigo() << " , " << raiz->getTamanho() << endl;
        MostrarArvore(raiz->getDir());
    }
}

static void Compactar()
{
    cout << "Digite o caminho do arquivo que deseja compactar: ";
    cin >> nomeArquivo;

    CriarFilaDePrioridade();
    novaFilaPrioridade = new Lista(filaPrioridade);

    if (abriuArquivo) 
    {
        arvore = CriarArvore();
        arvore->Codificar(arvore->getRaiz(), 0, 0);

        ofstream arq("B:\\ESCOLA\\MALIGNO/compactado.dat", ios::binary);
        //ofstream arq("D:\\ARMAG/compactado.dat", ios::binary);
        ifstream arqLeitura(nomeArquivo, ios::binary);

        size_t posPonto = nomeArquivo.find_last_of('.');
        string ext = nomeArquivo.substr(posPonto + 1);
        char c = 0;
        arq.write(&c, 1); // onde serão escritos os bitsAIgnorar
        int extTam = ext.length();
        arq.write(reinterpret_cast<const char*>(&extTam), sizeof(extTam));
        arq.write(ext.c_str(), ext.length());

        EscreverFila(arq);

        char byte = 0;
        int bitsEscritos = 0;
        for (char c; arqLeitura.get(c); )
        {
            unsigned int codigo = arvore->BuscarCodigo(arvore->getRaiz(), c);
            //verificar se é infinity
            int tamCodigo = arvore->getAtual()->getTamanho();

            if (tamCodigo <= (8 - bitsEscritos)) //verifica se tem espaço no byte para o codigo inteiro
            {
                byte <<= tamCodigo;
                byte |= codigo;
                bitsEscritos += tamCodigo;
                if (bitsEscritos == 8)
                {
                    arq.write(&byte, 1);
                    bitsEscritos = 0;
                }
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

                byte = 0b0; //zera o byteAtual denovo (00000000)

                unsigned __int8 mascara = ~(0b11111111 << bitsNGravados); //cria uma mascara para conseguir gravar
                //o numero certo de bits do codigo
                byte = codigo & mascara; //grava os bits restantes

                bitsEscritos = bitsNGravados; //atualiza o numero de bits gravados
            }
        }

        char bitsNaoUsados = 8 - bitsEscritos;

        byte <<= bitsNaoUsados;
        arq.write(&byte, 1);

        arq.seekp(0, ios::beg); //ajusta o ponteiro do arquivo pra primeira posicao
        arq.write(&bitsNaoUsados, 1); //escreve o numero de bits a ser desconsiderado

        arqLeitura.close();
        arq.close();

        system("pause");
    }
    else 
    {
        cout << "\nErro ao abrir arquivo! \n";
        system("pause");
    }
}

static void Descompactar()
{
    cout << "Digite o caminho do arquivo que deseja descompactar: ";
    cin >> nomeArquivo;

    ifstream arq(nomeArquivo);
    unsigned int tamFila;
    char bitsAIgnorar;
    int tamExt;
    arq.get(bitsAIgnorar);
    arq.read(reinterpret_cast<char*>(&tamExt), sizeof(int));

    char* buffer = new char[tamExt + 1]; 
    arq.read(buffer, tamExt);
    buffer[tamExt] = '\0';
    string ext(buffer);
    delete[] buffer;

    arq.read(reinterpret_cast<char*>(&tamFila), sizeof(tamFila));
    
    LerFila(arq, tamFila);
    Arvore* novaArvore = CriarArvore();

    ofstream arqEscrita("B:\\ESCOLA\\MALIGNO/descompactado." + ext, ios::binary);

    string bitsLidos;
    for (char b; arq.get(b);)
        bitsLidos += bitset<8>(b).to_string();
    bitsLidos = bitsLidos.substr(0, bitsLidos.size() - bitsAIgnorar);

    NoArvore* noAtual = novaArvore->getRaiz();
    string texto;
    for (size_t i = 0; bitsLidos[i] != '\0'; i++)
    {
        if (bitsLidos[i] == '0')
        {
            if (noAtual->getEsq())
                noAtual = noAtual->getEsq();
        }
        else if (bitsLidos[i] == '1')
        {
            if (noAtual->getDir())
                noAtual = noAtual->getDir();
        }

        if (!noAtual->getEsq() && !noAtual->getDir())
        {
            texto += noAtual->getCaracter();
            noAtual = novaArvore->getRaiz();
        }
    }

    arqEscrita << texto;

    arq.close();
    arqEscrita.close();
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
            case '1': Compactar(); break;
            case '2': Descompactar(); break;
            case '0': break;
            default:
            {
                break;
            } 
        }
    } 
    while (opcao != '0');

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
