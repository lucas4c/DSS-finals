#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <time.h>
#include "cppcsv.h"
#include "hashtable/PlayerHashTable.cpp"
#include "hashtable/UserHashTable.cpp"
#include "hashtable/TagHashTable.cpp"
#include "hashtable/PositionsList.cpp"
#include "trie/node.cpp"
#include "trie/trie.cpp"
#define PLAYER_CSV_SIZE 18946
#define RATING_CSV_SIZE 24188079
#define TAGS_CSV_SIZE 362693

int main() {

    clock_t gatherStart, gatherEnd; //Declarando vari�veis para contagem de tempo da constru��o dos dataframes
    clock_t playerStart, playerEnd;
    clock_t userStart, userEnd;
    clock_t tagStart, tagEnd;
    gatherStart = clock();          //Iniciando a vari�vel de contagem
    playerStart = clock();

    io::LineReader playerCSV("files/players.csv"); //Abrindo o arquivo players.csv

    int sofifaID;
    std::string name, positions;

    PlayerHashTable ht_players_df;  //Players HashTable - Key = (int)sofifaID
    Trie trie_playerNames_df;       //Players Trie - Key = (string)Name
    PositionsList lst_positions_df; //Players List - Key = (string)Positions

    char* line;                     //Vari�vel que vai ler o .csv linha por linha
    line = playerCSV.next_line();   //Lendo a linha do cabe�alho do .csv

    cout << "Gathering data from players." << endl;

    int load = 0;                       //Vari�vel inicial para contar o progresso de abertura do .csv dos players
    int loaded = PLAYER_CSV_SIZE / 10;  //Vari�vel contendo 10% do tamanho total de players.csv

    while(line  = playerCSV.next_line()) {  //Lendo players.csv linha por linha
        sofifaID = stoi(strtok(line, ","));
        name = strtok(NULL, ",");
        positions = strtok(NULL, "\n");

        ht_players_df.setItem(sofifaID, name, positions);   //Adicionando Jogadores na Hashtable na Hashtable de Jogadores
        lst_positions_df.setItem(positions, sofifaID);      //Adicionando posi��es e ids de jogadores na lista de posi��es
        trie_playerNames_df.setItem(name, sofifaID);         //Adicionando nomes e ids na trie de nomes dos jogadores

        load++;
        if (load%loaded==0) //Vari�vel load printa a cada 10% do carregado
            cout << "||";
    }

    playerEnd = clock();
    float playerTime = (float) (playerEnd - playerStart) / CLOCKS_PER_SEC * 1;  //Calculando delta do tempo de gather do dataframe dos jogadores
    cout << " Tempo de construcao: " << playerTime << "s" << endl;

//--------------------------------------[User]------------------------------------------------

    userStart = clock();

    io::LineReader ratingCSV("files/rating.csv");   //Abrindo o arquivo rating.csv

    int userID;
    float rating;

    UserHashTable ht_users_df;  //Users HashTable - Key = (int)userID

    line = ratingCSV.next_line();   //Lendo o cabe�alho

    cout << "Gathering data from ratings." << endl;

    load = 0;                       //Resetando vari�vel de carregamento
    loaded = RATING_CSV_SIZE / 10;  //Atualizando vari�vel de carregamento

    while(line = ratingCSV.next_line()) {   //Lendo rating.csv linha por linha
        userID = stoi(strtok(line, ","));
        sofifaID = stoi(strtok(NULL, ","));
        rating = stof(strtok(NULL, "\n"));

        ht_players_df.addRating(sofifaID, rating);      //Adicionando nos campos "Rating" e "Count da Hashtable de jogadores
        ht_users_df.setItem(userID, sofifaID, rating);  //Adicionando os reviews na Hashtable de usu�rios

        load++;
        if(load%loaded==0)  //Printando o progresso de carregamento
            cout << "||";
    }

    userEnd = clock();
    float userTime = (float) (userEnd - userStart) / CLOCKS_PER_SEC * 1;
    cout << " Tempo de construcao: " << userTime << "s" << endl;

//-------------------------------[TAGS]-------------------------------------

    tagStart = clock();

    io::LineReader tagCSV("files/tags.csv");    //Abrindo o arquivo tags.csv

    std::string tag;

    TagHashTable ht_tags_df;    //Tags HashTable - Key = (string)Tag

    line = tagCSV.next_line();  //Lendo cabe�alho

    cout << "Gathering data from tags." << endl;

    load = 0;                       //Atualizando vari�vel inicial de carregamento
    loaded = TAGS_CSV_SIZE / 10;    //Atualizando vari�vel parcial de carregamento

    while(line = tagCSV.next_line()) {  //Lendo tags.csv linha por linha
        if (strlen(line)>14) {          //Checando se a linha possui de fato tags
            userID = stoi(strtok(line, ","));
            sofifaID = stoi(strtok(NULL, ","));
            tag = strtok(NULL, "\n");

            ht_tags_df.setItem(tag, sofifaID, userID);  //Adicionando os valores na HashTable de tags

            load++;
            if(load%loaded==0)  //Printando o progresso de carregamento
                cout << "||";
        }
    }
    tagEnd = clock();
    float tagTime = (float) (tagEnd - tagStart) / CLOCKS_PER_SEC * 1;
    cout << " Tempo de construcao: " << tagTime << "s" << endl;

    gatherEnd = clock();    //Terminando a contagem do tempo de constru��o dos dataframes
    float gatherTime = (float) (gatherEnd - gatherStart) / CLOCKS_PER_SEC * 1;  //Calculando delta

    cout << "Tempo de build de dataframe: " << gatherTime << "s" << endl;

//------------------------------------------[END DATA GATHERING]--------------------------------------------

    bool exit = false;

    do {
        std::string func;

        cout << endl << "Insira o nome da funcao desejada: ";
        std::getline(std::cin, func);

        if(func=="exit") {
            exit = true;
            break;
        }
        if (func=="player") {   //Query nome
            std::string prefix;

            cout << "Insira um prefixo para a pesquisa: ";
            std::getline(std::cin, prefix);

            prefix[0] = std::toupper(prefix[0]);

            std::vector<int> ids;

            Player* a = new Player();

            trie_playerNames_df.getItem(prefix, ids);

            std::cout << "Resultado da pesquisa: " << endl;
            ht_players_df.printHeader(false);
            for (int i=0; i < ids.size(); i++) {
                if(ht_players_df.getItem(ids[i], a))
                    ht_players_df.printPlayer(a);
                else
                    printf("n�o achou");
            }
        }
        if (func=="user") { //Query users
            int query;

            std::cout << "Informe um id de usu�rio para a pesquisa: ";
            std::cin >> query;

            std::vector<pair<float, int>> user;
            Player* a = new Player();

            std::cout << "Resultado da pesquisa: " << endl;
            ht_players_df.printHeader(true);
            if(ht_users_df.getItem(query, user)) {
                for (int i = 0; i < user.size(); i++) {
                    if(ht_players_df.getItem(user[i].second, a)) {
                    sort(user.rbegin(), user.rend());
                    user.resize(20);
                    ht_players_df.printPlayer(a, user[i].first);
                    }
                }
            } else cout << "N�o achou chefia" << endl;
        }
        if (func=="top") {  //Query top<n> posi��es
            int querySize;
            std::string query;

            cout << "Insira o tamanho da tabela desejada: ";
            cin >> querySize;

            cout << "Insira a posicao desejada: ";
            cin >> query;

            std::vector<int> pos;
            std::vector<Player*> players;

            std::cout << "Resultado da pesquisa: " << endl;
            ht_players_df.printHeader(false);
            if(lst_positions_df.getItem(query, pos)) {
                    if(ht_players_df.getItem(pos, players))
                    sort(players.begin(), players.end(), &pSortDesc);
                    //players.erase(std::remove_if(players.begin(), players.end(), &removeFewRatings), players.end());
                    if(!players.empty())
                        players.resize(querySize);
                    for (Player* p : players) {
                        ht_players_df.printPlayer(p);
                    }
            }
        }
        if (func=="tags") { //Query tags
            std::vector<std::string> queryTags;
            std::string queryTag;

            cout << "Insira uma ou mais tags para pesquisa" << endl;
            cout << "Insira abaixo caso desejar inserir mais tags" << endl;
            cout << "'go' para realizar a pesquisa" << endl;
            std::getline(std::cin, queryTag);
            queryTag[0] = std::toupper(queryTag[0]);

            queryTags.push_back(queryTag);

            while (queryTag!="Go") {
                std::getline(std::cin, queryTag);
                queryTag[0] = std::toupper(queryTag[0]);
                if(queryTag=="Go")
                    break;
                queryTags.push_back(queryTag);
            }

            std::vector<int> tags;
            Player* a = new Player();

            std::cout << endl << "Resultado da pesquisa: " << endl;
            ht_players_df.printHeader(false);
            if(ht_tags_df.getItem(queryTags, tags)) {
                for (int i = 0; i < tags.size(); i++) {
                    if(ht_players_df.getItem(tags[i], a))
                    ht_players_df.printPlayer(a);
                    //cout << a->key << a->name << a->positions << a->rating << " | " << a->rCount << endl;
                }
            } else cout << "N�o achou chefia" << endl;
        }

    } while(!exit);


}
