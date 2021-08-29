//  Student: Thibault Soumoy
//  Rolnummer:
//

#include "SchaakStuk.h"
#include "game.h"

vector<pair<int, int>> Pion::geldige_zetten(const Game* g) const{
    /*
     * geeft de geldige zetten waar de pions zich naar kan verplaatsen in de meegegeven Game
     */
    vector<pair<int, int>> zetten;
    int addOne = 1;
    int addTwo = 2;
    if(getKleur() == wit){
        addOne = -1;
        addTwo = -2;
    }
    if(!g->containsPiece((r + addOne), k) && r + addOne <= 7 && r + addOne >= 0){
        zetten.emplace_back(r + addOne, k);
    }
    if(!g->containsPiece((r + addTwo), k) && !g->containsPiece((r + addOne), k) && ((r == 1 && addTwo > 0) || (r == 6 && addTwo < 0))) zetten.emplace_back(r + addTwo, k);

    //voor een schaakstuk te slaan
    if(g->tegenstanderPositie(r + addOne, k + addOne, this) && (r + addOne <= 7 && r + addOne >= 0) && (k + addOne <= 7 && k + addOne >= 0)) zetten.emplace_back(r + addOne, k + addOne);
    if(g->tegenstanderPositie(r + addOne, k - addOne, this) && (r + addOne <= 7 && r + addOne >= 0) && (k - addOne <= 7 && k - addOne >= 0)) zetten.emplace_back(r + addOne, k - addOne);

    //voor passant
    if(g->getPiece(r, k  + addOne) != nullptr && g->tegenstanderPositie(r, k + addOne, this) && g->getPiece(r, k + addOne)->toShortString()[0] == 'P' && g->getPiece(r, k + addOne)->passant) zetten.emplace_back(r + addOne, k + addOne);//passant
    if(g->getPiece(r, k  - addOne) != nullptr && g->tegenstanderPositie(r, k - addOne, this) && g->getPiece(r, k - addOne)->toShortString()[0] == 'P' && g->getPiece(r, k - addOne)->passant) zetten.emplace_back(r + addOne, k - addOne);//passant

    return zetten;
}

vector<pair<int, int>> Pion::aanval_zetten(const Game* g) const{
    /*
     * geeft terug naar waar dit schaakstuk zou kunnen aanvallen (ook indien het in deze sitatie niet mag)
     */
    vector<pair<int, int>> zetten;
    int addOne = 1;
    if(getKleur() == wit) addOne = -1;

    if((r + addOne <= 7 && r + addOne >= 0) && (k + addOne <= 7 && k + addOne >= 0)) zetten.emplace_back(r + addOne, k + addOne);
    if((r + addOne <= 7 && r + addOne >= 0) && (k - addOne <= 7 && k - addOne >= 0)) zetten.emplace_back(r + addOne, k - addOne);
    return zetten;
}

void SchaakStuk::hasMoved() {
    /*
     * telt 1 bij bij het aantal zetten
     */
    moveCount++;
}

string Pion::toShortString() const {
    /*
     * Geeft de ShortString van het schaakstuk [Letter][kleur(int)]
     */
    return "P" + to_string((int)this->getKleur());
}

vector<pair<int, int>> Toren::geldige_zetten(const Game* g) const{
    /*
     * geeft de geldige zetten waar de toren zich naar kan verplaatsen in de meegegeven Game
     */
    vector<pair<int, int>> zetten;
    for(int i = r + 1; i <= 7; i++){
        if((g->tegenstanderPositie(i, k, this) || !g->containsPiece(i, k)) && i != r) zetten.emplace_back(i, k);
        if(g->containsPiece(i, k) && i != r) break;
    }
    for(int i = r - 1; i >= 0; i--){
        if((g->tegenstanderPositie(i, k, this) || !g->containsPiece(i, k)) && i != r) zetten.emplace_back(i, k);
        if(g->containsPiece(i, k) && i != r) break;
    }

    for(int i = k + 1; i <= 7; i++){
        if((g->tegenstanderPositie(r, i, this) || !g->containsPiece(r, i)) && i != k) zetten.emplace_back(r, i);
        if(g->containsPiece(r, i) && i != k) break;
    }
    for(int i = k - 1; i >= 0; i--){
        if((g->tegenstanderPositie(r, i, this) || !g->containsPiece(r, i)) && i != k) zetten.emplace_back(r, i);
        if(g->containsPiece(r, i) && i != k) break;
    }

    return zetten;
}

string Toren::toShortString() const{
    /*
     * Geeft de ShortString van het schaakstuk [Letter][kleur(int)]
     */
    return "R" + to_string((int)this->getKleur());
}

