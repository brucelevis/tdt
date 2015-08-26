#pragma once

#include <Ogre.h>
#include "Enums.hpp"
class EntitySystem;

namespace TimeHelper
{
	/**
	 * Brief: Returns the time that has passed since the timer started.
	 * Param: Entity system which contains the timer.
	 * Param: ID of th entity.
	 */
	Ogre::Real get_curr_time(EntitySystem&, std::size_t);

	/**
	 * Brief: Adds a given time value to a given timer.
	 * Param: Entity system which contains the timer.
	 * Param: ID of the timer.
	 * Param: Time to add.
	 */
	void advance_curr_time(EntitySystem&, std::size_t, Ogre::Real);

	/**
	 * Brief: Completes a given timer (by maxing it's current time).
	 * Param: Entity system which contains the timer.
	 * Param: ID of the timer.
	 */
	void max_curr_time(EntitySystem&, std::size_t);

	/**
	 * Brief: Sets the time a given timer requires for completion.
	 * Param: Entity system which contains the timer.
	 * Param: ID of the timer.
	 * Param: The new time limit.
	 */
	void set_time_limit(EntitySystem&, std::size_t, Ogre::Real);

	/**
	 * Brief: Returns the time a given timer requires for completion.
	 * Param: Entity system which contains the timer.
	 * Param: ID of the timer.
	 */
	Ogre::Real get_time_limit(EntitySystem&, std::size_t);

	/**
	 * Brief: Sets the ID of the event a given timer starts/ends.
	 * Param: Entity system which contains the timer.
	 * Param: ID of the timer.
	 * Param: ID of the event.
	 */
	void set_target(EntitySystem&, std::size_t, std::size_t);

	/**
	 * Brief: Returns the ID of the event a given timer starts/ends.
	 * Param: Entity system which contains the timer.
	 * Param: ID of the timer.
	 */
	std::size_t get_target(EntitySystem&, std::size_t);

	/**
	 * Brief: Sets the type of a given timer.
	 * Param: Entity system which contains the timer.
	 * Param: ID of the timer.
	 * Param: The new type.
	 */
	void set_type(EntitySystem&, std::size_t, TIME_EVENT);

	/**
	 * Brief: Returns the type of a given timer.
	 * Param: Entity system which contains the timer.
	 * Param: ID of the timer.
	 */
	TIME_EVENT get_type(EntitySystem&, std::size_t);
}