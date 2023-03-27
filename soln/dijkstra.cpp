#include <iostream>
#include <string>
#include <unordered_set>
#include <queue>
#include "digraph.cpp"
#include "wdigraph.h"
#include <fstream>
#include <list>
#include "dijkstra.h"
using namespace std;

 void dijkstra(const WDigraph& graph, int startVertex, unordered_map<int, PIL>& tree) {
    
    // each active fire is stored as (d, (u, v)) 
    // which implies that it is a fire started at u
    // currently burning the (u,v) edge 
    // and will reach v at time d
    priority_queue< realPIPIL, std::vector<realPIPIL>, std::greater<realPIPIL>> realfires;
    // at time 0, the startVertex burns, we set the predecesor of
    // startVertex to startVertex (as it is the first vertex)
    realfires.push(realPIPIL(0, PIL(startVertex, startVertex)));

    // while there is an active fire
    while (!realfires.empty()) {
        // finding the fire that reaches its endpoint earliest
        realPIPIL earliestFire = realfires.top();

        int v = earliestFire.second.second; 
        int u = earliestFire.second.first; 
        long long d = earliestFire.first;
        realfires.pop();
        // if v is already "burned", there nothing to do
        if (tree.find(v) != tree.end()) {
            continue;
        }

        // record that 'v' is burned at time 'd' by a fire started from 'u'
        tree[v] = PIL(u, d);

        // now start fires from all edges exiting vertex 'v'
        for (auto iter = graph.neighbours(v); iter != graph.endIterator(v); iter++) {
            int nbr = *iter;

            // 'v' catches on fire at time 'd' and the fire will reach 'nbr'
            // at time d + (length of v->nbr edge)
            long long t_burn = d + graph.getCost(v, nbr);
            realfires.push(realPIPIL(t_burn, realPIL(v, nbr)));
        }
    }
}
