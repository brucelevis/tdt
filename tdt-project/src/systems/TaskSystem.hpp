#pragma once

#include <deque>
#include <map>
#include <string>
#include <Typedefs.hpp>
#include <Enums.hpp>
#include "System.hpp"
class GridSystem;
class CombatSystem;
class EntitySystem;
struct TaskHandlerComponent;

/**
 * System managing all entities with the TaskComponent, their creation, assignment,
 * lifetime checks and canceling.
 */
class TaskSystem : public System
{
	public:
		/**
		 * Constructor.
		 * \param Reference to the game's entity system.
		 * \param Reference to the game's grid system.
		 */
		TaskSystem(EntitySystem&, GridSystem&, CombatSystem&);

		/**
		 * Destructor.
		 */
		~TaskSystem() = default;

		/**
		 * \brief Manages the lifetime of tasks on each frame.
		 * \param Time since the last frame.
		 */
		void update(tdt::real) override;

		/**
		 * \brief Translates a task type enum value into a string that can be displayed
		 *        in the developer's console.
		 * \param Task type to be translated.
		 */
		const std::string& get_task_name(TASK_TYPE) const;

	private:
		/**
		 * \brief Set's the current task to the task in the front of the respective task queue
		 *        and removes it from the queue.
		 * \param Reference to the TaskHandlerComponent containing the aforementioned task queue.
		 */
		void next_task_(TaskHandlerComponent&);

		/**
		 * \brief Executes a single task.
		 * \param ID of the entity that the task is assigned to.
		 * \param Reference to the task handling component of the assigned entity.
		 *        (For easier look up of the blueprint.)
		 */
		bool handle_task_(tdt::uint, TaskHandlerComponent&);

		/**
		 * \brief Checks whether the current task of a given entity has been completed.
		 * \param ID of the handling entity.
		 * \param Reference to the entity's TaskHandlerComponent.
		 */
		bool current_task_completed_(tdt::uint, TaskHandlerComponent&);

		/**
		 * Reference to the game's entity system.
		 */
		EntitySystem& entities_;

		/**
		 * Reference to the game's grid system.
		 */
		GridSystem& grid_;

		/**
		 * Reference to the game's combat system used for line of sight checking.
		 */
		CombatSystem& combat_;

		/**
		 * Map used for task type translation.
		 */
		std::map<TASK_TYPE, std::string> task_names_;
};