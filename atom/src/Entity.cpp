#include "Entity.hpp"

#include <iostream>
#include <math.h>

Entity::Entity(Vector2D pos, double angle): pos(pos), dir({cos(angle), sin(angle)}), angle(angle) {}

Vector2D Entity::getPos() { return pos; }
void Entity::setPos(Vector2D p) { pos.x = p.x; pos.y = p.y; }
void Entity::addPosX(double x) { pos.x += x; }
void Entity::addPosY(double y) { pos.y += y; }

Vector2D Entity::getDir() { return dir; }
void Entity::setDir(Vector2D d) { dir.x = d.x; dir.y = d.y; }

double Entity::getAngle() { return angle; }
void Entity::setAngle(double angle) { this->angle = angle; }

double Entity::getShear() { return shear; }
void Entity::setShear(double shear) { this->shear = shear; }
