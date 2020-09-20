#include "game.h"
/*
bool game::bfs(int limit)
{
    std::vector<CheatStep> res;
    unsigned char mapr[MAPY][MAPX];
    for (int i = 0; i < MAPY; i++)
        for (int j = 0; j < MAPX; j++)
            mapr[i][j] = this->map[i][j];
    std::queue<BFSRecord> que;
    que.push(BFSRecord(map));
    std::vector<BFSRecord> vis;
    while (!que.empty())
    {
        BFSRecord now = que.back();
        for (int i = 0; i < MAPY; i++)
            for (int j = 0; j < MAPX; j++)
                map[i][j] = now.map[i][j];
        que.pop();
        std::vector<Car> carList = getCarList();
        std::vector<int> gp;
        srand(time(0));
        for (int i = 0; i < carList.size();)
        {
            int r = rand() % carList.size();
            bool isIn = false;
            for (int k = 0; k < gp.size(); k++)
                if (gp[k] == r)
                    isIn = true;
            if (!isIn)
            {
                i++;
                gp.push_back(r);
            }
        }
        int i;
        for (int ii = 0; ii < carList.size(); ii++)
        {
            i = gp[ii];
            for (int d = 1; d <= 4; d++)
            {
                for (int s = 1; s <= 4; s++)
                {
                    if (movable(carList[i].x, carList[i].y, d, s))
                    {
                        //try
                        move(carList[i].x, carList[i].y, d, s);
                        //if vis
                        bool ifVis = false;
                        for (int i = 0; i < vis.size(); i++)
                        {
                            if (vis[i] == map)
                                ifVis = true;
                        }
                        if (!ifVis)
                        {
                            if (ifGameComplete())
                            {
                                for (int i = 0; i < MAPY; i++)
                                    for (int j = 0; j < MAPX; j++)
                                        map[i][j] = mapr[i][j];
                                return true;
                            }
                            //add to que
                            if (limit>0)
                            {
                                BFSRecord tp(map);
                                que.push(tp);
                                limit--;
                            }
                        }
                        //recover
                        for (int i = 0; i < MAPY; i++)
                            for (int j = 0; j < MAPX; j++)
                                map[i][j] = now.map[i][j];
                    }
                }
            }
        }
        vis.push_back(now);
    }
    //recover map
    for (int i = 0; i < MAPY; i++)
        for (int j = 0; j < MAPX; j++)
            map[i][j] = mapr[i][j];
    return false;
}
*/
game::game()
{
    initGame();
}

void game::initGame(int level)
{
    //init map
    for (int i = 0; i < MAPX; i++)
    {
        for (int j = 0; j < MAPY; j++)
        {
            map[i][j] = 0;
        }
    }
    nowMaxNumber = 0;
    //init car
    if (level >= 0 && level < mapList.size())
    {
        for (int i = 0; i < MAPY; i++)
        {
            for (int j = 0; j < MAPX; j++)
            {
                map[i][j] = mapList[level].map[i][j];
            }
        }
    }
}

void game::initCrazy()
{
    srand(time(0));
    int level = rand() % mapList.size();
    initGame(level);
    int step = rand() % 50 + 50;
    for (int i = 0; i < step; i++)
    {
        std::vector<Car> carList = getCarList();
        int car = rand() % carList.size();
        int direction = rand() % 100 < 50 ? (rand() % 100 < 50 ? MOVE_UP : MOVE_DOWN) : (rand() % 100 < 50 ? MOVE_LEFT : MOVE_RIGHT);
        int step = rand() % (MAPY - 2) + 1;
        if (movable(carList[car].x, carList[car].y, direction, step))
        {
            move(carList[car].x, carList[car].y, direction, step);
            i++;
        }
    }
}


