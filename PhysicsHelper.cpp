#include "PhysicsHelper.hpp"
#include "Components.hpp"
#include "EntitySystem.hpp"

void PhysicsHelper::set_solid(EntitySystem& ents, std::size_t id, bool val)
{
	auto comp = ents.get_component<PhysicsComponent>(id);
	if(comp)
		comp->solid = val;
}

bool PhysicsHelper::is_solid(EntitySystem& ents, std::size_t id)
{
	auto comp = ents.get_component<PhysicsComponent>(id);
	if(comp)
		return comp->solid;
	else
		return false;
}

void PhysicsHelper::set_position(EntitySystem& ents, std::size_t id, const Ogre::Vector3& val)
{
	auto comp = ents.get_component<PhysicsComponent>(id);
	if(comp)
		comp->position = val;
}

const Ogre::Vector3& PhysicsHelper::get_position(EntitySystem& ents, std::size_t id)
{
	static Ogre::Vector3 NO_POSITION{0.f, 100.f, 0.f};
	auto comp = ents.get_component<PhysicsComponent>(id);
	if(comp)
		return comp->position;
	else
		return NO_POSITION;
}

void PhysicsHelper::set_half_height(EntitySystem& ents, std::size_t id, Ogre::Real val)
{
	auto comp = ents.get_component<PhysicsComponent>(id);
	if(comp)
		comp->half_height = val;
}

Ogre::Real PhysicsHelper::get_half_height(EntitySystem& ents, std::size_t id)
{
	auto comp = ents.get_component<PhysicsComponent>(id);
	if(comp)
		return comp->half_height;
	else
		return Ogre::Real{};
}

void PhysicsHelper::move_to(EntitySystem& ents, std::size_t id, Ogre::Vector3 pos)
{
	auto phys_comp = ents.get_component<PhysicsComponent>(id);
	if(phys_comp)
	{
		phys_comp->position = pos;

		auto graph_comp = ents.get_component<GraphicsComponent>(id);
		if(graph_comp)
			graph_comp->node->setPosition(pos);
	}
}

Ogre::Real PhysicsHelper::get_distance(EntitySystem& ents, std::size_t id1, std::size_t id2)
{
	auto comp1 = ents.get_component<PhysicsComponent>(id1);
	auto comp2 = ents.get_component<PhysicsComponent>(id2);
	if(comp1 && comp2)
	{
		auto pos1 = comp1->position;
		auto pos2 = comp2->position;
		pos1.y = pos2.y = 0;
		return pos1.squaredDistance(pos2);
	}
	else
		return std::numeric_limits<Ogre::Real>::max();
}

Ogre::Real PhysicsHelper::get_angle(Ogre::Vector3 v1, Ogre::Vector3 v2)
{
	return v1.angleBetween(v2).valueRadians();
}