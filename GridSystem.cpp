#include "GridSystem.hpp"

GridSystem::GridSystem(EntitySystem& ents, Ogre::SceneManager& scene)
	: entities_{ents}, scene_mgr_{scene}, width_{0}, height_{0},
	  start_{0, 0}, distance_{0}, board_{},
	  graphics_loaded_{false}, graph_visible_{false},
	  error_blueprint{"ERROR"}
{ /* DUMMY BODY */ }

void GridSystem::update(Ogre::Real)
{
	// TODO:
}

bool GridSystem::is_valid(std::size_t id) const
{
	return entities_.has_component<GridNodeComponent>(id) ||
		   entities_.has_component<GridLineComponent>(id);
}

std::size_t GridSystem::add_node(Ogre::Real x, Ogre::Real y, Ogre::Real z)
{
	auto id = entities_.create_entity();
	entities_.add_component<GridNodeComponent>(id);
	auto& phys_comp = entities_.add_component<PhysicsComponent>(id);
	phys_comp.position = Ogre::Vector3{x, y + 10, z};

	return id;
}

std::size_t GridSystem::add_line(std::size_t id1, std::size_t id2)
{
	auto id = entities_.create_entity();
	auto& line_comp = entities_.add_component<GridLineComponent>(id);

	line_comp.start_id = id1;
	line_comp.end_id = id2;
	auto pos_start = entities_.get_component<PhysicsComponent>(id1).position;
	auto pos_end = entities_.get_component<PhysicsComponent>(id2).position;
	line_comp.distance = pos_start.distance(pos_end);
	line_comp.line.reset(new Line{pos_start, pos_end});

	return id;
}

void GridSystem::create_graph(std::size_t width, std::size_t height, Ogre::Real dist,
							  Ogre::Real start_x, Ogre::Real start_y)
{
	width_ = width;
	height_ = height;
	distance_ = dist;
	start_.x = start_x;
	start_.y = start_y;

	board_.clear();
	board_.resize(width_ * height_);
	std::vector<GridNodeComponent*> comps(width_ * height_); // Keep postd::size_ters to components for fast access.

	Ogre::Real x{start_.x};
	Ogre::Real y{start_.y};
	for(std::size_t i = 0; i < board_.size(); ++i)
	{
		x = (i % width_) * distance_;
		y = (i / height_) * distance_;
		board_[i] = add_node(x, 0, y);
		comps[i] = &entities_.get_component<GridNodeComponent>(board_[i]);
		comps[i]->x = i % width_;
		comps[i]->y = i / height_;
	}

	// Link nodes.
	for(std::size_t i = 0; i < board_.size(); ++i)
		link_(i, comps);
}

std::size_t GridSystem::get_node(std::size_t w, std::size_t h) const
{
	if(in_board_(w + h * width_))
		return board_[w + h * width_];
	else
		return std::numeric_limits<std::size_t>::max();
}

std::size_t GridSystem::get_node_from_position(Ogre::Real x, Ogre::Real y) const
{
	x = (x - start_.x) / distance_;
	y = (y - start_.y) / distance_;
	std::size_t res_x = (std::size_t)x;
	std::size_t res_y = (std::size_t)y;
	Ogre::Real off_x = x - res_x;
	Ogre::Real off_y = y - res_y;

	// Finds the closest.
	if(off_x > distance_ / 200)
		++res_x;
	if(off_y > distance_ / 200)
		++res_y;

	return get_node(res_x, res_y);
}

