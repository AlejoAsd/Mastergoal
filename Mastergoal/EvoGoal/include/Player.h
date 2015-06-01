#ifndef PLAYER_H
#define PLAYER_H

namespace Common
{
	enum PLAYER_ID
	{
        ID_N = 1,
        ID_P,
        ID_L,
        ID_C,
        ID_G
    };

	enum ORIENTATION
	{
		RIGHT = 0,
		LEFT,
		NONE
	};

	class Player
	{
	public:
		// Constructores
		Player(ORIENTATION orientation, PLAYER_ID id);

		Player(const Player& player);

		Player& operator = (const Player& player);

		// Getters y Setters
		ORIENTATION GetOrientation() const { return this->orientation; }

		void SetOrientation(ORIENTATION orientation) { this->orientation = orientation; }

		PLAYER_ID GetId() const { return this->id; }

		void SetId(PLAYER_ID id) { this->id = id; }

		inline bool Equals(const Player& player) const
		{
			if (this->orientation == player.orientation
				&& this->id == player.id)
				return true;

			return false;
		}

		bool operator == (const Player& player);

	protected:
		ORIENTATION orientation;

		PLAYER_ID id;

		Player() {}
	};
}

#endif
