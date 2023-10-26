#include<SFML/Graphics.hpp>
#include<utility>      // std::pair
#include<map>
#include<set>
#include<cstdlib>
#include<cmath>


#define GRIDSEG 7
#define DENSITY 0.1
#define DAMPING 1.0f

class Particle{
public:
    Particle(int screen_height, int screen_width);
    void updatePosition(float dt);
    void drawToScreen(sf::RenderWindow &window);
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f accel;
    float box_w, box_h;
    int radius;
    int mass;
    sf::CircleShape circle;
};

class Box{
public:
    Box(int screen_height, int screen_width);
    std::pair<int, int> getGridcoord(sf::Vector2f position);
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

