#include "GameObject.hpp"
#include "Block.hpp"
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

	// displaying and animation
	void appear(float dt);
	void display();

protected:
	Sprite sprite;
	Movement movement;
	Direction direction;
	Block* owner;
};