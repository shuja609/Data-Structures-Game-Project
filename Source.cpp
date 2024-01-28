//               22i-8759  __  22i-2553  __ SE-D  __Final Project
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


//setting the length and width for printing border and grid
#define SCREEN_WIDTH 120  
#define SCREEN_HEIGHT 22  
#define WIN_WIDTH 80   





//defining a node of the grid or graph ..like what should be included in on node of graph 
struct Node {
    public:
    bool iswall = false;
    Node* cn[4]; // Connected nodes: Up, Down, Left, Right
    Node* parent = nullptr;
    Car* car = nullptr;
    bool isPath = false; // New: Indicates whether this node is part of the path
    bool isobstacle = false;
    int obstacle_value;
    bool isCollectingItem = false;
    collectedItems item;
    int distance = 0;
};



//our main class grid in which all the functions of game is designd
class Grid {

public:
    static const int SIZE = 20;
    Node* maze[SIZE][SIZE];
    Node* start = nullptr;
    Node* target = nullptr;
    Car* car = nullptr;

    int currentLevel = 1;
    int currentScore = 0;

    // Queue to manage random obstacles
    MyQueue<Obstacle> obstacleQueue;

    // Queue to manage random collective items
    MyQueue<collectedItems> itemsQueue;

    //linkedlist for storing collected items
    CollectedItemsList itemList;


    // Timer variables
    chrono::time_point<chrono::steady_clock> startTime;
    chrono::time_point<chrono::steady_clock> endTime;
    chrono::duration<double> elapsedSeconds;


    //defining custom designed mazes via strings
    string mazefile1 =
        "x x x x x x x x x x x x x x x x x x x x "
        "x o o o x o x o o o o o o o x o x o o x "
        "x x x o o o x x x o x x x o o o x o o x "
        "x o x o x o o o x o o o x o x o o o x x "
        "x o o o x x x o o o x o o o x x x o o x "
        "x o x x x o x x x o o o x x x o x x x x "
        "x o o x x o x x x x x o o o x o x o o x "
        "x o o x x o x x x x x o o o o o o o o x "
        "x x o x x o o o o o x x o x x o o o o x "
        "x o o o o o x o x o o o o o o o x o x x "
        "x x o o x o x x x x x x o o x o x o x x "
        "x o o o x o x o o o o x o o x o x o o x "
        "x x x o o o x x x o x x x o o o x x x x "
        "x o x o x o o o x o x o x o x o o o x x "
        "x o o o x x x o o o x o x o x x x o o x "
        "x o x x x o x x x o o o x o x o x x x x "
        "x o o o x o x o o o x o o o x o x o o x "
        "x o o o o o o o o x x o o o o o o o o x "
        "x o o x x o o o o x x x o x x o o o o x "
        "x x x x x x x x x x x x x x x x x x x x "
        ;
    string mazefile2 =
        "x x x x x x x x x x x x x x x x x x x x "
        "x o o o o o o o o o o x o x x o o o o x "
        "x o x o x x o x o x o x x x x o o x o x "
        "x x x o x x o x o o o x o o o o o x o x "
        "x o o o x o o x o o o o o o o o o x o x "
        "x o x x x x o x x x o o x x x o o x x x "
        "x o o o x x o o o o o o o o x o o o o x "
        "x o o x x o o x x x x x o o x o o o o x "
        "x x o x o o o x o x x o o o x o o o x x "
        "x o o o o x x x o o x o x x x x x o o x "
        "x x x x o o x o o o x o x o x x x x o x "
        "x o o o o o o o o o x o o o x o o x o x "
        "x x x x o x x x x x x o o x x x o o o x "
        "x o o o o x o o o o o o o o o o o x o x "
        "x o x o x x x x x o o x x x x o o x o x "
        "x x x o x x o o o o o x o o o o o x x x "
        "x o x o x x x o x o o o o o x o o o o x "
        "x o x o x x o o x x x x o o x x o x o x "
        "x o o o x o o o x o o o o o x o o x o x "
        "x x x x x x x x x x x x x x x x x x x x "
        ;

