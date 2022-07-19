#include <iostream>
#include <string>
#include "node.cpp"
#include <vector>

#ifndef TRIE_H
#define TRIE_H

class Trie
{
	friend class Node;
private:
	Node* root;
public:
	Trie() {root = new Node();}
	~Trie() {}
	void setItem(std::string data, int value);  //Insere um elemento na Trie
	void getItem(std::string data, std::vector<int>&); //Retorna o nodo contendo o último caractére do prefixo de pesquisa
	void getValue(Node* root, std::vector<int>& res);    //Retorna os valores de id que correspondem ao prefixo
};

#endif

void Trie::setItem(std::string data, int value) //Inserindo elemento na Trie
{
	Node* tmp = root;
	char cstr[data.length()];

	data.copy(cstr, data.length()); //Transformando string em um array de char
	for (int i = 0; i < data.length(); i++)
	{
		if (tmp->child[static_cast<int>(cstr[i])] != NULL)  //Caminha pelos ponteiros da Trie
			tmp = tmp->child[static_cast<int>(cstr[i])];
		else
		{
			tmp->child[static_cast<int>(cstr[i])] = new Node(); //Cria um novo nodo
			tmp = tmp->child[static_cast<int>(cstr[i])];
			tmp->setChar(cstr[i]);  //Insere o nodo na Trie
		}
	}
	tmp->setValue(value); //Seta o valor do nodo
}

void Trie::getValue(Node* root, std::vector<int>& res) //Retornando a lista de valores que correspondem com o prefixo
{
	for (int i = 0; i < 128; i++) //Checa todos os ponteiros para ver se acha um nodo child
	{
		Node* adv = root;
		if (adv->child[i] != NULL) //Caso achar um nodo child
		{
			adv = adv->child[i];    //Avança o ponteiro
			if (adv->isWord())  //Se o nodo é um nome e (nodo possui valor)
			{
				res.push_back(adv->value);  //Insere o valor do nodo correspondente no vetor de retorno
			}
			getValue(adv, res);    //Chamando a função recursiva para continuar caminhando pela Trie
		}
	}
}

void Trie::getItem(std::string prefix, std::vector<int>& res) //Função que acha o nodo que corresponde ao último caractere do prefixo
{
	Node* tmp = root;
	char cstr[prefix.length()];

	prefix.copy(cstr, prefix.length(), 0);
	for (int i = 0; i < prefix.length(); i++)     //Avança na Trie até encontrar o nodo que corresponde ao último caractere do prefixo
	{
		if (tmp->child[static_cast<int>(cstr[i])] == NULL)  //Caso o prefixo for encontrado na Trie mas não possuir nenhum nodo child
		{
			std::cout << "Nenhuma palavra com o prefixo na trie." << std::endl;
			return;
		}
		else
			tmp = tmp->child[static_cast<int>(cstr[i])];
		if (tmp->getChar() != cstr[i])  //Caso o prefixo não for encontrado mesmo
		{
			std::cout << "Prefixo não foi encontrado na trie." << std::endl;
			return;
		}
	}
	getValue(tmp, res);	//Chama a função para devolver a lista de valores usando o nodo encontrado (que avançou até o ultimo caractere do prefixo)
}
