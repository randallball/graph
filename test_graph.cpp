#include "graph.h"

using namespace std;

int main()
{
	graph g;
    double limit = 2000;
    init_graph_from_file(g, "world_cities.txt", limit);
}