    string mazefile3 =
        "x x x x x x x x x x x x x x x x x x x x "
        "x o o o o o x o o o x o o o x o x o o x "
        "x x o x x o x x x o x x x o o o x o o x "
        "x x o x x o o o x o o o x o x o o o x x "
        "x o o o x x x o o o x o o o x x x o o x "
        "x o x x x o x x x o o o x x x o x x x x "
        "x o o x x o x x x x x o o o x o x o o x "
        "x o o x x o x x x x x o o o o o o o o x "
        "x x o x x o o o o o x x x x x o o o o x "
        "x o o o o o x o x o o o o o o o x o x x "
        "x x o o x o x x x x x x o o x o x o x x "
        "x o o o x x x o o o o x o o x o x o o x "
        "x o x o o o x x x o x x x o o o x x x x "
        "x o x o x x o o x o x o x o x o o o x x "
        "x o o o x x x o o o x o x o x x x o o x "
        "x o x x x o x x x o o o x o x o x x x x "
        "x o o o x o x x o o x o x o x o x x o x "
        "x x o o o o o o o x x o o o o o o o o x "
        "x o o x x o x x o x x x o x x x x o o x "
        "x x x x x x x x x x x x x x x x x x x x "
        ;

    string mazefile4 =
        "x x x x x x x x x x x x x x x x x x x x "
        "x o o o o x o o x o o o o o x x o o o x "
        "x o x x x x x o x o x x x o x x o x o x "
        "x o o o x o x o x o x o o o x x o x o x "
        "x o x x x o o o o o x x o o x x o x o x "
        "x o x x x x o x x x x x x o o o o x o x "
        "x o o x x x o o o x o o o x x o o x o x "
        "x x o o o x o o x x x x o o o o o x o x "
        "x o o x o x o o o o o o o x o o x x x x "
        "x o o x o o o x o o o x x x o o o o o x "
        "x x o o x x o x o x o x o o o x o o o x "
        "x o o x o o x o o x o x o x x x x x o x "
        "x o x o o o o o o x o x o o o x o x o x "
        "x x o x x o x x o x x x o x o x o x o x "
        "x x o o o o o o o o o x o x o x o o o x "
        "x o o o o x x x x x o o o x o o o x x x "
        "x x o x o x o x o x o x x x o x o o o x "
        "x o x o x o x o x o x o x o x o x o o x "
        "x o o o o o o o o o o o o o o o o x o x "
        "x x x x x x x x x x x x x x x x x x x x "
        ;


