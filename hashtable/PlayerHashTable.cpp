#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>
#include "../cppcsv.h"
using namespace std;

struct Player { //Nodo com a estrutura para guardar os dados de cada jogador na HashTable.
    int key;    //sofifaID
    std::string name;
    std::string positions;
    float rating;
    int rCount;
};

class PlayerHashTable {
public:
    static const int tableSize = 50021; //Vari�vel est�tica para guardar o tamanho da tabela
    vector<Player> *table = (vector<Player>*) malloc(sizeof(vector<Player>)*tableSize); //Alocando mem�ria manualmente (tabela de usu�rio dava stack overflow)
public:
    int hashFunction(int key);
    void setItem(int key, std::string name, std::string positions);
    void addRating(int key, float rating);
    bool getItem(int key, Player *node);
    bool getItem(vector<int>& keys, vector<Player*>& res);
    void printHeader(bool r);
    void printPlayer(Player *node);
    void printPlayer(Player& node);
    void printPlayer(Player *node, float usrRating);
    void dump();
};

int PlayerHashTable::hashFunction(int key) {    //Fun��o hash para inteiros (prefer�velmente o tamanho da tabela deve ser um n�mero primo)
    return key % tableSize;
}

void PlayerHashTable::setItem(int key, std::string name, std::string positions) {   //Adiciona um nodo na HashTable
    int hashKey = hashFunction(key);
    auto& cell = table[hashKey];    //Tabela no �ndice retornado pela fun��o hash
    auto beginIterator = begin(cell);   //Iterador para caminhar pelo vetor do �ndice
    bool keyExists = false;
    for (; beginIterator != end(cell); beginIterator++) {
        if (beginIterator->key == key) {    //Caso haja um nodo com a mesma chave, substitui o nodo
            keyExists = true;
            beginIterator->name = name;
            beginIterator->positions = positions;
            break;
        }
    }
    if (!keyExists) {   //Caso seja um nodo com chave distinta
        Player node;    //Aloca um nodo
        node.key = key;
        node.name  = name;
        node.positions  = positions;
        node.rating = 0;
        node.rCount = 0;
        cell.push_back(node);   //Adiciona o nodo no vetor do �ndice
    }
}

void PlayerHashTable::addRating(int key, float rating) {    //Fun��o que adiciona ratings para um jogador
    int hashKey = hashFunction(key);
    auto& cell = table[hashKey];    //Tabela no �ndice retornado pela fun��o hash
    auto beginIterator = begin(cell);   //Iterador para caminhar pelo vetor do �ndice
    for (; beginIterator != end(cell); beginIterator++) {
        if(beginIterator->key == key) { //Caso haja um nodo com a chave especificada
            beginIterator->rating = beginIterator->rating * beginIterator->rCount;  //Calcula o valor total de ratings
            beginIterator->rCount++;                                                //Incrementa o contador de ratings
            beginIterator->rating += rating;                                        //Soma com o novo rating
            beginIterator->rating = beginIterator->rating / beginIterator->rCount;  //Calcula a nova m�dia global
        }
    }
}

bool pSortDesc(Player* p1, Player* p2) {    //Fun��o de ordena��o por rating do jogador
    return p1->rating>p2->rating;
}

bool PlayerHashTable::getItem(int key, Player *node) {  //Tenta achar um jogador usando sua chave
	int hashKey = hashFunction(key);
	auto& cell = table[hashKey];    //Tabela no �ndice retornado pela fun��o hash
	auto beginIterator = begin(cell);   //Iterador para caminhar pelo vetor do �ndice

	for (; beginIterator != end(cell); beginIterator++) {
		if (beginIterator->key == key) {    //Caso haja um jogador com a chave informada
            node->key = beginIterator->key;
            node->name = beginIterator->name;
            node->positions = beginIterator->positions;
            node->rating = beginIterator->rating;
            node->rCount = beginIterator->rCount;
			return true;    //Copia os dados para o ponteiro no argumento, e retorna true
		}
	}
    return false;   //Retorna false caso n�o consiga encontrar o jogador
}

bool PlayerHashTable::getItem(vector<int>& keys, std::vector<Player*>& res) {   //Retorna uma vetor de jogadores, usando um vetor de keys
    bool keyFound = false;
    for (int i = 0; i < keys.size(); i++) { //Percorre o vetor de chaves
        Player* p = new Player();   //Aloca um p tempor�rio
        if(getItem(keys[i], p)) {   //Caso ache p na HashTable
            keyFound = true;
            res.push_back(p);   //Coloca o jogador na lista
        }
    }
    if (keyFound)
        return true;    //Caso ache pelo menos um jogador
    else
        return false;   //Caso n�o ache nenhum jogador
}

bool removeFewRatings(Player* p1) { //Fun��o para checar se um jogador tem mais de 1000 avalia��es
    return p1->rCount<1000;
}

void PlayerHashTable::printHeader(bool r) { //Fun��o para imprimir cabe�alho (r informa se deve imprimir o campo auxiliar rating para pesquisas com usu�rios)
    cout << endl;
    cout << "| " << "sofifa_id";
    cout << " | " << "name" << setw(56-4);
    cout << " | " << "positions" << setw(24-9);
    cout << " | " << "global_rating";
    cout << " | " << "count" << setw(8);
    if(r) cout << " | " << "rating";    //imprime o campo rating caso pesquise por usu�rio
    cout << " | " << endl;
}

void PlayerHashTable::printPlayer(Player *node) {   //Fun��o para imprimir um jogador
    cout << "| " << node->key << setw(12-std::to_string(node->key).length());
    cout << " | " << node->name << setw(56-node->name.length());
    cout << " | " << node->positions << setw(24-node->positions.length());
    cout << " | " << std::fixed << std::setprecision(6) << node->rating << setw(8);
    cout << " | " << node->rCount << setw(13-std::to_string(node->rCount).length());
    cout << " | " << endl;
}

void PlayerHashTable::printPlayer(Player *node, float usrRating) {  //Fun��o para imprimir jogador com o campo rating de usu�rio espec�fico
    cout << "| " << node->key << setw(12-std::to_string(node->key).length());
    cout << " | " << node->name << setw(56-node->name.length());
    cout << " | " << node->positions << setw(24-node->positions.length());
    cout << " | " << std::fixed << std::setprecision(6) << node->rating << setw(8);
    cout << " | " << node->rCount << setw(13-std::to_string(node->rCount).length());
    cout << " | " << std::fixed << std::setprecision(1) << usrRating << setw(6);
    cout << " | " << endl;
}

void PlayerHashTable::dump() {  //Imprime todos os jogadores na HashTable
    for (int i{}; i < tableSize; i++) {
        if (table[i].size() == 0) continue;

        auto beginIterator = table[i].begin();
        cout << "slot[" << i << "]: ";
        for (;beginIterator != table[i].end(); beginIterator++) {
            cout << "[Key: " << beginIterator->key << " | Name: " << beginIterator->name << " | Positions: " << beginIterator->positions << " | Rating: " << beginIterator->rating << " | Count: " << beginIterator->rCount;
        }
        cout << endl;
    }
    return;
}

//int main() {  //Teste
//
//    PlayerHashTable ht;
//
//    Player* a = new Player();
//
//    a->key = 123456;
//    a->name = "Guina o putao da vila";
//    a->positions = "4";
//    a->rating = 3.5234328;
//    a->rCount = 1234;
//
//    ht.printHeader(true);
//    ht.printPlayer(a, 2.2);
//
//    return 1337;
//
//}

