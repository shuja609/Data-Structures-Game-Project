//                22i-8759   __  22i-2553  __ SE-D  __ Final Project

#pragma once
#include <iostream>
#include "gameH.h"
#include <string>
#include <sstream>
#include <conio.h> // For _getch() 
#include <windows.h> 
#include <queue>
#include <cstdlib>  // For rand() and srand()
#include<thread>
#include <fstream>
using namespace std;




HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;


//function to get the x y coordinates to cout anything lke grid or border and any text
void getxycoordinates(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}


// Function to set  text color
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}





//struct of car for indicating the position of car
struct Car {
public:
    int positionX;
    int positionY;
};


// structure of obstacle to save the x,y position and score value 
struct Obstacle {
    int positionX;
    int positionY;
    int scoreImpact;
};


//structure of collected items for saving x,y coordinates
//  on grid ,score value, its type and its representation sign
struct collectedItems {
    int positionX;
    int positionY;
    int scoreValue;
    string type;
    char sign;
};







//creating a linked list class especially for collected items data type
//  to store the collected items in linked list as mentionned in project document
class CollectedItemsList {
private:
    struct listNode {
        collectedItems item;
        listNode* next;

        listNode(const collectedItems& newItem) : item(newItem), next(nullptr) {}
    };

    listNode* head; // Points to the head of the linked list

public:
    // Constructor
    CollectedItemsList() : head(nullptr) {}

    // Destructor
    ~CollectedItemsList() {
        clear();
    }

    // Function to add a new collected item to the linked list
    void addItem(const collectedItems& newItem) {
        listNode* newNode = new listNode(newItem);
        newNode->next = head;
        head = newNode;
    }

    // Function to calculate the total number of items of a given type
    int calculateTotalByType(const std::string& itemType) const {
        int totalCount = 0;

        // Traverse the linked list
        listNode* current = head;
        while (current != nullptr) {
            if (current->item.type == itemType) {
                totalCount++;
            }

            current = current->next;
        }

        return totalCount;
    }

    // Function to clear the linked list and free memory
    void clear() {
        listNode* current = head;
        while (current != nullptr) {
            listNode* next = current->next;
            delete current;
            current = next;
        }

        head = nullptr;
    }
};


//creating a template class of queue as to store obstacle and collectd items in a
//  queue with random x,y grid location and  show them on the grid by poppng
template <typename T>
class MyQueue {
private:
    struct QueueNode {
        T data;
        QueueNode* next;
        QueueNode(const T& value) : data(value), next(nullptr) {}
    };

    QueueNode* frontNode;
    QueueNode* rearNode;

public:
    MyQueue() : frontNode(nullptr), rearNode(nullptr) {}

    ~MyQueue() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& value) {
        QueueNode* newNode = new QueueNode(value);
        if (empty()) {
            frontNode = rearNode = newNode;
        }
        else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
    }

    void pop() {
        if (empty()) {
            std::cerr << "Error: Queue is empty. Cannot pop." << std::endl;
            return;
        }
        QueueNode* temp = frontNode;
        frontNode = frontNode->next;
        delete temp;
        if (frontNode == nullptr) {
            rearNode = nullptr;
        }
    }

    T front() const {
        if (empty()) {
            std::cerr << "Error: Queue is empty. No front element." << std::endl;
            exit(EXIT_FAILURE);
        }
        return frontNode->data;
    }

    bool empty() const {
        return frontNode == nullptr;
    }
};

// saving the data of  game to a file for leader  board
void saveLeaderboard(int currentLevel, int coins, int wheels, int trophy, int elapsedSeconds, int currentScore) {
    std::ofstream file("leaderboard.txt", std::ios::app);

    if (file.is_open()) {
        file << ".----------------------------." << std::endl;
        file << "|  Level " << currentLevel << " Completed!        |" << std::endl;
        file << ":----------------------------:" << std::endl;
        file << "|                            |" << std::endl;
        file << "|  Collected Coins:" << coins << "!        |" << std::endl;
        file << "|  Collected Golden Wheels:" << wheels << "!|" << std::endl;
        file << "|  Collected Trophies:" << trophy << "!     |" << std::endl;
        file << "|  Time: " << elapsedSeconds << "s                  |" << std::endl;
        file << "|  Score: " << currentScore << "                  |" << std::endl;
        file << "'----------------------------'" << std::endl;

        file.close();
    }
    else {
        std::cerr << "Error: Unable to open the leaderboard file for writing." << std::endl;
    }
    return;
}

// Function to clear the leaderboard
void clearLeaderboard() {
    std::ofstream file("leaderboard.txt", std::ios::trunc);

    return;
}

