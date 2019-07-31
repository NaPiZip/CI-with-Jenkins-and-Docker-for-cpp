
#ifndef _HEADER_WORKSPACE
#define _HEADER_WORKSPACE
#include "gmock/gmock.h"
#include <iostream>
#include <limits>
#include <unordered_set>
#include <stack>
#include <queue>
#include <list>

#define DEBUG

#ifdef DEBUG
    #define DEBUG_PRINT(expression) {std::cout << expression << std::endl;}
#else
    #define DEBUG_PRINT(expression)
#endif

int partition(std::string &str, int start, int end);
void swap(char & a, char &b);
void sort(std::string &str, int start, int end);
bool is_rotation(std::string a, std::string b);

struct sNode{
  int data;
  sNode* next = nullptr;
  sNode(int value):data(value){};
};

void remove_duplicate(sNode* root);
void remove_duplicate_inplace(sNode *root);
void print_list(sNode *root);

int count_elements(sNode *root);
int find_nth_element_from_end(sNode *root, int nth);

void delete_node(sNode* &node);
sNode* add_two_linked_lists(sNode* listA, sNode* listB);

struct sItem{
  int data,
      min;
  sItem* next = nullptr;
  sItem(int value):data(value){};
};

class cStack{
  protected:
    sItem* top = nullptr;
  public:
    void push(int value);
    int pop(void);
    sItem front(void);
    int min(void);
};

class cMyQueue{
  std::stack<int> enqueStack,
                  dequeStack;
  public:
    void enque(int value);
    int deque();
};

std::stack<int> stack_sort(std::stack<int> stack);

struct sTreeNode {
  int data;
  sTreeNode *left = nullptr,
            *right= nullptr;
  sTreeNode(int value):data(value){}
};

int count_depth(sTreeNode * branch);
bool is_balanced(sTreeNode * root);

bool path_exists(std::vector<std::vector<int>> mat, int start, int end);
bool path_exists(int mat[][5] );
void tree_to_linked_list(sTreeNode* node, size_t start, std::vector<std::list<int>> &out);
std::vector<int> get_path(sTreeNode* tree, sTreeNode* node, bool &found);

class cCard{
public:
  cCard();
protected:
  unsigned int symbol = 0;
  unsigned int value = 0;
  unsigned int color = 0;
};

class cPokerDeck{
  cPokerDeck(){}
protected:
  enum value{ ace, two, three, four,
              five, six, seven, eight, nine,
              ten, boy, quen, king};

  enum color: char { red = 'r', black = 'b'};
  //enum symbol: unsigned char{ spade = '♠', hart = '♥', club = '♣', dimond = '♦'};
  constexpr static unsigned int numberOfCards = 52;
  std::vector<cCard> cards;
};

class cGraph
{
  std::list<std::pair<int,int>> *adj;
  unsigned int nVertices;
public:
  cGraph(unsigned int n): nVertices{n}
  {
    adj = new std::list<std::pair<int,int>>[n];
  }
  bool add_connection(unsigned int nodeA, unsigned int nodeB, unsigned int weight)
  {

    struct match
    {
      std::pair<int,int> value;
      match(std::pair<int,int> v):value{v}{}
      bool operator() (std::pair<int,int> const &ref)
      {
        return ((ref.first == value.first) && (ref.second == value.second))?true:false;
      }
    };
    bool success = false;
    auto itA = std::find_if(adj[nodeA].begin(), adj[nodeA].end(), match(std::make_pair(nodeB,weight)));
    auto itB = std::find_if(adj[nodeB].begin(), adj[nodeB].end(), match(std::make_pair(nodeA,weight)));

    if( itA == adj[nodeA].end() )
    {
      adj[nodeA].emplace_back(nodeB, weight);
      success = true;
    }
    if( itB == adj[nodeB].end() )
    {
      adj[nodeB].emplace_back(nodeA, weight);
      success = true;
    }
    return success;
  }

  find_shortest_path(int start){
    std::vector<unsigned int> distance(nVertices, std::numeric_limits<unsigned int>::max());
    std::vector<int> parent(nVertices, -1);

    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> pq;
    pq.push(std::make_pair(start,0));
    distance[start] = 0;

    while(!pq.empty())
    {
      auto currentNode = pq.top().first;
      pq.pop();
      DEBUG_PRINT("Processing Node: " << currentNode)

      for(auto const & edge_pair : adj[currentNode])
      {

        unsigned int newDistance = edge_pair.second + distance[currentNode];
        if(newDistance < distance[edge_pair.first])
        {
          distance[edge_pair.first] = newDistance;
          parent[edge_pair.first] = currentNode;
          pq.push(edge_pair);
          DEBUG_PRINT("From " << currentNode << " to " << edge_pair.first << " w " << edge_pair.second);
        }
      }
    }

  for(auto i = 0; i< nVertices; i++)
  {
    std::cout << "Vertex: " << i << " Parent: " << parent[i] << " Distance: " << distance[i] << std::endl;
  }
}

};
#endif  /*_HEADER_WORKSPACE*/
