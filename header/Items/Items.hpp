#include "GameObject.hpp"
#include "Structs.hpp"

class Items : public GameObject {
public:
	// getters
	Sprite getSprite();
	Movement getMovement();
	Direction getDirection();

	// setters
	void setSprite(Sprite sprite);
	void setMovement(Movement movement);
	void setDirection(Direction direction);

	// logic
	void appear();

protected:
	Sprite sprite;
	Movement movement;
	Direction direction;
};