/*
void game::initCrazy(int carCounts,int limit)
{
    srand(time(0));
    bool ifGetIt = false;
    do
    {
        initGame(-1);
        add(0, 2, DIRECTION_ROW, 2);
        int carAdded = 0;
        for (carAdded = 0; carAdded < carCounts; carAdded++)
        {
            bool ifAdded = false;
            try 100 times to add a car
            for (int k = 0; !ifAdded && k < 100; k++)
            {
                int x, y, direction, length;
                y = rand() % MAPY;
                x = rand() % MAPX;
                direction = rand() % 2 + 1;
                length = rand() % 2 + 2;
                if (addable(x, y, direction, length))
                {
                    add(x, y, direction, length);
                    ifAdded = true;
                }
            }
            if (!ifAdded)
                break;
        }
        int crazyCount = 0;
        for (int i = 2; i < MAPX; i++)
        {
            if (map[2][i] != 0)
                crazyCount++;
        }
        if (crazyCount < 3)
            continue;
        if (carAdded != carCounts || ifGameComplete() || !bfs(limit))
            continue;
        else ifGetIt = true;
    } while (!ifGetIt);
}
*/

void game::loadMap()
{
    std::ifstream file("level.data");
    int countLevel,x,y,direction,step;
    file >> countLevel;
    for (int i = 0; i < countLevel; i++)
    {
        MapRecord temp;
        int x;
        for (int m = 0; m < MAPY; m++)
        {
            for (int n = 0; n < MAPX; n++)
            {
                file >> x;
                temp.map[m][n] = x;
            }
        }
        int stepCount;
        file >> stepCount;
        for (int j = 0; j < stepCount; j++)
        {
            file >> x >> y >> direction >> step;
            temp.steps.push_back(CheatStep(x, y, direction, step));
        }
        mapList.push_back(temp);
    }
}

bool game::isCar(int x, int y)
{
    return map[y][x] != 0;
}

int game::getCarDirection(int x, int y)
{
    if (map[y][x] == 0)
        return -1;
    else return map[y][x] / 100;
}

std::vector<Car> game::getCarList()
{
    std::vector<Car> cars;
    cars.resize(nowMaxNumber);
    for (int i = 0; i < MAPY; i++)
    {
        for (int j = 0; j < MAPX; j++)
        {
            if (getCarNumber(j, i) != -1)
            {
                while (cars.size() <= getCarNumber(j, i))
                    cars.push_back(Car());
                if (cars[getCarNumber(j, i)].length != 0)
                    continue;
                cars[getCarNumber(j, i)].x = j;
                cars[getCarNumber(j, i)].y = i;
                cars[getCarNumber(j, i)].direction = getCarDirection(j, i);
                cars[getCarNumber(j, i)].length = getCarLength(j, i);
            }
        }
    }
    return cars;
}

int game::getCarLength(int x, int y)
{
    if (map[y][x] == 0)
        return -1;
    else return (map[y][x] - getCarDirection(x, y) * 100)/10;
}

int game::getCarNumber(int x, int y)
{
    if (map[y][x] == 0)
        return -1;
    else return map[y][x] - getCarDirection(x, y) * 100 - getCarLength(x, y) * 10;
}

bool game::ifGameComplete()
{
    bool ifComplete = false;
    for (int j = MAPX - 1; j > 0; j--)
    {
        if (map[2][j] != 0)
        {
            if (map[2][j] == 121)//get game!
                ifComplete = true;
            break;
        }
    }
    return ifComplete;
}

