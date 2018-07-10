#ifndef PLAYER_H
#define PLAYER_H
#include <string>

enum Position{GK = 0, DEF, MID, FWR};

class Player
{
public:
    Player(int number, std::string name, int birth, int weight, int height, int pos, int id = -1);

    void setNumber(int);
    void setName(std::string);
    void setBirth(int);
    void setHeight(int);
    void setWeight(int);
    void setPos(int);
    void setId(int);

    int getNumber() const;
    std::string getName() const;
    int getBirth() const;
    int getHeight() const;
    int getWeight() const;
    Position getPos() const;
    int getId() const;

    virtual ~Player();
private:
    int number;
    std::string name;
    int birth;
    int weight;
    int height;
    Position pos;
    int id;
};

#endif // PLAYER_H
