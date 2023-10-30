#include "../include/particles.h"
#include<SFML/Graphics.hpp>
#include<time.h>
#include<iostream>

#define HEIGHT 1600
#define WIDTH 1600
#define FPS 90
#define PARTCOUNT 100
#define SUBSTEPS 1
void test2()
{
    Box b(HEIGHT, WIDTH);
    for(int i = 0; i < PARTCOUNT; i++)
    {
        Particle *p = new Particle(HEIGHT, WIDTH);
        b.particleList.push_back(p);
    }
    b.particleList[0]->circle.setFillColor(sf::Color(100, 250, 50));
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

void test3()
{
    Box b(HEIGHT, WIDTH);

    Particle *p = new Particle(HEIGHT, WIDTH);
    p->pos = sf::Vector2f(100,100);
    p->id = 1;
    p->vel.x = 200;
    p->vel.y = 0;
    p->radius = 25;
    p->mass = 100;
    p->circle = sf::CircleShape(p->radius);
    p->circle.setOrigin(p->radius, p->radius);
    b.particleList.push_back(p);
    
    p = new Particle(HEIGHT, WIDTH);
    p->pos = sf::Vector2f(200,100);
    p->id = 2;
    p->vel.x = 0;
    p->vel.y = 0;
    p->radius = 25;
    p->mass = 100;
    p->circle = sf::CircleShape(p->radius);
    p->circle.setOrigin(p->radius, p->radius);
    b.particleList.push_back(p);

    p = new Particle(HEIGHT, WIDTH);
    p->pos = sf::Vector2f(400,100);
    p->id = 3;
    p->vel.x = 0;
    p->vel.y = 0;
    p->radius = 25;
    p->mass = 100;
    p->circle = sf::CircleShape(p->radius);
    p->circle.setOrigin(p->radius, p->radius);
    b.particleList.push_back(p);

    b.particleList[0]->circle.setFillColor(sf::Color::Green);
    b.particleList[1]->circle.setFillColor(sf::Color::Magenta);
    sf::RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "Particles");
    window.setFramerateLimit(FPS);
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

        for(auto p : b.particleList)
        {
            p->updatePosition(1.0F/FPS);
        }
        b.collisionUpdate(1.0F/FPS);
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