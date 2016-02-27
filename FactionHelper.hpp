#pragma once

#include <cstdlib>
#include "Enums.hpp"
class EntitySystem;

/**
 * Auxiliary namespace containing functions that help with the management
 * of the faction component.
 */
namespace FactionHelper
{
	/**
	 * Brief: Changes the FACTION of a given entity.
	 * Param: Reference to the entity system containing components.
	 * Param: ID of the entity.
	 * Param: The new faction.
	 */
	void set_faction(EntitySystem&, std::size_t, FACTION);

	/**
	 * Brief: Returns the FACTION of a given entity.
	 * Param: Reference to the entity system containing components.
	 * Param: ID of the entity.
	 */
	FACTION get_faction(EntitySystem&, std::size_t);
}