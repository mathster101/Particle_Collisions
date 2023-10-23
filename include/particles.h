#include<SFML/Graphics.hpp>
#include<utility>      // std::pair


class Particle{
public:
    Particle();
    std::pair<float, float> getCoords();
    std::pair<float, float> getVelocities();
    void updatePosition();
    void drawToScreen(sf::RenderWindow);
    
private:
    float x,y;
    float vx,vy;
    float radius;
    float mass;
};

std::pair<int, int> getGridcoord(float x, float y);
void updateGridmap(std::map<std::pair<int,int>, std::vector<Particle*>>, Particle*);