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
int main()
{
  int n;
  vector<pair<double,double>> temp_tour;
  vector<pair<double,double>> all_points;
  cin>>n;
  Point_2 input[n];
  for(int i=0;i<n;i++)
  {
      double a,b;
      std::cin>>a>>b;
      all_points.push_back(make_pair(a,b));
      input[i]=Point_2(a,b);
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
 cout<<endl<<"total travel distance is "<<d<<endl;

  return 0;
}