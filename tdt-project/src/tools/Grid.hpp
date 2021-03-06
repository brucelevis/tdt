#pragma once

#include <OGRE/Ogre.h>
#include <set>
#include <vector>
#include <Typedefs.hpp>
class EntitySystem;
struct GridNodeComponent;

/**
 * Class representing the pathfinding grid.
 */
class Grid
{
	/**
	 * GameSerializer is a friend class so that it can easily access the grid realted data
	 * (like dimensions and node distance) when saving the game.
	 */
	friend class GameSerializer;
	public:
		/**
		 * \brief Returns true if a given node is in the grid.
		 * \param ID of the node.
		 */
		bool in_board(tdt::uint) const;

		/**
		 * \brief Returns a constant reference to the list of
		 *        freed nodes.
		 */
		const std::set<tdt::uint>& get_freed() const;

		/**
		 * \brief Returns a constant reference to the list of
		 *        unfreed nodes.
		 */
		const std::set<tdt::uint>& get_unfreed() const;

		/**
		 * \brief Removes all nodes from the list of freed nodes.
		 */
		void clear_freed();

		/**
		 * \brief Removes all nodes from the list of unfreed nodes.
		 */
		void clear_unfreed();

		/**
		 * \brief Created a new node at the given position.
		 * \param EntitySystem that contains the node.
		 * \param 2D position of the node.
		 */
		tdt::uint add_node(EntitySystem&, Ogre::Vector2);

		/**
		 * \brief Adds a given node to the list of the freed nodes.
		 * \param ID of the node.
		 */
		void add_freed(tdt::uint);

		/**
		 * \brief Adds a given node to the list of the unfreed nodes.
		 * \param ID of the node.
		 */
		void add_unfreed(tdt::uint);

		/**
		 * \brief Removes a given node from the node list.
		 * \param ID of the node.
		 * TODO: Possibly implement "unlink"?
		 */
		void remove_node(tdt::uint);

		/**
		 * \brief Returns the ID of a node at a given position in the grid.
		 * \param Column number.
		 * \param Row number.
		 */
		tdt::uint get_node(tdt::uint, tdt::uint) const;
		
		/**
		 * \brief Returns the ID of a node that is closed to a given world coorinate.
		 * \param X axis coordinate.
		 * \param Z axis coordinate.
		 * \note Adding the ability to specify in what direction the node must be might
		 *       be beneficial for pathfinding.
		 */
		tdt::uint get_node_from_position(tdt::real, tdt::real) const;

		/**
		 * \brief Generates a grid graph with the given parameters to be used for
		 *        pathfinding.
		 * \param EntitySystem that will contain the nodes.
		 * \param Starting position (x,z axes) of the grid. 
		 * \param Width of the graph (in node count).
		 * \param Height of the graph (in node count).
		 * \param Distance between adjascent nodes.
		 */
		void create_graph(EntitySystem&, Ogre::Vector2, tdt::uint, tdt::uint, tdt::real);

		/**
		 * \brief Returns the distance between two nodes in the four non-diagonal
		 *        directions.
		 */
		tdt::real get_distance() const;

		/**
		 * \brief Returns a reference to the static instance of this class.
		 * \note Handles initialization and safe destruction by itself.
		 */
		static Grid& instance();

		/**
		 * Breif: Returns a random node within the graph.
		 */
		tdt::uint get_random_free_node() const;

		/**
		 * \brief Returns the 2D position of the central node of the grid (or one of them
		 *        if the grid has even dimensions).
		 * \param EntitySystem that contains components of the nodes.
		 */
		Ogre::Vector2 get_center_position(EntitySystem&) const;

		/**
		 * Since there should be only one grid at all times accesible from the Grid::instance
		 * method, all copy/move operations are disabled for this class.
		 */
		Grid(const Grid&) = delete;
		Grid& operator=(const Grid&) = delete;
		Grid(Grid&&) = delete;
		Grid& operator=(Grid&&) = delete;

		/**
		 * \brief Places a given entity at a random node that is not obstructed by a building.
		 *		  Returns true if the placement was possible, false otherwise.
		 * \param Entity system containing the entity.
		 * \param ID of the entity.
		 */
		bool place_at_random_free_node(EntitySystem&, tdt::uint);

		/**
		 * \brief Distributes a given set of entities on free nodes adjacent to a given central node.
		 *		  Returns true if the placement was possible, false otherwise.
		 * \param Entity system containing the entitites.
		 * \param ID of the central node.
		 * \param Vector of IDs of the entities.
		 */
		bool distribute_to_adjacent_free_nodes(EntitySystem&, tdt::uint, const std::vector<tdt::uint>&);

	private:
		/**
		 * Constructor. 
		 * Kept private since there should be only one grid at all times.
		 */
		Grid() = default;

		/**
		 * Destructor.
		 */
		~Grid() {}

		/**
		 * \brief Generates a neighbour list for a given node (thus linking it to the graph).
		 * \param ID of the node.
		 * \param Auxuliary vector containing component pointers for fast access.
		 *       (This method will ever be called only in the GridSystem::create_graph method,
		 *        which already has such a vector and so it's used here too.)
		 */
		void link_(tdt::uint, std::vector<GridNodeComponent*>&);

		/**
		 * Vector containing the IDs of the nodes in the grid, basically
		 * representing a 2D matrix stored in a 1D container.
		 */
		std::vector<tdt::uint> nodes_;

		/**
		 * Auxiliary vectors containing IDs of the nodes that have been
		 * freed/unfreed on last frame. Used for pathfinding correction
		 * and structure model changes.
		 */
		std::set<tdt::uint> freed_, unfreed_;

		/**
		 * Dimensions of the grid in node count.
		 * (Actual dimensions = dimensions * distance.)
		 */
		tdt::uint width_, height_;

		/**
		 * Distance between two adjascent nodes.
		 */
		tdt::real distance_;

		/**
		 * ID of the first node, this allows for the node IDs to be outside the
		 * (0, width_ * height_) range,
		 */
		tdt::uint starting_index_;

		/**
		 * Coordinates of the starting node of the grid.
		 * (This 2D vector contains X and Z coordinates despite it's second member
		 *  being names Y).
		 */
		Ogre::Vector2 start_;

		/**
		 * Used for easier returning of a random free node.
		 */
		std::vector<tdt::uint> free_nodes_;
};