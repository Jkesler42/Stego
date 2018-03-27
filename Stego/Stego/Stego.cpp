// Steganography Application Entry Point
// Start with RAW BMP files. They are not compressed so should be easier to work with.

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
using namespace std;

int getHeaderSize(string);

int getMaxFilesize(string, int);

int getHiddenFileSize(string);

void doTheStego(string, string, string);

void unStego(string, string);

int main()
{
	string filePath = "c:\\users\\aajxk\\source\\repos\\stego\\bmptest.bmp";
	string stegoFile = "c:\\test\\binary.bmp";
	string hiddenFile = "c:\\test\\Hacker.jpg";
	string unstegoFile = "c:\\test\\unstego.jpg";
	/*cout << "Please enter the name of the host file you want to use: ";
	getline(cin, filename);
	cout << filename;
	*/
	int headerSize = getHeaderSize(filePath);
	cout << headerSize << "\n";
	cout << getMaxFilesize(filePath, headerSize) << "\n";
	cout << getHiddenFileSize(hiddenFile) << "\n";
	bitset<32> fileSize = 165444;
	cout << fileSize << "\n";
	doTheStego(filePath, hiddenFile, stegoFile);
	unStego(stegoFile, unstegoFile);
	cin >> stegoFile; //Too lazy to put a pause in. This is just to pause to troubleshoot.
}

int getHeaderSize(string path) {
	char * memblock = new char [4];
	ifstream header;
	header.open(path, ios::binary | ios::in);
	if (!header.is_open()) {
		cout << "error!";
		return -1;
	}
	header.seekg(10);
	header.read(memblock, 4);
	return memblock[0];
	header.close();
}

int getMaxFilesize(string path, int headerSize){
	int availableSize;
	ifstream file(path, ios::binary | ios::ate);
	if (!file.is_open()) {
		return -1;
	}
	availableSize = ((int)file.tellg() - headerSize) / 8;
	file.close();
	return availableSize;
}

int getHiddenFileSize(string path) {
	int totalSize;
	ifstream file(path, ios::binary | ios::ate);
	if (!file.is_open()) {
		return -1;
	}
	totalSize = (int)file.tellg();
	file.close();
	return totalSize;
}

void doTheStego(string hostFile, string hiddenFile,string outputPath) {
	 //First 32 changed bits are going to be the hidden file size
	int hiddenFileSize = getHiddenFileSize(hiddenFile);
	bitset<32> bitsHiddenFileSize = hiddenFileSize;
	int headerSpace = getHeaderSize(hostFile);
	cout << bitsHiddenFileSize;
	ifstream host(hostFile, ios::binary);
	ifstream hidden(hiddenFile, ios::binary);
	ofstream output(outputPath, ios::out | ios::binary);
	
	vector<char> hostBuffer((istreambuf_iterator<char>(host)), (istreambuf_iterator<char>()));
	vector<char> hiddenBuffer((istreambuf_iterator<char>(hidden)), (istreambuf_iterator<char>()));
	int bitSetCounter = 0;
	int hiddenCounter = 8;
	bitset<8> currentHiddenByte;

	for (int i = 0; i < hostBuffer.size(); i++) {
		if (i < headerSpace) {
			output << hostBuffer[i];
		} 
		else if (i >= headerSpace && i < headerSpace + 32) {
			bitset<8> currentByte = hostBuffer[i];
			if (currentByte[0] != bitsHiddenFileSize[bitSetCounter]) {
				currentByte.flip(0);
			}
			output << hex << char(currentByte.to_ulong());
			bitSetCounter++;
		}
		else if (i >= headerSpace + 32 && i < headerSpace + (hiddenFileSize*8) + 32) {
			if (hiddenCounter > 7) {
				hiddenCounter = 0;
				currentHiddenByte = hiddenBuffer[0];
				hiddenBuffer.erase(hiddenBuffer.begin());
			}
			bitset<8> currentHostByte = hostBuffer[i];
			if (currentHostByte[0] != currentHiddenByte[hiddenCounter]) {
				currentHostByte.flip(0);
			}
			output << hex << char(currentHostByte.to_ulong());
			hiddenCounter++;
		}
		else {
			output << hostBuffer[i];
		}
	}
}

void unStego(string stegoFile, string outputPath) {
	int headerSpace = getHeaderSize(stegoFile);
	bitset<32> sizeBits;
	int sizeCounter = 0;
	ifstream stego(stegoFile, ios::binary);
	ofstream output(outputPath, ios::out | ios::binary);
	vector<char> stegoBuffer((istreambuf_iterator<char>(stego)), (istreambuf_iterator<char>()));
	int extractCounter = 0;
	bitset<8> extractedByte;

	for (int i = headerSpace; i < headerSpace + 32; i++) {
		bitset<8> currentByte = stegoBuffer[i];
		if (currentByte[0] != sizeBits[sizeCounter]) {
			sizeBits.flip(sizeCounter);
		}
		sizeCounter++;
	}
	int size = int(sizeBits.to_ulong());

	for (int i = headerSpace + 32; i < headerSpace + 32 + (size * 8); i++) {
		if (extractCounter > 7) {
			extractCounter = 0;
			output << hex << char(extractedByte.to_ulong());
		}
		bitset<8> currentByte = stegoBuffer[i];
		if (currentByte[0] != extractedByte[extractCounter]) {
			extractedByte.flip(extractCounter);
		}
		extractCounter++;
	}
}