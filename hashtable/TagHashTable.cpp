#include <iostream>
#include <vector>
#include <cstring>
#include "../cppcsv.h"
using namespace std;

struct Tag {    //Nodo com a estrutura para guardar os dados de cada Tag na HashTable
    std::string key;    //Tag
    int sofifaID;
    int userID;
};

class TagHashTable {
public:
    static const int tableSize = 456193;    //Variável estátia para guardar o tamanho da tabela
    vector<Tag> *table = (vector<Tag>*) malloc(sizeof(vector<Tag>)*tableSize);  //Alocando memória manualmente (tablea de usuário dava stackoverflow)
public:
    unsigned int hashFunction(std::string key);
    void setItem(std::string key, int sofifaID, int userID);
    bool getItem(std::string key, std::vector<int>& res);
    bool getItem(std::vector<std::string> keys, std::vector<int>& res);
    void dump();
};

unsigned int TagHashTable::hashFunction(std::string key) {  //Função hash para std::string
    unsigned long int hashedKey = 0;
    for(int i = 0; i<key.length(); i++) {
        hashedKey = (hashedKey * 83) + key[i];
    }
    return hashedKey % tableSize;
}

void TagHashTable::setItem(std::string key, int sofifaID, int userID) { //Adiciona um nodo na HashTable
    int hashKey = hashFunction(key);
    auto& cell = table[hashKey];    //Tabela no índice retornado pela função Hash
    auto beginIterator = begin(cell);   //Iterador para caminhar pelo vetor no índice
    bool keyExists = false;
    for (; beginIterator != end(cell); beginIterator++) {
        if (beginIterator->key == key) {    //Caso ache um nodo com a mesma chave
            Tag node;
            node.key = key;
            node.sofifaID  = sofifaID;
            node.userID  = userID;
            cell.push_back(node);   //Adiciona o novo nodo no vetor
            return;
        }
    }
    if (!keyExists) {   //Caso o nodo seja distindo
        Tag node;
        node.key = key;
        node.sofifaID = sofifaID;
        node.userID  = userID;
        cell.push_back(node);   //Adiciona o nodo ao vetor
    }
}

std::vector<int> intersection(const std::vector<std::vector<int>> &vecs) {  //Função dada para dar a intersecção entre n vetores

    auto lastIntersection = vecs[0];
    std::vector<int> currentIntersection;

    for(std::size_t i=1; i < vecs.size(); ++i) {
        std::set_intersection(lastIntersection.begin(), lastIntersection.end(),
                              vecs[i].begin(), vecs[i].end(),
                              std::back_inserter(currentIntersection));
        std::swap(lastIntersection, currentIntersection);
        currentIntersection.clear();
    }
    return lastIntersection;
}

bool TagHashTable::getItem(std::string key, std::vector<int>& res) {    //Tenta achar ids usando uma chave
	int hashKey = hashFunction(key);
	auto& cell = table[hashKey];    //Tabela no índice retornado pela função Hash
	auto beginIterator = begin(cell);   //Iterador para caminhar pelo vetor do índice
	bool keyFound = false;
	for (; beginIterator != end(cell); beginIterator++) {
		if (beginIterator->key == key) {    //Caso ache um valor com a chave informada
            res.push_back(beginIterator->sofifaID); //Adiciona o id no vetor de retorno
            if (!keyFound)
                keyFound = true;
		}
	}
    if (keyFound)
        return true;    //Caso ache pelo menos uma chave
    else
        return false;   //Caso não ache nenhuma chave
}

bool TagHashTable::getItem(std::vector<std::string> keys, std::vector<int>& res) {  //Tenta achar ids usando um vetor de chaves
    vector<int> vec;    //Aloca um vetor temporário
    std::vector<std::vector<int>> vecs; //Aloca o vetor de vetores para fazer a intersecção
    bool keyFound = false;
    for (int i = 0; i < keys.size(); i++) { //Percorre todas as chaves
        if(getItem(keys[i], vec)) { //Caso ache um valor para a chave especificada
            keyFound = true;
            sort(vec.begin(), vec.end());   //ordena o vetor
            vec.erase(unique(vec.begin(), vec.end()), vec.end());   //Remove itens duplicados
            vecs.push_back(vec);    //Adiciona o vetor temporário no vetor de vetores
            vec.clear();    //Limpa o vetor temporário
        }
    }
    if (keyFound) {
        res = intersection(vecs);   //Faz a intersecção do vetor de vetores, e joga no vetor de retorno
        return true;    //Caso ache pelo menos uma chave
    }
    else
        return false;   //Caso não ache nenhuma chave
}

void TagHashTable::dump() { //Imprime todos os valores da HashTable
    for (int i{}; i < tableSize; i++) {
        if (table[i].size() == 0) continue;

        auto beginIterator = table[i].begin();
        cout << "slot[" << i << "]: ";
        for (;beginIterator != table[i].end(); beginIterator++) {
            cout << "[Tag: " << beginIterator->key << " | sofifaID: " << beginIterator->sofifaID << " | userID: " << beginIterator->userID;
        }
        cout << endl;
    }
    return;
}

//int main() {  //Testes
//
//    TagHashTable ht;
//
//    ht.setItem("Brazil", 123123, 321321);
//    ht.setItem("Dribbler", 123123, 321321);
//    ht.setItem("Brazil", 22222, 321321);
//    ht.setItem("Dribbler", 321321, 321321);
//    ht.setItem("Brazil", 21, 321321);
//    ht.setItem("Dribbler", 23, 321321);
//
//    std::vector<std::string> keys = {"Dribbler", "Brazil"};
//    std::vector<int> res;
//
//    ht.getItem(keys, res);
//
//    for (int i : res) {
//        cout << i << ", ";
//    }
//
//    //ht.dump();
//
//
//}
