#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>


#include<bits/stdc++.h>
#define INF 0x7fffffff;
#include <fstream>

using namespace std;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
float dist(pair<double,double> a,pair<double,double> b)
{
	return sqrt((a.first-b.first)*(a.first-b.first)+(a.second-b.second)*(a.second-b.second));
}
class tsp_convexhull_heuristic
{

public:
  vector<int> tour;
  double tour_cost;
  tsp_convexhull_heuristic(int n,double* input_x,double* input_y)
  {
      vector<pair<double,double>> temp_tour;
      vector<pair<double,double>> all_points;
      Point_2 input[n];
      for(int i=0;i<n;i++)
      {
        double a,b;
        all_points.push_back(make_pair(input_x[i],input_y[i]));
        input[i]=Point_2(input_x[i],input_y[i]);
      }
      Point_2 result[n];
    Point_2 *ptr = CGAL::convex_hull_2( input, input+n, result );
    cout<<endl;
    std::cout <<  ptr - result << " points on the convex hull and they are" << std::endl;

    for(int i = 0; i < ptr - result; i++){
      cout<<result[i].x()<<","<<result[i].y()<<endl;
      auto p=make_pair(result[i].x(),result[i].y());
      temp_tour.push_back(p);
      all_points.erase(find(all_points.begin(), all_points.end(),p)) ;
    }
    int cover=all_points.size();
    for(int h=0;h<cover;h++)
    {
      int index_j=-1;
      int index_i=-1;
      int min_d=INF;
      for(int j=0;j<all_points.size();j++)
      {
        for(int i=0;i<temp_tour.size();i++)
        {
          int h=dist(all_points[j],temp_tour[i])+dist(all_points[j],temp_tour[(i+1)%temp_tour.size()])-dist(temp_tour[i],temp_tour[(i+1)%temp_tour.size()]);
          if(h<min_d)
          {
            min_d=h;
            index_i=i;
            index_j=j;
          }
        }
      }
      temp_tour.insert(temp_tour.begin() + (index_i+1)%temp_tour.size(), all_points[index_j]);
      all_points.erase(all_points.begin()+index_j);
    }  
    cout<<endl<<"the tour is:"<<endl;
     for(int i=0;i<temp_tour.size();i++)
     {
      cout<<temp_tour[i].first<<" "<<temp_tour[i].second<<endl;

     }
     double d=0;
     for(int i=0;i<temp_tour.size();i++)
     {
      d=d+dist(temp_tour[i],temp_tour[(i+1)%temp_tour.size()]);
     }
     tour_cost=d;
    }
 
};
int main(int argc, char** argv)
{
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);

  int n;
  cin >> n;

  double x[n], y[n];
  double MAX = -1.0;
  int id;
  for(u_int i = 0; i < n; i++) {
    cin>>id>> x[i] >> y[i];
    MAX = max(abs(x[i]), MAX);
    MAX = max(abs(y[i]), MAX);
  }

  string type = string(argv[1]);
  double sigma = atof(argv[2]);
  u_int numOuts = atoi(argv[3]);
  cout<< type<<sigma<<numOuts<<endl;

  double sum = 0.0;
  double values[numOuts];
  if(string(type) == "uniform") {
    uniform_real_distribution<double> distribution(-sigma, sigma);

    for(u_int j = 0; j < numOuts; j++) {
      double out_x[n], out_y[n];
      for(u_int i = 0; i < n; i++) {
        out_x[i] = x[i] / MAX + distribution(generator);
        out_y[i] = y[i] / MAX + distribution(generator);
      }

      tsp_convexhull_heuristic tsp(n,out_x, out_y);
      sum += tsp.tour_cost;
      double cost = tsp.tour_cost * MAX;
      sum += cost;
      values[j] = cost;
    }
  }
  else {
    normal_distribution<double> distribution(0.0, sigma);
    for(u_int j = 0; j < numOuts; j++) {
      double out_x[n], out_y[n];
      for(u_int i = 0; i < n; i++) {
        out_x[i] = x[i] / MAX + distribution(generator);
        out_y[i] = y[i] / MAX + distribution(generator);
      } 
      tsp_convexhull_heuristic tsp(n,out_x, out_y);
      sum += tsp.tour_cost;
      double cost = tsp.tour_cost * MAX;
      sum += cost;
      values[j] = cost;
    }
  }

  double variance = 0.0;
  double average = sum / numOuts;
  average *= MAX;
  for(u_int i = 0; i < numOuts; i++) {
    variance += (values[i] - average) * (values[i] - average);
  }
  variance /= numOuts;

  cout << "Scaling factor: " << MAX << endl;
  cout << "Perturbed tour cost: " << average << endl;
  cout << "Variance: " << variance << endl;

  return 0;
}
