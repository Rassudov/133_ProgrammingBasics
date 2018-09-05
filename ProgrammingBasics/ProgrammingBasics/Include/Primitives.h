#ifndef __PRIMITIVES
#define __PRIMITIVES
#define PI 3.141592653589793 

#include "Vector2.h"

class Primitive {
private:
	const ID id;
	const object_type type;

public:
	Primitive(ID, object_type);
	virtual double GetDistance(const Vector2&) const = 0;
	ID GetID() const;
	object_type GetType();
};

class Point : public Primitive {
private:
	Primitive* parent;
public:
	Vector2 position;

	Point(const Vector2&);
	Point(double, double);
	Point(const Point&);

	double GetDistance(const Vector2&) const;

	// do we need this function

	Vector2 GetPosition() const;
	void SetPosition(const Vector2&);
	void SetPosition(double, double);

	Primitive* GetParent();
	void DeleteParent();
	bool SetParent(Primitive*);
	//
};

class Segment : public Primitive {
private:
	// temp class - move to requirement!!
	class Equation {
	public:
		double a;
		double b;
		double c;
	};
	Equation* CreateEquation() {
		Equation* NewEquation = new Equation;
		Vector2 vector1 = point1->GetPosition();
		Vector2 vector2 = point2->GetPosition();
		NewEquation->a = vector1.y - vector2.y;
		NewEquation->b = vector2.x - vector1.x;
		NewEquation->c = vector1.x * vector2.y - vector2.x  * vector1.y;
		return NewEquation;
	}
	//

public:
	Point* point1;
	Point* point2;

	Segment(Point*, Point*);
	double GetDistance(const Vector2&) const;
	double GetLength() const;

	// temp functions
	ID GetPoint1_ID() const;
	ID GetPoint2_ID() const;
	Vector2 GetPoint1_pos() const;
	Vector2 GetPoint2_pos() const;
	void SetPoint1_pos(Vector2);
	void SetPoint2_pos(Vector2);
	//

	// move to requirement!!!
	double Inequality(Vector2);
};

class Arc : public Primitive {
private:
	Vector2 center;
public:
	Point* point1;
	Point* point2;
	// from 0 to 2pi
	double angle;

	Arc(Point*, Point*, double);

	double GetDistance(const Vector2&) const;
	void RestoreCenter();
	Vector2 GetCenter() const;

	// temp functions
	ID GetPoint1_ID() const;
	ID GetPoint2_ID() const;
	Vector2 GetPoint1_pos() const;
	Vector2 GetPoint2_pos() const;
	void SetPoint1_pos(Vector2);
	void SetPoint2_pos(Vector2);
	double GetAngle() const;
	void SetAngle(double);
	//
};

class Circle : public Primitive {
private:
public:
	double radius;
	Point* center;

	Circle(Point*, double);

	double GetDistance(const Vector2&) const;
	Vector2 GetCenter() const;

	// temp functions
	ID GetCenter_ID() const;
	void SetCenter_pos(Vector2);
	double GetRadius() const;
	void SetRadius(double);
	//
};

#endif