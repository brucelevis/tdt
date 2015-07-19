#pragma once

#include <Ogre.h>
#include <OIS.h>
#include <stdexcept>
#include <memory>

#include "System.hpp"
#include "EntitySystem.hpp"
#include "Components.hpp"

class InputSystem : public System
{
	public:
		InputSystem(EntitySystem&, OIS::Keyboard&, Ogre::Camera&);
		~InputSystem() {}

		void update(std::size_t, Ogre::Real);
		bool is_valid(std::size_t) const;
		bool is_first_person() const;
		void set_first_person(bool, std::size_t = 0);
		void rebind(int, int);
		void set_input_handler(std::size_t, const std::string&);
	private:
		EntitySystem& entities_;
		bool first_person_;
		std::size_t first_person_id_;
		OIS::Keyboard& keyboard_;
		int KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT; // Allow for rebinding.
		Ogre::Camera& cam_;

		// Backup of the camera info.
		Ogre::Vector3 cam_position_;
		Ogre::Quaternion cam_orientation_;
		
		// Backup of the AI component when entering first person view.
		std::unique_ptr<AIComponent> ai_backup_;
		bool delete_input_;
};