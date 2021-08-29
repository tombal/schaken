//  Student: Thibault Soumoy
//  Rolnummer:
//

#ifndef SCHAKEN_SCHAAKSTUK_H
#define SCHAKEN_SCHAAKSTUK_H
#include <vector>
#include <chessboard.h>

using namespace std;

class Game;

enum zw{zwart,wit};

class SchaakStuk {
public:
    SchaakStuk(zw kleur): kleur(kleur) {}
    virtual vector<pair<int,int>> geldige_zetten(const Game* g) const = 0;
    virtual vector<pair<int,int>> aanval_zetten(const Game* g) const = 0;
    virtual string toString() const =0;
    virtual Piece piece() const=0;      // Deze functie krijg je cadeau
                                        // Je hebt die nodig om het stuk in de
                                        // GUI te tekenen
                                        // Meer info: kijk naar mainwindow.cpp, lijn 27
    void hasMoved();

    zw getKleur() const {
        return kleur;
    }
    virtual void updateLocation(int r, int k){
        this->r = r;
        this->k = k;
    }
    pair<int, int>  getLocation(){
        pair<int, int> location;
        location.first = r;
        location.second = k;
        return location;
    }
    virtual bool isKing() const {
        return false;
    }
    virtual bool isPion() const {
        return false;
    }
    virtual string toShortString() const = 0;
    bool passant = false;
    bool isNotUsed() const {
        return moveCount == 0;
    }
    int moveCount = 0;
protected:
    int r, k;
private:
    zw kleur;
};

class Pion:public SchaakStuk {
public:
    Pion(zw kleur):SchaakStuk(kleur) {}
    vector<pair<int,int>> geldige_zetten(const Game*) const override;
    vector<pair<int,int>> aanval_zetten(const Game* g) const override;
    virtual Piece piece() const override {
        return Piece(Piece::Pawn,getKleur()==wit?Piece::White:Piece::Black);
    }
    virtual string toString() const {
        return "Pion";
    }
    bool isPion() const override {return true;}
    virtual string toShortString() const;

};

class Toren:public SchaakStuk {
public:
    Toren(zw kleur):SchaakStuk(kleur) {}
    vector<pair<int,int>> geldige_zetten(const Game*) const override;
    vector<pair<int,int>> aanval_zetten(const Game* g) const override {return geldige_zetten(g);}
    Piece piece() const override {
        return Piece(Piece::Rook,getKleur()==wit?Piece::White:Piece::Black);
    }
    virtual string toString() const {return "Toren";}
    virtual string toShortString() const;
};

class Paard:public SchaakStuk {
public:
    Paard(zw kleur):SchaakStuk(kleur) {}
    vector<pair<int,int>> geldige_zetten(const Game*) const override;
    vector<pair<int,int>> aanval_zetten(const Game* g) const override {return geldige_zetten(g);}
    Piece piece() const override {
        return Piece(Piece::Knight,getKleur()==wit?Piece::White:Piece::Black);
    }
    virtual string toString() const {return "Paard";}
    virtual string toShortString() const;
};

class Loper:public SchaakStuk {
public:
    Loper(zw kleur):SchaakStuk(kleur) {}
    vector<pair<int,int>> geldige_zetten(const Game*) const override;
    vector<pair<int,int>> aanval_zetten(const Game* g) const override {return geldige_zetten(g);}
    Piece piece() const override {
        return Piece(Piece::Bishop,getKleur()==wit?Piece::White:Piece::Black);
    }
    virtual string toString() const {return "Loper";}
    virtual string toShortString() const;
};

class Koning:public SchaakStuk {
public:
    Koning(zw kleur):SchaakStuk(kleur) {}
    vector<pair<int,int>> geldige_zetten(const Game*) const override;
    vector<pair<int,int>> aanval_zetten(const Game* g) const override {return geldige_zetten(g);}
    Piece piece() const override {
        return Piece(Piece::King,getKleur()==wit?Piece::White:Piece::Black);
    }
    virtual string toString() const {return "Koning";}
    virtual string toShortString() const;
    bool isKing() const override {return true;}
};

class Koningin:public SchaakStuk {
public:
    Koningin(zw kleur):SchaakStuk(kleur) {}
    vector<pair<int,int>> geldige_zetten(const Game*) const override;
    vector<pair<int,int>> aanval_zetten(const Game* g) const override {return geldige_zetten(g);}
    Piece piece() const override {
        return Piece(Piece::Queen,getKleur()==wit?Piece::White:Piece::Black);
    }
    virtual string toString() const {return "Koningin";}
    virtual string toShortString() const;
};

#endif //SCHAKEN_SCHAAKSTUK_H
