//  Student: Thibault Soumoy
//  Rolnummer:

#include "possibleMoves.h"
#include "iostream"

using namespace  std;
vector<pair<SchaakStuk*, pair<int, int>>> const possibleMoves::getPossibleAttacs() const{
    /*
     * geeft een vector van pairs met locatie's voor andere schaakstukken te slaan
     */
    return aanval;
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

bool possibleMoves::insertPossibleAttacks(SchaakStuk *s, pair<int, int> m) {
    /*
     * insert een schaakstuk met de mogelijke locatie om een ander schaakstuk te slaan
     */
    if(s != nullptr){
        aanval.emplace_back(s, m);
        return true;
    }
    return false;
}

bool possibleMoves::insertPossibleSchaak(SchaakStuk *s, pair<int, int> m) {
    /*
     * insert een schaakstuk met de mogelijke locatie om een ander schaakstuk te slaan
     */
    if(s != nullptr){
        schaak.emplace_back(s, m);
        return true;
    }
    return false;
}

bool possibleMoves::insertPossibleSchaakmat(SchaakStuk *s, pair<int, int> m) {
    /*
     * insert een schaakstuk met de mogelijke locatie om de andere speler schaakmat te zetten
     */
    if(s != nullptr){
        schaakmat.emplace_back(s, m);
        return true;
    }
    return false;
}

bool possibleMoves::insertPossibleSchaakStuk(SchaakStuk *s, pair<int, int> m) {
    /*
     * insert een schaakstuk met de mogelijke locatie om zch naar te verplaatsen
     */
    if(s != nullptr){
        schaakstuk.emplace_back(s, m);
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
    aanval.insert(aanval.end(), p.aanval.begin(), p.aanval.end());
    schaakstuk.insert(schaakstuk.end(), p.schaakstuk.begin(), p.schaakstuk.end());
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomAttac() const{
    /*
     * geeft een random positie om een ander schaakstuk te slaan
     */
    return getRandom(&aanval);
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomSchaak() const{
    /*
     * geeft een random positie om de andere speler schaak te zetten
     */
    return getRandom(&schaak);
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomSchaakmat() const{
    /*
     * geeft een random positie om de andere speler schaakmat te zetten
     */
    return getRandom(&schaakmat);
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomSchaakstuk() const{
    /*
     * geeft een random positie naar waar de speleren een schaakstuk kan verplaatsen
     */
    return getRandom(&schaakstuk);
}

pair<SchaakStuk *, pair<int, int>> const possibleMoves::getRandomBest() const{
    /*
     * geeft de beste willekeurige optie om een schaakstuk te verplaatsen
     * (beste:schaakmat > schaak > slagen > random)
     */
    if(!schaakmat.empty()){
        return getRandomSchaakmat();
    }
    else if(!schaak.empty()){
        return getRandomSchaak();
    }
    else if(!aanval.empty()){
        return getRandomAttac();
    }
    else{
        return getRandomSchaakstuk();
    }
}

pair<SchaakStuk *, pair<int, int>> possibleMoves::getRandom(const vector<pair<SchaakStuk*, pair<int, int>>>* v) {
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
    all.insert(all.end(), aanval.begin(), aanval.end());
    all.insert(all.end(), schaakstuk.begin(), schaakstuk.end());
    return getRandom(&all);
}

int const possibleMoves::getMoveCount() {
    /*
     * geeft het aantal opties waar de schaakstukken zich naar kunnen verplaatsen
     */
    return schaakmat.size() + schaak.size() + aanval.size() + schaakstuk.size();
}

