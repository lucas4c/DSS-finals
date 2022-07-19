#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

class PositionsList {   //Lista para pesquisar por posições

public:
    std::vector<std::pair<std::string, int>> players;   //Lista composta por pares no formato <posição, id>
public:
    void setItem(std::string positions, int sofifaID);
    bool getItem(std::string position, vector<int>& res);
    void dump();

};

void PositionsList::setItem(std::string positions, int sofifaID) {  //Adiciona um elemento na lista
    players.push_back(make_pair(positions, sofifaID));
}

bool PositionsList::getItem(std::string position, vector<int>& res) {   //Retorna todos os ids dos elementos que tenham o substring informado
    bool keyFound;
    for (pair<std::string, int> p : players) {  //Percorre toda a lista
        if (p.first.find(position)!=std::string::npos) {    //Checa se o argumento é substring
            keyFound = true;
            res.push_back(p.second);    //Adiciona o valor do elemento no vetor de retorno
        }
    }
    if (keyFound)
        return true;    //Caso ache pelo menos um id
    else
        return false;   //Caso não ache nenhum id
}

void PositionsList::dump() {    //Imprime todos os pares da lista
    if (players.empty())
        cout << "Lista vazia.";
    else {
        for (pair<std::string, int> p : players) {
            cout << "sofifaID: " << p.second << " Posicoes: " << p.first << endl;
        }
    }

}

//int main() {  //Teste
//
//    PositionsList p;
//
//    p.setItem("\"ST, GK, MC\"", 12312);
//    p.setItem("ST", 321123);
//    p.setItem("\"GK, MC\"", 321111);
//
//    std::vector<int> res;
//
//    p.getItem("GK", res);
//
//    for (int i=0; i < res.size(); i++) {
//        cout << res.size() << ", " << res[i] << endl;
//    }
//
//    //p.dump();
//
//    return 0;
//}
