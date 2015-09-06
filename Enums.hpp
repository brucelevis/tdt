#pragma once

enum class ENTITY_STATE
{
	NONE, NORMAL, TURNING,
	COUNT
};

enum class FACTION
{
	FRIENDLY, ENEMY, NEUTRAL
};

enum class ENTITY_TYPE
{
	NONE = 0,
	WALL = 1 << 0,
	MINION = 1 << 1,
	ENEMY = 1 << 2,
	BUILDING = 1 << 3,
	MISC = 1 << 4
};

enum class EVENT_TYPE
{
	NONE, KILL_ENTITY, GOLD_DROPPED,
	COUNT
};

enum class TASK_TYPE
{
	NONE, GO_TO, GO_NEAR, GO_KILL, KILL, GET_IN_RANGE,
	GO_PICK_UP_GOLD, PICK_UP_GOLD,
	COUNT
};

enum class ATTACK_TYPE
{
	NONE, MELEE, RANGED,
	COUNT
};

enum class GAME_STATE
{
	RUNNING, ENDED, PAUSED, MENU
};

enum class TIME_EVENT
{
	NONE, START_EVENT, END_EVENT,
	COUNT
};

namespace DIRECTION
{
	enum VAL
	{
		UP = 1, DOWN, LEFT, RIGHT,
		UP_LEFT, UP_RIGHT,
		DOWN_LEFT, DOWN_RIGHT,
		NONE
	};
}