#ifndef GAMES_CATASTROPHE_AI_HPP
#define GAMES_CATASTROPHE_AI_HPP


#include "impl/catastrophe.hpp"
#include "game.hpp"
#include "game_object.hpp"
#include "job.hpp"
#include "player.hpp"
#include "structure.hpp"
#include "tile.hpp"
#include "unit.hpp"

#include "../../joueur/src/base_ai.hpp"
#include "../../joueur/src/attr_wrapper.hpp"

// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add additional #includes here
// <<-- /Creer-Merge: includes -->>

namespace cpp_client
{

namespace catastrophe
{

/// <summary>
/// This is the header file for building your Catastrophe AI
/// </summary>
class AI : public Base_ai
{
public:
    /// <summary>
    /// This is a reference to the Game object itself, it contains all the information about the current game
    /// </summary>
    Game game;

    /// <summary>
    /// This is a pointer to your AI's player. This AI class is not a player, but it should command this Player.
    /// </summary>
    Player player;

	bool first_turn = true;
	bool death_squad = false;

    //<<-- Creer-Merge: class variables -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can add additional class variables here.
    //<<-- /Creer-Merge: class variables -->>

    /// <summary>
    /// This returns your AI's name to the game server.
    /// Replace the string name.
    /// </summary>
    /// <returns>The name of your AI.</returns>
    virtual std::string get_name() const override;

    /// <summary>
    /// This is automatically called when the game first starts, once the game objects are created
    /// </summary>
    virtual void start() override;

    /// <summary>
    /// This is automatically called when the game ends.
    /// </summary>
    /// <param name="won">true if you won, false otherwise</param>
    /// <param name="reason">An explanation for why you either won or lost</param>
    virtual void ended(bool won, const std::string& reason) override;

    /// <summary>
    /// This is automatically called the game (or anything in it) updates
    /// </summary>
    virtual void game_updated() override;

    /// <summary>
    /// This is called every time it is this AI.player's turn.
    /// </summary>
    /// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
    bool run_turn();

    // <<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can add additional methods here.
    // <<-- /Creer-Merge: methods -->>
	std::vector<Tile> shortest_path_to_materials(const Unit& unit);
	//bool builder_turn(Unit& builder);
	std::vector<Tile> find_closest_shelter(const Unit& unit);
	std::vector<Tile> find_closest_neutral_human(const Unit& unit);
	std::vector<Tile> find_closest_soldier(const Unit& unit);
	std::vector<Tile> find_closest_helper(const std::vector<Tile>& nodes_to_try, const Unit& unit);
	bool converter_turn(Unit& converter);
	bool soldier_turn(Unit& unit);
	std::vector<Tile> find_closest_enemy(const Unit& unit);
	bool gatherer_turn(Unit& unit);
	std::vector<Tile> find_closest_food(const Unit& unit);
	void mover(Unit& unit, std::vector<Tile> path);
	bool fresh_turn(Unit& unit);
	bool is_adj_to_cat(const Tile& tile);
	bool choose_job(Unit& unit);
	std::vector<Tile> find_full_deposit(Unit& unit);
	std::vector<Tile> find_closest_deposit(Unit& unit);
	std::vector<Tile> find_nearest_empty_tile(Unit& unit);
	std::vector<Tile> find_closest_structure(Unit& unit);
	bool builder_turn(Unit& unit);
	bool defender_turn(Unit& unit);
	std::vector<Tile> find_closest_enemy_defender(Unit& unit);
	bool death_squad_turn(Unit& unit);
	std::vector<Tile> find_enemy_cat(Unit& unit);

/// A very basic path finding algorithm (Breadth First Search) that when given a starting Tile, will return a valid path to the goal Tile.
/// <param name="start">the starting Tile</param>
/// <param name="goal">the goal Tile</param>
/// <return>A List of Tiles representing the path, the the first element being a valid adjacent Tile to the start, and the last element being the goal. Or an empty list if no path found.</return>
std::vector<Tile> find_path(const Tile& start, const Tile& goal);



    // ####################
    // Don't edit these!
    // ####################
    /// \cond FALSE
    virtual std::string invoke_by_name(const std::string& name,
                                       const std::unordered_map<std::string, Any>& args) override;
    virtual void set_game(Base_game* ptr) override;
    virtual void set_player(std::shared_ptr<Base_object> obj) override;
    virtual void print_win_loss_info() override;
    /// \endcond
    // ####################
    // Don't edit these!
    // ####################

};

} // CATASTROPHE

} // cpp_client

#endif // GAMES_CATASTROPHE_AI_HPP
