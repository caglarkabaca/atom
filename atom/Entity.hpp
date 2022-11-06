#pragma once

#include "Structs.hpp"

class Entity
{
public:
	Entity(Vector2D pos, Vector2D dir, Vector2D plane);

	Vector2D getPos();
	void setPos(Vector2D pos);

	Vector2D getDir();
	void setDir(Vector2D dir);

	Vector2D getPlane();
	void setPlane(Vector2D plane);

private:
	Vector2D pos, dir, plane;
};