    //constructor 
    Grid() {
        initializeMaze(mazefile1);
        car = new Car();
        placeCar(1, 1);
        car->positionX = 1;
        car->positionY = 1;
        start = maze[1][1];
        start->car = car;
        target = maze[18][18];
    }
   //destructor
    ~Grid() {
        // Clean up maze nodes
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                delete maze[i][j];
            }
        }

        // Clean up the car
        delete car;

        // Clean up items in the obstacle queue
        while (!obstacleQueue.empty()) {
            obstacleQueue.pop();
        }

        // Clean up items in the items queue
        while (!itemsQueue.empty()) {
            itemsQueue.pop();
        }

    }

    //create a graph by rading our created maze string files
    void initializeMaze(const string& mazefile) {
        stringstream ss(mazefile, ios::in);
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                char buffer;
                Node* n = new Node();
                ss >> buffer;
                if (buffer == 'x') {
                    n->iswall = true;
                }
                maze[i][j] = n;
            }
        }

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                // Initialize connected nodes
                maze[i][j]->cn[0] = (i - 1 >= 0) ? maze[i - 1][j] : nullptr; // Up
                maze[i][j]->cn[1] = (i + 1 < SIZE) ? maze[i + 1][j] : nullptr; // Down
                maze[i][j]->cn[2] = (j - 1 >= 0) ? maze[i][j - 1] : nullptr; // Left
                maze[i][j]->cn[3] = (j + 1 < SIZE) ? maze[i][j + 1] : nullptr; // Right
            }
        }

       // calling function to initialize obstacles in a queue
        initializeObstacles();
        // calling function to initialize collected items in a queue
        initializeCollectedItems();
    }

    // Function to initialize obstacles based on the current level
    void initializeObstacles() {
        //first of all  Clear all existing obstacles
        while (!obstacleQueue.empty()) {
            obstacleQueue.pop();
        }

        // Add new obstacles based on the current level
        for (int i = 0; i < currentLevel * 100; ++i) {
            Obstacle obstacle;
            do {
              //randomly assigning the x, y coordinates on grid
                obstacle.positionX = rand() % SIZE;
                obstacle.positionY = rand() % SIZE;
            } while (maze[obstacle.positionX][obstacle.positionY]->iswall); // Check if the position is a wall

            // Set score impact based on obstacle type or any other logic
            //creating three types of obstacles
            obstacle.scoreImpact = (i % 3 == 0) ? -2 : ((i % 3 == 1) ? -5 : -10);

            // Enqueue the obstacle
            obstacleQueue.push(obstacle);
        }
    }

    // Function to initialize collected items based on the current level
    void initializeCollectedItems() {
        //first of all Clear existing collected items
        while (!itemsQueue.empty()) {
            itemsQueue.pop();
        }

        // Add new obstacles based on the current level
        for (int i = 0; i < currentLevel * 100; ++i) {
            collectedItems items;
            do {
                //randomly assigning the x,y coordinates on the grid
                items.positionX = rand() % SIZE;
                items.positionY = rand() % SIZE;
            } while (maze[items.positionX][items.positionY]->iswall); // Check if the position is a wall

            // Set score impact based on obstacle type or any other logic
            //creating three types of collective items
            items.scoreValue = (i % 3 == 0) ? 5 : ((i % 3 == 1) ? 10 : 20);
            //setting types and representation signs of collective items
            switch (items.scoreValue) {
            case 5: {
                items.type = "Coin\n";
                items.sign = char(184);
                break;
            }
            case 10: {
                items.type = "Golden Wheel\n";
                items.sign = char(214);
                break;
            }
            case 20: {
                items.type = "Trophy\n";
                items.sign = char(231);
                break;
            }
            default:
                cout << "T\n";
            }

            // Enqueue the collective items
            itemsQueue.push(items);
        }
    }

    // Function to process obstacles and update player's score
    void processObstacles() {
        if (!obstacleQueue.empty()) {
            Obstacle obstacle = obstacleQueue.front();
            obstacleQueue.pop();
            maze[obstacle.positionX][obstacle.positionY]->isobstacle = true;
            maze[obstacle.positionX][obstacle.positionY]->obstacle_value = obstacle.scoreImpact;

            // Check if the car collided with the obstacle
            if (maze[car->positionX][car->positionY]->isobstacle) {
                // Update the score based on the obstacle's impact
                currentScore += maze[car->positionX][car->positionY]->obstacle_value;
                maze[car->positionX][car->positionY]->isobstacle= false;
                Beep(900, 200);
              
            }
            if (currentScore < 0) {
                displayGameOver();
            }
        }
     
    }


    //placing the collective items randomly in the grid
    void PlaceItems() {

        if (!itemsQueue.empty()) {
            collectedItems item = itemsQueue.front();
            itemsQueue.pop();
            maze[item.positionX][item.positionY]->isCollectingItem = true;
            maze[item.positionX][item.positionY]->item = item;
        }
    }

    //function to collect items in a linked list when car and collective item has same position
    void CollectItems() {
        if (maze[car->positionX][car->positionY]->isCollectingItem) { //checking if car and collective item has same x,y  
                                                                       //cordinates and that node iscollective item is true


            switch (maze[car->positionX][car->positionY]->item.scoreValue) { //checking the score value of that collected item and 
            case 5: {                                                          //updating the score respectively while marking the iscolletiveitem of
                                                                                //that node equal to false. also adding the item in linkedlist
                maze[car->positionX][car->positionY]->item.type = "Coin";
                itemList.addItem(maze[car->positionX][car->positionY]->item);
                currentScore += 5;
                maze[car->positionX][car->positionY]->isCollectingItem = false;
                break;
            }
            case 10: {
                maze[car->positionX][car->positionY]->item.type = "Golden Wheel";
                itemList.addItem(maze[car->positionX][car->positionY]->item);
                currentScore += 10;
                maze[car->positionX][car->positionY]->isCollectingItem = false;
                break;
            }
            case 20: {
                maze[car->positionX][car->positionY]->item.type = "Trophy";
                itemList.addItem(maze[car->positionX][car->positionY]->item);
                currentScore += 20;
                maze[car->positionX][car->positionY]->isCollectingItem = false;
                break;
            }
            default:
                cout << "0\n";
            }
            
            Beep(900, 200);


        }
    }


    //updating the graph of maze based on levels
    void updateMaze(int level) {
        // Update the maze based on the level
        string mazefile;
        if (level == 1) {
            mazefile = mazefile1;
        }
        else if (level == 2) {
            mazefile = mazefile2;
        }
        else if (level == 3) {
            mazefile = mazefile3;
        }
        else if (level == 4) {
            mazefile = mazefile4;
        }
        // we will add more conditions in future when more levels will be added

        // Reseting the car and other properties
        initializeMaze(mazefile);
        placeCar(1, 1);
        car->positionX = 1;
        car->positionY = 1;
        start = maze[1][1];
        start->car = car;
        target = maze[18][18];
    }



    //function to place the at a speific positio on the grid
    //if the location is not a wall
    void placeCar(int x, int y) {
        if (!maze[x][y]->iswall) {
            car->positionX = x;
            car->positionY = y;
            maze[x][y]->car = car;
        }
    }
   
    //function to move the car in specific direction according to the pressed key
    void moveCar(int direction) {
        int newX = car->positionX;
        int newY = car->positionY;

        switch (direction) {
        case 72: // if Up arrow is pressed subtract 1 from x to move up on grid
            if (newX - 1 >= 0 && maze[newX][newY]->cn[0] != nullptr && !maze[newX - 1][newY]->iswall) {
                newX--;
            }
            break;
        case 80: // if Down arrow is pressed add 1 in x to move down on grid
            if (newX + 1 < SIZE && maze[newX][newY]->cn[1] != nullptr && !maze[newX + 1][newY]->iswall) {
                newX++;
            }
            break;
        case 75: // if Left arrow is pressed subtract 1 from y to move left on grid
            if (newY - 1 >= 0 && maze[newX][newY]->cn[2] != nullptr && !maze[newX][newY - 1]->iswall) {
                newY--;
            }
            break;
        case 77: // if Right arrow is pressed add 1 in y to move right on grid
            if (newY + 1 < SIZE && maze[newX][newY]->cn[3] != nullptr && !maze[newX][newY + 1]->iswall) {
                newY++;
            }
            break;
        default:
            return;
        }

        //inceasing score each time the car moves if the next position is not wall,obstacle or collective item
        if (!maze[car->positionX][car->positionY]->isobstacle && !maze[car->positionX][car->positionY]->iswall && !maze[car->positionX][car->positionY]->isCollectingItem) {
            currentScore++;
        }

        // Mark the current node as part of the path
        maze[car->positionX][car->positionY]->isPath = true;

        maze[car->positionX][car->positionY]->car = nullptr;
        car->positionX = newX;
        car->positionY = newY;
        maze[newX][newY]->car = car;

        CollectItems();
      
    }


    //function to draw the borders around the game
    void drawBorder() {
        for (int i = 0; i < SCREEN_HEIGHT; i++) {
            for (int j = 0; j < 17; j++) {
                setColor(6);
                getxycoordinates(0 + j, i); cout << "±";

                getxycoordinates(WIN_WIDTH - j, i);
                setColor(6);
                cout << "±";
            }
        }
      
    }
    //function to display the main interface of game 

    void displayMaze() {
        system("cls");
        
        //drawing border
        drawBorder();
        //calculating time for each level
       elapsedSeconds = endTime - startTime;

       //setting the necessary informstion at side of game
        setColor(1);
            getxycoordinates(WIN_WIDTH + 7, 0); cout << "Car Field";
            setColor(8);
            getxycoordinates(WIN_WIDTH + 7, 3); cout << "Level: " << currentLevel;
            getxycoordinates(WIN_WIDTH + 7, 4); cout << "Score: "<<currentScore;
            setColor(14);
            getxycoordinates(WIN_WIDTH + 6, 1); cout << "----------";
            if(elapsedSeconds.count()>=0)
            {
                getxycoordinates(WIN_WIDTH + 7, 5); cout << "Time: " << static_cast<int>(elapsedSeconds.count()) << "s";
            }
            else {
                getxycoordinates(WIN_WIDTH + 7, 5); cout << "Time: " << '0' << "s";
            }
            setColor(1);
            getxycoordinates(WIN_WIDTH + 7, 7); cout << "Obstacles";
            setColor(1);
            getxycoordinates(WIN_WIDTH + 3, 8); cout << char(247);
            setColor(8);
            getxycoordinates(WIN_WIDTH + 5, 8); cout<< "  Water waves -2";
            setColor(10);
            getxycoordinates(WIN_WIDTH + 3, 9); cout << char(239);
            setColor(8);
            getxycoordinates(WIN_WIDTH + 5, 9); cout<< "  Bump -5";
            setColor(4);
            getxycoordinates(WIN_WIDTH + 3, 10); cout << char(234);
            setColor(8);
            getxycoordinates(WIN_WIDTH + 5, 10);cout << "  Dig Pit  -10";
            setColor(1);
            getxycoordinates(WIN_WIDTH + 7, 12); cout << "Collective Items";
            setColor(6);
            getxycoordinates(WIN_WIDTH + 3, 13); cout << char(233);
            setColor(8);
            getxycoordinates(WIN_WIDTH + 5, 13); cout << "  Coins  +5";
            setColor(6);
            getxycoordinates(WIN_WIDTH + 3, 14); cout << char(153);
            setColor(8);
            getxycoordinates(WIN_WIDTH + 5, 14); cout << "  Golden Wheel  +10";
            setColor(4);
            getxycoordinates(WIN_WIDTH + 3, 15); cout << char(231);
            setColor(8);
            getxycoordinates(WIN_WIDTH + 5, 15); cout << "  Trophy  +20";

            setColor(6);
            getxycoordinates(WIN_WIDTH + 7, 17); cout << "Control ";
            setColor(14);
            getxycoordinates(WIN_WIDTH + 6, 18); cout << "-------- ";
            setColor(8);
            getxycoordinates(WIN_WIDTH + 2, 18); cout << " Left Key -  Left";
            getxycoordinates(WIN_WIDTH + 2, 19); cout << " Right Key - Right";
            getxycoordinates(WIN_WIDTH + 2, 20); cout << " Up Key -    Up";
            getxycoordinates(WIN_WIDTH + 2, 21); cout << " Down Key -  down";

        for (int i = 0; i < SIZE; i++) {
            // Move the cursor to the correct position inside the border
            getxycoordinates(WIN_WIDTH / 2 - SIZE / 2 -9, i + 1);

            //displaying grid
            for (int j = 0; j < SIZE; j++) {
                if (maze[i][j]->iswall) {
                    setColor(3);
                    cout << char(254) << " ";
                }
                else if (maze[i][j]->car != nullptr) {
                    setColor(8);
                    cout << "C ";
                }
                else if (maze[i][j]->isPath) {
                    setColor(11);
                    cout << char(175) << " ";
                }
                else if (maze[i][j] == target) {
                    setColor(111);
                    cout << "F ";
                }
                else if (maze[i][j]->isobstacle) {
                    setColor(29);
                    switch (maze[i][j]->obstacle_value) {
                    case -2:
                        setColor(1);
                        cout << char(247) << " "; //water waves
                        break;
                    case -5:
                        setColor(10);
                        cout << char(239) << " ";  //bump
                        break;
                    case -10:
                        setColor(4);
                        cout << char(234) << " ";  //dig pit
                        break;
                    default:
                        cout << "O ";
                    }
                }
                else if (maze[i][j]->isCollectingItem) {
                    setColor(1);
                    switch (maze[i][j]->item.scoreValue) {
                    case 5: {
                        setColor(6);
                        cout << char(233) << " "; //coins
                        break;
                    }
                    case 10: {
                        setColor(6);
                        cout << char(153) << " "; //golden wheel
                        break;
                    }
                    case 20: {
                        setColor(14);
                        cout << char(231) << " ";  // trophy
                        break;
                    }
                    default:
                        cout << "0\n";
                    }
                }
                else {
                    // Check if there is an obstacle at this position
                    bool obstacleFound = false;
                    Obstacle tempObstacle;

                    // Check if there is an obstacle in the queue
                    if (!obstacleQueue.empty()) {
                        tempObstacle = obstacleQueue.front();
                        obstacleQueue.pop();
                        obstacleQueue.push(tempObstacle);

                        if (tempObstacle.positionX == i && tempObstacle.positionY == j) {
                            obstacleFound = true;
                        }
                    }

                    if (obstacleFound) {
                        // Display the obstacle based on its type
                        switch (tempObstacle.scoreImpact) {
                        case -2:
                            setColor(1);
                            cout << char(247) << " "; //water waves
                            break;
                        case -5:
                            setColor(10);
                            cout << char(239) << " ";  //bump
                            break;
                        case -10:
                            setColor(4);
                            cout << char(234) << " ";  //dig pit
                            break;
                        default:
                            cout << "O ";
                        }
                    }
                    else {
                        // Empty space'
                        setColor(15);
                        cout << ". ";
                    }
                }
            }
            cout << endl;
        }

       

    }


    // Function to display the leaderboard from a file
    void displayLeaderboard() {
        
        system("cls");
        Beep(400, 1000);
        drawBorder();
        std::ifstream file("leaderboard.txt");
        if (file.is_open()) {
            string line;
            setColor(13); // Set color to light cyan
            int n = 2;
            while (getline(file, line)) {
              
                getxycoordinates(WIN_WIDTH / 2 - 13, n);
                cout << line << endl;
                n++;
            }

            file.close();
        }
        else {
            cout << "Error: Unable to open the leaderboard file for reading." << endl;
        }
        setColor(6);
        getxycoordinates(WIN_WIDTH / 2 - 13, 17);
        cout << "Press any key to go back.";
        _getch();
    }


    // Function to display the winning screen
    char displayWinScreen() {
        system("cls");
        Beep(400, 1000);

        //drawing border
        drawBorder();
        //calculating collective items form the linked list
       int coins =itemList.calculateTotalByType("Coin");
       int wheels = itemList.calculateTotalByType("Golden Wheel");
       int trophy = itemList.calculateTotalByType("Trophy");
        cout << endl << endl << endl << endl << endl;

        //printing winning board
        setColor(13); // Set color to light cyan
        getxycoordinates(WIN_WIDTH / 2 - 13, 2);
        cout << ".----------------------------." << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 3);
        cout << "|  Level " << currentLevel << " Completed!        |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 4);
        cout << ":----------------------------:" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 5);
        cout << "|                            |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 7);
        cout << "|  Collected Coins:" << coins << "!        |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 9);
        cout << "|  Collected Golden Wheels:" << wheels << "!|" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 11);
        cout << "|  Collected Trophies:" << trophy << "!     |" << endl;
        getxycoordinates(WIN_WIDTH / 2-13, 13); 
         cout << "|  Time: " << static_cast<int>(elapsedSeconds.count()) << "s                  |" << endl;
         getxycoordinates(WIN_WIDTH / 2 - 13, 15);
         cout << "|  Score: " << currentScore <<"                  |" << endl;
        
        getxycoordinates(WIN_WIDTH / 2 - 13, 17);
        cout << "'----------------------------'" << endl;

        setColor(12); // Set color back to white
        getxycoordinates(WIN_WIDTH / 2 - 18, 20);
        cout << "n->Next Level    any key->exit.";

        //saving data to file
        saveLeaderboard(currentLevel, coins, wheels, trophy, static_cast<int>(elapsedSeconds.count()), currentScore);
        currentLevel++;
        if (currentLevel >= 4) {
            displayCongratulations();
        }
        currentScore = 0;

        char key = _getch();
        
        if (key != 'n') {
           // exit(0);
           // _getch(); // Exit the game
            return 'x';
        }
        return key;
    }


    //function to display the game over when score is less then 0
    void displayGameOver() {
        system("cls"); // Clear the console
        drawBorder();

        
        cout << endl << endl << endl << endl;
        Beep(400, 1000);
        getxycoordinates(WIN_WIDTH / 2 - 5, 4);
        cout << ".-'--`-._\n";
        getxycoordinates(WIN_WIDTH / 2 - 5, 5);//display ascii car
        cout << "'-X---X--'\n";

        // Set position for the game over message
        setColor(12);
        getxycoordinates(WIN_WIDTH / 2 - 13, 7);
        cout << ".-----------------------------." << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 8);
        cout << "|          Game Over          |" << endl;// Display game over text
        getxycoordinates(WIN_WIDTH / 2 - 13, 9);
        cout << ":-----------------------------:" << endl;

        setColor(3); // Set color
        getxycoordinates(WIN_WIDTH / 2 - 13, 11); // Set position for the message
        cout << "| Press any key to exit.      |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 12);
        cout << "'-----------------------------'" << endl;

        Beep(400, 800);
        // Wait for any key press
        _getch();
        exit(0); // Exit the console
    }





    Node* getTarget() const {
        return target;
    }

    Node* getCarPosition() const {
        return maze[car->positionX][car->positionY];
    }




    // Function to display the front page menu
    void displayMenu() {
        system("cls"); // Clear the console

        drawBorder(); //dawing border
        

        cout << endl << endl << endl << endl;


      
        Beep(400, 800);
        getxycoordinates(WIN_WIDTH/2 -5, 4);
        cout << ".-'--`-._\n";
        getxycoordinates(WIN_WIDTH / 2-5, 5);  //drawing ascci car
            cout<<"'-O---O--'\n";

       // Set position for the menu
        setColor(9);
        getxycoordinates(WIN_WIDTH/2-13 , 7);
        cout << ".----------------------." << endl;
        getxycoordinates(WIN_WIDTH/2 - 13, 8);
        cout << "|      Car   Field     |" << endl;
        getxycoordinates(WIN_WIDTH/2 - 13, 9);
        cout << ":----------------------:" << endl;

        
        setColor(3); // Set color 
        getxycoordinates(WIN_WIDTH/2 - 13, 10); // Set position for the menu
        cout << "|1. Start Game         |" << endl;
        getxycoordinates(WIN_WIDTH/2 - 13, 11);
        cout << "|2. Automatic Mode     |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 12);
        cout << "|3. LeaderBoard        |" << endl;
        getxycoordinates(WIN_WIDTH/2 - 13, 13);
        cout << "|4. Instructions       |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 14);
        cout << "|5. Clear LeaderBoard  |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 15);
        cout << "|6.Exit                |" << endl;
        getxycoordinates(WIN_WIDTH/2 - 13, 16);
        cout << "'----------------------'" << endl;

        getxycoordinates(WIN_WIDTH/2 - 13, 18); // Set position for the menu
        setColor(6); // Set color 
        cout << "Enter your choice (1-6): ";
    }
    // Function to display game instructions
    void displayInstructions() {
        system("cls");
        drawBorder();
        Beep(400, 800);
        cout << endl << endl << endl << endl;

        setColor(9);
        getxycoordinates(WIN_WIDTH / 2 - 13, 0);
        cout << ".---------------------------." << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 1);
        cout << "|        Instructions       |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 2);
        cout << ":---------------------------:" << endl;
        setColor(3); // Set color
        getxycoordinates(WIN_WIDTH / 2 - 13, 3);
        cout << "| Use arrow keys to control |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 4);
        cout << "| the car.                  |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 5);
        cout << "|                           |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 6);
        cout << "| Collect coins and trophies|" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 7);
        cout << "| to score.                 |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 8);
        cout << "|                           |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 9);
        cout << "| Avoid obstacles:          |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 10);
        cout << "| - Water waves:   -2 pts   |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 11);
        cout << "| - Bump:          -5 pts   |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 12);
        cout << "| - Dig Pit:      -10 pts   |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 13);
        cout << "|                           |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 14);
        cout << "| Collective Items  :       |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 15);
        cout << "| - Coins:          5 pts   |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 16);
        cout << "| - Golden Wheel:   10 pts  |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 17);
        cout << "| - Trophy:         20 pts  |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 18);
        cout << "|                           |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 19);
        cout << "| if score < 0 GAME OVER    |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 20);
        cout << "'---------------------------'" << endl;

        setColor(6); 
        getxycoordinates(WIN_WIDTH / 2 - 13, 21);
        cout << "Press any key to go back.";
        _getch();
    }


    //function tp display congratulations if player completes all levels
    void displayCongratulations() {
        system("cls"); // Clear the console
        drawBorder();

        Beep(400, 800);
        cout << endl << endl << endl << endl;
        Beep(400, 1000);
        getxycoordinates(WIN_WIDTH / 2 - 5, 4);
        cout << ".-'--`-._\n";
        getxycoordinates(WIN_WIDTH / 2 - 5, 5);
        cout << "'-O---O--'\n";

        // Set position for the congratulatory message
        setColor(9);
        getxycoordinates(WIN_WIDTH / 2 - 13, 7);
        cout << ".-----------------------------." << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 8);
        cout << "| Congratulations! You Won!   |" << endl;// Display congratulatory text
        getxycoordinates(WIN_WIDTH / 2 - 13, 9);
        cout << "|    All Levels Completed     |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 10);
        cout << ":-----------------------------:" << endl;

        setColor(3); // Set color
        getxycoordinates(WIN_WIDTH / 2 - 13, 12); // Set position for the message
        cout << "| Press any key to exit.      |" << endl;
        getxycoordinates(WIN_WIDTH / 2 - 13, 13);
        cout << "'-----------------------------'" << endl;

        // Wait for any key press
        _getch();
        exit(0); // Exit the console
    }

    //function fot he game loop
    void playGame() {
        startTime = chrono::steady_clock::now(); // Start the timer
        currentLevel = 1;
        bool exitGame = false;
        //entering the game loop
        while (!exitGame) {
            //placing the items to collect
            PlaceItems();
            //displayingn the maze
            displayMaze();

            // Capture arrow key input
            int key = _getch();
            if (key == 224) {
                // Special keys, e.g., arrow keys on Windows
                key = _getch();
            }

            // Move the car
            moveCar(key);
            //if there is an obstcle at car positon
            processObstacles();

         

            // Check if the car reached the goal (winning condition)
            if (getTarget() == getCarPosition()) {
                char nextLevelKey = displayWinScreen();

                if (nextLevelKey == 'n') {
                    // Update the maze for the next level
                    updateMaze(currentLevel);
                }
                else {
                    // Exit the game loop
                    exitGame = true;
                }

                startTime = chrono::steady_clock::now(); // Restart the timer for the next level
            }

            endTime = chrono::steady_clock::now(); // Stop the timer
        }
        Grid newgame;
       newgame.startGame();
    }


    //function to play automatic mode in which shortest path is shown
    void playAutoGame() {
        startTime = chrono::steady_clock::now(); // Start the timer
        currentLevel = 1;
        bool exitGame = false;

        while (!exitGame) {
            PlaceItems();
            findShortestPath();
            displayMaze();

            // Capture arrow key input
            int key = _getch();
            if (key == 224) {
                // Special keys, e.g., arrow keys on Windows
                key = _getch();
            }

            processObstacles();

            // Move the car
            moveCar(key);

            // Check if the car reached the goal (winning condition)
            if (getTarget() == getCarPosition()) {
                char nextLevelKey = displayWinScreen();

                if (nextLevelKey == 'n') {
                    // Update the maze for the next level
                    updateMaze(currentLevel);
                    findShortestPath();
                }
                else {
                    // Exit the game loop
                    exitGame = true;
                }

                startTime = chrono::steady_clock::now(); // Restart the timer for the next level
            }

            endTime = chrono::steady_clock::now(); // Stop the timer
        }
        Grid newgame;
        newgame.findShortestPath();
        newgame.startGame();
    }


    //the very first function of our game which will be called in main
    void startGame() {
        while (true) {
            //displaying menu

            displayMenu();

            char choice = _getch();

            switch (choice) {
            case '1':
                playGame();
                break;
            case '2':
                
                playAutoGame();
                break;
            case '3':
                displayLeaderboard();
                break;
            case '4':
                displayInstructions();
                break;
            case '5':
                clearLeaderboard();
                break;
            case '6':
                exit(0);
                break;
            default:
                startGame();
                break;
            }
        }
    }




  
  //finding the shortest path in our graph
    void findShortestPath() {
        using ournodepair = pair<int, Node*>;
        priority_queue<ournodepair, vector<ournodepair>, greater<ournodepair>> pq;


        // Initialize distances to infinity
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                maze[i][j]->distance = INT_MAX;
            }
        }

        // Mark start distance as 0
        start->distance = 0;

        // Enqueuing the start node with distance 0
        pq.push({ 0, start });

        while (!pq.empty()) {
            Node* current = pq.top().second;
            int currentDistance = pq.top().first;
            pq.pop();

            //  connected nodes
            for (int i = 0; i < 4; ++i) {
                Node* neighbor = current->cn[i];
                if (neighbor != nullptr && !neighbor->iswall) {
                    int newDistance = currentDistance + 1;

                    // If a shorter path is found, update the distance
                    if (newDistance < neighbor->distance) {
                        neighbor->distance = newDistance;
                        neighbor->parent = current;

                        // Enqueuing the neighbor with the updated distance
                        pq.push({ newDistance, neighbor });
                    }
                }
            }
        }

        // Tracing back the path from target to start
        Node* current = target;
        while (current != nullptr) {
            current->isPath = true;
            current = current->parent;
        }
    }






};


//main of our game 
int main() {
    Grid game;

  game.startGame();

   
      return 0;
}


