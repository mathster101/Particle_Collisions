#include "../include/particles.h"
#include<SFML/Graphics.hpp>
#include<time.h>
#include<iostream>

#define HEIGHT 1000
#define WIDTH 1000

void test()
{
    Box b(HEIGHT, WIDTH); 
    Particle a1(HEIGHT, WIDTH);
    Particle a2(HEIGHT, WIDTH);
    a1.x = 100;
    a1.y = 100;
    a2.x = 101;
    a2.y = 102;
    b.particleList.push_back(&a1);
    b.particleList.push_back(&a2);
    b.dbgMap();
    b.collisionUpdate();
}


void loop()
{
    Particle p(HEIGHT, WIDTH);
    sf::RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "Particles");
    window.setFramerateLimit(60);
    //window.setVerticalSyncEnabled(1);
    while (window.isOpen())
    {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed)
                window.close();
            
        }

        p.updatePosition();
        p.drawToScreen(window);


        window.display();
    }    
}


int main(){

    srand (time(NULL));
    test();
    return 0;
}