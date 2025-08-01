#pragma once

#include "Items.hpp"
#include "Mario.hpp"

class GrowMushroom : public Items {
public:
	GrowMushroom();
	void affect(Mario* mario);
	void update() override;
};