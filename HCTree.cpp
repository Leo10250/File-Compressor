#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <queue>
#include "HCTree.hpp"
using namespace std;

void HCTree::recursiveClear(HCNode* node){
    if(node == nullptr){
        return;
    }

    recursiveClear(node->c0);
    recursiveClear(node->c1);

    delete node;
}

HCNode* HCTree::getRoot(){
    return root;
}

HCTree::~HCTree(){
    leaves.clear();
    recursiveClear(root);
}

void HCTree::build(const vector<int>& freqs){
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;
    for(long unsigned int i = 0; i < freqs.size(); i++){
        if(freqs[i] == 0){
            continue;
        }
        unsigned char c = i;
        HCNode* node = new HCNode(freqs[i], c);
        pq.push(node);
    }

    while(!pq.empty()){
        // cout << pq.top()->symbol << endl;
        // pq.pop();
        if(pq.size() == 1){
            root = new HCNode(pq.top()->count, pq.top()->symbol);
            leaves[(int)pq.top()->symbol] = root;
            return;
        }

        HCNode* left = pq.top();
        if(left->c0 == nullptr && left->c1 == nullptr){
            leaves[(int)left->symbol] = left;
            // cout << "left leave" << endl;
        }
        pq.pop();

        HCNode* right = pq.top();
        if(right->c0 == nullptr && right->c1 == nullptr){
            leaves[(int)right->symbol] = right;
            // cout << "right leave" << endl;
        }
        HCNode* node = new HCNode(left->count + right->count, left->symbol);

        node->c0 = left;
        node->c1 = right;
        left->p = node;
        right->p = node;

        // cout << "Size before pop = " << pq.size() << endl;
        pq.pop();
        pq.push(node);
        if(pq.size() == 1){
            root = node;
            return;
        }
    }
}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const{
    HCNode* current = leaves[(int)symbol];
    vector<int> vec;
    while(current != root){
        HCNode* child = current;
        current = current->p;

        if(current->c0 == child){
            vec.push_back(0);
        }   
        else if(current->c1 == child){
            vec.push_back(1);
        }
    }

    for(int i = vec.size() - 1; i >= 0; i--){
        out.write_bit(vec[i]);
    }
}


unsigned char HCTree::decode(HCNode* current, FancyInputStream & in) const{
    while(current != nullptr){
        if(current->c0 == nullptr && current->c1 == nullptr){
            return current->symbol;
        }
        if(in.read_bit() == 0){
            current = current->c0;
        }
        else{
            current = current->c1;
        }
    }
    return current->symbol;
}
