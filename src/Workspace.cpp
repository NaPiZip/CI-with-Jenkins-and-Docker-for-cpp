#include "Workspace.hpp"
#include "gtest/gtest.h"

using ::testing::AtLeast;

void swap(char & a, char &b){
  char tmp = a;
  a = b;
  b = tmp;
}

TEST(Swap, singleTest){
  char a = 'a',
       b = 'b';
  swap(a,b);
  EXPECT_EQ(a, 'b');
  EXPECT_EQ(b, 'a');
}

int partition(std::string &str, int start, int end){
 int p   = end,
    min  = start;
 for(auto i = start; i<= end; i++){
   if(str[i]<str[p])
     swap(str[i],str[min++]);
 }
 swap(str[p],str[min]);
 return min;
}

TEST(Partition, singleTest){
  std::string a = "acdb";
  EXPECT_EQ(partition(a,0, a.size()-1),1);
}

void sort(std::string &str, int start, int end){
  if(start > end)
    return;
  int p = partition(str, start, end);
  sort(str, start, p-1);
  sort(str, p+1, end);
}

TEST(Sort, singleTest){
  std::string str = "dcba";
  sort(str,0,str.size()-1);
  EXPECT_EQ(str, "abcd");
}

bool is_rotation(std::string a, std::string b){
  sort(a, 0, a.size()-1);
  sort(b, 0, b.size()-1);

  std::size_t found  = a.find(b);
  return (found == std::string::npos)?false:true;
}

TEST(ISRotation, multiTest){
  EXPECT_EQ(is_rotation("Nawin","winNa"),true);
  EXPECT_EQ(is_rotation("Nawin","inNaw"),true);
  EXPECT_EQ(is_rotation("Nawin","nNawi"),true);
  EXPECT_EQ(is_rotation("Nawin","asd"),false);
  EXPECT_EQ(is_rotation("Nawin","Naw"),false);
}

void remove_duplicate(sNode* root){
  if(root == nullptr)
    return;
  std::unordered_set<int> values;
  sNode* current = root,
       * next    = root->next;
  while(next!=nullptr){
    values.insert(current->data);
    if(values.find(next->data)!=values.end()){
      current->next = next->next;
      delete next;
      next = current->next;
    }else{
      current = next;
      next = next->next;
    }
  }
}

void print_list(sNode *root){
  if(root  == nullptr)
    return;
  while(root!= nullptr){
    std::cout<< root->data << ' ';
    root = root->next;
  }
  std::cout << std::endl;
}
TEST(RemoveDuplicate, multiTest){
  sNode * root = new sNode(5);
  root->next = new sNode(5);

  remove_duplicate(root);
  EXPECT_EQ(root->data, 5);
  EXPECT_EQ(root->next, nullptr);

  root->next = new sNode(4);
  root->next->next = new sNode(5);
  remove_duplicate(root);

  EXPECT_EQ(root->data, 5);
  EXPECT_EQ(root->next->data, 4);
  EXPECT_EQ(root->next->next, nullptr);

  root->next->next = new sNode(3);
  root->next->next->next = new sNode(5);
  root->next->next->next->next = new sNode(3);
  print_list(root);
  remove_duplicate(root);

  EXPECT_EQ(root->data, 5);
  EXPECT_EQ(root->next->data, 4);
  EXPECT_EQ(root->next->next->data, 3);
  EXPECT_EQ(root->next->next->next, nullptr);
}

void remove_duplicate_inplace(sNode *root){
  if(root == nullptr)
    return;

  sNode* current = nullptr,
       * next    = nullptr;

  while(root != nullptr){
    current = root;
    next = root->next;
    while (next != nullptr) {
      if(next->data == root->data){
        current->next = next->next;
        delete next;
        next = current->next;
      }else{
        current = next;
        next = next->next;
      }
    }
    root = root->next;
  }
}
TEST(RemoveDuplicateInplace, multiTest){
  sNode * root = new sNode(5);
  root->next = new sNode(5);

  remove_duplicate_inplace(root);
  EXPECT_EQ(root->data, 5);
  EXPECT_EQ(root->next, nullptr);

  root->next = new sNode(4);
  root->next->next = new sNode(5);
  remove_duplicate_inplace(root);

  EXPECT_EQ(root->data, 5);
  EXPECT_EQ(root->next->data, 4);
  EXPECT_EQ(root->next->next, nullptr);

  root->next->next = new sNode(3);
  root->next->next->next = new sNode(5);
  root->next->next->next->next = new sNode(3);
  print_list(root);
  remove_duplicate_inplace(root);

  EXPECT_EQ(root->data, 5);
  EXPECT_EQ(root->next->data, 4);
  EXPECT_EQ(root->next->next->data, 3);
  EXPECT_EQ(root->next->next->next, nullptr);
}

