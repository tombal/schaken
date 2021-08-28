//
// Created by stein on 16/01/2021.
//

#include "possibleMoves.h"
#include "iostream"

using namespace  std;
vector<pair<SchaakStuk*, pair<int, int>>> const possibleMoves::getPossibleAttacs() const{
    /*
     * geeft een vector van pairs met locatie's voor andere schaakstukken te slaan
     */
    return attac;
}

vector<pair<SchaakStuk*, pair<int, int>>> const possibleMoves::getPossibleSchaak() const{
    /*
     * geeft een vector van pairs met locatie's voor de andere schaak te zetten
     */
    return schaak;
}

vector<pair<SchaakStuk*, pair<int, int>>> const possibleMoves::getPossibleSchaakmat() const{
    /*
     * geeft een vector van pairs met locatie's voor de andere schaakmat te zetten
     */
    return schaakmat;
}

vector<pair<SchaakStuk*, pair<int, int>>> const possibleMoves::getPossibleSchaakstuken() const{
    /*
     * geeft een vector van pairs met locatie's van alle schaakstukken die verplaatst kunnen worden behalve degene die schaak, schaakmat, of kunnen slagen
     */
    return schaakstuk;
}

bool possibleMoves::insertPossibleAttacs(SchaakStuk *s, pair<int, int> m) {
    /*
     * insert een schaakstuk met de mogelijke locatie om een ander schaakstuk te slaan
     */
    if(s != nullptr){
        attac.push_back(pair<SchaakStuk*, pair<int, int>>(s, m));
        return true;
    }
    return false;
}

bool possibleMoves::insertPossibleSchaak(SchaakStuk *s, pair<int, int> m) {
    /*
     * insert een schaakstuk met de mogelijke locatie om een ander schaakstuk te slaan
     */
    if(s != nullptr){
        schaak.push_back(pair<SchaakStuk*, pair<int, int>>(s, m));
        return true;
    }
    return false;
}

bool possibleMoves::insertPossibleSchaakmat(SchaakStuk *s, pair<int, int> m) {
    /*
     * insert een schaakstuk met de mogelijke locatie om de andere speler schaakmat te zetten
     */
    if(s != nullptr){
        schaakmat.push_back(pair<SchaakStuk*, pair<int, int>>(s, m));
        return true;
    }
    return false;
}

bool possibleMoves::insertPossibleSchaakStuk(SchaakStuk *s, pair<int, int> m) {
    /*
     * insert een schaakstuk met de mogelijke locatie om zch naar te verplaatsen
     */
    if(s != nullptr){
        schaakstuk.push_back(pair<SchaakStuk*, pair<int, int>>(s, m));
        return true;
    }
    return false;
}

void possibleMoves::insert(possibleMoves p) {
    /*
     * insert een andere possibleMoves in deze possibleMoves (possibleMoves = possibleMoves + p)
     */
    schaakmat.insert(schaakmat.end(), p.schaakmat.begin(), p.schaakmat.end());
    schaak.insert(schaak.end(), p.schaak.begin(), p.schaak.end());
    attac.insert(attac.end(), p.attac.begin(), p.attac.end());
    schaakstuk.insert(schaakstuk.end(), p.schaakstuk.begin(), p.schaakstuk.end());
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomAttac() const{
    /*
     * geeft een random possitie om een ander schaakstuk te slaan
     */
    return getRandom(&attac);
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomSchaak() const{
    /*
     * geeft een random possitie om de andere speler schaak te zetten
     */
    return getRandom(&schaak);
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomSchaakmat() const{
    /*
     * geeft een random possitie om de andere speler schaakmat te zetten
     */
    return getRandom(&schaakmat);
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomSchaakstuk() const{
    /*
     * geeft een random possitie naar waar de speleren een schaakstuk kan verplaatsen
     */
    return getRandom(&schaakstuk);
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomBest() const{
    /*
     * geeft de beste willekeurige optie om een schaakstuk te verplaatsen
     * (beste:schaakmat > schaak > slagen > random)
     */
    if(schaakmat.size() > 0){
        return getRandomSchaakmat();
    }
    else if(schaak.size() > 0){
        return getRandomSchaak();
    }
    else if(attac.size() > 0){
        return getRandomAttac();
    }
    else{
        return getRandomSchaakstuk();
    }
}

pair<SchaakStuk *, pair<int, int>> possibleMoves::getRandom(const vector<pair<SchaakStuk*, pair<int, int>>>* v) const{
    /*
     * geeft een random item uit de meegegeven vector
     */
    srand(time(NULL));
    int randomNumber = rand() % v->size();
    auto it = v->begin();
    cout << randomNumber << endl;
    std::advance(it, randomNumber);
    return *it;
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomMove() const{
    /*
     * geeft de beste willekeurige optie om een schaakstuk te verplaatsen
     */
    vector<pair<SchaakStuk*, pair<int, int>>> all;
    all = schaakmat;
    all.insert(all.end(), schaak.begin(), schaak.end());
    all.insert(all.end(), attac.begin(), attac.end());
    all.insert(all.end(), schaakstuk.begin(), schaakstuk.end());
    return getRandom(&all);
}

int const possibleMoves::getMoveCount() {
    /*
     * geeft het aantal opties waar de schaakstukken zich naar kunnen verplaatsen
     */
    return schaakmat.size() + schaak.size() + attac.size() + schaakstuk.size();
}

