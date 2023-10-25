#include<SFML/Graphics.hpp>
#include<utility>      // std::pair
#include<map>
#include<set>
#include<cstdlib>
#include<cmath>


#define GRIDSEG 5
#define DENSITY 0.1


class Particle{
public:
    Particle(int screen_height, int screen_width);
    void updatePosition();
    void drawToScreen(sf::RenderWindow &window);
    float x,y;
    float vx,vy;
    float box_w, box_h;
    int radius;
    int mass;
//private:

    sf::CircleShape circle;
};

class Box{
public:
    Box(int screen_height, int screen_width);
    std::pair<int, int> getGridcoord(int x, int y);
    void updateGridmap(Particle* p);
    std::vector<Particle*> getGridnbrs(int x, int y);
    void collisionUpdate();
    void calcKinetic();
    void dbgMap();
    std::vector<Particle*> particleList;
private:
    int height, width;
    std::map<std::pair<int,int>, std::vector<Particle*>> gridMap;
};