int count_elements(sNode *root){
  int nElements = 0;
  while(root != nullptr){
    root = root->next;
    nElements++;
  }
  return nElements;
}
TEST(CountElements, multiTest){
  sNode* root = nullptr;
  EXPECT_EQ(count_elements(root),0);

  root = new sNode(2);
  EXPECT_EQ(count_elements(root),1);

  root->next = new sNode(3);
  EXPECT_EQ(count_elements(root),2);

  root->next->next = new sNode(4);
  EXPECT_EQ(count_elements(root),3);

  root->next->next->next = new sNode(5);
  EXPECT_EQ(count_elements(root),4);

  print_list(root);
}

int find_nth_element_from_end(sNode *root, int nth){
  if(root == nullptr)
    return -1;
  int n = count_elements(root),
      nElementsToNth = n - 1 - nth;
  if(nElementsToNth <0)
    return -1;
  while(nElementsToNth > 0){
    root= root->next;
    nElementsToNth--;
  }
  return root->data;
}

TEST(FindNthElementFromEnd, singleTest){
  sNode* root = new sNode(1);
  root->next = new sNode(2);
  root->next->next = new sNode(3);
  root->next->next->next = new sNode(4);
  root->next->next->next->next = new sNode(5);

  EXPECT_EQ(find_nth_element_from_end(root, 1),4);
  EXPECT_EQ(find_nth_element_from_end(root, 2),3);
  EXPECT_EQ(find_nth_element_from_end(root, 3),2);
  EXPECT_EQ(find_nth_element_from_end(root, 4),1);
  EXPECT_EQ(find_nth_element_from_end(root, 5),-1);
}

void delete_node(sNode* &node){
  if(node == nullptr)
    return;
  if(node->next == nullptr)
    return;
  sNode* next = node->next;
  node->data = next->data;
  node->next = next->next;
  delete next;
}

TEST(DeleteNode, multiTest){
  sNode* root = new sNode(1);
  root->next = new sNode(2);
  root->next->next = new sNode(3);
  root->next->next->next = new sNode(4);
  root->next->next->next->next = new sNode(5);

  delete_node(root->next);
  EXPECT_EQ(root->data,1);
  EXPECT_EQ(root->next->data,3);
  EXPECT_EQ(root->next->next->data, 4);
  EXPECT_EQ(root->next->next->next->data,5);
}


sNode* add_two_linked_lists(sNode* listA, sNode* listB){
  if((listA == nullptr) &&
     (listB == nullptr))
     return nullptr;
  int carryFlag = 0;

  sNode* result = nullptr,
       * currentNode = nullptr;
  while(listA != nullptr || listB != nullptr){
    sNode* node = new sNode(0);
    node->data = carryFlag;

    if(listA != nullptr){
      node->data += listA->data;
      listA = listA->next;
    }
    if(listB != nullptr){
      node->data += listB->data;
      listB = listB->next;
    }
    carryFlag = node->data /10;
    node->data = node->data%10;
    if(result == nullptr){
      result = node;
      currentNode = node;
    }else{
      currentNode->next = node;
      currentNode = node;
    }
  }
  if(carryFlag == 1){
    currentNode->next = new sNode(1);
  }
  return result;
}

TEST(AddTowLinkedLists, multiTest){
  sNode * listA = nullptr,
        * listB = nullptr,
        * result = nullptr;
  result = add_two_linked_lists(listA, listB);
  EXPECT_EQ(result, nullptr);

  listA = new sNode(2);
  result = add_two_linked_lists(listA, listB);
  EXPECT_EQ((result != nullptr), true);
  EXPECT_EQ(result->data,2);

  delete result;
  delete listA;
  listA = new sNode(9);
  listB = new sNode(2);

  result = add_two_linked_lists(listA, listB);
  EXPECT_EQ(result->data, 1);
  EXPECT_EQ(result->next->data,1);

  delete result->next;
  delete result;
  delete listA;
  delete listB;
  listA = new sNode(3);
  listA->next = new sNode(1);
  listA->next->next = new sNode(5);
  listB = new sNode(5);
  listB->next = new sNode(9);
  listB->next->next = new sNode(2);
  result = add_two_linked_lists(listA, listB);
  EXPECT_EQ((result!=nullptr), true);
  EXPECT_EQ(result->data, 8);
  EXPECT_EQ(result->next->data, 0);
  EXPECT_EQ(result->next->next->data, 8);
  EXPECT_EQ(result->next->next->next, nullptr);
}

