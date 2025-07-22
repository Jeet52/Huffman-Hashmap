//
// STARTER CODE: util.h
//
// TODO:  Write your own header and fill in all functions below.
//

/*
-------------------------------------------
Project 4: Huffman File Compression with Hashmap
Course: CS 251,Summer 2024, UIC
System: Advanced zyLab
Author: Scott Reckinger
-------------------------------------------
*/

#include <iostream>
#include <fstream>
#include <map>
#include <queue>          // std::priority_queue
#include <vector>         // std::vector
#include <functional>     // std::greater
#include <string>
#include "bitstream.h"
#include "hashmap.h"
#pragma once

struct HuffmanNode {
    int character;
    int count;
    HuffmanNode* zero;
    HuffmanNode* one;
};

//
// *This method frees the memory allocated for the Huffman tree.
//
void freeTree(HuffmanNode* node) {
    
    if (node == nullptr) {
        return; // Base case: If the node is null, there's nothing to free.
    }

    // Recursively free the left and right subtrees.
    freeTree(node->zero);
    freeTree(node->one);

    // Delete the current node.
    delete node;

}

//
// *This function builds the frequency map.  
//  If isFile is true, read from the file with name filename.  
//  If isFile is false, read directly from string filename.
//
void buildFrequencyMap(string filename, bool isFile, hashmap &map) {
    if (isFile == true){
        fstream read;
        read.open(filename);

        char ch;
        while(read.get(ch)){
            if (map.containsKey(ch)){
                int s = map.get(ch);
                map.put(ch, s + 1);
            }
            else {
                map.put(ch, 1);
            }
        }

        map.put(PSEUDO_EOF, 1);
        read.close();
    }
    else {
        int fSize = filename.size();
        for(int i = 0; i < fSize; i++){
            char ch = filename.at(i);
            if (map.containsKey(ch)){
                int s = map.get(ch);
                map.put(ch, s + 1);
            }
            else {
                map.put(ch, 1);
            }
        }
         map.put(PSEUDO_EOF, 1);
    }
   
}

//Prioritize
struct prioritize {
    bool operator()(HuffmanNode* left, HuffmanNode* right) {
        return left->count > right->count;
    }
};
//
// *This function builds an encoding tree from the frequency map.
//
HuffmanNode* buildEncodingTree(hashmap &map) {
    
    // TO DO:  Write this function here.
    //         this will include call similar to the one below,
    //         which is provided as an example only
    
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, prioritize> pq;
    
    // TODO: use pq to build encoding tree

    //replace this return line; only here for starter code compilation
    
    vector<int> keys = map.keys();
    // Create leaf nodes for each character and add to the priority queue
    for (int k: keys) {
        HuffmanNode* node = new HuffmanNode;
        node->character = k;
        node->count = map.get(k);
        node->zero = nullptr;
        node->one = nullptr;
        pq.push(node);
    }
    
    // Build the Huffman tree
    while (pq.size() > 1) {
        // Pop the two nodes with the smallest counts
        HuffmanNode* left = pq.top();
        pq.pop();
        HuffmanNode* right = pq.top();
        pq.pop();
        
        // Create a new internal node with these two nodes as children
        HuffmanNode* parent = new HuffmanNode;
        parent->character = NOT_A_CHAR;
        parent->count = left->count + right->count;
        parent->zero = left;
        parent->one = right;
        
        // Push the new node back into the priority queue
        pq.push(parent);
    }
    
    // The last remaining node is the root of the Huffman encoding tree
    return pq.top();  // TO DO: update this return
}


//Helper Function

void buildEncodingMapHelper (HuffmanNode* node, map<int, string>& encodingMap, string path){
    // Base case: if the node is a leaf node
    if (node->zero == nullptr && node->one == nullptr) {
        encodingMap[node->character] = path;
        return;
    }
    
    // Recursive case: traverse left and right children
    if (node->zero != nullptr) {
        buildEncodingMapHelper(node->zero, encodingMap, path + "0");
    }
    if (node->one != nullptr) {
        buildEncodingMapHelper(node->one, encodingMap, path + "1");
    }
}







//
// *This function builds the encoding map from an encoding tree.
//
map<int,string> buildEncodingMap(HuffmanNode* tree) {
    map<int,string> encodingMap;
    
    // TO DO:  Write this function here.

    if (tree != nullptr){
        buildEncodingMapHelper(tree, encodingMap, "");
    }
    
    return encodingMap;  // TO DO: update this return
}

