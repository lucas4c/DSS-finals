#include <iostream>
#include <vector>
#include <cstring>
#include "../cppcsv.h"
using namespace std;

struct User {   //Nodo com a estrutura para guardar os dados de cada user na Hashtable
    int key;    //userID
    int sofifaID;
    float rating;
};

class UserHashTable {
public:
    static const int tableSize = 30235103;  //Variável estática para guardar o tamanho da tabela
    vector<User> *table = (vector<User>*) malloc(sizeof(vector<User>)*tableSize);   //Alocando memória manualmente (tabela com mais de 80000 elementos dava stack overflow)
public:
    int hashFunction(int key);
    void setItem(int key, int sofifaID, float rating);
    bool getItem(int key, std::vector<pair<float, int>>& res);
    void dump();
};

int UserHashTable::hashFunction(int key) {  //Função hash para inteiros (preferívelmente o tamanho da tabela deve ser um número primo)
    return key % tableSize;
}

void UserHashTable::setItem(int key, int sofifaID, float rating) {  //Adiciona um nodo na HashTable
    int hashKey = hashFunction(key);
    auto& cell = table[hashKey];    //Tabela no índice retornado pela função hash
    auto beginIterator = begin(cell);   //Iterador para caminhar pelo vetor do índice
    bool keyExists = false;
    for (; beginIterator != end(cell); beginIterator++) {
        if (beginIterator->key == key) {    //Caso haja um nodo com a mesma chave
            User node;  //Aloca um nodo nodo
            node.key = key;
            node.sofifaID  = sofifaID;
            node.rating  = rating;
            cell.push_back(node);   //Adiciona no vetor do índice
            return;
        }
    }
    if (!keyExists) {   //Caso seja um usuário distinto
        User node;  //Aloca um novo nodo
        node.key = key;
        node.sofifaID = sofifaID;
        node.rating  = rating;
        cell.push_back(node);   //Adiciona o nodo no vetor do índice
    }
}

bool UserHashTable::getItem(int key, std::vector<pair<float, int>>& res) {  //Tenta achar um usuário usando uma chave
	int hashKey = hashFunction(key);
	auto& cell = table[hashKey];    //Tabela no índice retornado pela função hash
	auto beginIterator = begin(cell);   //Iterador para caminhar pelo vetor do índice
	bool keyFound = false;
	for (; beginIterator != end(cell); beginIterator++) {
		if (beginIterator->key == key) {    //Caso haja um usuário com a chave informada
            res.push_back(make_pair(beginIterator->rating, beginIterator->sofifaID));   //Adiciona o jogador e a avaliação no vetor de retorno
            if (!keyFound)
                keyFound = true;    //Marca que achou pelo menos uma chave
		}
	}
    if (keyFound)
        return true;    //Caso ache pelo menos uma chave
    else
        return false;   //Caso não ache nenhuma chave
}

void UserHashTable::dump() {    //Imprime todos os valores da HashTable
    for (int i{}; i < tableSize; i++) {
        if (table[i].size() == 0) continue;

        auto beginIterator = table[i].begin();
        cout << "slot[" << i << "]: ";
        for (;beginIterator != table[i].end(); beginIterator++) {
            cout << "[Key: " << beginIterator->key << " | sofifaID: " << beginIterator->sofifaID << " | Rating: " << beginIterator->rating;
        }
        cout << endl;
    }
    return;
}

//int main() {  //Testes
//
//    UserHashTable ht;
//
//    ht.setItem(4, 123123, 4.3);
//    ht.setItem(4, 321321, 4.2);
//    ht.setItem(4, 321123, 4.34);
//    ht.setItem(4, 123321, 4.342);
//    ht.setItem(4, 555234, 4.2);
//
//    ht.dump();
//
//    std::vector<pair<float, int>> usr;
//
//    ht.getItem(3, usr);
//
//    for (int i=0; i < usr.size(); i++) {
//        cout << usr[i].first << " " << usr[i].second << endl;
//    }
//
//    return 0;
//
//}

