#ifndef MAP_H
#define MAP_H

#include <map>
#include <random>

class Map
{
    public:
        Map(int width, int height);
        ~Map();

        void createGenerator();

        void generateNoize(int minValue, int maxValue);
        void generateGaussianNoize();
        void generateGradiant(int startValue, int stopValue, int startLine, int stopLine);
        void generateCellularAutomata(int step);

        int get(int index);
        void set(int index, int value);
        int getMapSize();
        int getWidth();
        int getHeight();
        void insert(std::pair<int,int>);
    protected:
        int getLine(int index);
        bool isEdge(int index);
        bool isNorthEdge(int index);
        bool isSouthEdge(int index);
        bool isWestEdge(int index);
        bool isEastEdge(int index);

        void configureUniformRandomGenerator(int minValue, int maxValue);
        int random();
    private:
        int width, height;
        int mapSize;

        std::default_random_engine generator;
        std::uniform_int_distribution<int> uniformIntDistribution;
        std::map<int,int> data;
};
#endif // MAP_H