//
// *This function encodes the data in the input stream into the output stream
// using the encodingMap.  This function calculates the number of bits
// written to the output stream and sets result to the size parameter, which is
// passed by reference.  This function also returns a string representation of
// the output file, which is particularly useful for testing.
//
string encode(ifstream& input, map<int,string> &encodingMap,
              ofbitstream& output, int &size, bool makeFile) {
    
    // TO DO:  Write this function here.
    string encodedString = "";
    size = 0;  // Initialize the size to 0

    char ch;
    while (input.get(ch)) {
        string encodedChar = encodingMap[ch];
        encodedString += encodedChar;
        size += encodedChar.length();

        if (makeFile) {
            for (char bit : encodedChar) {
                output.writeBit(bit - '0');
            }
        }
    }

    // Append the encoding for PSEUDO_EOF
    string eofEncoding = encodingMap[PSEUDO_EOF];
    encodedString += eofEncoding;
    size += eofEncoding.length();

    if (makeFile) {
        for (char bit : eofEncoding) {
            output.writeBit(bit - '0');
        }
    }

    return encodedString; // TO DO: update this return
}


//
// *This function decodes the input stream and writes the result to the output
// stream using the encodingTree.  This function also returns a string
// representation of the output file, which is particularly useful for testing.
//
string decode(ifbitstream &input, HuffmanNode* encodingTree, ofstream &output) {
    
    // TO DO:  Write this function here.
    string decodedString = "";
    HuffmanNode* currentNode = encodingTree;

    while (true) {
        int bit = input.readBit();
        if (bit == -1) {
            break; // End of input stream
        }

        if (bit == 0) {
            currentNode = currentNode->zero;
        } else {
            currentNode = currentNode->one;
        }

        // Check if we've reached a leaf node
        if (currentNode->zero == nullptr && currentNode->one == nullptr) {
            if (currentNode->character == PSEUDO_EOF) {
                break; // Reached the end of the encoded data
            }

            // Output the character and append to the decoded string
            output.put(currentNode->character);
            decodedString += currentNode->character;

            // Reset to the root of the encoding tree for the next character
            currentNode = encodingTree;
        }
    }

    return decodedString;  // TO DO: update this return
}

//
// *This function completes the entire compression process.  Given a file,
// filename, this function (1) builds a frequency map; (2) builds an encoding
// tree; (3) builds an encoding map; (4) encodes the file (don't forget to
// include the frequency map in the header of the output file).  This function
// should create a compressed file named (filename + ".huf") and should also
// return a string version of the bit pattern.
//
string compress(string filename) {
    
    // TO DO:  Write this function here.

    //1.Frequency Map
    hashmap frequencyMap; 
    buildFrequencyMap(filename, true, frequencyMap);

    //2.Encoding Tree
    HuffmanNode* encodingTree = buildEncodingTree(frequencyMap);

    //3.Encoding Map
    map<int, string> encodingMap = buildEncodingMap(encodingTree);

    //4.Encode 
    ifstream inputFile(filename, ios::binary);
    ofbitstream outputFile(filename + ".huf");

    // Write the frequency map to the output file as the header
    outputFile << frequencyMap;

    int size = 0;
    string encodedString = encode(inputFile, encodingMap, outputFile, size, true);

    // Clean up the encoding tree to free memory
    freeTree(encodingTree);

    return encodedString; // TO DO: update this return
}

// *This function completes the entire decompression process. Given the file,
// filename (which should end with ".huf"), (1) extract the header and build
// the frequency map; (2) build an encoding tree from the frequency map; (3)
// use the encoding tree to decode the file. This function should create a
// decompressed file using the following convention.
// If filename = "example.txt.huf", then the uncompressed file should be named
// "example_unc.txt". The function should return a string version of the
// uncompressed file. Note: this function should reverse what the compress
// function does.
//
string decompress(string filename) {
    // Step 1: Open the input file
    ifbitstream inputFile(filename);

    // Extract the base filename (remove the ".huf" extension)
    string baseFilename = filename.substr(0, filename.size() - 4);

    // Remove the ".txt" extension if it exists
    size_t txtPos = baseFilename.rfind(".txt");
    if (txtPos != string::npos) {
        baseFilename = baseFilename.substr(0, txtPos);
    }

    string outputFilename = baseFilename + "_unc.txt";

    // Step 2: Extract the header (frequency map)
    hashmap frequencyMap;
    inputFile >> frequencyMap;

    // Step 3: Build the encoding tree from the frequency map
    HuffmanNode* encodingTree = buildEncodingTree(frequencyMap);

    // Step 4: Decode the file
    ofstream outputFile(outputFilename);
    string decodedString = decode(inputFile, encodingTree, outputFile);

    // Clean up the encoding tree to free memory
    freeTree(encodingTree);

    // Close the files
    inputFile.close();
    outputFile.close();

    return decodedString;
}