bool game::movable(int x, int y, int direction,int step)
{
    if (getCarNumber(x, y) != -1)//there is a car
    {
        //not the same direction
        if ((direction == MOVE_UP || direction == MOVE_DOWN) && getCarDirection(x, y) == DIRECTION_ROW)
            return false;
        if ((direction == MOVE_LEFT || direction == MOVE_RIGHT) && getCarDirection(x, y) == DIRECTION_COL)
            return false;
        //judge
        switch (direction)
        {
        case MOVE_UP:
            while (map[y][x] == map[y - 1][x])
                y--;
            for (int t = y - 1; t >= 0 && map[t][x] == 0 && step != 0; t--)
                step--;
            if (step == 0)
                return true;
            break;
        case MOVE_DOWN:
            while (map[y][x] == map[y + 1][x])
                y++;
            for (int t = y + 1; t < MAPY && map[t][x] == 0 && step != 0; t++)
                step--;
            if (step == 0)
                return true;
            break;
        case MOVE_LEFT:
            while (map[y][x] == map[y][x - 1])
                x--;
            for (int t = x - 1; t >= 0 && map[y][t] == 0 && step != 0; t--)
                step--;
            if (step == 0)
                return true;
            break;
        case MOVE_RIGHT:
            while (map[y][x] == map[y][x + 1])
                x++;
            for (int t = x + 1; t < MAPX && map[y][t] == 0 && step != 0; t++)
                step--;
            if (step == 0)
                return true;
            break;
        }
    }
    return false;
}

void game::move(int x, int y, int direction, int step)
{
    if (movable(x, y, direction, step))
    {
        int length = getCarLength(x, y);
        switch (direction)
        {
        case MOVE_UP:
            while (map[y][x] == map[y - 1][x])
                y--;
            for (int i = 0; i < length; ++i)
            {
                map[y - step + i][x] = map[y + i][x];
                map[y + i][x] = 0;
            }
            break;
        case MOVE_DOWN:
            while (map[y][x] == map[y + 1][x])
                y++;
            for (int i = 0; i < length; ++i)
            {
                map[y + step - i][x] = map[y - i][x];
                map[y - i][x] = 0;
            }
            break;
        case MOVE_LEFT:
            while (map[y][x] == map[y][x - 1])
                x--;
            for (int i = 0; i < length; ++i)
            {
                map[y][x - step + i] = map[y][x + i];
                map[y][x + i] = 0;
            }
            break;
        case MOVE_RIGHT:
            while (map[y][x] == map[y][x + 1])
                x++;
            for (int i = 0; i < length; ++i)
            {
                map[y][x + step - i] = map[y][x - i];
                map[y][x - i] = 0;
            }
            break;
        }
    }
}

bool game::addable(int x, int y, int direction, int length)
{
    bool ifAddable = true;
    switch (direction)
    {
    case DIRECTION_ROW:
        for (int i = 0; i < length; i++)
            if (x + i >= MAPX || map[y][x + i] != 0)
                ifAddable = false;
        break;
    case DIRECTION_COL:
        for (int i = 0; i < length; i++)
            if (y + i >=MAPY || map[y + i][x] != 0)
                ifAddable = false;
        break;
    }
    return ifAddable;
}

bool game::add(int x, int y, int direction, int length)
{
    if (length <10 && addable(x, y, direction, length))
    {
        nowMaxNumber++;
        if (nowMaxNumber >= 10)
            return false;
        int code = direction * 100 + length * 10 + nowMaxNumber;
        switch (direction)
        {
        case DIRECTION_ROW:
            for (int i = 0; i < length; i++)
                map[y][x + i] = code;
            break;
        case DIRECTION_COL:
            for (int i = 0; i < length; i++)
                map[y + i][x] = code;
            break;
        }
    }
    else return false;
    return true;
}

std::vector<MapRecord>& game::getMapList()
{
    return mapList;
}

std::ostream & operator<<(std::ostream & out, game & g)
{
    for (int i = 0; i < MAPY; ++i)
    {
        for (int j = 0; j < MAPX; ++j)
        {
            int dir = g.getCarDirection(j, i);
            int length = g.getCarLength(j, i);
            int number = g.getCarNumber(j, i);
            dir = dir == -1 ? 0 : dir;
            length = length == -1 ? 0 : length;
            number = number == -1 ? 0 : number;
            out << std::setw(6) << dir * 100 + length * 10 + number;
        }
        out << std::endl << std::endl;
    }
    return out;
}
