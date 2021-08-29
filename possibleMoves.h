//  Student: Thibault Soumoy
//  Rolnummer:

#ifndef SCHAKEN_POSSIBLEMOVES_H
#define SCHAKEN_POSSIBLEMOVES_H

#include <map>
#include "SchaakStuk.h"

class possibleMoves {
public:

    vector<pair<SchaakStuk*, pair<int, int>>> const getPossibleSchaakstuken() const;
    vector<pair<SchaakStuk*, pair<int, int>>> const getPossibleAttacs() const;
    vector<pair<SchaakStuk*, pair<int, int>>> const getPossibleSchaak() const;
    vector<pair<SchaakStuk*, pair<int, int>>> const getPossibleSchaakmat() const;

    bool insertPossibleSchaakStuk(SchaakStuk* s, pair<int, int> m);
    bool insertPossibleAttacks(SchaakStuk* s, pair<int, int> m);
    bool insertPossibleSchaak(SchaakStuk* s, pair<int, int> m);
    bool insertPossibleSchaakmat(SchaakStuk* s, pair<int, int> m);

    void insert(possibleMoves p);

    pair<SchaakStuk*, pair<int, int>> const getRandomSchaakstuk() const;
    pair<SchaakStuk*, pair<int, int>> const getRandomAttac() const;
    pair<SchaakStuk*, pair<int, int>> const getRandomSchaak() const;
    pair<SchaakStuk*, pair<int, int>> const getRandomSchaakmat() const;

    pair<SchaakStuk*, pair<int, int>> const getRandomBest() const;
    pair<SchaakStuk*, pair<int, int>> const getRandomMove() const;
    int const getMoveCount();

private:
    vector<pair<SchaakStuk*, pair<int, int>>> schaakstuk;
    vector<pair<SchaakStuk*, pair<int, int>>> aanval;
    vector<pair<SchaakStuk*, pair<int, int>>> schaak;
    vector<pair<SchaakStuk*, pair<int, int>>> schaakmat;
    static pair<SchaakStuk *, pair<int, int>> getRandom(const vector<pair<SchaakStuk*, pair<int, int>>>* v) ;
};


#endif //SCHAKEN_POSSIBLEMOVES_H