void GridSystem::create_graphics()
{
	for(auto& ent : entities_.get_component_container<GridNodeComponent>())
	{
		auto& phys_comp = entities_.get_component<PhysicsComponent>(ent.first);
		auto& graph_comp = entities_.add_component<GraphicsComponent>(ent.first);

		graph_comp.mesh = "cube.mesh";
		graph_comp.material = "colour/red";
		entities_.init_graphics_component(ent.first);
		graph_comp.entity->setQueryFlags(2);

		((Ogre::Entity*)graph_comp.entity)->setMaterialName(graph_comp.material);
		graph_comp.node->setScale(5, 10, 5);
		graph_comp.node->setPosition(phys_comp.position);
		graph_comp.node->setVisible(false);
	
	}

	for(auto& ent : entities_.get_component_container<GridLineComponent>())
	{
		auto& line_comp = entities_.get_component<GridLineComponent>(ent.first);
		auto& graph_comp = entities_.add_component<GraphicsComponent>(ent.first);

		graph_comp.entity = line_comp.line.get();
		graph_comp.node = scene_mgr_.getRootSceneNode()->createChildSceneNode();
		graph_comp.node->attachObject(graph_comp.entity);
		graph_comp.material = "colour/blue";
		graph_comp.mesh = "LINE";
		graph_comp.visible = true;
		graph_comp.node->setVisible(false);
	}

	graphics_loaded_ = true;
}

void GridSystem::delete_graphics()
{
	for(auto& ent : entities_.get_component_container<GridNodeComponent>())
		entities_.delete_component<GraphicsComponent>(ent.first);
	for(auto& ent : entities_.get_component_container<GridLineComponent>())
		entities_.delete_component<GraphicsComponent>(ent.first);
	graphics_loaded_ = false;
	graph_visible_ = false;
}

void GridSystem::set_visible(bool on_off)
{
	if(!graphics_loaded_ || on_off == graph_visible_)
		return;
	
	graph_visible_ = on_off;

	// Nodes.
	for(auto& ent : entities_.get_component_container<GridNodeComponent>())
	{
		auto& graph_comp = entities_.get_component<GraphicsComponent>(ent.first);
		graph_comp.node->setVisible(on_off);
	}

	// Links.
	for(auto& ent : entities_.get_component_container<GridLineComponent>())
	{
		auto& graph_comp = entities_.get_component<GraphicsComponent>(ent.first);
		graph_comp.node->setVisible(on_off);
	}
}

bool GridSystem::is_visible() const
{
	if(!graphics_loaded_)
		return false;
	else
		return graph_visible_;
}

std::array<std::size_t, 4> GridSystem::get_neighbours(std::size_t id) const
{
	if(entities_.has_component<GridNodeComponent>(id))
		return entities_.get_component<GridNodeComponent>(id).neighbours;
	else
		return std::array<std::size_t, 4>();
}

bool GridSystem::is_free(std::size_t id) const
{
	if(entities_.has_component<GridNodeComponent>(id))
		return entities_.get_component<GridNodeComponent>(id).free;
	else
		return false;
}

void GridSystem::set_free(std::size_t id, bool on_off)
{
	if(entities_.has_component<GridNodeComponent>(id))
		entities_.get_component<GridNodeComponent>(id).free = on_off;
}

std::size_t GridSystem::get_manhattan_distance(std::size_t id1, std::size_t id2) const
{
	std::size_t x1, y1;
	std::tie(x1, y1) = get_board_coords(id1);

	std::size_t x2, y2;
	std::tie(x2, y2) = get_board_coords(id2);

	return abs_(int(x1 - x2)) + abs_(int(y1 - y2));
}

