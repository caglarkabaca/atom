#pragma once

#include "Structs.hpp"

class Entity
{
public:
	Entity(Vector2D pos, double angle);

	Vector2D getPos();
	void setPos(Vector2D pos);
	void addPosX(double x);
	void addPosY(double y);

	Vector2D getDir();
	void setDir(Vector2D dir);

	double getAngle();
	void setAngle(double angle);

	double shear = 0;
private:
	Vector2D pos, dir;
	double angle;
};

