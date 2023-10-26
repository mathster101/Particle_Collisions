#include "../include/particles.h"
#include<SFML/Graphics.hpp>
#include<time.h>
#include<iostream>

#define HEIGHT 1900
#define WIDTH 1900
#define FPS 60

void test1()
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
    b.collisionUpdate();
    b.dbgMap();
}

void test2()
{
    Box b(HEIGHT, WIDTH);
    for(int i = 0; i < 2000; i++)
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

        for(auto p : b.particleList)
        {
            p->updatePosition(1.0/FPS);
        }
        for(auto p : b.particleList)
        {
            p->drawToScreen(window);
        }
        b.collisionUpdate();
        b.calcKinetic();
        window.display();
    }  
}

void test3()
{
    Box b(HEIGHT, WIDTH);

    Particle *p = new Particle(HEIGHT, WIDTH);
    p->x = 100;
    p->y = 100;
    p->vx = 200;
    p->vy = 0;
    p->radius = 25;
    p->mass = 100;
    p->circle = sf::CircleShape(p->radius);
    b.particleList.push_back(p);
    
    p = new Particle(HEIGHT, WIDTH);
    p->x = 220;
    p->y = 100;
    p->vx = 10;
    p->vy = 0;
    p->radius = 25;
    p->mass = 50;
    p->circle = sf::CircleShape(p->radius);
    b.particleList.push_back(p);

    p = new Particle(HEIGHT, WIDTH);
    p->x = 280;
    p->y = 100;
    p->vx = 30;
    p->vy = 0;
    p->radius = 25;
    p->mass = 50;
    p->circle = sf::CircleShape(p->radius);
    b.particleList.push_back(p);

    b.particleList[0]->circle.setFillColor(sf::Color(100, 250, 50));
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
        for(auto p : b.particleList)
        {
            p->drawToScreen(window);
        }
        b.collisionUpdate();
        //b.calcKinetic();
        window.display();
    }     
}


// void loop()
// {
//     Particle p(HEIGHT, WIDTH);
//     sf::RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "Particles");
//     window.setFramerateLimit(60);
//     //window.setVerticalSyncEnabled(1);
//     while (window.isOpen())
//     {
//         window.clear();
//         sf::Event event;
//         while (window.pollEvent(event))
//         {
            
//             if (event.type == sf::Event::Closed)
//                 window.close();
            
//         }

//         p.updatePosition();
//         p.drawToScreen(window);


//         window.display();
//     }    
// }


int main(){

    srand (time(NULL));
    test3();
    return 0;
}