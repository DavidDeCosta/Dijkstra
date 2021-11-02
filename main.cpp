//Name: David DeCosta
//Date: 29 October 2021
//File Name: main.cpp
/*Program Description: This program reads in Vertices(Node Names) from a file and the Upper Triangular portion of a cost matrix. It then uses Dijkstra's algorithm
                        to find the shorest path from the source node to each other node in the graph. It displays the total cost and path to that Node. 
*/


#include<iostream>
#include<fstream>
#include <string>
#include <iomanip>
using namespace std;

//------------------------------------------------------------------------------------------------------------
//                                  Global Variables

int costMatrix[100][100]; //holds the values of the cost matrix
bool visited[100] = {false}; //boolen array to keep track if we have visited a city(node) or not
int dist[100]; //holds the cost for a particular index... updates with the smallest distance
int prevNode[100]; //holds the index value of the node a cost came from
string nameArr[100]; // an array to hold all the city names
const int INF = INT_MAX;
int source; //To see what node(city) we will start from


//-------------------------------------------------------------------------------------------------------------
//                                 Prototypes

void init(int size);
int getnearest(int size);
void dijkstra(int size);
void displayMatrix(int size);
void displayPath(int size);


int main(){

    ifstream i_stream;
    int twoDarr[100][100] = {0,0}; // initialized to all zeros .. is used for holding the values from the text file initially
    char filename[20]; //to capture the file name
    int numOfCity;   //to know how many lines have city names (also so we know the number of vertices in our graph)
    int line = 1; //to calculate the lines in the text file
    char ch; 
    char names[5]; //to grab the city names

    cout << " Enter the file name for the Cities and cost matrix: \n";
    cin>> filename;

//------------------------------------------------------------------------------------------------------
//This section of code calculates the number of lines in the file

    i_stream.open(filename);
    i_stream.seekg(0,ios::beg);
    while(i_stream){
        i_stream.get(ch);
        if(ch == '\n'){
            line++;    //if finds new line increases line counter by 1
        }
    }
    numOfCity = line/2 +1;    //will always return the number of lines for the cities(vertices)
    i_stream.close();i_stream.open(filename);

//---------------------------------------------------------------------------------------------------------
//This section of code gets the Names of the vertices and stores the names into a string array

    for(int i = 0; i <numOfCity;i++){
        i_stream.getline(names,5);
        nameArr[i] = names;
    }
//---------------------------------------------------------------------------------------------------------------
//This section of code gets the numbers from the file and stores them into a 2D array called twoDarr
    
    for(int i = 0; i < numOfCity-1; i++){
        for(int j = 0; j < numOfCity; j++){
            i_stream >> twoDarr[i][j];
            if(i_stream.peek() == '\n'){ //when it finds a new line it forces the inner loops condition to be met
                j = numOfCity;
            }
        }
    }
//---------------------------------------------------------------------------------------------------------------------
//This section of code takes the numbers from the twoDarr and creates a cost matrix (2D int array)

    for(int i = 0; i < numOfCity; i++){ //loop to keep moving "inward" from left to right on the matrix
        costMatrix[i][i] = INF;  //makes sure the diagonal is set to INF
        int next = 0;
        for(int j = i+1; j < numOfCity;j++){ //loop to assign the vertical column
            if(twoDarr[i][next] == 0){
                costMatrix[j][i] = INF;
            }
            else{
                costMatrix[j][i] = twoDarr[i][next]; // first time though sets costMatrix[1][0] to first number from file
            }
            next++;
        }
        next  = 0;
        for(int j = i+1; j < numOfCity;j++){  //loop to assign the horizonal row
            if(twoDarr[i][next] == 0){
                costMatrix[i][j] = INF;
            }
            else{
                costMatrix[i][j] = twoDarr[i][next];
            }
            next++;
        }
    }
//---------------------------------------------------------------------------------------------------------------------------

    cout << "Enter the source node (0) for first node (size-1) for last node: \n";
    cin >> source;
    init(numOfCity);
    dijkstra(numOfCity);
    displayMatrix(numOfCity);
    cout << endl << endl;
    displayPath(numOfCity);
    
    i_stream.close();

    return 0;
}

void init(int size){
    for(int i = 0; i < size; i++){
        prevNode[i] = i; // sets each node "coming from" itself
        dist[i] = INF; // sets each index to INF initially
    }
    dist[source] = 0; //wherever our source node is will have a cost of 0 to itself
}

int getnearest(int size){
    int minCost = INF;
    int nearestNode = 0;

    for(int i =0; i < size; i++){
        if(!visited[i] && dist[i] < minCost){ //checks if the node has been visited and if the distance is less than the cost already assigned
            minCost = dist[i];                //if the condition is met the mincost is updated to the cost stored in position i
            nearestNode = i;                       // then the nearestNode is assigned that index
        }
    }
        return nearestNode;  //will initially be 0  then 1 on the second call then 3 then 2 then 4 then 5
}

void dijkstra(int size){
    for(int i = 0; i < size;i++){
        int nearest = getnearest(size);
        visited[nearest] = true;

        for(int j = 0;j < size;j++){
            if(costMatrix[nearest][j] != INF && dist[nearest] + costMatrix[nearest][j] < dist[j]){
                dist[j] = dist[nearest] + costMatrix[nearest][j]; // updates the distance if it is smaller then the previous distance
                prevNode[j] = nearest; // keeps track of the node we traveled from
            }
        }
    }
}

void displayMatrix(int size){
    cout << endl;
    cout << setw(50) << "Adjacency Matrix \n";
    for(int i=0 ;i < size;i++){
    cout<< setw(15)<< nameArr[i]; // displays the city names horizontally
    }
    cout<< endl;
    for(int j = 0; j < size; j++){
        cout << nameArr[j];   //displays the city names vertically
        for(int k = 0; k < size; k++){
            if(k ==0){
                cout << setw(10);
            } 
            else{        
               cout<< setw(15);
            }
            if(costMatrix[j][k] == INF){
            cout << 0; //makes it so the display table shows 0's and not the MAX_INT
            }
            else{
            cout << costMatrix[j][k]; //displays the cost value
            }
        }
        cout << endl;
    }
}

void displayPath(int size){
    for(int i = 0; i <size; i++){
        if(nameArr[source] != nameArr[i]){ //this makes sure it doesn't display the path to itself (which would be 0)
        if(dist[i] != INF){
        cout << "************************************************************************************************************* \n";
        cout << "The distance from "<< nameArr[source] << " to " <<  nameArr[i] << " is " <<  dist[i] << " \n";
        cout << "A shortest Path is: "; 
        cout << nameArr[source] << " ";
        int parNode = prevNode[i];
        while(parNode != source){
            cout << "to " << nameArr[parNode] << " ";
            parNode = prevNode[parNode];
        }
        if(parNode == source){ //makes sure it displays the final destination
            cout << "to " << nameArr[i];
        }
        cout << endl;
    }
    }
    }
}