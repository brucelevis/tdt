#include <lppscript/LppScript.hpp>
#include <helpers/Helpers.hpp>
#include "EventSystem.hpp"
#include "EntitySystem.hpp"

EventSystem::EventSystem(EntitySystem& ents)
	: entities_{ents}, update_period_{1.f}, curr_update_time_{REAL_ZERO},
	  update_time_multiplier_{1.f}
{ /* DUMMY BODY */ }

void EventSystem::update(tdt::real delta)
{
	if(curr_update_time_ < update_period_)
	{
		curr_update_time_ += delta * update_time_multiplier_;
		return;
	}
	else
		curr_update_time_ = REAL_ZERO;

	bool destroy_evt{false};
	for(auto& evt : entities_.get_component_container<EventComponent>())
	{
		if(!evt.second.active)
			continue;
		
		if(evt.second.handler != Component::NO_ENTITY)
		{ // Targeted events.
			auto handler = entities_.get_component<EventHandlerComponent>(evt.second.handler);
			if(handler && handler->possible_events.test((int)evt.second.event_type))
				destroy_evt = handle_event_(evt.second.handler, evt.first);
		}
		else
		{ // Area events.
			auto radius = evt.second.radius;
			radius *= radius; // get_distance returns squared distance.
			
			for(auto& handler : entities_.get_component_container<EventHandlerComponent>())
			{
				if(handler.second.possible_events.test((int)evt.second.event_type)
				   && PhysicsHelper::get_distance(entities_, evt.first, handler.first) <= radius
				   && handle_event_(handler.first, evt.first))
				{
					destroy_evt = true;
					break;
				}

				// No handler found, increase radius.
				if(evt.second.radius * evt.second.radius < std::numeric_limits<tdt::real>::max() - 100.f)
					evt.second.radius += 5.f;
			}
		}
		if(destroy_evt)
			entities_.delete_component<EventComponent>(evt.first); // Will delete the entity if no other components exist.
	}
}

void EventSystem::set_update_period(tdt::real val)
{
	update_period_ = val;
}

tdt::real EventSystem::get_update_period() const
{
	return update_period_;
}

void EventSystem::set_update_time_multiplier(tdt::real val)
{
	update_time_multiplier_ = val;
}

tdt::real EventSystem::get_update_time_multiplier() const
{
	return update_time_multiplier_;
}

bool EventSystem::handle_event_(tdt::uint handler, tdt::uint evt)
{
	auto type = EventHelper::get_event_type(entities_, evt);
	switch(type)
	{
		case EVENT_TYPE::NONE:
			return true;
		case EVENT_TYPE::KILL_ENTITY:
			DestructorHelper::destroy(entities_, handler);
			return true;
		case EVENT_TYPE::RESTORE_SPEED:
		{
			auto comp = entities_.get_component<MovementComponent>(handler);
			if(comp)
				comp->speed_modifier = comp->original_speed;
			return true;
		}
		default: // Allows custom events handled in scripts.
			return lpp::Script::instance().call<bool, tdt::uint, tdt::uint>(
				EventHandlerHelper::get_handler(entities_, handler) + ".handle_event",
				handler, evt
			);
	}
}
