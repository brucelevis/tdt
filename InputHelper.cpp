#include "InputHelper.hpp"
#include "Components.hpp"
#include "EntitySystem.hpp"

void InputHelper::set_input_handler(EntitySystem& ents, std::size_t id, const std::string& handler)
{
	auto comp = ents.get_component<InputComponent>(id);
	if(comp)
		comp->input_handler = handler;
}

const std::string & InputHelper::get_input_handler(EntitySystem& ents, std::size_t id)
{
	static const std::string NO_HANDLER{"ERROR"};
	auto comp = ents.get_component<InputComponent>(id);
	if(comp)
		return comp->input_handler;
	else
		return NO_HANDLER;
}