#include "mover.h"

#include <iostream>

#include <SFML/System/Vector2.hpp>

#include "log.h"
#include "map/passage_site.h"

void Mover::find_paths() {
    GAME_INFO("Initializing path finding algorithm.");

    create_table();

    const auto ocup   = _unit->get_ocupation();
    const auto mv_pts = _unit->get_mv_points();
    _paths.resize(mv_pts + 1);

    _paths[0].insert(ocup);
    ocup->set_highlighted(true);
    /* TODO
    implement finding the most optimal route
    */

    for (int i = 0; i <= mv_pts; ++i)
        for (auto& x : _paths[i])
            for (int j = 0; j < 6; ++j) {
                auto side = x->get_side(static_cast<Directions>(j));

                if (side != nullptr) {
                    if (!side->is_highlighted()) {
                        const auto st = side->get_site_type();
                        auto mp_cost  = i;

                        if (st == Site_type::hexagon) {
                            auto side1 = static_cast<Hex_site*>(side);
                            mp_cost += _hex_table[side1->get_hex_type()];
                            if (mp_cost <= mv_pts) {
                                side1->set_highlighted(true);
                                _paths[mp_cost].insert(side1);
                            }

                        } else if (st == Site_type::passage) {
                            auto side1 = static_cast<Passage_site*>(side);
                            mp_cost += _pass_table[side1->get_passage_type()];

                            auto side2 = side1->other_side(x);

                            if (!side2->is_highlighted()) {
                                mp_cost += _hex_table[side2->get_hex_type()];

                                if (mp_cost <= mv_pts) {
                                    side1->set_highlighted(true);
                                    side2->set_highlighted(true);
                                    _passages.insert(side1);
                                    _paths[mp_cost].insert(side2);
                                }
                            }
                        }
                    }
                }
            }
}

void Mover::move(const sf::Vector2f& mouse_pos) {
    GAME_INFO("Moving unit.");
    for (int used_mp = 0; used_mp < _paths.size(); ++used_mp) {
        for (auto& x : _paths[used_mp]) {
            auto hex = static_cast<Hex_site*>(x);
            if (hex->contains(mouse_pos)) {
                _unit->place_on_hex(hex);
                _unit->reduce_mv_points(used_mp);

                clear();
                return;
            }
        }
    }
    clear();
    return;
}

void Mover::clear() {
    for (auto& vec : _paths)
        for (auto& y : vec)
            y->set_highlighted(false);

    for (auto& x : _passages)
        x->set_highlighted(false);

    _unit = nullptr;
    _paths.clear();
    _passages.clear();
}


void dijkstra(std::map<int std::vector<int>> graph, int src) 
{ 
     int dist[V];     // The output array.  dist[i] will hold the shortest 
                      // distance from src to i 
   
     bool sptSet[V]; // sptSet[i] will be true if vertex i is included in shortest 
                     // path tree or shortest distance from src to i is finalized 
   
     // Initialize all distances as INFINITE and stpSet[] as false 
     for (int i = 0; i < V; i++) 
        dist[i] = INT_MAX, sptSet[i] = false; 
   
     // Distance of source vertex from itself is always 0 
     dist[src] = 0; 
   
     // Find shortest path for all vertices 
     for (int count = 0; count < V-1; count++) 
     { 
       // Pick the minimum distance vertex from the set of vertices not 
       // yet processed. u is always equal to src in the first iteration. 
       int u = minDistance(dist, sptSet); 
   
       // Mark the picked vertex as processed 
       sptSet[u] = true; 
   
       // Update dist value of the adjacent vertices of the picked vertex. 
       for (int v = 0; v < V; v++) 
   
         // Update dist[v] only if is not in sptSet, there is an edge from  
         // u to v, and total weight of path from src to  v through u is  
         // smaller than current value of dist[v] 
         if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX  
                                       && dist[u]+graph[u][v] < dist[v]) 
            dist[v] = dist[u] + graph[u][v]; 
     } 
   
     // print the constructed distance array 
     printSolution(dist, V); 
} 



