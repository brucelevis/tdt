#pragma once

#include <cstdlib>
#include "Helpers.hpp"
#include "Enums.hpp"
class EntitySystem;

namespace util
{
	/**
	 * Functors.
	 */
	struct IS_ENEMY
	{
		IS_ENEMY(EntitySystem&, std::size_t);

		bool operator()(std::size_t);

		private:
			FACTION enemy_faction_;
			EntitySystem& entities_;
	};

	struct IS_FRIENDLY
	{
		IS_FRIENDLY(EntitySystem&, std::size_t);

		bool operator()(std::size_t);

		private:
			FACTION faction_;
			EntitySystem& entities_;
	};

	struct IS_FRIENDLY_OR_NEUTRAL
	{
		IS_FRIENDLY_OR_NEUTRAL(EntitySystem&, std::size_t);

		bool operator()(std::size_t);

		private:
			FACTION faction_;
			EntitySystem& entities_;
	};

	struct EntityDestroyer
	{
		friend void DestructorHelper::destroy(EntitySystem&, std::size_t);
		private:
			static void destroy(EntitySystem&, std::size_t);
	};
}