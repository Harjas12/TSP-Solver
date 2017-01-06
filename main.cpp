#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <cstdlib>

using namespace std;

struct node
{
  double xcord;
  double ycord;
};
struct path
{
  vector<node> path;
  double distance;
};
double calcDistance(double x, double y, double nx, double ny)
{
  return sqrt(pow(nx-x,2) + pow(ny-y,2));
}
void setDistance(path& p)
{
  vector<node> path = p.path;
  double distance = 0;
  int i;
  for(i = 0; i < path.size() - 1; i++)
  {
    distance += calcDistance(path[i].xcord, path[i].ycord, path[i+1].xcord, path[i+1].ycord);
  }
  p.distance = distance;
}
void randomizeCords(vector<node>& cordinates)
{
  int i;
  for(i = 0; i < cordinates.size(); i++)
  {
    int n1 = rand() % cordinates.size();
    int n2 = rand() % cordinates.size();
    node temp = cordinates[n2];
    cordinates[n2] = cordinates[n1];
    cordinates[n1] = temp;
  }
}
void createPopulation(vector<path>& population, vector<node>& cordinates, int popSize)
{
  int i;
  for(i = 0; i < popSize; i++)
  {
    path p;
    p.path = cordinates;
    setDistance(p);
    population.push_back(p);
    randomizeCords(cordinates);
  }
}
void seedCordinates(vector<node>& cordinates)
{
    int num = rand() % 10;
    int i;
    for(i = 0; i < num; i++)
    {
      node n;
      n.xcord = rand() % 100;
      n.ycord = rand() % 100;
      cordinates.push_back(n);
    }
}
void sort(vector<path>& population, int left, int right)
{
  int i = left, j = right;
  double temp;
  double pivot = population[(left + right)/2].distance;
  while(i <= j)
  {
    while(population[i].distance < pivot)
      i++;
    while(population[j].distance > pivot)
      j--;
    if(i <= j)
    {
      temp = population[i].distance;
      population[i].distance = population[j].distance;
      population[j].distance = temp;
      i++;
      j--;
    }
  }
  if(left < j)
    sort(population, left, j);
  if(i < right)
    sort(population, i, right);
}
void kill(vector<path>& population, int popSize)
{
  population.erase(population.begin() + ((popSize/2)-1), population.begin() + (popSize-1));
}
path combine(path p1, path p2)
{
  path p;
  vector<node> path;
  int r1 = rand()%p1.path.size();
  int r2 = rand()%p2.path.size();
  int i;
  for(i = 0; i < p1.path.size(); i++)
  {
    if(i < r1 || i > r2)
      path.push_back(p1.path[i]);
    else
      path.push_back(p2.path[i]);
  }
  p.path = path;
  setDistance(p);
  return p;
}
void reproduce(vector<path>& population)
{
  int initialSize = population.size();
  int i;
  for(i = 0; i < initialSize - 1; i++)
  {
    population.push_back(combine(population[i], population[i + 1]));
  }
  population.push_back(combine(population[0], population[1]));
}
void mutate(path& p)
{
  int i;
  for(i = 0; i < p.path.size()/2; i++)
  {
    int r1 = rand()%p.path.size();
    int r2 = rand()%p.path.size();
    if(r1 == 0)
      r1 = rand()%p.path.size();
    node temp = p.path[r1];
    p.path[r1] = p.path[r2];
    p.path[r2] = temp;
  }
  setDistance(p);
}
void pickMutate(vector<path>& population)
{
  int i;
  for(i = 0; i < 25; i++)
  {
    mutate(population[rand()%population.size()]);
  }
}
void loadCordinates(vector<node>& cordinates)
{
  ifstream in;
  in.open("data.txt");
  int num;
  in >> num;
  int i;
  for(i = 0; i < num; i++)
  {
    double xcord;
    double ycord;
    in >> xcord;
    in >> ycord;
    node n;
    n.xcord = xcord;
    n.ycord = ycord;
    cordinates.push_back(n);
  }
}
/*
void *run(void *cords)
{
  vector<node> cordinates;
  cordinates = (vector<node>)cords;
  vector<path> population;
  createPopulation(population, cordinates);
  path smallest;
  int i;
  for(i = 0; i < 100; i++)
  {
    sort(population, 0, population.size() - 1);
    smallest = population[0];
    kill(population);
    reproduce(population);
    pickMutate(population);
  }
  sort(population, 0 , population.size() - 1);c
}
*/
path getShortestPath(vector<path> population)
{
  path smallest = population[0];
  int i;
  for(i = 1; i < population.size(); i++)
  {
    if(smallest.distance > population[i].distance)
      smallest = population[i];
  }
  return smallest;
}
void runSequence()
{
  vector<path> population;
  vector<node> cordinates;
  srand(time(NULL));
  loadCordinates(cordinates);
  cout << "Coordinates Loaded" << endl;
  int popSize;
  cout << "Enter population size: ";
  cin >> popSize;
  createPopulation(population, cordinates, popSize);
  cout << "Population Created" << endl;;
  int genNum;
  cout << "Number of generations: ";
  cin >> genNum;
  int i;
  for(i = 0; i < genNum; i++)
  {
    sort(population, 0, population.size() - 1);
    kill(population, popSize);
    reproduce(population);
    pickMutate(population);
    cout << "\rNumber of generations completed: " << (i+1) << flush;
  }
  cout << endl;
  cout << getShortestPath(population).distance << endl;
}
int main()
{
  runSequence();
  return 0;
  /*
  vector<path> population;
  vector<node> cordinates;
  srand(time(NULL));
//  seedCordinates(cordinates);
  loadCordinates(cordinates);
  cout << "Coordinates Loaded" << endl;
  int popSize;
  cout << "Enter population size: ";
  cin >> popSize;
  createPopulation(population, cordinates, popSize);
  cout << "Population Created" << endl;
  int genNum;
  cout << "Number of generations: ";
  cin >> genNum;
  int i;
  for(i = 0; i < genNum; i++)
  {
//    cout << "Starting generation" << endl;
    sort(population, 0, population.size() - 1);
  //  cout << "sort complete" << endl;
    kill(population, popSize);
  //  cout << "kill complete" << endl;
    reproduce(population);
  //  cout << "reproduce complete" << endl;
    pickMutate(population);
  //  cout << "mutate complete" << endl;
  //  cout << "generation complete" << endl;
  //  cout << "====================" << endl;
  cout << "\rNumber of generations completed: " << (i+1) << flush;
  }
  cout << endl;
  cout << getShortestPath(population).distance << endl;
  */
}