void cStack::push(int value){
  if(top == nullptr){
    top = new sItem(value);
    top->min = value;
  }else{
    sItem* newItem = new sItem(value);
    if(value < top->min)
      newItem->min = value;
    else
      newItem->min = top->min;
    newItem->next = top;
    top = newItem;
  }
}

int cStack::pop(void){
  int result = -1;
  if(top != nullptr){
    result = top->data;
    sItem* previousItem = top;
    top = top->next;
    delete previousItem;
  }
  return result;
}

int cStack::min(void){
  if(top != nullptr)
  {
    return top->min;
  }
  return -1;
}

TEST(CStack, singleTest){
/*
   top          end
    6 - 5 - 1 - 8
*/

  cStack stack;
  stack.push(6);
  stack.push(5);
  stack.push(1);
  stack.push(8);
  EXPECT_EQ(stack.min(),1);
  EXPECT_EQ(stack.pop(),8);
  EXPECT_EQ(stack.min(),1);
  EXPECT_EQ(stack.pop(),1);
  EXPECT_EQ(stack.min(),5);
  EXPECT_EQ(stack.pop(),5);
  EXPECT_EQ(stack.min(),6);
  EXPECT_EQ(stack.pop(),6);
  EXPECT_EQ(stack.min(),-1);
}

sItem cStack::front(void){
  if(top !=nullptr){
    return *top;
  }
  return sItem(-1);
}

TEST(CStackReturnItem, singleTest){
  cStack stack;
  stack.push(5);
  sItem element = stack.front();

  EXPECT_EQ(element.data,5);
  EXPECT_EQ(element.min,5);
  EXPECT_EQ(element.next, nullptr);
}

void cMyQueue::enque(int value){
  while(dequeStack.size() != 0)
  {
    enqueStack.push(dequeStack.top());
    dequeStack.pop();
  }
  enqueStack.push(value);
}

int cMyQueue::deque(){
  while(enqueStack.size() != 0){
    dequeStack.push(enqueStack.top());
    enqueStack.pop();
  }
  int ret = dequeStack.top();
  dequeStack.pop();
  return ret;
}

TEST(CMyQueue, singleTest){
  cMyQueue queue;
  queue.enque(5);
  queue.enque(6);
  queue.enque(7);
  EXPECT_EQ(queue.deque(),5);
  queue.enque(8);
  EXPECT_EQ(queue.deque(),6);
  EXPECT_EQ(queue.deque(),7);
  EXPECT_EQ(queue.deque(),8);
}

std::stack<int> stack_sort(std::stack<int> stack){
  std::stack<int> buffer;
  if(stack.empty())
    return buffer;
  while(!stack.empty()){
    int top = stack.top();
    stack.pop();
    if(buffer.empty())
      buffer.push(top);
    else if(top > buffer.top())
      buffer.push(top);
    else{
      while(!buffer.empty()){
        stack.push(buffer.top());
        buffer.pop();
        if(buffer.empty())
          break;
        if(top > buffer.top())
          break;
      }
      buffer.push(top);
    }
  }
  return buffer;
}

TEST(StackSort, singleTest){
  std::stack<int> st, result;
  st.push(8);
  st.push(1);
  st.push(7);
  st.push(5);

  result = stack_sort(st);

  EXPECT_EQ(result.top(),8);
  result.pop();
  EXPECT_EQ(result.top(),7);
  result.pop();
  EXPECT_EQ(result.top(),5);
  result.pop();
  EXPECT_EQ(result.top(),1);
}

int count_depth(sTreeNode* branch){
  if(branch == nullptr)
    return 0;
  else if((branch->left == nullptr) &&
          (branch->right == nullptr))
    return 1;
  else {
    int nLeft = count_depth(branch->left),
        nRight= count_depth(branch->right);
    return (nLeft>nRight)?(++nLeft):(++nRight);
  }
}

TEST(CountDepth, multiTest){
  sTreeNode* root = new sTreeNode(5);
  root->left = new sTreeNode(3);
  root->right = new sTreeNode(8);
  root->left->right = new sTreeNode(2);
  root->left->left = new sTreeNode(7);
  root->left->left->left = new sTreeNode(1);
  EXPECT_EQ(count_depth(root->left),3);
  EXPECT_EQ(count_depth(root->right),1);
  EXPECT_EQ(count_depth(root),4);
}

bool is_balanced(sTreeNode * root){
  if(root == nullptr)
    return true;
  else{
    int nRight = count_depth(root->right),
        nLeft = count_depth(root->left);
      if((nLeft-nRight > 1) ||
         (nLeft-nRight < -1))
         return false;
      else
        return true;
  }
}

