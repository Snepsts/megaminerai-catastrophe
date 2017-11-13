// AI
// This is where you build your AI

#include "ai.hpp"

// <<-- Creer-Merge: includes -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add #includes here for your AI.
// <<-- /Creer-Merge: includes -->>
using std::cout;
using std::endl;

namespace cpp_client
{

namespace catastrophe
{

Unit defender;

/// <summary>
/// This returns your AI's name to the game server.
/// Replace the string name.
/// </summary>
/// <returns>The name of your AI.</returns>
std::string AI::get_name() const
{
    // <<-- Creer-Merge: get-name -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // REPLACE WITH YOUR TEAM NAME!
    return "Arrays Start at Zero";
    // <<-- /Creer-Merge: get-name -->>
}

/// <summary>
/// This is automatically called when the game first starts, once the game objects are created
/// </summary>
void AI::start()
{
    // <<-- Creer-Merge: start -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // This is a good place to initialize any variables
	cout << "Game start." << endl;
    // <<-- /Creer-Merge: start -->>
}

/// <summary>
/// This is automatically called the game (or anything in it) updates
/// </summary>
void AI::game_updated()
{
    // <<-- Creer-Merge: game-updated -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // If a function you call triggers an update this will be called before it returns.
    // <<-- /Creer-Merge: game-updated -->>
}

/// <summary>
/// This is automatically called when the game ends.
/// </summary>
/// <param name="won">true if you won, false otherwise</param>
/// <param name="reason">An explanation for why you either won or lost</param>
void AI::ended(bool won, const std::string& reason)
{
    //<<-- Creer-Merge: ended -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // You can do any cleanup of your AI here.  The program ends when this function returns.
	cout << "Game end." << endl;
    //<<-- /Creer-Merge: ended -->>
}

/// <summary>
/// This is called every time it is this AI.player's turn.
/// </summary>
/// <returns>Represents if you want to end your turn. True means end your turn, False means to keep your turn going and re-call this function.</returns>
bool AI::run_turn()
{
    // <<-- Creer-Merge: runTurn -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
    // Put your game logic here for run_turn here
	cout << "Running turn." << endl;

	if (first_turn) { //NOTE: We had to do this for the first turn because start does not wait until the game begins.
		auto player_units = player->units;

		int counter = 0;

		for (auto unit : player_units) {
			if (unit->job->title == "cat overlord") {
				cout << "Reached cat" << endl; //do nothing
			} else {
				if (counter == 0) { //first cat will be missionary
					if (unit->change_job("missionary")) {
						counter++;
					}
				} else if (counter == 1) {
					if (unit->change_job("soldier")) {
						counter++;
					}
				} else { //counter == 2
					if (unit->change_job("soldier")) {
						counter++;
					}
				}
			}
		}

		first_turn = false;
	}

	//grab all enemy units
	int enemy_soldier_count = 0;
	int player_soldier_count = 0;
	auto loser_units = player->opponent->units;

	for (auto unit : loser_units) {
		if (unit->job->title == "soldier") {
			enemy_soldier_count++;
		}
	}

	if (enemy_soldier_count < 1 && game->current_turn >= 20) {
		death_squad = true;
	}

	auto player_units = player->units;

	for (auto unit : player_units) {
		if (unit->job->title == "soldier") {
			player_soldier_count++;
		}
	}

	//NOTE: This code is wrong, it should have been if their soldier count is less than 2
	if (enemy_soldier_count < player_soldier_count-1 && game->current_turn >= 20) {
		death_squad = true;
	}

	if (death_squad) { //call in the helicopters
		auto player_units = player->units;

		for (auto unit : player_units) {
			if (unit->job->title == "missionary") {
				converter_turn(unit);
			} else { //FREE HELICOPTER RIDES
				death_squad_turn(unit);
			}
		}
	} else {
		//grab all units
		auto player_units = player->units;
		int soldier_count = 0;

		for (auto unit : player_units) {
			if (unit->job->title == "soldier") {
				soldier_count++;
			}
		}

		bool is_protected = false; //for defender

		for (auto unit : player_units) {
			if (unit->job->title == "soldier" && soldier_count > 1 && !is_protected) {
				defender = unit;
				defender_turn(unit);
				is_protected = true;
			}
			if (unit->job->title == "soldier") {
				soldier_turn(unit);
			}
			if (unit->job->title == "missionary") {
				converter_turn(unit);
			}
			if (unit->job->title == "gatherer") {
				gatherer_turn(unit);
			}
			if (unit->job->title == "fresh human") {
				fresh_turn(unit);
			}
			if (unit->job->title == "builder") {
				builder_turn(unit);
			}
		}
	}

	cout << "Ending turn." << endl;
    // <<-- /Creer-Merge: runTurn -->>
    return true;
}

/// A very basic path finding algorithm (Breadth First Search) that when given a starting Tile, will return a valid path to the goal Tile.
/// <param name="start">the starting Tile</param>
/// <param name="goal">the goal Tile</param>
/// <return>A List of Tiles representing the path, the the first element being a valid adjacent Tile to the start, and the last element being the goal. Or an empty list if no path found.</return>
std::vector<Tile> AI::find_path(const Tile& start, const Tile& goal)
{
    // no need to make a path to here...
    if (start == goal)
    {
        return {};
    }

    // the tiles that will have their neighbors searched for 'goal'
    std::queue<Tile> fringe;

    // How we got to each tile that went into the fringe.
    std::unordered_map<Tile,Tile> came_from;

    // Enqueue start as the first tile to have its neighbors searched.
    fringe.push(start);

    // keep exploring neighbors of neighbors... until there are no more.
    while(fringe.size() > 0)
    {
        // the tile we are currently exploring.
        Tile inspect = fringe.front();
        fringe.pop();

        // Note, we are using the `auto` keyword here
        //   The compiler can discern that this is a `std::vector<Tile>` based on the return type of `getNeighbors()`
        auto neighbors = inspect->get_neighbors();

        // cycle through the tile's neighbors.
        for (unsigned i = 0; i < neighbors.size(); i++)
        {
            Tile neighbor = neighbors[i];

            // If we found the goal we've found the path!
            if (neighbor == goal)
            {
                // Follow the path backward starting at the goal and return it.
                std::deque<Tile> path;
                path.push_front(goal);

                // Starting at the tile we are currently at, insert them retracing our steps till we get to the starting tile
                for (Tile step = inspect; step != start; step = came_from[step])
                {
                    path.push_front(step);
                }

                // we want to return a vector as that's what we use for all containers in C++
                // (and they don't have push_front like we need)
                // So construct the vector-ized path here
                std::vector<Tile> vector_path;
                for (auto& tile : path)
                {
                    vector_path.push_back(tile);
                }
                return vector_path;
            }

            // if the tile exists, has not been explored or added to the fringe yet, and it is pathable
            if (neighbor && came_from.count(neighbor) == 0 && neighbor->is_pathable())
            {
                // add it to the tiles to be explored and add where it came from.
                fringe.push(neighbor);
                came_from[neighbor] = inspect;
            }

        } // for each neighbor

    } // while fringe not empty

    // if you're here, that means that there was not a path to get to where you want to go.
    //   in that case, we'll just return an empty path.
    return {};
}

//<<-- Creer-Merge: methods -->> - Code you add between this comment and the end comment will be preserved between Creer re-runs.
// You can add additional methods here for your AI to call
//<<-- /Creer-Merge: methods -->>

////////////////////////////////////////////////////////////////////
//Finder functions:
//These functions make use of the predefined find_path function and
//a "filter" that was defined by us in order to return the shortest
//path to whatever the AI is currently searching for
////////////////////////////////////////////////////////////////////

std::vector<Tile> AI::shortest_path_to_materials(const Unit& unit)
//return the shortest path to a gatherable neutral structure we
//can take materials from or a empty vector if theres nothing
{
	std::vector<Tile> nodes_to_try;
	for (auto q : game->tiles) {
		if ((q->structure != NULL) && (q->structure->type == "neutral")) {
			Tile temp = q;
			nodes_to_try.push_back(temp);
		}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_shelter(const Unit& unit)
//return the shortest path to a shelter owned by us, or a empty vector if theres nothing
{
	std::vector<Tile> nodes_to_try;
	for (auto q : player->structures) {
		if (q->type == "shelter") {
			Tile temp = q->tile;
			nodes_to_try.push_back(temp);
		}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_neutral_human(const Unit& unit)
//return the shortest path to a neutral unowned unit, or a empty vector if theres nothing
{
	std::vector<Tile> nodes_to_try;
	for (auto q : game->units) {
		if (q->owner == NULL) {
			Tile temp = q->tile;
			nodes_to_try.push_back(temp);
		}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_soldier(const Unit& unit)
//return the shortest path to a soldier, or a empty vector if theres nothing
{
	std::vector<Tile> nodes_to_try;
	for (auto q : player->units) {
		if (q->job->title == "soldier" && q != defender) {
			Tile temp = q->tile;
			nodes_to_try.push_back(temp);
		}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_enemy(const Unit& unit)
//return the shortest path to a soldier, or a empty vector if theres nothing
{
	std::vector<Tile> nodes_to_try;
	for (auto q : player->opponent->units) {
			Tile temp = q->tile;
			nodes_to_try.push_back(temp);
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_food(const Unit& unit)
//return the shortest path to a gatherable food bush, or a empty vector if theres nothing
{
	std::vector<Tile> nodes_to_try;
	for (auto q : game->tiles) {
		if ((q->harvest_rate > 0) && (q->turns_to_harvest == 0)) {
			Tile temp = q;
			nodes_to_try.push_back(temp);
		}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_full_deposit(const Unit& unit)
//return the shortest path to a tile with enough materials on it to build a shelter, or a empty vector if theres nothing
{
	std::vector<Tile> nodes_to_try;
	for (auto q : game->tiles) {
		if ((q->materials >= 50) && (q->structure == NULL)) {
			Tile temp = q;
			nodes_to_try.push_back(temp);
		}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_deposit(const Unit& unit)
//find the closest tile with materials dropped on it
{
	std::vector<Tile> nodes_to_try;
	for (auto q : game->tiles) {
		if ((q->materials > 0) && (q->structure == NULL)) {
			Tile temp = q;
			nodes_to_try.push_back(temp);
		}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_empty_tile(const Unit& unit)
//find the closest tile that is free of units and structures
{
	std::vector<Tile> nodes_to_try;
	for (auto q : game->tiles) {
		if ((q->is_pathable()) && (q->structure == NULL)) {
			Tile temp = q;
			nodes_to_try.push_back(temp);
		}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_structure(const Unit& unit)
//find the closest neutral structure we can gather from
{
	std::vector<Tile> nodes_to_try;
	for (auto q : game->tiles) {
		if ((q->structure != NULL) && (q->structure->type == "neutral")) {
			Tile temp = q;
			nodes_to_try.push_back(temp);
		}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_enemy_defender(const Unit& unit)
//find the closest enemy within a limited range, in other words a enemy within the range of a defender
{
	std::vector<Tile> nodes_to_try;
	for (auto q : player->opponent->units) { //grab a list of enemies
			if(is_adj_to_cat(q->tile)) {
				Tile temp = q->tile;
				nodes_to_try.push_back(temp);
			}
	}
	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_enemy_cat(const Unit& unit)
//don't judge this was written on no sleep, it lets us attack while moving to the cat
{
	std::vector<Tile> nodes_to_try;
	std::vector<Tile> tilesToTry;
	tilesToTry.push_back(player->opponent->cat->tile);
	Tile Ntile = player->opponent->cat->tile->tile_north;
	Tile Stile = player->opponent->cat->tile->tile_south;
	Tile Etile = player->opponent->cat->tile->tile_east;
	Tile Wtile = player->opponent->cat->tile->tile_west;
	bool ourNUnit = false;
	bool ourSUnit = false;
	bool ourEUnit = false;
	bool ourWUnit = false;
	for (auto q : player->units) {
		if ((Ntile != NULL) && (q == Ntile->unit)) {
			ourNUnit = true;
		}
		if ((Stile != NULL) && (q == Stile->unit)) {
			ourSUnit = true;
		}
		if ((Etile != NULL) && (q == Etile->unit)) {
			ourEUnit = true;
		}
		if ((Wtile != NULL) && (q == Wtile->unit)) {
			ourWUnit = true;
		}
	}
	if ((Ntile != NULL) && (!ourNUnit) && (Ntile->unit != NULL)) {
		tilesToTry.push_back(player->opponent->cat->tile->tile_north);
	}
	if ((Stile != NULL) && (!ourSUnit) && (Stile->unit != NULL)) {
		tilesToTry.push_back(player->opponent->cat->tile->tile_south);
	}
	if ((Etile != NULL) && (!ourEUnit) && (Etile->unit != NULL)) {
		tilesToTry.push_back(player->opponent->cat->tile->tile_east);
	}
	if ((Wtile != NULL) && (!ourWUnit) && (Wtile->unit != NULL)) {
		tilesToTry.push_back(player->opponent->cat->tile->tile_west);
	}
	for (auto q : tilesToTry) {
		Tile temp = q;
		nodes_to_try.push_back(temp);
	}

	return find_closest_helper(nodes_to_try, unit);
}

std::vector<Tile> AI::find_closest_helper(const std::vector<Tile>& nodes_to_try, const Unit& unit)
//this is called by all of the find_ functions it takes the filted list of acceptable nodes
//and finds the node thats closest to the given unit, then returns a path to that node
{
	std::vector<std::vector<Tile>> possible_paths;
	for (auto q : nodes_to_try) {
		std::vector<Tile> temp = find_path(unit->tile, q);
		if (!temp.empty())
			possible_paths.push_back(temp);
	}
	if (possible_paths.empty()) {
		std::vector<Tile> temp;
		return temp;
	}
	unsigned size = possible_paths[0].size();
	int index = 0;
	for (unsigned i = 0; i < possible_paths.size(); ++i) {
		if (possible_paths[i].size() < size) {
			size = possible_paths[i].size();
			index = i;
		}
	}
	return possible_paths[index];
}

/////////////////////////
//Misc utility functions
/////////////////////////

void AI::mover(Unit& unit, std::vector<Tile> path)
//moves the unit one tile forward along the given path
{
	if ((unit->moves > 0) && (path.size() > 1)) {
		unit->move(path[0]);
	}
}

bool AI::is_adj_to_cat(const Tile& tile)
//check if the tile is a tile that is adjacent to our cat
{
	return (tile->tile_north == player->cat->tile ||
	        tile->tile_south == player->cat->tile ||
	        tile->tile_west == player->cat->tile ||
	        tile->tile_east == player->cat->tile);
}

bool AI::choose_job(Unit& unit)
//allows fresh humans to choose a "job" or class of human
{
	int enemy_soldier_count = 0;
	int player_soldier_count = 0;
	auto loser_units = player->opponent->units;
	auto player_units = player->units;

	for (auto unit : loser_units) {
		if (unit->job->title == "soldier") {
			enemy_soldier_count++;
		}
	}

	for (auto unit : player_units) {
		if (unit->job->title == "soldier") {
			player_soldier_count++;
		}
	}
	cout << "Choosing job." << endl;

	if (death_squad) { //intiate death
		cout << "DEATH SQUAD INITATED, CHOOSE SOLDIER." << endl;
		if (!unit->change_job("soldier")) {
			cout << "NOT ENOUGH ENERGY. REST AND THEN DESTROY." << endl;
			unit->rest();
		}
	}

	int builders = 0, soldiers = 0, missionaries = 0, gatherers = 0;

	for (auto unit : player_units) {
		if (unit->job->title == "builder")
			builders++;
		else if (unit->job->title == "soldier")
			soldiers++;
		else if (unit->job->title == "missionary")
			missionaries++;
		else if (unit->job->title == "gatherer")
			gatherers++;
	}

	if (missionaries < 1 || missionaries < soldiers / 3) { //one missionary for every 3 soldiers
		cout << "Choosing missionary" << endl;
		return unit->change_job("missionary");
	} else if (enemy_soldier_count+1 > player_soldier_count) { //more soldiers
		cout << "They have soldiers choose soldier" << endl;
		return unit->change_job("soldier");
	} else if (gatherers < 1) { //need a food source
		cout << "Choosing gatherer" << endl;
		return unit->change_job("gatherer");
	} else if (builders < 1) { //need a shelter builder
		cout << "Choosing builder" << endl;
		return unit->change_job("builder");
	} else { //more soldiers!
		cout << "Choosing soldier" << endl;
		return unit->change_job("soldier");
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
//Turn Functions:
//These function calls specifiy the actions of the various jobs(or classes) of the units
/////////////////////////////////////////////////////////////////////////////////////////

bool AI::move_to_shelter_to_rest(Unit& unit)
//have the unit move to the nearest shelter, and if we make it then rest
{
	std::vector<Tile> closestShelter = find_closest_shelter(unit);
	if (closestShelter.size() == 0) {//error, we dont have a shelter lets move to the cat instead
		closestShelter = find_path(unit->tile, player->cat->tile);
	}
	if (closestShelter.size() == 1) {//we are adjacent a shelter so lets rest
		unit->rest();
		return true;
	} else {//we haven't reached the shelter, lets move towards it
		while((closestShelter.size() > 1) && (unit->moves > 0)) {
			mover(unit, closestShelter);
			closestShelter = find_closest_shelter(unit);
		}
		if (closestShelter.size() == 1) {//if we reach the shelter this turn then go ahead and rest
			unit->rest();
		}
		return true;
	}
}

bool AI::converter_turn(Unit& unit)
{
	if (unit->energy < 75.0) { //not enough energy so try to rest by moving to a rest point
		return move_to_shelter_to_rest(unit);
	}

	//energy higher than 75 so lets try to convert a enemy
	std::vector<Tile> closestNeutralHuman = find_closest_neutral_human(unit);
	if (closestNeutralHuman.empty()) { //no neutral enemies to convert, so lets follow a soilder instead
		std::vector<Tile> soldierToFollow = find_closest_soldier(unit);
		if (soldierToFollow.size() == 1) {
			return true; //already at soldier
		}
		if ((soldierToFollow.size() > 1) && (unit->moves > 0)) { //move towards soldier
			while((soldierToFollow.size() > 1) && (unit->moves > 0)) {
				mover(unit, soldierToFollow);
				soldierToFollow = find_closest_soldier(unit);
			}
			return true; //at soilder, or moved as far as possible
		} else {
			return false; //error no soldier so what do we want to do?
		}
	} else { //neutral enemy to convert
		while((closestNeutralHuman.size() > 1) && (unit->moves > 0)) { //move to the neutral humans
			mover(unit, closestNeutralHuman);
			closestNeutralHuman = find_closest_neutral_human(unit);
		}
		if (closestNeutralHuman.size() == 1) {
			unit->convert(closestNeutralHuman[0]);
			Unit temp = game->get_tile_at(closestNeutralHuman[0]->x, closestNeutralHuman[0]->y)->unit;
			fresh_turn(temp);
		}
		//converted an enemy so lets head towards a restpoint
		return move_to_shelter_to_rest(unit);
	}
	return false; //error something went wrong the turn couldn't finish
}

bool AI::soldier_turn(Unit& unit)
{
	if (unit->energy <= 25.0) {//not enough energy so lets go rest
		move_to_shelter_to_rest(unit);
	} else {//if we aren't in danger and already resting lets not stop until we are healed
		std::vector<Tile> closestStructure = find_closest_shelter(unit);
		if (closestStructure.size() == 1 && unit->energy < 100.0) {
			std::vector<Tile> closestEnemy = find_closest_enemy(unit);
			if ((closestEnemy.size() != 0 && closestEnemy.size() > 2) || unit->energy <= 75) { //if we aren't in danger or if the unit energy is too low to do anything anyway then rest
				unit->rest();
				return true;
			}
		}
		//have energy or we are in danger lets hunt for a enemy
		std::vector<Tile> closestEnemy = find_closest_enemy(unit);
		if (closestEnemy.empty()) {
			return false; //error no enemy on the map, we don't know what to do
		} else {//move to and attack the closest enemy
			while((closestEnemy.size()) > 1 && (unit->moves > 0)) {
				mover(unit, closestEnemy);
				closestEnemy = find_closest_enemy(unit);
			}
			if (closestEnemy.size() == 1) {
				unit->attack(closestEnemy[0]);
			}
			return true;
		}
	}
	return false; // error the turn was not able to finish
}

bool AI::gatherer_turn(Unit& unit)
{
	//if storage is full or energy is low return and deposit food
	int temp = unit->food;
	std::cout << "Carrying " << temp << endl;

	if ((unit->food >= unit->job->carry_limit) || (unit->energy < 75.0)) { //going to return food or sleep
		std::vector<Tile> closestShelter = find_closest_shelter(unit);
		while((closestShelter.size() > 1) && (unit->moves > 0)) {
			mover(unit, closestShelter);
			closestShelter = find_closest_shelter(unit);
		}

		if (closestShelter.size() == 1) { //if we are at the shelter
			std::cout << "Dropping: " << unit->food << " food" << endl;
			unit->drop(game->get_tile_at(closestShelter[0]->x, closestShelter[0]->y), "food");
			if (unit->energy < 75.0) {//rest if we need to
				unit->rest();
				return true;
			}
		}
	}
	//hunt for food to gather
	std::vector<Tile> foodSource = find_closest_food(unit);
	while((foodSource.size() > 1) && (unit->moves > 0)) {
		mover(unit, foodSource);
		foodSource = find_closest_food(unit);
	}
	if (foodSource.size() == 1) { //if we are at the food source
			Tile temp = game->get_tile_at(foodSource[0]->x, foodSource[0]->y);
			unit->harvest(temp);
			if (unit->moves > 0) { //if we can lets start back towards the shelter
				std::vector<Tile> closestShelter = find_closest_shelter(unit);
				while((closestShelter.size() > 1) && (unit->moves > 0)) {
					mover(unit, closestShelter);
					closestShelter = find_closest_shelter(unit);
				}
				if (closestShelter.size() == 1) {//we made it to a shelter
					unit->drop(game->get_tile_at(closestShelter[0]->x, closestShelter[0]->y), "food");
					std::cout << "Dropping: " << unit->food << " food" << endl;
					unit->rest();
				}
				return true;
			}
		}
		return false; //something went wrong
	}

bool AI::builder_turn(Unit& unit)
{
	//if there is any tile with enough materials and we have the energy go construct a shelter
	if (unit->energy >= 75.0) {
		std::vector<Tile> closestFullMaterialDeposit = find_full_deposit(unit);

		if ((closestFullMaterialDeposit.size() > 0) && (closestFullMaterialDeposit.size() < 15) && (unit->energy >= 75.0)) { //build!
			while(closestFullMaterialDeposit.size() > 1 && unit->moves > 0) { //move to deposit
				mover(unit, closestFullMaterialDeposit);
				closestFullMaterialDeposit = find_full_deposit(unit);
			}
			if (closestFullMaterialDeposit.size() == 1) { //where we wanna construct
				if (unit->energy >= 75.0) { //construct
					unit->construct(game->get_tile_at(closestFullMaterialDeposit[0]->x, closestFullMaterialDeposit[0]->y), "shelter");
					unit->rest();
					return true;
				}
			} else { //wait till next turn to construct
					return true;
				}
		}
	}

	if (unit->materials >= unit->job->carry_limit) { //if inventory is full go to closest tile with materials
		std::vector<Tile> closestMaterialDeposit = find_closest_deposit(unit);
		if (closestMaterialDeposit.size() == 0 || closestMaterialDeposit.size() > 15) {
			//no tile with materials close by on it so lets just find the nearest empty tile
			std::vector<Tile> closestEmptyTile = find_closest_empty_tile(unit);
			while(closestEmptyTile.size() > 1 && unit->moves >0) { //move to empty tile
				mover(unit, closestEmptyTile);
				closestEmptyTile = find_closest_empty_tile(unit);
			}
			if (closestEmptyTile.size() == 1) { //drop materials on empty tile
				unit->drop(game->get_tile_at(closestEmptyTile[0]->x, closestEmptyTile[0]->y), "material");
				closestMaterialDeposit = find_closest_deposit(unit); //recalc the closestMaterial Deposit
				if (game->get_tile_at(closestMaterialDeposit[0]->x, closestMaterialDeposit[0]->y)->materials >= 50) { //build
					if (unit->energy > 75.0) { //build shelter
						unit->construct(game->get_tile_at(closestMaterialDeposit[0]->x, closestMaterialDeposit[0]->y), "shelter");
						unit->rest();
						return true;
					} else { //wait till next turn to build
						return true;
					}
				}
			}
		} else { //found a tile with materials
			while(closestMaterialDeposit.size() > 1 && unit->moves > 0) { //move to tile with materials
				mover(unit, closestMaterialDeposit);
				closestMaterialDeposit = find_closest_deposit(unit);
			}
			if (closestMaterialDeposit.size() == 1) { //drop materials if we can
				unit->drop(game->get_tile_at(closestMaterialDeposit[0]->x, closestMaterialDeposit[0]->y), "material");
				if (game->get_tile_at(closestMaterialDeposit[0]->x, closestMaterialDeposit[0]->y)->materials >= 50) { //build
					if (unit->energy > 75.0) { //build shelter
						unit->construct(game->get_tile_at(closestMaterialDeposit[0]->x, closestMaterialDeposit[0]->y), "shelter");
						unit->rest();
						return true;
					} else { //wait till next turn to build
						return true;
					}
				}
			}
		}
	}

	if (unit->energy < 75.0) { //if energy is low go rest
		return move_to_shelter_to_rest(unit);
	} else { //if we have energy go get materials then deposit
		std::vector<Tile> closestNeutralStructure = find_closest_structure(unit);

		while(closestNeutralStructure.size() > 1 && unit->moves > 0) {
			mover(unit, closestNeutralStructure);
			closestNeutralStructure = find_closest_structure(unit);
		}

		if (closestNeutralStructure.size() == 1) { //at neutral structure
			if (unit->energy >= 75.0) { //harvest
				unit->deconstruct(game->get_tile_at(closestNeutralStructure[0]->x, closestNeutralStructure[0]->y));
				//harvested so head towards shelter
				return move_to_shelter_to_rest(unit);
			} else { //wait till next turn
				return true;
			}
		} else { //can't reach the neutral structure this turn
			return true;
		}
	}
	return false; //error something went wrong with the turn
}

bool AI::fresh_turn(Unit& unit)
{
	if (is_adj_to_cat(unit->tile) && unit->energy == 100) {
		return choose_job(unit); //try to chnage to a new job
	} else if (is_adj_to_cat(unit->tile)) {
		//not enough ENERGY
		unit->rest();
	}

	auto cat_path = find_path(unit->tile, player->cat->tile); //move to the cat

	while((cat_path.size() > 1) && (unit->moves > 0)) { //GET TO THA CAT
		mover(unit, cat_path);
		cat_path = find_path(unit->tile, player->cat->tile);
	}

	if (cat_path.size() == 1) { //try changing the job again
		if (is_adj_to_cat(unit->tile)) {
			return choose_job(unit);
		}
	}

	return true;
}

bool AI::defender_turn(Unit& unit)
{
	auto cat_attacker_path = find_closest_enemy_defender(unit);

	if (cat_attacker_path.size() > 0) { //if people are attacking our cat
		while (cat_attacker_path.size() > 1 && unit->moves > 0) { //move to them
			mover(unit, cat_attacker_path);
			cat_attacker_path = find_closest_enemy_defender(unit);
		}
		if (cat_attacker_path.size() == 1) { //kill
			if (unit->energy >= 25.0)
				unit->attack(cat_attacker_path[0]);
		}
	} else { //people are not attacking our cat
		if (is_adj_to_cat(unit->tile)) {
			if (unit->energy < 100) //try to rest
				unit->rest();
			return true;
		} else { //otherwise get back to our cat
			auto cat_path = find_path(unit->tile, player->cat->tile);

			while (cat_path.size() > 1 && unit->moves > 0) {
				mover(unit, cat_path);
				cat_path = find_path(unit->tile, player->cat->tile);
			}
			if (is_adj_to_cat(unit->tile) && unit->moves > 0) {
				if (unit->energy < 100) //try to rest
					unit->rest();
			}
		}
	}

	return true;
}

bool AI::death_squad_turn(Unit& unit)
{
	if ((unit->job->title == "soldier") && (unit->energy > 25))  {
		auto path_to_enemy_cat = find_enemy_cat(unit);

		if (path_to_enemy_cat.size() == 0) { //cat is blocked off
			std::vector<Tile> closestEnemy = find_closest_enemy(unit); //try to find an enemy

			if (closestEnemy.empty()) {
				std::cout << "error no enemy, death_squad turn" << endl;
				return false; // no enemy so what do you want to do?
			} else {
				while ((closestEnemy.size()) > 1 && (unit->moves > 0)) {
					mover(unit, closestEnemy); //mobilize to attack!
					closestEnemy = find_closest_enemy(unit);
				}
				if (closestEnemy.size() == 1) { //attack!
					unit->attack(closestEnemy[0]);
				}
				return true;
			}
		} else {
			while(path_to_enemy_cat.size() > 1 && unit->moves > 0) { //move to cat
				mover(unit, path_to_enemy_cat);
				path_to_enemy_cat = find_enemy_cat(unit);
			}
			if (path_to_enemy_cat.size() == 1) { //next to cat, attack!
				unit->attack(game->get_tile_at(path_to_enemy_cat[0]->x, path_to_enemy_cat[0]->y));
			}
		}
	} else {
		if (unit->job->title == "soldier") { //let's heal
			return move_to_shelter_to_rest(unit);
		} else { //go turn into a soldier!
			return fresh_turn(unit);
		}
	}
	return true;
}

} // catastrophe

} // cpp_client