void GridSystem::perform_a_star(std::size_t id, std::size_t start, std::size_t end)
{
	if(!entities_.has_component<PathfindingComponent>(id))
		return;

	std::map<std::size_t, std::size_t> path_edges{};
	std::set<std::size_t> closed{};
	std::set<std::size_t> open{start};
	std::map<std::size_t, std::size_t> score;
	std::map<std::size_t, std::size_t> estimate;

	for(auto& node : entities_.get_component_container<GridNodeComponent>())
	{ // Starting score and estimate is "infinity".
		score.emplace(node.first, std::numeric_limits<std::size_t>::max());
		estimate.emplace(node.first, std::numeric_limits<std::size_t>::max());
	}
	score[start] = 0;
	estimate[start] = get_manhattan_distance(start, end);

	std::size_t current{};
	bool success{false};

	while(!open.empty())
	{
		// Find the best candidate in the open set.
		current = *std::min_element(open.begin(), open.end(),
				     	            [&estimate](const std::size_t& lhs, const std::size_t& rhs) -> bool
		                            { return estimate[lhs] < estimate[rhs]; });
		if(current == end)
		{ // Reached goal.
			success = true;
			break;
		}
		open.erase(current);
		closed.insert(current);
	
		for(const auto& neighbour : get_neighbours(current))
		{
			if(neighbour >= board_.size() || closed.find(neighbour) != closed.end() || !is_free(neighbour))
				continue;
			auto new_score = score[current] + 1;

			// Either unvisited or we found a better path to it.
			bool not_in_open = open.find(neighbour) == open.end();
			if(not_in_open || new_score < score[neighbour])
			{
				path_edges[neighbour] = current;
				score[neighbour] = new_score;
				estimate[neighbour] = new_score + get_manhattan_distance(neighbour, end);
			
				if(not_in_open)
					open.insert(neighbour);
			}
		
		}
	}

	if(success)
	{ // Reconstruct the path.
		std::deque<std::size_t> path;
		path.push_back(current);

		// Using assignment to an auxiliary iterator in the condition saves another lookup.
		auto it = path_edges.begin();
		while((it = path_edges.find(current)) != path_edges.end())
		{
			current = it->second;
			path.push_front(current);

			if(graphics_loaded_) // Highlight the path for testing.
				((Ogre::Entity*)entities_.get_component<GraphicsComponent>(current).entity)->setMaterialName("colour/green");
		}

		auto& path_comp = entities_.get_component<PathfindingComponent>(id);
		path_comp.path_queue.swap(path);
		path_comp.last_id = start;
		path_comp.target_id = end;
	}
}

const std::string& GridSystem::get_pathpfinding_blueprint(std::size_t id) const
{
	if(entities_.has_component<PathfindingComponent>(id))
		return entities_.get_component<PathfindingComponent>(id).blueprint;
	else
		return error_blueprint;
}

void GridSystem::set_pathfinding_blueprint(std::size_t id, const std::string& blueprint)
{
	if(entities_.has_component<PathfindingComponent>(id))
		entities_.get_component<PathfindingComponent>(id).blueprint = blueprint;
}

bool GridSystem::can_break(std::size_t, std::size_t) const
{
	return false;
}

bool GridSystem::can_pass(std::size_t, std::size_t) const
{
	return false;
}

std::tuple<std::size_t, std::size_t> GridSystem::get_board_coords(std::size_t id) const
{
	if(entities_.has_component<GridNodeComponent>(id))
	{
		auto& comp = entities_.get_component<GridNodeComponent>(id);
		return std::make_tuple(comp.x, comp.y);
	}
	else // Should not happen as this will be accessed only from within the GridSystem.
		return std::tuple<std::size_t, std::size_t>{};
}

void GridSystem::pathfinding_test(Console& console)
{
	auto id = entities_.create_entity();
	auto& comp = entities_.add_component<PathfindingComponent>(id);
	perform_a_star(id, get_node(0,0), get_node(width_ - 1, height_ - 1));

	for(auto node : comp.path_queue)
		console.print_text(std::to_string(node), Console::ORANGE_TEXT);
}

bool GridSystem::in_board_(std::size_t index) const
{
	return 0 <= index && index < board_.size();
}

void GridSystem::link_(std::size_t index, std::vector<GridNodeComponent*>& comps)
{
	if(!in_board_(index))
		return;

	// Note: Adding a line only from left to right and from top to bottom,
	//       otherwise it would be wasting resources by adding since the
	//       lines are only used visually and would thus overlap.
	if(in_board_(index + 1) && (index + 1) % width_ != 0)
	{
		add_line(index, index + 1);
		comps[index]->neighbours[0] = index + 1;
	}

	if(in_board_(index - 1))
		comps[index]->neighbours[1] = index - 1;

	if(in_board_(index + width_))
	{
		add_line(index, index + width_);
		comps[index]->neighbours[2] = index + width_;
	}

	if(in_board_(index - width_))
		comps[index]->neighbours[3] = index - width_;
}

std::size_t GridSystem::abs_(int val) const
{
	if(val < 0)
		return (std::size_t) -1 * val;
	else
		return val;
}
