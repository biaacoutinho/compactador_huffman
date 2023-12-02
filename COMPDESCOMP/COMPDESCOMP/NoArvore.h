#pragma once

class NoArvore {
private:
	char caracter;
	unsigned int frequencia;
	unsigned int codigo;
	unsigned int tamanho;
	NoArvore* dir;
	NoArvore* esq;
public:
	NoArvore();
	NoArvore(char, unsigned int);
	char getCaracter();
	unsigned int getFrequencia();
	unsigned int getCodigo();
	unsigned int getTamanho();
	NoArvore* getDir();
	NoArvore* getEsq();
	void setCaracter(char);
	void setFrequencia(unsigned int);
	void setDir(NoArvore*);
	void setEsq(NoArvore*);
	void setCodigo(unsigned int);
	void setTamanho(unsigned int);
};