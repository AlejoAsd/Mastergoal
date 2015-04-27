#include "stdafx.h"
#include "Player.h"

namespace Common
{
	Player::Player(ORIENTATION orientation, PLAYER_ID id)
	{
		this->orientation = orientation;
		this->id = id;
	}

	Player::Player(const Player& player)
	{
		this->orientation = player.orientation;
		this->id = player.id;
	}

	Player& Player::operator = (const Player& player)
	{
		this->orientation = player.orientation;
		this->id = player.id;

		return *this;
	}

	bool Player::operator ==(const Common::Player &player)
	{
		if (this->orientation == player.orientation
			&& this->id == player.id)
			return true;

		return false;
	}
}
