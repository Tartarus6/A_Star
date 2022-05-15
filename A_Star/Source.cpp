# include <iostream>
# include <vector>
# include <algorithm>
# include <map>
# include <string>
# include <limits>
# include <chrono>
# include <fstream>

using namespace std;

void print(string s) {
	cout << s << endl;
}

bool iVectorIsIn(vector<int> v, int e) {
	for (int i : v) {
		if (i == e) {
			return true;
		}
	}
	return false;
}

bool fVectorIsIn(vector<float> v, float e) {
	for (float i : v) {
		if (i == e) {
			return true;
		}
	}
	return false;
}

bool viVectorIsIn(vector<vector<int>> v, vector<int> e) {
	for (vector<int> i : v) {
		if (i == e) {
			return true;
		}
	}
	return false;
}

void fvectorPrint(vector<float> l) {
	cout << "[";
	for (float i = 0; i < l.size(); i++) {
		cout << l.at(i);

		if (i != l.size() - 1) {  // if the current element is the last in the vector
			cout << ", ";
		}
	}
	cout << "]" << endl;
}

void ivectorPrint(vector<int> l) {
	cout << "[";
	for (int i = 0; i < l.size(); i++) {
		cout << l.at(i);

		if (i != l.size() - 1) {  // if the current element is the last in the vector
			cout << ", ";
		}
	}
	cout << "]" << endl;
}

void gridPrint(vector<vector<float>> grid) {
	for (vector<float> i : grid) {
		fvectorPrint(i);
	}
}

void gridPrint(vector<vector<int>> grid) {
	for (vector<int> i : grid) {
		ivectorPrint(i);
	}
}

int minIndex(vector<int> v) {
	int min_value = v[0];
	int min_index = 0;
	for (int i = 0; i < v.size(); i++) {
		if (v[i] < min_value) {
			min_value = v[i];
			min_index = i;
		}
	}
	return min_index;
}

vector<int> minKey(map<vector<int>, float> map, vector<vector<int>> _keys) {
	vector<vector<int>> keys;
	vector<int> values;

	for (auto my_pair : map) {
		if (viVectorIsIn(_keys, my_pair.first)) {
			keys.push_back(my_pair.first);
			values.push_back(my_pair.second);
		}
	}

	return keys[minIndex(values)];
}

float getValue(vector<int> position, vector<vector<float>> maze) {
	return maze[position[0]][position[1]];
}

int getIndex(vector<vector<int>> v, vector<int> K)
{
	auto it = find(v.begin(), v.end(), K);

	// If element was found
	if (it != v.end())
	{
		// calculating the index
		// of K
		int index = it - v.begin();
		return index;;
	}
	else {
		// If the element is not
		// present in the vector
		return -1;
	}
}

vector<vector<int>> neighbors(vector<int> position, vector<vector<float>> maze) {
	vector<vector<int>> neighbors_vector;

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			vector<int> neighbor = { position[0] + i, position[1] + j };
			if (i == j) {  // makes it so that the input position won't be added to the output as one of the neighbors and won't be outside the maze
				continue;
			}
			// if ((((position[0] + i < 0) || (position[1] + j < 0)) || (position[0] + i >= maze.size())) || (position[1] + j >= maze[i].size())) {  // makes it so that the output won't be outside the maze
			// 	continue;
			// }
			if (position[0] + i < 0) {
				continue;
			}
			if (position[1] + j < 0) {
				continue;
			}
			if (position[0] + i >= maze.size()) {
				continue;
			}
			if (position[1] + j >= maze[0].size()) {
				continue;
			}
			if (getValue(neighbor, maze) == -1) {
				continue;
			}
			if (!((i == 0) || (j == 0))) {
				continue;
			}
			neighbors_vector.push_back(neighbor);
		}
	}

	return neighbors_vector;
}

float heuristic(vector<int> current_position, vector<int> ending_position, float heuristic_guess) {
	float average_value = heuristic_guess;

	float distance_to_goal = abs(max(current_position[0], ending_position[0]) - min(current_position[0], ending_position[0])) + abs(max(current_position[1], ending_position[1]) - min(current_position[1], ending_position[1]));

	return distance_to_goal * average_value;
}

void cameFromPrint(map<vector<int>, vector<int>> came_from) {
	vector<vector<int>> keys;
	vector<vector<int>> values;

	for (auto my_pair : came_from) {
		keys.push_back(my_pair.first);
		values.push_back(my_pair.second);
	}
	cout << "came_from = {";
	for (int i = 0; i < keys.size(); i++) {
		cout << "[" << keys[i][0] << ", " << keys[i][1] << "]: [" << values[i][0] << ", " << values[i][1] << "]";
		if (i != keys.size() - 1) {
			cout << ", ";
		}
	}
	cout << "}" << endl;
}

