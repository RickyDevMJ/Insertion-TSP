#include <bits/stdc++.h>
using namespace std;

u_int edge_hash(u_int i, u_int j, u_int m) {
	return i * m + j;
}

float distance(float x_i, float y_i, float x_j, float y_j) {
	float l1 = x_i - x_j;
	float l2 = y_i - y_j;
	return sqrt(l1*l1 + l2*l2);
}
#define distance(coordinates, i, j) distance(coordinates[i].first, coordinates[i].second, coordinates[j].first, coordinates[j].second)

int main() {
	auto start = chrono::high_resolution_clock::now();
	u_int n;
	vector<pair<float, float>> coordinates;
	unordered_map<u_int, priority_queue<pair<float, u_int>>> insertion_cost;
	unordered_map<u_int, pair<u_int, u_int>> edge_map; 
	unordered_set<u_int> current_vertices, current_edges;

	// Reading inputs
	cin >> n;
	for (u_int i = 0; i < n; i++) {
		u_int id;
		float x, y;
		cin >> id >> x >> y;
		coordinates.push_back(make_pair(x, y));
	}

	// Initial vertex is vertex 0
	current_vertices.insert(0);

	// Choosing the second vertex
	float min_distance = MAXFLOAT;
	u_int vertex_index = 0, edge_index;
	for (u_int i = 1; i < n; i++) {
		float dist = distance(coordinates, 0, i);
		if (dist < min_distance) {
			min_distance = dist;
			vertex_index = i;
		}
	}
	current_vertices.insert(vertex_index);
	edge_index = edge_hash(0, vertex_index, n);
	edge_map[edge_index] = make_pair(0, vertex_index);
	current_edges.insert(edge_index);
	

	// Choosing remaining vertices
	for (u_int i = 0; i < n; i++) {
		if (current_vertices.find(i) != current_vertices.end())
			continue;

		priority_queue<pair<float, u_int>> temp;
		insertion_cost.emplace(pair<u_int, priority_queue<pair<float, u_int>>>(i, temp));

		u_int v1 = edge_map[edge_index].first, v2 = edge_map[edge_index].second;
		float c = distance(coordinates, v1, v2), d1 = distance(coordinates, i, v1), d2 = distance(coordinates, i, v2);
		insertion_cost[i].push(make_pair(-(d1+d2-c), edge_index));
	}
	
	for (u_int num = 2; num < n; num++) {
		min_distance = MAXFLOAT;
		vertex_index = 0;
		for (u_int i = 0; i < n; i++) {
			if (current_vertices.find(i) != current_vertices.end())
				continue;
			
			pair<float, u_int> top = insertion_cost[i].top();
			while (current_edges.find(top.second) == current_edges.end()) {
				insertion_cost[i].pop();
				top = insertion_cost[i].top();
			}
			if (top.first < min_distance) {
				min_distance = top.first;
				vertex_index = i;
				edge_index = top.second;
			}
		}

		u_int v1 = edge_map[edge_index].first, v2 = edge_map[edge_index].second;
		u_int edge_index_1 = edge_hash(vertex_index, v1, n), edge_index_2 = edge_hash(vertex_index, v2, n);
		current_vertices.insert(vertex_index);
		if(num != 2)
			current_edges.erase(edge_index);
		current_edges.insert(edge_index_1);
		current_edges.insert(edge_index_2);
		edge_map[edge_index_1] = make_pair(vertex_index, v1);
		edge_map[edge_index_2] = make_pair(vertex_index, v2);

		for (u_int i = 0; i < n; i++) {
			if (current_vertices.find(i) != current_vertices.end())
				continue;

			float c1 = distance(coordinates, vertex_index, v1), c2 = distance(coordinates, vertex_index, v2);
			float d = distance(coordinates, i, vertex_index), d1 = distance(coordinates, i, v1), d2 = distance(coordinates, i, v2);
			
			insertion_cost[i].push(make_pair(-(d+d1-c1), edge_index_1));
			insertion_cost[i].push(make_pair(-(d+d2-c2), edge_index_2));
		}
	}

	// Output answer
	float ans = 0.f;
	assert(current_edges.size() == n);
	//cout << endl << "Edges in tour: " << endl; 
	unordered_set<u_int> first_occurence, second_occurence;
	for (const auto& edge: current_edges) {
		u_int e1 = edge_map[edge].first, e2 = edge_map[edge].second;

		// Correctness check
		if (first_occurence.find(e1) == first_occurence.end())
			first_occurence.insert(e1);
		else if (second_occurence.find(e1) == second_occurence.end())
			second_occurence.insert(e1);
		else {
			cout << "Error in output !!" << endl;
			return 1;
		}
		if (first_occurence.find(e2) == first_occurence.end())
			first_occurence.insert(e2);
		else if (second_occurence.find(e2) == second_occurence.end())
			second_occurence.insert(e2);
		else {
			cout << "Error in output !!" << endl;
			return 1;
		}

		//cout << e1 << " " << e2 << endl;
		ans += distance(coordinates, e1, e2);
	}

	// Another correctness check
	for (u_int e1 : first_occurence) {
		if (second_occurence.find(e1) == second_occurence.end()) {
			cout << "Error in output !!" << endl;
			return 1;
		}
	}

	//cout << endl;
	cout << "Cost: " << ans << endl;
	auto end = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
	cout << "Program time : " << duration.count() << endl;
	return 0;
}
