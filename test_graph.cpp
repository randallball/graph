#include "graph.h"

using namespace std;

int main()
{
	graph g;
    double limit = 1500;
    init_graph_from_file(g, "world_cities.txt", limit);
    g.does_bfs_path_exist("Veracruz, Mexico", "Sao Paulo, Brazil");
}
