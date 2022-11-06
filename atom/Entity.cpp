#include "Entity.hpp"

#include <iostream>

Entity::Entity(Vector2D pos, Vector2D dir, Vector2D plane): pos(pos), dir(dir), plane(plane) {}

Vector2D Entity::getPos() { return pos; }
void Entity::setPos(Vector2D p) { pos.x = p.x; pos.y = p.y; }

Vector2D Entity::getDir() { return dir; }
void Entity::setDir(Vector2D d) { dir.x = d.x; dir.y = d.y; }

Vector2D Entity::getPlane() { return plane; }
void Entity::setPlane(Vector2D pl) { plane.x = pl.x; plane.y = pl.y; }