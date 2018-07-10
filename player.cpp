#include "player.h"

Player::Player(int number, std::string name, int birth, int weight, int height, int pos, int id):
    number(number),
    name(name),
    birth(birth),
    weight(weight),
    height(height),
    pos((Position)pos),
    id(id)
{

}

void Player::setNumber(int num)
{
    this->number = num;
}

void Player::setName(std::string name)
{
    this->name = name;
}

void Player::setBirth(int birth)
{
    this->birth = birth;
}

void Player::setHeight(int height)
{
    this->height = height;
}

void Player::setWeight(int weight)
{
    this->weight = weight;
}

void Player::setPos(int pos)
{
    this->pos = (Position)pos;
}

int Player::getNumber() const
{
    return this->number;
}

std::string Player::getName() const
{
    return this->name;
}

int Player::getBirth() const
{
    return this->birth;
}

int Player::getHeight() const
{
    return this->height;
}

int Player::getWeight() const
{
    return this->weight;
}

Position Player::getPos() const
{
    return this->pos;
}

int Player::getId() const
{
    return this->id;
}

void Player::setId(int id)
{
    this->id  = id;
}

Player::~Player(){

}