TEST(IsBalanced, multiTest){
  sTreeNode* root = new sTreeNode(5);
  root->left = new sTreeNode(3);
  root->right = new sTreeNode(8);
  root->left->right = new sTreeNode(2);
  root->left->left = new sTreeNode(7);
  root->left->left->left = new sTreeNode(1);
  EXPECT_FALSE(is_balanced(root));
  root->right->right = new sTreeNode(4);
  EXPECT_TRUE(is_balanced(root));
}
bool path_exists(std::vector<std::vector<int>> mat, int start, int end)
{
  if(!mat.size())
    return false;
  if(!mat[0].size())
    return false;

  std::vector<int> parent(mat.size(),-1);
  std::vector<bool> visited(mat.size(), false);
  std::queue<int> toProcess;
  toProcess.push(start);

  while(!toProcess.empty()){
    int currentNode = toProcess.front();
    toProcess.pop();
    visited[currentNode] = true;
    for(size_t edge = 0; edge < mat[currentNode].size(); edge++){
      if((visited[edge] == false) && (mat[currentNode][edge] == 1)){
        parent[edge] = currentNode;
        toProcess.push(edge);
      }
    }
  }
  return visited[end];
}


TEST(PathExists, singleTest){
 /*
(1) ------------>(2)<------------(0)
                  \
                  \
                  v
                  (3)
 */

  std::vector<std::vector<int>> mat = {{0,0,1,0},
                                       {0,0,1,0},
                                       {0,0,0,1},
                                       {0,0,0,0}};

EXPECT_TRUE(path_exists(mat,1,3));
EXPECT_FALSE(path_exists(mat,1,0));
}

void tree_to_linked_list(sTreeNode* node, size_t start, std::vector<std::list<int>> &out){
  if(node == nullptr)
    return;
  if(out.size() <= start){
    std::list<int> tmp;
    tmp.push_back(node->data);
    out.push_back(tmp);
  }else
    out[start].push_back(node->data);
  tree_to_linked_list(node->left, start+1, out);
  tree_to_linked_list(node->right, start+1, out);
}
TEST(TreeToLinkedList,singleTest){
 /*                 (5)
                  /    \
              (3)       (7)
              / \       /  \
            (1)       (6)  (9)
 */

  sTreeNode* root = new sTreeNode(5);
  root->left = new sTreeNode(3);
  root->left->left = new sTreeNode(1);
  root->right = new sTreeNode(7);
  root->right->left = new sTreeNode(6);
  root->right->right = new sTreeNode(9);
  std::vector<std::list<int>> result;
  tree_to_linked_list(root, 0, result);
  EXPECT_EQ(result.size(),3u);
  EXPECT_EQ(result[0].size(),1u);
  EXPECT_EQ(result[1].size(),2u);
  EXPECT_EQ(result[2].size(),3u);
  EXPECT_EQ(result[0].front(),5);
  EXPECT_EQ(result[1].front(),3);
  EXPECT_EQ(result[2].front(),1);
}
std::vector<int> get_path(sTreeNode* tree, sTreeNode* node, bool &found)
{
  if(tree == nullptr || node == nullptr)
    return{};
  if(tree == node || found == true){
    DEBUG_PRINT("---> Found");
    found = true;
    return std::vector<int>(1,tree->data);
  }
  DEBUG_PRINT("Node: " << tree->data);
  auto leftPaht = get_path(tree->left,node,found);
  auto rightPaht = get_path(tree->right,node,found);

  leftPaht.insert(leftPaht.end(),rightPaht.begin(),rightPaht.end());

  return leftPaht;
}
TEST(FindPaht, singleTest){
  /*                 (5)
                   /    \
               (3)       (7)
               / \       /  \
             (1)       (6)  (9)
  */

   sTreeNode* root = new sTreeNode(5);
   root->left = new sTreeNode(3);
   root->left->left = new sTreeNode(1);
   root->right = new sTreeNode(7);
   root->right->left = new sTreeNode(6);
   root->right->right = new sTreeNode(9);
   std::cout << (3 >> 1) << std::endl;
   //EXPECT_EQ(get_path(root, root->left->left,found),(std::vector<int>{9,7,5}));
}

TEST(cGraphTest,setup)
{
  cGraph g(5);
  EXPECT_EQ(g.add_connection(0,1,3),true);
  EXPECT_EQ(g.add_connection(0,1,3),false);
}

TEST(cGraphTest,Dijkstras_algo)
{
  cGraph g(6);
  g.add_connection(0,1,7);
  g.add_connection(0,2,1);
  g.add_connection(2,3,1);
  g.add_connection(1,3,1);
  g.add_connection(1,4,2);
  g.add_connection(4,5,5);

  g.find_shortest_path(0);

    // TODO
    // https://www.geeksforgeeks.org/a-search-algorithm/
}
