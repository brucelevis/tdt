#include "TaskHandlerHelper.hpp"
#include "Components.hpp"
#include "EntitySystem.hpp"

std::deque<std::size_t>& TaskHandlerHelper::get_task_queue(EntitySystem& ents, std::size_t id)
{
	static std::deque<std::size_t> NO_QUEUE{};
	auto comp = ents.get_component<TaskHandlerComponent>(id);
	if(comp)
		return comp->task_queue;
	else
		return NO_QUEUE;
}

bool TaskHandlerHelper::task_possible(EntitySystem& ents, std::size_t ent_id, std::size_t task_id)
{
	auto comp1 = ents.get_component<TaskHandlerComponent>(ent_id);
	auto comp2 = ents.get_component<TaskComponent>(task_id);
	if(comp1 && comp2)
	{
		return comp1->possible_tasks.test((int)comp2->task_type);
	}
	else
		return false;
}

bool TaskHandlerHelper::task_possible(EntitySystem& ents, std::size_t id, TASK_TYPE val)
{
	auto comp = ents.get_component<TaskHandlerComponent>(id);
	if(comp && (int)val > 0 && (int)val < (int)TASK_TYPE::COUNT)
		return comp->possible_tasks.test((int)val);
	else
		return false;
}

void TaskHandlerHelper::clear_task_queue(EntitySystem& ents, std::size_t id)
{
	auto comp = ents.get_component<TaskHandlerComponent>(id);
	if(comp)
	{
		auto& task_queue = comp->task_queue;
		for(auto& task : task_queue)
		{
			if(ents.has_component<TaskComponent>(task))
				DestructorHelper::destroy(ents, task);
		}
		task_queue.clear();
	}
}

void TaskHandlerHelper::add_possible_task(EntitySystem& ents, std::size_t id, TASK_TYPE type)
{
	auto comp = ents.get_component<TaskHandlerComponent>(id);
	if(comp)
		comp->possible_tasks.set((int)type);
}

void TaskHandlerHelper::delete_possible_task(EntitySystem& ents, std::size_t id, TASK_TYPE type)
{
	auto comp = ents.get_component<TaskHandlerComponent>(id);
	if(comp)
		comp->possible_tasks.set((int)type, false);
}

void TaskHandlerHelper::set_blueprint(EntitySystem& ents, std::size_t id, const std::string& val)
{
	auto comp = ents.get_component<TaskHandlerComponent>(id);
	if(comp)
		comp->blueprint = val;
}

const std::string& TaskHandlerHelper::get_blueprint(EntitySystem& ents, std::size_t id)
{
	static const std::string NO_BLUEPRINT{"ERROR"};

	auto comp = ents.get_component<TaskHandlerComponent>(id);
	if(comp)
		return comp->blueprint;
	else
		return NO_BLUEPRINT;
}
