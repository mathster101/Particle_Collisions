#include "../include/particles.h"
#include<SFML/Graphics.hpp>
#include<time.h>
#include<iostream>

#define HEIGHT 1600
#define WIDTH 1600
#define FPS 90
#define PARTCOUNT 2000
#define SUBSTEPS 2
void test2()
{
    Box b(HEIGHT, WIDTH);
    for(int i = 0; i < PARTCOUNT; i++)
    {
        Particle *p = new Particle(HEIGHT, WIDTH);
        b.particleList.push_back(p);
    }
    b.particleList[0]->set_color(sf::Color(100, 250, 50));
    sf::RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "Particles");
    window.setFramerateLimit(FPS);
 
    while (window.isOpen())
    {
        window.clear();
        sf::Event event;
        while (window.pollEvent(event))
        {
            
            if (event.type == sf::Event::Closed)
                window.close();
            
        }

        for(int ss = 0; ss < SUBSTEPS; ss++)
        {
            for(auto p : b.particleList)
            {
                p->updatePosition((1.0/FPS)/SUBSTEPS);
            }
            b.collisionUpdate((1.0/FPS)/SUBSTEPS);
        }
        //b.dbgMap();
        for(auto p : b.particleList)
        {
            p->drawToScreen(window);
        }
        
        //b.calcKinetic();
        window.display();
    }  
}



int main(){

    srand (time(NULL));
    test2();
    return 0;
}

