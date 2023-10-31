/*
 * Name: Vibhav Khare
 * Date: 06/06/23
 * Description: This code is a program that generates and manipulates a 2D landmass grid. It allows the user to specify the dimensions of the grid, drop dirt balls on the landmass, and perform various
 * operations on the landmass data.
 * Usage: g++ -g -Wall Khare_Vibhav_hw3.cpp -o asnn3exe
 */
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>
#include "termcolor.hpp"

using namespace std;

// Function to check if a given point (x, y) is within the specified bounds
bool boundsCheck(int x, int y, int minx, int miny, int maxx, int maxy) {
    return (x >= minx && x < maxx && y >= miny && y < maxy);
}

// Function to drop a dirt ball on the landmass, affecting the terrain
void dropDirtBall(int** landmass, int maxWidth, int maxHeight, int centerX, int centerY, int radius, int power) {
    for (int i = 0; i < maxHeight; i++) {
        for (int j = 0; j < maxWidth; j++) {
            if (boundsCheck(j, i, 0, 0, maxWidth, maxHeight)) {
                double distance = sqrt((pow(j - centerX, 2)) + (pow(i - centerY, 2)));
                if (distance <= radius) {
                    int impact = power - floor(distance);
                    if (impact > 0) {
                        landmass[i][j] += impact;
                    }
                }
            }
        }
    }
}

// Function to find the maximum value in a 2D map
int findMax(int** map, int width, int height) {
    int maxVal = map[0][0];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (map[i][j] > maxVal) {
                maxVal = map[i][j];
            }
        }
    }
    return maxVal;
}

// Function to normalize the values in the map to a range of 0-255
void normalizeMap(int** map, int width, int height, int maxVal) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            map[i][j] = static_cast<double>((map[i][j] / static_cast<double>(maxVal)) * 255);
        }
    }
}

// Function to polish the map and convert the numeric values to ASCII characters representing different terrains
void polishMap(int** map, int width, int height, int waterLine, char** polishedMap) {
    int landZone = 255 - waterLine;
    int halfWater = waterLine * 0.5; //50% of waterline
    int fifteenPercent = landZone * 0.15; //15% of landzone
    int fourtyPercent = landZone * 0.4; //40% of landzone
    int eightyPercent = landZone * 0.8; //80% of landzone
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            if(map[i][j] <= waterLine)
            {
                if(map[i][j] < floor(halfWater))
                {
                    polishedMap[i][j] = '#'; //deep water
                }
                else
                {
                    polishedMap[i][j] = '~'; //shallow water 
                }
            }
            else
            {
                if(map[i][j] < (waterLine) + floor(fifteenPercent))
                {
                    polishedMap[i][j] = '.'; //beach
                }
                else if(map[i][j] >= waterLine + floor(fifteenPercent) && map[i][j] < waterLine + floor(fourtyPercent))
                {
                    polishedMap[i][j] = '-'; //grass/plains
                }
                else if(map[i][j] >= waterLine + floor(fourtyPercent) && map[i][j] < waterLine + floor(eightyPercent))
                {
                    polishedMap[i][j] = '*'; //forest
                }
                else
                {
                    polishedMap[i][j] = '^'; //mountains
                }
            }
        }
    }
}

// Function to print the numeric map
void printMap(int** map, int width, int height) {
    cout << termcolor::colorize;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cout << setw(4) << map[i][j];
        }
        cout << endl;
    }
}

int colorCheck(char value)
{
    switch (value)
    {
	case '#':
	   return 0;
	case '~':
	   return 0;
	case '.':
	   return 1;
	default:
	   return 2;
    }
}

// Function to print the polished ASCII character map
void printCharMap(char** map, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
	    if (colorCheck(map[i][j]) == 0)
	    {
		cout << termcolor::blue;
	    }
	    else if (colorCheck(map[i][j]) == 1)
	    {
		cout << termcolor::yellow;
	    }
	    else
	    {
		cout << termcolor::green;
	    }
            cout << map[i][j];
        }
        cout << endl;
    }
    cout << termcolor::reset;
}

// Function to save the numeric map to a file
void saveMapToFile(int** map, int width, int height, const char* filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                file << setw(4) << map[i][j];
            }
            file << endl;
        }
        file.close();
        cout << "Map saved to " << filename << endl;
    } else {
        cout << "Failed to save the map to " << filename << endl;
    }
}

// Function to save the polished ASCII character map to a file
void saveCharMapToFile(char** map, int width, int height, const char* filename) {
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                file << map[i][j];
            }
            file << endl;
        }
        file.close();
        cout << "Map saved to " << filename << endl;
    } else {
        cout << "Failed to save the map to " << filename << endl;
    }
}

// Function to generate a random floating-point number between 0 and 1
double frand() {
    return (double)rand() / ((double)RAND_MAX+1);
}

//Main function
int main(int argc, char** argv) {
    int seed;
    if (argc > 2 && argv[1][1] == 's')
    {
        seed = atoi(argv[2]);
    } 
    else
    {
        seed = -1;
    }
    if (seed == -1)
    {
        srand(time(0));
    }
    else
    {
        srand(seed);
    }

    int width, height;
    cout << "Welcome to Vibhav's Terraformer!" << endl;
    cout << "Enter grid width: ";
    cin >> width;
    cout << "Enter grid height: ";
    cin >> height;

    // Create a 2D array for the landmass and initialize all elements to 0
    int** landmass = new int*[height];
    for (int i = 0; i < height; i++) {
        landmass[i] = new int[width];
        for (int j = 0; j < width; j++) {
            landmass[i][j] = 0;
        }
    }

    // Create a 2D array for the polished ASCII character map
    char** polishedMap = new char*[height];
    for (int i = 0; i < height; i++) {
        polishedMap[i] = new char[width];
    }
    
    int waterLine;
    cout << "Enter value for waterline (40-200): ";
    cin >> waterLine;

    int radius;
    cout << "Enter dirtball radius (minimum 2): ";
    cin >> radius;

    int power;
    cout << "Enter dirtball power rating (minimum = radius): ";
    cin >> power;

    int numDirtballs;
    cout << "Enter number of dirtballs to drop: ";
    cin >> numDirtballs;

    // Generate and drop dirt balls on the landmass
    for (int k = 0; k < numDirtballs; k++) {
        int centerX = frand()  * width;
        int centerY = frand() * height;
        dropDirtBall(landmass, width, height, centerX, centerY, radius, power);
    }

    // Print and save the raw landmass map
    cout << "Raw 2D array:" << endl;
    printMap(landmass, width, height);
    cout << endl;
    saveMapToFile(landmass, width, height, "raw_landmass.txt");

    // Normalize the landmass and print/save the normalized map
    int maxVal = findMax(landmass, width, height);
    normalizeMap(landmass, width, height, maxVal);

    // Polish the landmass and convert it to an ASCII character map
    polishMap(landmass, width, height, waterLine, polishedMap);

    cout << "Normalized 2D array:" << endl;
    printMap(landmass, width, height);
    cout << endl;
    saveMapToFile(landmass, width, height, "normalized_landmass.txt");

    // Print and save the polished ASCII character map
    cout << "Polished ASCII character array:" << endl;
    printCharMap(polishedMap, width, height);
    cout << endl;
    saveCharMapToFile(polishedMap, width, height, "final_landmass.txt");

    // Clean up dynamically allocated memory
    for (int i = 0; i < height; i++) {
        delete[] landmass[i];
        delete[] polishedMap[i];
    }
    delete[] landmass;
    delete[] polishedMap;

    return 0;
}
