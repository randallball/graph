#include "graph.h"
using namespace std;

// default constructor at work here; pretty cute!
graph::graph()
{
}	

// destructor is easy, we just clear our vertex vector and our edge map
graph::~graph()
{
	vertices.clear();
	edges.clear();
}	

// add a vertex to the graph by adding it to the vector
void graph::add_vertex(const vertex& v) 
{
	vertices.push_back(v);
	vector<vertex*> empty;
	edges.insert(pair<vertex*, vector<vertex*>> (&v, empty));
}

// add an edge to the graph as long as it's under the distance limit
void graph::add_edge(vertex* v, vertex* u, double limit)
{
	if (great_circle_distance(*v, *u) <= limit)
	{
		edges.at(v).push_back(u);
		edges.at(u).push_back(v);
	}
}

// compute distance from one lat/long to another as the crow flies
double graph::great_circle_distance(const vertex& v, const vertex& u) const
{
	double PI = 3.1415926535897932;
	double lat1, lat2, long1, long2, dist;

	lat1 = ((double) v.get_latitude_degrees()) + (((double) v.get_latitude_minutes()) / 60.0);
	lat1 *= PI/180.0;
	long1 = ((double) v.get_longitude_degrees()) + (((double) v.get_longitude_minutes()) / 60.0);
	long1 *= PI/180.0;
	lat2 = ((double) u.get_latitude_degrees()) + (((double) u.get_latitude_minutes()) / 60.0);
	lat2 *= PI/180.0;
	long2 = ((double) u.get_longitude_degrees()) + (((double) u.get_longitude_minutes()) / 60.0);
	long2 *= PI/180.0;

	// from http://www.meridianworlddata.com/Distance-Calculation.asp
	// result in km
	dist = 6378.7 * acos((sin(lat1) * sin(lat2)) + (cos(lat1) * cos(lat2) * cos(long2 - long1)));

	return dist;
}

// read in 120 cities and their latitude/longitude
// cities within limit km of each other are connected by edges
void init_graph_from_file(graph& g, const string& filename, double limit)
{
	string line;
	string city_name, country, N, W;
	int lat1, lat2, long1, long2;
	ifstream file_to_read;
	// open the data file of cities
	open_for_read(file_to_read, filename);
	while (is_more_stuff_there(file_to_read))
	{
		file_to_read >> city_name >> country >> lat1 >> lat2 >> N >> long1 >> long2 >> W;
		if (N == "S")
		{
			lat1 *= -1;
			lat2 *= -1;
		}
		if (W == "W")
		{
			long1 *= -1;
			long2 *= -1;
		}

		country.erase(country.length()-1,1);
		city_name += country;
		vertex city(city_name, lat1, lat2, long1, long2);
		g.add_vertex(city);
	}
	for (unsigned int i = 0; i < g.vertices.size(); i++)
	{
		for (unsigned int j = i; j < g.vertices.size(); j++)
		{
			g.add_edge(&g.vertices[i], &g.vertices[j], limit);
		}
	}

	// now we are done with our file
	file_to_read.close();

}

// function defined by Michael Main for input data
void open_for_read(ifstream& f, string filename)
{
	f.open(filename);
	if (f.fail())
	{
		cerr << "Could not open input file." << endl;
		exit(0);
	}
}

// function defined by Michael Main for input data
bool is_more_stuff_there(ifstream& f)
{
		return (f && (f.peek() != EOF));
}

bool does_dfs_path_exist(const string& city1, const string& city2)
{
	map<vertex*, bool> visited;
	map<vertex*, vertex*> path;
	vertex* v;
	vertex* u;
	for (int i = 0; i < vertices.size(); ++i)
	{
		visited.insert(pair<vertex*, bool> (&vertices[i], false));
		path.insert(pair<vertex*, vertex*> (&vertices[i], nullptr));
		if (vertices[i].get_city_name() == city1)
			v = &vertices[i];
		if (vertices[i].get_city_name() == city2)
			u = &vertices[i];
	}
	deque<vertex*> yet_to_explore;
	visited[v] = true;
	yet_to_explore.push_back(v);
	graph::depth_first_search(u, visited, &yet_to_explore, path);
}

bool depth_first_search(vertex* u, map<vertex*, bool>& visited, deque<vertex*>& yet_to_explore, map<vertex*, vertex*>& path)
{
	if (!yet_to_explore.empty())
	{
		vertex* w = yet_to_explore.back();
		yet_to_explore.pop_back();
		if (w->get_city_name() == u->get_city_name())
			return true;
		vector<vertex*>::iterator it;
		for (it=edges[w].begin(); it != edges[w].end(); ++it)
		{
			vertex* neighbor = *it;
			if (visited[neighbor] == false)
			{
				yet_to_explore.push_back(neighbor);
				visited[neighbor] = true;
				path[neighbor] = w;
			}
		}
		return depth_first_search(u, visited, yet_to_explore, path);
	}
	else
		return false;
}

bool does_bfs_path_exist(const string& city1, const string& city2);
{
	map<vertex*, bool> visited;
	map<vertex*, vertex*> path;
	vertex* v;
	vertex* u;
	for (int i = 0; i < vertices.size(); ++i)
	{
		visited.insert(pair<vertex*, bool> (&vertices[i], false));
		path.insert(pair<vertex*, vertex*> (&vertices[i], nullptr));
		if (vertices[i].get_city_name() == city1)
			v = &vertices[i];
		if (vertices[i].get_city_name() == city2)
			u = &vertices[i];
	}
	deque<vertex*>& yet_to_explore;
	visited[v] = true;
	yet_to_explore.push_back(v);
	if (depth_first_search(u, visited, yet_to_explore, path))
	{
		vertex* cursor = u;
		while (cursor != v)
		{
			cout << path[cursor].get_city_name();
			cursor = path[cursor];
		}
	}
	
}

bool breadth_first_search(vertex* u, map<vertex*, bool>& visited, deque<vertex*>& yet_to_explore, map<vertex*, vertex*>& path)
{
	if (!yet_to_explore.empty())
	{
		vertex* w = yet_to_explore.front();
		yet_to_explore.pop_front();
		if (w->get_city_name() == u->get_city_name())
			return true;
		vector<vertex*>::iterator it;
		for (it=edges[w].begin(); it != edges[w].end(); ++it)
		{
			vertex* neighbor = *it;
			if (visited[neighbor] == false)
			{
				yet_to_explore.push_back(neighbor);
				visited[neighbor] = true;
				path[neighbor] = w;
			}
		}
		return depth_first_search(u, visited, yet_to_explore, path);
	}
	else
		return false;
}
