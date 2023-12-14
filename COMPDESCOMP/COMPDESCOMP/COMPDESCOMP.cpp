#include <iostream>
#include <string.h>
#include <fstream>
#include <string.h>
#include <filesystem>
#include <map>
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

    unsigned int tamanho = novaFilaPrioridade->Tamanho();
    arq.write(reinterpret_cast<const char*>(&tamanho), sizeof(unsigned int));

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

void gerarCodigosHuffman(NoArvore* root, string& code, map<char, string>& codes) {
    if (!root->getEsq() && !root->getDir()) {
        codes[root->getCaracter()] = code == "" ? "0" : code;

        return;
    }

    string previous_code = code;

    if (root->getEsq()) {
        code += "0"; 
        gerarCodigosHuffman(root->getEsq(), code, codes);

        code = previous_code;
    }

    if (root->getDir()) {
        code += "1"; 
        gerarCodigosHuffman(root->getDir(), code, codes);

        code = previous_code;
    }
}

void EscreverBitNoArquivo(ofstream& file, char bit, char& current_byte, int& bit_count) {
    if (bit != '0' && bit != '1') {
        // Throw an exception with an error message
        throw runtime_error("Invalid bit: " + bit);
    }

    current_byte = (current_byte << 1) | (bit - '0'); // Shift left by 1 bit to make
    // room for the new bit and add
    // the new bit to the buffer.

    bit_count++; // Increment the number of bits in the buffer.

    // If the buffer is full, write the byte to the file:
    if (bit_count == 8) {
        file.write(&current_byte, 1); // Write the byte to the file.
        current_byte = 0;
        bit_count = 0;
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

        ofstream arq("C:\\Users\\u22121\\Documents\\GitHub\\compactador_huffman/compactado.dat", ios::binary);
        //ofstream arq("D:\\ARMAG/compactado.dat", ios::binary);
        ifstream arqLeitura(nomeArquivo, ios::binary);

        size_t posPonto = nomeArquivo.find_last_of('.');
        string ext = nomeArquivo.substr(posPonto + 1);
        char c = 0;
        arq.write(&c, 1); // onde serão escritos os bitsAIgnorar
        int extTam = ext.length();
        arq.write(reinterpret_cast<const char*>(&extTam), sizeof(int));
        arq.write(ext.c_str(), ext.length());

        EscreverFila(arq);

        string code;
        map<char, string> codes;

        if (arvore->getRaiz())
            gerarCodigosHuffman(arvore->getRaiz(), code, codes);

        char bitsLidos[10000000];

        char byte = 0;
        int bitsEscritos = 0;
        for (char c; arqLeitura.get(c); )
        {
            string codigo = codes.at(c);
            for (int j = 0; codigo[j] != '\0'; j++)
            {
                bitsLidos[bitsEscritos] = codigo[j];
                bitsEscritos++;
            }
        }

        char current_byte = 0;
        int  bit_count = 0;

        for (char bit : bitsLidos) {
            EscreverBitNoArquivo(arq, bit, current_byte, bit_count);
        }


        // If there are remaining bits in the buffer, write them to the file:
        if (bit_count > 0) {
            current_byte <<= (8 - bit_count);
            arq.write(&current_byte, 1);
        }

        char redundant_bits = 8 - bit_count; // Keep track of the amount of redundant bits.

        // Write the amount of redundant bits to the first line of the file
        arq.seekp(0, ios::beg);
        arq.write(reinterpret_cast<const char*>(&redundant_bits), sizeof(redundant_bits));

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

    arq.read(reinterpret_cast<char*>(&tamFila), sizeof(unsigned int));
    
    LerFila(arq, tamFila);
    Arvore* novaArvore = CriarArvore();

    ofstream arqEscrita("C:\\Users\\u22121\\Documents\\GitHub\\compactador_huffman/descompactado." + ext, ios::binary);

    int i = 0;
    string bitsLidos;
    for (char b; arq.get(b);)
    {
        i++;
        bitsLidos += bitset<8>(b).to_string();
    }
    bitsLidos = bitsLidos.substr(0, bitsLidos.size() - bitsAIgnorar);
    if (arq.eof())
        cout << "a";

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