#include<bits/stdc++.h> 
using namespace std; 
# define INF 0x3f3f3f3f 
  
// iPair ==>  Integer Pair 
typedef pair<int, int> iPair; 
  
// This class represents a directed graph using 
// adjacency list representation 
class Graph 
{ 
    int V;    // No. of vertices 
  
    // In a weighted graph, we need to store vertex 
    // and weight pair for every edge 
    list< pair<int, int> > *adj; 
  
public: 
    Graph(int V);  // Constructor 
  
    // function to add an edge to graph 
    void addEdge(int u, int v, int w); 
  
    // prints shortest path from s 
    void shortestPath(int s); 
}; 
  
// Allocates memory for adjacency list 
Graph::Graph(int V) 
{ 
    this->V = V; 
    adj = new list<iPair> [V]; 
} 
  
void Graph::addEdge(int u, int v, int w) 
{ 
    adj[u].push_back(make_pair(v, w)); 
    adj[v].push_back(make_pair(u, w)); 
} 
  
// Prints shortest paths from src to all other vertices 
void Graph::shortestPath(int src) 
{ 
    // Create a priority queue to store vertices that 
    // are being preprocessed. This is weird syntax in C++. 
    // Refer below link for details of this syntax 
    // https://www.geeksforgeeks.org/implement-min-heap-using-stl/ 
    priority_queue< iPair, vector <iPair> , greater<iPair> > pq; 
  
    // Create a vector for distances and initialize all 
    // distances as infinite (INF) 
    vector<int> dist(V, INF); 
  
    // Insert source itself in priority queue and initialize 
    // its distance as 0. 
    pq.push(make_pair(0, src)); 
    dist[src] = 0; 
  
    /* Looping till priority queue becomes empty (or all 
      distances are not finalized) */
    while (!pq.empty()) 
    { 
        // The first vertex in pair is the minimum distance 
        // vertex, extract it from priority queue. 
        // vertex label is stored in second of pair (it 
        // has to be done this way to keep the vertices 
        // sorted distance (distance must be first item 
        // in pair) 
        int u = pq.top().second; 
        pq.pop(); 
  
        // 'i' is used to get all adjacent vertices of a vertex 
        list< pair<int, int> >::iterator i; 
        for (i = adj[u].begin(); i != adj[u].end(); ++i) 
        { 
            // Get vertex label and weight of current adjacent 
            // of u. 
            int v = (*i).first; 
            int weight = (*i).second; 
  
            //  If there is shorted path to v through u. 
            if (dist[v] > dist[u] + weight) 
            { 
                // Updating distance of v 
                dist[v] = dist[u] + weight; 
                pq.push(make_pair(dist[v], v)); 
            } 
        } 
    } 
  
    // Print shortest distances stored in dist[] 
    printf("Vertex   Distance from Source\n"); 
    for (int i = 0; i < V; ++i) 
        printf("%d \t\t %d\n", i, dist[i]); 
} 
  
// Driver program to test methods of graph class 
int main() 
{ 
    // create the graph given in above fugure 
    int V = 9; 
    Graph g(V); 
  
    //  making above shown graph 
    g.addEdge(0, 1, 4); 
    g.addEdge(0, 7, 8); 
    g.addEdge(1, 2, 8); 
    g.addEdge(1, 7, 11); 
    g.addEdge(2, 3, 7); 
    g.addEdge(2, 8, 2); 
    g.addEdge(2, 5, 4); 
    g.addEdge(3, 4, 9); 
    g.addEdge(3, 5, 14); 
    g.addEdge(4, 5, 10); 
    g.addEdge(5, 6, 2); 
    g.addEdge(6, 7, 1); 
    g.addEdge(6, 8, 6); 
    g.addEdge(7, 8, 7); 
  
    g.shortestPath(0); 
  
    return 0; 
} 