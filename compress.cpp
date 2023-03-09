#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <cstring>
#include <bitset>
#include "HCTree.hpp"
using namespace std;

void writeHeader(HCNode* node, FancyOutputStream & out){
	// out.write_bit(1);
	// char c = 'u';
	// for(int i = 7; i >= 0; i--){
	// 	out.write_bit((c >> i) & 1);
	// }


	// for(int i = 0; i < 8; i++){
	// 	out.write_bit(0);
	// }
	// for(int i = 0; i < 8; i++){
	// 	out.write_bit(1);
	// }

	// out.write_byte(0);
	// out.write_byte(1);
	// out.flush_bitwise();
	// out.write_byte('u');


	if(node == nullptr){
		return;
	}
	if(node->c0 == nullptr && node->c1 == nullptr){
	    out.write_byte(1);
		out.write_byte(node->symbol);
	}
	else{
	    out.write_byte(0);
	}
	writeHeader(node->c0, out);
	writeHeader(node->c1, out);
}

int main(int argc, char *argv[]){
	if(argc != 3){
		error("Incorrect Number of Arguments");
		return 1;
	}
	
	FancyInputStream* input = new FancyInputStream(argv[1]);
	FancyOutputStream* output = new FancyOutputStream(argv[2]);
	vector<int> freq(256,0);
	int fileS = input->filesize();
	for(int i = 0; i < fileS; i++){
		freq[input->read_byte()] += 1;
	}
	
	HCTree tree;
	tree.build(freq);
	
	if(fileS == 0){
		return 0;
	}

	if(fileS < 0){
		error("File size less than 0");
		return 1;
	}

	output->write_int(fileS);
	writeHeader(tree.getRoot(), *output);

	input->reset();
	for(int i = 0; i < fileS; i++){
		char c = (input->read_byte());
		tree.encode(c, *output);
	}
	
	delete output;
	delete input;
	freq.clear();

	return 0;
}
