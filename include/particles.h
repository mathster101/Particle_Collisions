#include<SFML/Graphics.hpp>
#include<utility>      // std::pair
#include<map>

#define GRIDSEG 10
#define DENSITY 0.1


class Particle{
public:
    Particle(int screen_height, int screen_width);
    void updatePosition();
    void drawToScreen(sf::RenderWindow &window);
    float x,y;
    float vx,vy;
    float box_w, box_h;
private:
    float radius;
    float mass;
    sf::CircleShape circle;
};

class Box{
public:
    Box(int screen_height, int screen_width);
    std::pair<int, int> getGridcoord(float x, float y);
    void updateGridmap(Particle* p);
    std::vector<Particle*> getGridnbrs(float x, float y);
    void collisionUpdate();
private:
    int height, width;
    std::map<std::pair<int,int>, std::vector<Particle*>> gridMap;
};

