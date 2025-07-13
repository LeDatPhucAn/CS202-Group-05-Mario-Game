#include "Block.hpp"

Block::Block(int _gid, Vector2 _pos, Vector2 _size,
          	Texture2D _tex, Rectangle _src)
        : GameObject(_pos, _size), gid(_gid), texture(_tex), srcRec(_src)
		{
}

Block::Block(tson::Object &obj,
          Vector2 _pos, Vector2 _size,
          Texture2D _tex, Rectangle _src)
        : GameObject(_pos, _size), gid(obj.getGid()), texture(_tex), srcRec(_src) {
	// Dùng obj.get<T>(name, default) từ Tileson
	isSolid = obj.get<bool>("isSolid");
	isBreakable = obj.get<bool>("isBreakable");
	isQuestion = obj.get<bool>("isQuestion");
	isInvisible = obj.get<bool>("isInvisible");
	isUsed = obj.get<bool>("isUsed");

	// containsItem trong Tiled lưu dưới dạng int
	{
		int iv = obj.get<int>("containsItem");
		//contains = static_cast<Contains>(iv);
	}

	//isPipeEntrance = obj.get<bool>("isPipeEntrance");
	isFlagPole = obj.get<bool>("isFlagPole");
}

void Block::update() {}

void Block::display() {
	if (isInvisible && !isUsed)
		return;
	DrawTextureRec(texture, srcRec, getPosition(), WHITE);
}

void Block::updateCollision(GameObject *other, int type) {
}

GroundBlock::GroundBlock(int _gid, Vector2 _pos, Vector2 _size, Texture2D _tex, Rectangle _src) : Block(_gid, _pos, _size, _tex, _src) {
	isSolid = true;
	isBreakable = false;
	isQuestion = false;
	isInvisible = false;
	isUsed = false;
	isFlagPole = false;
}

GroundBlock::GroundBlock(tson::Object &obj, Vector2 _pos, Vector2 _size, Texture2D _tex, Rectangle _src) : Block(obj, _pos, _size, _tex, _src) {
	isSolid = true;
	isBreakable = false;
	isQuestion = false;
	isInvisible = false;
	isUsed = false;
	isFlagPole = false;
}

BrickBlock::BrickBlock(int _gid, Vector2 _pos, Vector2 _size, Texture2D _tex, Rectangle _src) : Block(_gid, _pos, _size, _tex, _src) {
	isSolid = true;
	isBreakable = true;
	isQuestion = false;
	isInvisible = false;
	isUsed = false;
	isFlagPole = false;
}

BrickBlock::BrickBlock(tson::Object &obj, Vector2 _pos, Vector2 _size, Texture2D _tex, Rectangle _src) : Block(obj, _pos, _size, _tex, _src) {
	isSolid = true;
	isBreakable = true;
	isQuestion = false;
	isInvisible = false;
	isUsed = false;
	isFlagPole = false;
}