vector<pair<int, int>> Loper::geldige_zetten(const Game* g) const{
    /*
     * geeft de geldige zetten waar de loper zich naar kan verplaatsen in de meegegeven Game
     */
    vector<pair<int, int>> zetten;

    int i = r;
    int a = k;
    //diagonaal naar onder rechts
    while(i <= 7 && a <= 7){
        if((g->tegenstanderPositie(i, a, this) || !g->containsPiece(i, a))) zetten.emplace_back(i, a);
        if(g->containsPiece(i, a) && i != r && a != k) break;
        a++;
        i++;
    }

    i = r;
    a = k;
    //diagonaal naar boven links
    while(i >= 0 && a >= 0){
        if((g->tegenstanderPositie(i, a, this) || !g->containsPiece(i, a))) zetten.emplace_back(i, a);
        if(g->containsPiece(i, a) && i != r && a != k) break;
        a--;
        i--;
    }

    i = r;
    a = k;
    //diagonaal naar onder links
    while(i <= 7 && a>= 0){
        if((g->tegenstanderPositie(i, a, this) || !g->containsPiece(i, a))) zetten.emplace_back(i, a);
        if(g->containsPiece(i, a) && i != r && a != k) break;
        a--;
        i++;
    }

    i = r;
    a = k;
    //diagonaal naar boven recht
    while(i >= 0 && a <= 7){
        if((g->tegenstanderPositie(i, a, this) || !g->containsPiece(i, a))) zetten.emplace_back(i, a);
        if(g->containsPiece(i, a) && i != r && a != k) break;
        a++;
        i--;
    }
    return zetten;
}

string Loper::toShortString() const{
    /*
     * Geeft de ShortString van het schaakstuk [Letter][kleur(int)]
     */
    return "B" + to_string((int)this->getKleur());
}

vector<pair<int, int>> Paard::geldige_zetten(const Game* g) const{
    /*
     * geeft de geldige zetten waar het paard zich naar kan verplaatsen in de meegegeven Game
     */
    vector<pair<int, int>> zetten;

    for(int i = -2; i <= 2; i++){
        for(int a = -2; a <= 2; a++){
            if((g->tegenstanderPositie(r + i, k + a, this) || !g->containsPiece(r + i, k + a)) && (r + i <= 7 && k + a <= 7 && k + a >= 0 && r + i >= 0 && abs(i) != abs(a) && i != 0 && a != 0)) zetten.emplace_back(r + i, k + a);
        }
    }
    return zetten;
}

string Paard::toShortString() const{
    /*
     * Geeft de ShortString van het schaakstuk [Letter][kleur(int)]
     */
    return "H" + to_string((int)this->getKleur());
}

vector<pair<int, int>> Koning::geldige_zetten(const Game* g) const{
    /*
     * geeft de geldige zetten waar het koning zich naar kan verplaatsen in de meegegeven Game
     */
    vector<pair<int, int>> zetten;

    for(int i = -1; i <= 1; i++){
        for(int a = -1; a <= 1; a++){
            if((g->tegenstanderPositie(r + i, k + a, this) || !g->containsPiece(r + i, k + a)) && (r + i >= 0 && r + i <= 7 && k + a >= 0 && k + a <= 7)) zetten.emplace_back(r + i, k + a);
        }
    }

    //indien korte rokade mogelijk is
    if(isNotUsed() && g->containsPiece(r, k + 3) && g->getPiece(r, k + 3)->isNotUsed()) {
        bool allowRokade = true;
        for(int i = 1; i <= 2; i++){
            if(g->containsPiece(r, k + i)) allowRokade = false;
        }
        if(allowRokade){
            zetten.emplace_back(r, k + 2);
        }
    }

    //indien lange rokade mogelijk is
    if(isNotUsed() && g->containsPiece(r, k - 4) && g->getPiece(r, k - 4)->isNotUsed()) {
        bool allowRokade = true;
        for(int i = 1; i <= 3; i++){
            if(g->containsPiece(r, k - i)) allowRokade = false;
        }
        if(allowRokade){
            zetten.emplace_back(r, k - 2);
        }
    }


    return zetten;
}

string Koning::toShortString() const{
    /*
     * Geeft de ShortString van het schaakstuk [Letter][kleur(int)]
     */
    return "K" + to_string((int)this->getKleur());
}

vector<pair<int, int>> Koningin::geldige_zetten(const Game* g) const{
    /*
     * geeft de geldige zetten waar het koningin zich naar kan verplaatsen in de meegegeven Game
     */

    //dit zijn de zetten waar de loper zich naar kan verplaatsen + de toren

    Loper lp = Loper(getKleur());
    lp.updateLocation(r, k);

    Toren tr = Toren(getKleur());
    tr.updateLocation(r, k);

    vector<pair<int, int>> zetten = lp.geldige_zetten(g);
    vector<pair<int, int>> toren = tr.geldige_zetten(g);
    zetten.insert(zetten.end(), toren.begin(), toren.end());
    return zetten;
}

string Koningin::toShortString() const{
    /*
     * Geeft de ShortString van het schaakstuk [Letter][kleur(int)]
     */
    return "Q" + to_string((int)this->getKleur());
}
