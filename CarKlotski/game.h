#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <fstream>

//map size
#define MAPX 6
#define MAPY 6
//car direction
#define DIRECTION_ROW 1
#define DIRECTION_COL 2
//move direction
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_LEFT 3
#define MOVE_RIGHT 4

struct CheatStep
{
    int x;
    int y;
    int direction;
    int step;
    CheatStep(int x, int y, int direction,int step)
    {
        this->x = x;
        this->y = y;
        this->direction = direction;
        this->step = step;
    }
};

struct MapRecord
{
    unsigned char map[MAPY][MAPX];
    std::vector<CheatStep> steps;
};

struct BFSRecord
{
    unsigned char map[MAPY][MAPX];
    bool operator==(unsigned char m[MAPY][MAPX])
    {
        bool ifSame = true;
        for (int i = 0; i < MAPY; i++)
            for (int j = 0; j < MAPX; j++)
                if (map[i][j] != m[i][j])
                    ifSame = false;
        return ifSame;
    }
    BFSRecord(unsigned char m[MAPY][MAPX])
    {
        for (int i = 0; i < MAPY; i++)
            for (int j = 0; j < MAPX; j++)
                map[i][j] = m[i][j];
    }
};

struct Car
{
    int x;
    int y;
    int direction;
    int length;
};

class game
{
private:
    unsigned char map[MAPY][MAPX];
    int nowMaxNumber;
    //maplist
    std::vector<MapRecord> mapList;
    //bool bfs(int limit);
public:
    game();
    void initGame(int level = -1);
    void initCrazy();
    void loadMap();
    bool isCar(int x,int y);
    int getCarNumber(int x, int y);
    int getCarLength(int x, int y);
    int getCarDirection(int x,int y);//1 row 2 col
    std::vector<Car> getCarList();
    bool ifGameComplete();//if game complete
    bool movable(int x, int y, int direction,int step);
    void move(int x, int y, int direction, int step);
    bool addable(int x, int y, int direction,int length);
    bool add(int x, int y, int direction, int length);
    friend std::ostream& operator<<(std::ostream &out, game &g);
    std::vector<MapRecord>& getMapList();
};
