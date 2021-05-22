#include<bits/stdc++.h>
using namespace std;

#define INF 0x7fffffff;

class Graph{
private:
  int n;
  vector<unordered_map<int,double> > adj; //Adjacency list
  vector<set<pair<double,int> > > inAdj; //Edges between the vertices in tour and vertices not in tour 
  vector<int> tourPos; 
  vector<int> tourV; //Vertices in the tour

  int selectV(){ //Returns i such that dist(i,j) for all j in tour is minimum
      double min_dist = INF;
      int minVertex = -1;

      for(int i=0;i<n;i++){
          if(tourPos[i] != -1)
          {
              if(inAdj[i].size() && inAdj[i].begin()->first < min_dist){
                  min_dist = inAdj[i].begin()->first;
                  minVertex = inAdj[i].begin()->second;
              }
          }
      }
      assert(minVertex != -1);
      return minVertex;
  }
  double dist(int i,int j){
      if(i == j)  return 0;
      return adj[i][j];
  }
  int insertV(int u){ //Inserts vertex u to the tour such the increase in cost is minimum
    int l = tourV.size();
    int pos = -1;
    double min_dist = INF;

    for(int i=0;i<l-1;i++){
      if(min_dist > dist(tourV[i], u) + dist(u, tourV[i+1]) - dist(tourV[i], tourV[i+1])){
          min_dist  = dist(tourV[i], u) + dist(u, tourV[i+1]) - dist(tourV[i], tourV[i+1]);
          pos = i+1;
      }
    }

    if(min_dist > dist(tourV[l-1], u) + dist(u, tourV[0]) - dist(tourV[l-1], tourV[0])){
        min_dist = dist(tourV[l-1], u) + dist(u, tourV[0]) - dist(tourV[l-1], tourV[0]);
        pos = 0;
    }
    return pos;
  }

  void moveTo(int v, int pos) //Inserting vertex v to tourSet at position pos
  {
    for(int i=0;i<n;i++){
        if(tourPos[i] >= pos) tourPos[i]++;
    }
    tourPos[v] = pos;
    tourV.insert(tourV.begin() + pos, v);

    for(auto pr:adj[v]){
        if(tourPos[pr.first] == -1)
          inAdj[v].insert(make_pair(pr.second, pr.first)) ;
        else
          inAdj[pr.first].erase(make_pair(pr.second, v));
    }
  }

public:

  Graph(int n){
    this->n = n;

    adj = vector<unordered_map<int,double> >(n, unordered_map<int,double>());
    inAdj = vector<set<pair<double,int>> >(n, set<pair<double,int>>());
    tourPos = vector<int>(n,-1);
    tourV = vector<int>();
  }

  int addEdge(int i,int j,double w){
    adj[i][j] = w;
    adj[j][i] = w;
  }

  int calcTour(){
      moveTo(0,0); //Initialization
      for(int i=1;i<n;i++)
      {
          int u = selectV(); //Selection step
          moveTo(u, insertV(u)); //Insertion step
      }
  }

  void printPath()
  {
    cout<<"TSP tour: ";
    for(int i:tourV){
       cout<<i<<' ';
    }
    cout<<endl;
  }
  double getDistance()
  {
    double distance = 0;
    for(int i=0;i<n-1;i++)
      distance += dist(tourV[i],tourV[i+1]);

    distance += dist(tourV[0],tourV[n-1]);
    return distance;
  }
};

double dist_l2(double x1, double y1, double x2, double y2){
    return sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

int main()
{
    int n,a;
    cin>>n;
    
    double x[n], y[n];
    for(int i=0;i<n;i++)
      cin>>a>>x[i]>>y[i];

    Graph g(n);

    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            g.addEdge(i,j, dist_l2(x[i],y[i],x[j],y[j]));
        }
    }

    g.calcTour();
    cout<< g.getDistance();
    
    return 0;
}
