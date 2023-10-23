#include "../include/particles.h"
#include<cstdlib>
#include<cmath>
#include<iostream>

Particle::Particle(int screen_height, int screen_width)
{
    x = rand()%screen_width;
    y = rand()%screen_height;
    vx = rand()%10;
    vy = rand()%10; 
    radius = rand()%20;
    mass = pow(radius, 3) * DENSITY;

    box_h = screen_height;
    box_w = screen_width;

    circle = sf::CircleShape(radius);//need to set color
}

void Particle::updatePosition()
{
    float x_new = x + vx;
    float y_new = y + vy;

    //wall checks
    if(x_new > box_w)
    {
        vx = -1 * vx;
        x_new = box_w - (x_new - box_w);
    }
    if(x_new < 0)
    {
        vx = -1 * vx;
        x_new *= -1;
    }
    if(y_new > box_h)
    {
        vy = -1 * vy;
        y_new = box_h - (y_new - box_h);
    }
    if(y_new < 0)
    {
        vy = -1 * vy;
        y_new *= -1;
    }

    x = x_new;
    y = y_new;

}

void Particle::drawToScreen(sf::RenderWindow &window)
{
    circle.setPosition(x,y);
    window.draw(circle);
}

/********************************************************************/
Box::Box(int screen_height, int screen_width)
{
    //todo
}

std::pair<int, int> Box::getGridcoord(float x, float y)
{//maps pixel value to grid box;
}

void Box::updateGridmap(Particle* p)
{

}

std::vector<Particle*> Box::getGridnbrs(float x, float y)
{
    
}

void Box::collisionUpdate()
{
}
