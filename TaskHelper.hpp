#pragma once

#include <cstdlib>
#include "Enums.hpp"
class EntitySystem;

/**
 * Auxiliary namespace containing functions that help with the management of the task
 * component.
 */
namespace TaskHelper
{
	/**
	 * Brief: Sets the source of a given task (that is, the entity that is completing the task).
	 * Param: Reference to the entity system containing components.
	 * Param: Task ID.
	 * Param: Source ID.
	 */
	void set_task_source(EntitySystem&, std::size_t, std::size_t);

	/**
	 * Brief: Returns the source of a given task (that is, the entity that is completing the task).
	 * Param: Reference to the entity system containing components.
	 * Param: Task ID.
	 */
	std::size_t get_task_source(EntitySystem&, std::size_t);

	/**
	 * Brief: Sets the target entity of a given task.
	 * Param: Reference to the entity system containing components.
	 * Param: Task ID.
	 * Param: Target ID.
	 */
	void set_task_target(EntitySystem&, std::size_t, std::size_t);

	/**
	 * Brief: Returns the target entity of a given task.
	 * Param: Reference to the entity system containing components.
	 * Param: Task ID.
	 */
	std::size_t get_task_target(EntitySystem&, std::size_t);

	/**
	 * Brief: Sets the task type of a given task.
	 * Param: Reference to the entity system containing components.
	 * Param: Task ID.
	 * Param: The new task type.
	 */
	void set_task_type(EntitySystem&, std::size_t, TASK_TYPE);

	/**
	 * Returns the task type of a given task.
	 * Param: Reference to the entity system containing components.
	 * Param: Task ID.
	 */
	TASK_TYPE get_task_type(EntitySystem&, std::size_t);

	/**
	 * Brief: Assigns a new task to an entity (by adding it to the task queue).
	 * Param: Reference to the entity system containing components.
	 * Param: ID of the entity.
	 * Param: ID of the task.
	 * Param: If true, the task will be added to the fron of the task queue.
	 */
	void add_task(EntitySystem&, std::size_t, std::size_t, bool = false);

	/**
	 * Brief: Creates a new task of a given tasks and returns it's ID.
	 * Param: Reference to the entity system containing components.
	 * Param: ID of the task's target (goto location, kill target etc.).
	 * Param: Type of the task.
	 */
	std::size_t create_task(EntitySystem&, std::size_t, TASK_TYPE);

	/**
	 * Brief: Destroys the TaskComponent of a given task, effectively stopping
	 *        it's completion.
	 * Param: Reference to the entity system containing components.
	 * Param: ID of the task.
	 */
	void cancel_task(EntitySystem&, std::size_t);

	/**
	 * Brief: Sets a given task to a complete state.
	 * Param: Reference to the entity system containing components.
	 * Param: ID of the task.
	 */
	void set_complete(EntitySystem&, std::size_t);

	/**
	 * Brief: Returns true if a given task is complete, false otherwise.
	 * Param: Reference to the entity system containing components.
	 * Param: ID of the task.
	 */
	bool is_complete(EntitySystem&, std::size_t);
}