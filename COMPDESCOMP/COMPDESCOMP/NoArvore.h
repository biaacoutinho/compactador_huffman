#pragma once

class NoArvore {
private:
	char caracter;
	unsigned int frequencia;
	NoArvore* dir;
	NoArvore* esq;
public:
	NoArvore();
	NoArvore(char, unsigned int);
	char getCaracter();
	unsigned int getFrequencia();
	NoArvore* getDir();
	NoArvore* getEsq();
	void setCaracter(char);
	void setFrequencia(unsigned int);
	void setDir(NoArvore*);
	void setEsq(NoArvore*);
};