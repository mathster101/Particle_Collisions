#include<SFML/Graphics.hpp>
#include<utility>      // std::pair
#include<map>
#include<set>
#include<cstdlib>
#include<cmath>


#define GRIDSEG 20
#define DENSITY 0.1
#define DAMPING 0.9F

class Particle{
private:
    int id;
    sf::Vector2f pos;
    sf::Vector2f vel;
    sf::Vector2f accel;
    float box_w, box_h;
    int radius;
    int mass;
    sf::CircleShape circle;
public:
    Particle(int screen_height, int screen_width);

    void set_pos(sf::Vector2f new_pos){pos = new_pos;};
    void set_vel(sf::Vector2f new_vel){vel = new_vel;};
    void set_accel(sf::Vector2f new_accel){accel = new_accel;};
    void set_color(sf::Color color){circle.setFillColor(color);}
    
    sf::Vector2f get_pos(){ return pos;};
    sf::Vector2f get_vel(){return vel;};
    sf::Vector2f get_accel(){return accel;};
    int get_mass(){return mass;};
    int get_radius(){return radius;};
    void updatePosition(float dt);
    void drawToScreen(sf::RenderWindow &window);
};

class Box{
public:
    Box(int screen_height, int screen_width);
    std::pair<int, int> getGridcoord(sf::Vector2f position);
    void updateGridmap(Particle* p);
    std::vector<Particle*> getGridnbrs(int x, int y);
    void collisionUpdate(float dt);
    void calcKinetic();
    void dbgMap();
    std::vector<Particle*> particleList;
private:
    int height, width;
    std::map<std::pair<int,int>, std::vector<Particle*>> gridMap;
};

