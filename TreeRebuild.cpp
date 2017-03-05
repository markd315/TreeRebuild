#include <iostream>
#include <stdlib.h>
#include <vector>
#include <memory.h>
#include <cstring>
using namespace std;

struct Node{
    int left;
    int right;
    int data;
    Node(){
        left=-1;
        right=-1; //Defaults to null pointers.
        data=-1;
    }
};
struct Tree{
    int size;
    std::vector<Node> arr;
    int currentIndex;
    bool insert(int val, int rightIndex, int leftIndex){
        Node* newNode = new Node();
            newNode->right = rightIndex;
            newNode->left = leftIndex;
            newNode->data = val;
        arr[currentIndex] = *newNode;
        currentIndex++;
        size++;
        return true;
    }
    int getVectorIndex(int val){
        int l = arr.size();
        for(int i=0; i<arr.size(); i++){
            if((arr[i].data) == val)
                return i;
        }
        return -1;
    }
    bool replace(int val, int rightIndex, int leftIndex, int targetIndex){
        Node* newNode = new Node();
            newNode->right = rightIndex;
            newNode->left = leftIndex;
            newNode->data = val;
        arr[targetIndex] = *newNode;
        return true;
    }
    bool replace(Node* newNode, int targetIndex){
        arr[targetIndex] = *newNode;
        return true;
    }
    Tree(){
        currentIndex=0;
        size=0;
    }
};

void preOrder(std::vector<Node> *vec, int index){
    Node* root = &vec->at(index);
    cout<<root->data<<" ";
    if(root->left != -1)
    preOrder(vec, root->left);
    if(root->right != -1)
    preOrder(vec, root->right);
}
void inOrder(std::vector<Node> *vec, int index){
    Node* root = &vec->at(index);
    if(root->left != -1)
    inOrder(vec, root->left);
    cout<<root->data<<" ";
    if(root->right != -1)
    inOrder(vec, root->right);
}
void postOrder(std::vector<Node> *vec, int index){
    Node* root = &vec->at(index);
    if(root->left != -1)
    postOrder(vec, root->left);
    if(root->right != -1)
    postOrder(vec, root->right);
    cout<<root->data<<" ";
}
int height(Tree* tree, int rootIndex)
{
    if (rootIndex>=tree->size)
        return 0;
    if (rootIndex==-1)
        return 0;
    Node* node = &tree->arr.at(rootIndex);
    {
        int lh = height(tree, node->left);
        int rh = height(tree, node->right);
        return max(rh+1,lh+1);
    }
}

void printGivenLevel(Tree* tree, int rootIndex, int level)
{
    if (rootIndex == -1)
        return;
    Node* root = &tree->arr.at(rootIndex);
    if (level == 1)
        std::cout<<root->data<<" ";
    else if (level > 1)
    {
        printGivenLevel(tree, root->left, level-1);
        printGivenLevel(tree, root->right, level-1);
    }
}
void levelOrder(Tree* tree, int rootIndex)
{
    int h = height(tree, rootIndex);
    int i;
    for (i=1; i<=h; i++)
        printGivenLevel(tree, rootIndex, i);
}
std::vector<int> leftSubtree(std::vector<int> in, int pivotIndex){ //TODO bump these methods
    if(in.size() == 0)
        return in;
    std::vector<int> ret;

    for(int i=0; i<pivotIndex; i++){
        int temp = in[i];
        ret.push_back(temp);
    }
    return ret;
}
std::vector<int> rightSubtree(std::vector<int> in, int pivotIndex){
    if(in.size() == 0)
        return in;
    std::vector<int> ret;
    int l = in.size();
    for(int i=pivotIndex+1; i<in.size(); i++){
        int temp = in[i];
        ret.push_back(temp);
    }
    return ret;
}
bool isInArr(std::vector<int> arr, int value){ //Pass by reference bc less memory?
    if(arr.size() == 0)
        return false;
    for(int i=0; i<arr.size(); i++){
        if(arr[i] == value)
            return true;
    }
    return false;
}
int getIndex(std::vector<int> arr, int value){ //Pass by reference bc less memory?
    if(arr.size() == 0)
        return -1;
    for(int i=0; i<arr.size(); i++){
        if(arr[i] == value)
            return i;
    }
    return -1;
}
std::vector<int> trimPost(std::vector<int> in, std::vector<int> post){ //Trims output to be equal to the in elements, but in post order.
    if(in.size() == 0)
        return in;
    std::vector<int> trimmedPost = *(new vector<int>(in.size()));
    int j =0;
    for(int i=0; i<post.size(); i++){
        if(isInArr(in, post[i])){
            trimmedPost[j] = post[i];
            j++;
        }
    }
    return trimmedPost;
}
int tweakChildren(std::vector<int> in, std::vector<int> post, Tree* tree, int rootIndex){ //Returns ROOT index.
int pivotValue = post[post.size() -1];
int pivotIndex = tree->getVectorIndex(pivotValue);
Node* updatedNode= &(tree->arr.at(pivotIndex));
   updatedNode->data = pivotValue;
   updatedNode->left = -1;
   updatedNode->right = -1;
if(in.size() == 1){//Base case
    tree->replace(updatedNode, pivotIndex);
    return rootIndex;
}
int inorderIndex = getIndex(in, pivotValue);
std::vector<int> left = leftSubtree(in, inorderIndex);
if(left.size()>0){
    std::vector<int> leftPost = trimPost(left, post);
    int leftChild= leftPost[leftPost.size()-1];
    updatedNode->left = tree->getVectorIndex(leftChild);
    tweakChildren(left, leftPost, tree, rootIndex);
}
std::vector<int> right = rightSubtree(in, inorderIndex);
if(right.size()>0){ //Precondition, right is RIGHT, just not rightPost.
    std::vector<int> rightPost = trimPost(right, post);//Error here, gives zero instead of 8 in base case.
    int rightChild = rightPost[rightPost.size() -1];
    updatedNode->right = tree->getVectorIndex(rightChild);
    int sizePost = rightPost.size();
    int sizeIn = right.size();
    tweakChildren(right, rightPost, tree, rootIndex);
}
//delete &in;
//delete &post;
if(rootIndex = -1){ //In the initial case.
    rootIndex = pivotIndex;
}
tree->replace(updatedNode, pivotIndex); //Update pointer
return rootIndex;
}
int main(){
ios::sync_with_stdio(false); //faster is better.
int nodeCount;
std::cin>>nodeCount; //Elements of tree
Tree* tree = new Tree();
    tree->size = nodeCount;
    std::vector<Node>* arrz = new vector<Node>(nodeCount);
    tree->arr = *arrz;
std::vector<int> post = *(new vector<int>(nodeCount));
std::vector<int> in = *(new vector<int>(nodeCount));
int i=0;
for(i=0; i<nodeCount; i++){ //Populate the traversal
    std::cin>>post[i];
}
for(i=0; i<nodeCount; i++){ //Populate the traversal
    std::cin>>in[i];
}
if(nodeCount == 0){
//Just exit
}else if(nodeCount == 1){
    cout<<in[0];
}
else{
for(i=0; i<nodeCount; i++){
    tree->insert(in[i],-1,-1);
}
int rootIndex = tweakChildren(in, post, tree, -1);
levelOrder(tree, rootIndex);
}
}
