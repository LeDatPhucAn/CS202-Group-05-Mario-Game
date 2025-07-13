/*
#include "BlockFactory.hpp"
#include <string>

using namespace std;

Block* BlockFactory::create(tson::Object& obj, Vector2 pos, Vector2 size, Texture2D tex, Rectangle src) {
	string type = obj.get<string>("type"); // From Tiled
	if (type == "QuestionBlock") {
		Contains item = static_cast<Contains>(obj.get<int>("containsItem"));
		return new QuestionBlock(obj.getGid(), pos, size, tex, src, item);
	} else if (type == "BrickBlock") {
		return new BrickBlock(obj.getGid(), pos, size, tex, src);
	} else {
		return new Block(obj, pos, size, tex, src); // default fallback
	}
}
*/