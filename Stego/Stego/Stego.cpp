// Steganography Application Entry Point
// Start with RAW BMP files. They are not compressed so should be easier to work with.

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

int main()
{
	string filePath = "c:\\users\\jake\\source\\repos\\stego\\bmptest.png";
	string outputPath = "c:\\test\\binary.lol";
	/*cout << "Please enter the name of the host file you want to use: ";
	getline(cin, filename);
	cout << filename;
	*/
	
	ifstream input(filePath, ios::binary);
	ofstream output(outputPath, ios::binary);
	
	std::copy(istreambuf_iterator<char>(input), std::istreambuf_iterator<char>(), std::ostreambuf_iterator<char>(output));
}