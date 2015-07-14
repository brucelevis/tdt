#include "EntityTestComponents.hpp"
#include "../Entity.hpp"

EntityTestInputComponent::EntityTestInputComponent(std::string id)
	: name_{"entity_" + id}, script_{lpp::Script::get_singleton()}
{ // TODO: Move entity name to entity, maybe also the script reference.
	script_.execute(name_ + " = test_entity:new{x = 0.0, y = 100.0, z = -60.0}");
}

void EntityTestInputComponent::update(Entity& entity, Ogre::Real delta)
{
	script_.call<void, Ogre::Real>(name_ + ".update", delta, true);
}

EntityTestPhysicsComponent::EntityTestPhysicsComponent(std::string id)
	: name_{"entity_" + id}, script_{lpp::Script::get_singleton()}
{
	// TODO:
}

void EntityTestPhysicsComponent::update(Entity& entity, Ogre::Real delta)
{
	// Update the new position.
	float x = script_.get<float>(name_ + ".x");
	float y = script_.get<float>(name_ + ".y");
	float z = script_.get<float>(name_ + ".z");
	Ogre::Vector3 tmp_vec{x, y, z};

	if(tmp_vec != entity.get_component<PhysicsComponent>()->get_position())
	{
		entity.get_component<PhysicsComponent>()->set_position(tmp_vec);
		entity.get_component<PhysicsComponent>()->set_moved(true);
	}
}

EntityTestGraphicsComponent::EntityTestGraphicsComponent(Ogre::SceneManager& scene)
{
	entity_ = scene.createEntity("ogrehead.mesh");
	node_ = scene.getRootSceneNode()->createChildSceneNode();
	node_->attachObject(entity_);
}

void EntityTestGraphicsComponent::update(Entity& entity, Ogre::Real delta)
{
	if(entity.get_component<PhysicsComponent>()->has_moved())
	{
		node_->setPosition(entity.get_component<PhysicsComponent>()->get_position());
		entity.get_component<PhysicsComponent>()->set_moved(false);
	}
}