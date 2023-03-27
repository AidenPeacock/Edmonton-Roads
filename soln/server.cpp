#include <iostream>
#include <string>
#include <unordered_set>
#include <queue>
#include "wdigraph.h"
#include <fstream>
#include <list>
#include "dijkstra.cpp"
#include "dijkstra.h"
using namespace std;


struct Point {
long long lat; // latitude of the point
long long lon; // longitude of the point
};


// calculates the "manhattan distance" between 2 points, using equation abs(x1-x2)+abs(y1-y2)
long long manhattan(const Point& pt1, const Point& pt2);

// given input file, an empty graph, and an empty map, will represent the graph stored in the file
void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points);



long long manhattan(const Point& pt1, const Point& pt2) {
    long long ans;
    ans = abs(pt1.lat-pt2.lat)+abs(pt1.lon-pt2.lon);
    return ans;
}

void readGraph(string filename, WDigraph& graph, unordered_map<int, Point>& points) {
  string filetxt, partgraph, x, id1, id2, lats, longs;
  // initalizing file object for the provided file
  ifstream readfile(filename);
  int commacount = 0, count = 0, lastcom;
  // going line by line through the file
  while (getline (readfile, filetxt)) {
  while(true){
    // count is the index of the string being analyzed
    count +=1;
    x = filetxt[count];
    if (x == ","){
      // there will only ever be one letter at the start of each line before a comma, so the substr is constant
      commacount += 1;
      if (commacount == 1){
        partgraph = filetxt.substr(0,1);
        lastcom = count;
      }
      // lastcom + 1 represents the last time we detect a comma in the string, and count - lastcom -1 represents the distance to the next comma
      if (commacount == 2 && partgraph == "V"){
        id1 = filetxt.substr(lastcom+1, count-lastcom-1);
        lastcom = count;
      }
      else if(commacount == 2 && partgraph == "E"){
        id1 = filetxt.substr(lastcom+1, count-lastcom-1);
        lastcom = count;
        
      }
      // since there are 3 numbers we are interested when providing an edge we must store an extra string
      if (commacount == 3 && partgraph == "E"){
        id2 = filetxt.substr(lastcom+1,count-lastcom-1);
        break;
      }
      else if (commacount == 3 && partgraph == "V"){
        lats = filetxt.substr(lastcom+1,count-lastcom-1);
        longs = filetxt.substr(count+1);
        break;
      }
    }

  }
  if (partgraph == "V"){
    // stod turns a string into a double 
    double x = std::stod(lats);
    double y = std::stod(longs);
    Point coordinates;
    long long latitude, longitude;
    // casting type double onto long long
    latitude = static_cast<long long>(x*100000);
    longitude = static_cast<long long>(y*100000);
    coordinates.lat = latitude;
    coordinates.lon = longitude;
    // we instert the identification number and a vertex into the map, and place it in the graph
    points.insert({stoi(id1), coordinates});
    graph.addVertex(stoi(id1));
  }
  else{
    Point point1, point2;
    long long dist;
    point1 = points[stoi(id1)];
    point2 = points[stoi(id2)];
    dist = manhattan(point1, point2);
    // we store the 2 verticies the edge connects and the cost of travelling the edge
    // calulated using the manhattan distance between the two verticies connected by an edge
    graph.addEdge(stoi(id1), stoi(id2), dist);
  }

  commacount = 0;
  count = 0;
  }

readfile.close();
}




int main(){
long long x1, y1, x2, y2;
string request, type;

while (request != "R"){
cin >> request;
}
// start and end vertex
cin >> x1 >> y1 >> x2 >> y2;
int startVertex, endVertex;
WDigraph edgraph;
// map for readGraph
unordered_map<int, Point> mappy;
// map for dijkstra
unordered_map<int, PIL> mapper;
Point point1 = {x1, y1};
Point point2 = {x2, y2};
string file = "edmonton-roads-2.0.1.txt";
readGraph(file, edgraph, mappy);
int id1;
int id2;
// finding the vertex with the smallest manhattan distance from the requested starting coordinates
long long lowest = 999999;
for (auto& it: mappy) {
    long long x = manhattan(point1, it.second);
    if (x < lowest){
        lowest = x;
        startVertex = it.first; // start point identifier
    }
}
// finding the vertex with the smallest manhattan distance from the requested ending location
lowest = 999999;
for (auto& it: mappy) {
    long long x = manhattan(point2, it.second);
    if (x < lowest){
        lowest = x;
        endVertex = it.first; // end point identifier
    }

}
    dijkstra(edgraph, startVertex, mapper);

    // read off a path
    list<int> path;
    if (mapper.find(endVertex) == mapper.end()) {
      string messg;
      cout << "N 0" << endl;
      cin >> messg;
      if (messg == "A"){
        cout << "E" << endl;
      }
    }
    else {
      int stepping = endVertex;
      while (stepping != startVertex) {
        path.push_front(stepping);

        // crawl up the search tree one step
        stepping = mapper[stepping].first;
      }
      path.push_front(startVertex);
      cout << "N " << path.size() << endl;
      string message, message2;
      
      cin >> message2;
      for (auto it : path) {
        cout << "W " << mappy[it].lat << ' ' << mappy[it].lon << endl;
        cin >> message;
      }
      cout << "E" << endl;

    }







}
