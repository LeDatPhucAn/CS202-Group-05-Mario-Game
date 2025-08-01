#include "Items.hpp"

void Items::appear(float dt, Block *owner)
{
	// TODO: animate the item moving into view from the block beneath it
	DrawRectangleLines(
		owner->getPosition().x,
		owner->getPosition().y - 1,
		sprite.frameRecs[0].width,
		sprite.frameRecs[0].height,
		{168, 168, 0, 255});
	DrawTextureRec(sprite.texture, sprite.frameRecs[0], (Vector2)pos, WHITE);
}

void Items::test_appear()
{
	DrawRectangleLines(
		GetMouseX(),
		GetMouseY() - 1,
		sprite.frameRecs[0].width,
		sprite.frameRecs[0].height,
		{168, 168, 0, 255});
	DrawTextureRec(sprite.texture, sprite.frameRecs[0], (Vector2)pos, WHITE);
}

void Items::display()
{
	DrawRectangleLines(
		getPosition().x,
		getPosition().y,
		sprite.frameRecs[0].width,
		sprite.frameRecs[0].height,
		{168, 168, 0, 255});
	DrawTextureRec(sprite.texture, sprite.frameRecs[0], (Vector2)pos, WHITE);
}