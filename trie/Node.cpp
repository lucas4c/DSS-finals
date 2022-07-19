#ifndef NODE_H
#define NODE_H

#include <iostream>

class Node //Nodo da Trie
{
	friend class Trie;
public:
	char c;
	int value; //Caso possua valor, marca que o nodo representa uma palavra na Trie
	Node* child[128]; //ponteiros para próximos nodos; ASCII Table (0-127)
public:
	Node();
	~Node() {}
	void setValue(int value) {
	    this->value = value;
	    }
	int getValue() {
	    return this->value;
	    }
	bool isWord() {
	    return this->value != 0;
	    }
	void setChar(char ch) {
	    this->c = ch;
	    }
	char getChar() const {
	    return this->c;
	    }
};

Node::Node() //Node constructor
{
	this->setChar('\0');
	this->setValue(0);
	for (int i = 0; i < 128; i++)
		this->child[i] = NULL;
}

#endif
