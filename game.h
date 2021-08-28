//  Student:
//  Rolnummer:
//  Opmerkingen: (bvb aanpassingen van de opgave)
//

#ifndef SCHAKEN_GAME_H
#define SCHAKEN_GAME_H
#include "SchaakStuk.h"
#include <iostream>
#include <QMessageBox>
#include <QPushButton>
#include "possibleMoves.h"
using namespace std;

class Game {
// variabelen om de status van het spel/bord te bewaren

public:
    Game();
    ~Game();

    bool move(SchaakStuk* s,pair<int, int> newLocation, bool AIplayer = false); // Verplaats stuk s naar rij r en kolom k

    bool schaak(zw kleur);
    bool thread(pair<int,int> location, zw kleur);
    bool schaakmat(zw kleur);
    bool pat(zw kleur);
    void setStartBord();

    SchaakStuk* getPiece(int r, int k) const;
    SchaakStuk* dead = nullptr;

    void setPiece(int r, int k, SchaakStuk* s);
    bool containsPiece(int r, int k) const;
    bool containsAnnemie(int r, int k,const SchaakStuk *s) const;
    bool deletePiece(int r, int k);

    pair<int,int> getkoningLocation(zw kleur) const;
    pair<pair<int, int>, pair<int,int>> AIplayer(zw kleur,bool bestMove = true);
    possibleMoves getAttacPossitions(zw kleur, SchaakStuk* s);
    bool mogelijkSchaak(int r, int k, SchaakStuk *s, zw kleur);
    bool mogelijkSchaakMat(int r, int k, SchaakStuk *s, zw kleur);
private:
    SchaakStuk* bord[8][8];
    void checkBijzondereRegels(pair<int, int> prevLocation, pair<int, int> newLocation, bool AIplayer = false);
    void passand(pair<int, int> prevLocation, pair<int, int> newLocation);
    void rokade(pair<int, int> prevLocation, pair<int, int> newLocation);
    void promotie(pair<int, int> newLocation, bool AIplayer = false);
};


#endif //SCHAKEN_GAME_H
