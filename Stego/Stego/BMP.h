#pragma once
#include <string>


class BMP {
private:
	std::string path;
	int fileSize;
	int dataLocation;

	void findDataLocation();

public:
	BMP(std::string filePath);
	int getDataLocation();
};