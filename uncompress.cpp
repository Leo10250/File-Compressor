#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include "Helper.hpp"
#include "HCTree.hpp"
using namespace std;

int sum = 0;

void recursiveClear(HCNode* node){
    if(node == nullptr){
        return;
    }

    recursiveClear(node->c0);
    recursiveClear(node->c1);

    delete node;
}

HCNode* readHeader(FancyInputStream & in){
	// cout << in.read_bit() << endl;
	// cout << in.read_bit() << endl;
	// cout << in.read_byte() << endl;
	// cout << in.read_bit() << endl;
	// cout << in.read_bit() << endl;
	// cout << in.read_bit() << endl;
	// cout << in.read_bit() << endl;
	
	
	// cout << in.read_byte() << endl;
	// cout << in.read_byte() << endl;
	// cout << (char)in.read_byte() << endl;

	// for(int i = 7; i >= 0; i--){
	// 	in.read_bit((node->symbol << i) & 1);
	// }
	// cout << (unsigned char)in.read_byte() << endl;


	if(in.read_byte() == 1){
		// cout << 1 << endl;
		return new HCNode(0, in.read_byte());
	}
    else{
		// cout << 0 << endl;
        HCNode* left = readHeader(in);
        HCNode* right = readHeader(in);
		// cout << 0.1 << endl;
        HCNode* nHCNode = new HCNode(0, 'c');
        nHCNode->c0 = left;
        nHCNode->c1 = right;
		// cout << 0.2 << endl;
		left->p = nHCNode;
		right->p = nHCNode;
        return nHCNode;
    }
}


int main(int argc, char *argv[]){
    if(argc != 3){
		error("Incorrect Number of Arguments");
		return 1;
	}
	FancyInputStream* input = new FancyInputStream(argv[1]);
	FancyOutputStream* output = new FancyOutputStream(argv[2]);
	HCTree tree;

	if(input->filesize() == 0){
		return 0;
	}

	if(input->filesize() < 0){
		error("File size less than 0");
		return 1;
	}

	int file_size = input->read_int();

	HCNode* tree_root = readHeader(*input);

	for(int i = 0; i < file_size; i++){
		output->write_byte(tree.decode(tree_root, *input));
	}

	recursiveClear(tree_root);

	delete input;
	delete output;
	return 0;
}