vector<vector<int>> reconstruct_path(map<vector<int>, vector<int>> came_from, vector<int> _current) {
	vector<vector<int>> path = {};

	vector<int> current = { 2, 2 };

	while (came_from.find(current) != came_from.end()) {
		path.push_back(current);
		current = came_from[current];
	}
	path.push_back(current);

	return path;
}

vector<vector<float>> getMaze() {
	vector<vector<float>> maze;

	fstream my_file;
	my_file.open("my_file.txt", ios::in);
	string ch;


	vector<float> temp;
	while (1) {


		my_file >> ch;
		if (my_file.eof())
			break;
		if (ch == "end") {
			maze.push_back(temp);
			temp = {};
			continue;
		}

		temp.push_back(stof(ch));

	}

	my_file.close();

	return maze;
}

class AStarOutput {
public:
	vector<vector<float>> maze;
	vector<vector<int>> path;
	chrono::milliseconds elapsed;
	bool failed = false;
};

AStarOutput aStar(vector<vector<float>> maze, float heuristic_guess, vector<int> starting_position, vector<int> ending_position) {
	auto start = chrono::high_resolution_clock::now();  // start time, used for timing the code

	float infinity = std::numeric_limits<float>::infinity();

	// vector of Nodes that need to be explored
	vector<vector<int>> ToDo = { starting_position };

	// vector of Nodes that have been explored
	vector<vector<int>> explored = {};

	// the complete history of the explored vector, not used in algorithm, just for visual output
	vector<vector<vector<int>>> explored_history = {};
	// the complete history of the to_do vector, not used in algorithm, just for visual output
	vector<vector<vector<int>>> to_do_history = {};
	// vector of the paths taken to explore nodes, not used in algorithm, just for visual output
	vector<vector<vector<int>>> path_history = {};

	// For node n, came_from[n] is the node immediately preceding it on the cheapest path from start to n currently known.
	map<vector<int>, vector<int>> came_from;

	// gScore[n] on wiklipedia will be shortest_path_cost[n] on here
	map<vector<int>, float> shortest_path_cost;

	// fScore[n] on wiklipedia will be heuristic_dict[n] on here
	map<vector<int>, float> heuristic_dict;

	// set all position's shortest_path_cost and heuristic_dict to infinity
	for (int i = 0; i < maze.size(); i++) {  // for each row in g
		for (int j = 0; j < maze.at(i).size(); j++) {  // for each position in the row
			shortest_path_cost[{i, j}] = infinity;
			heuristic_dict[{i, j}] = infinity;
		}
	}
	shortest_path_cost[{0, 0}] = 0;  // the cost to reach the starting position will be 0

	// here's the algorithm part
	while (ToDo.size() > 0) {  // while there are still nodes on the ToDo list
		vector<int> current_node = minKey(shortest_path_cost, ToDo);

		if (current_node == ending_position) {
			vector<vector<int>> final_path = reconstruct_path(came_from, current_node);

			auto stop = chrono::high_resolution_clock::now();  // stop time, used for timing the code
			auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);  // code run duration, used for timing the code

			AStarOutput output;
			output.path = final_path;
			output.elapsed = duration;
			output.maze = maze;
			return output;
		}

		ToDo.erase(ToDo.begin() + getIndex(ToDo, current_node));
		explored.push_back(current_node);

		// insert history lists here

		for (auto neighbor : neighbors(current_node, maze)) {
			float tentative_shortest_path_cost = shortest_path_cost[current_node] + getValue(neighbor, maze);
			if (tentative_shortest_path_cost < shortest_path_cost[neighbor]) {
				came_from[neighbor] = current_node;

				shortest_path_cost[neighbor] = tentative_shortest_path_cost;
				heuristic_dict[neighbor] = tentative_shortest_path_cost + heuristic(neighbor, ending_position, heuristic_guess);

				if (!(viVectorIsIn(ToDo, neighbor))) {
					ToDo.push_back(neighbor);
				}
			}
		}
	}

	auto stop = chrono::high_resolution_clock::now();  // stop time, used for timing the code
	auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);  // code run duration, used for timing the code

	AStarOutput output;
	output.failed = true;
	output.elapsed = duration;
	return output;
}

// arguments start
vector<vector<float>> maze = getMaze();
vector<int> starting_position = { 0, 0 };
vector<int> ending_position = { 2, 2 };
float heuristic_guess = 2.5;
// arguments end

int main() {
	vector<vector<float>> maze = getMaze();

	AStarOutput a_star = aStar(maze, heuristic_guess, starting_position, ending_position);

	cout << "elapsed: " << a_star.elapsed.count() << " milliseconds" << endl << endl;

	cout << "final path: " << endl;
	gridPrint(a_star.path);

	return 0;
}
