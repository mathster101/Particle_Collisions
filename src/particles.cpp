#include "../include/particles.h"
#include<iostream>

Particle::Particle(int screen_height, int screen_width)
{
    x = rand()%screen_width;
    y = rand()%screen_height;
    vx = 3 + rand()%6;
    vy = 3 + rand()%6; 
    radius = 20 + rand()%20;
    mass = pow(radius, 3) * DENSITY;

    box_h = screen_height;
    box_w = screen_width;

    circle = sf::CircleShape(radius);//need to set color

    //std::cout<<x<<" "<<y<<"\n";
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
    height = screen_height;
    width = screen_width;
}

std::pair<int, int> Box::getGridcoord(int x, int y)
{
    //maps pixel value to grid box;
    float segSizew = width/GRIDSEG;
    float segSizeh = height/GRIDSEG;

    int xCoord = x/segSizew;
    int yCoord = y/segSizew;

    return std::pair<int, int>(xCoord, yCoord);
}

void Box::updateGridmap(Particle* p)
{
    auto coords = getGridcoord(p->x, p->y);
    if(gridMap.find(coords) == gridMap.end())
        gridMap[coords] = {};
    gridMap[coords].push_back(p);
}

std::vector<Particle*> Box::getGridnbrs(int x, int y)
{
    //get all particles in this grid location
    return gridMap[std::pair<int,int>{x,y}];
    
}

void Box::collisionUpdate()
{
    gridMap.clear();
    for(auto p : particleList)
    {
        updateGridmap(p);
    }

    std::set<Particle*> alreadyHandled;

    for(auto p : particleList)
    {

        //skip if physics already taken care of
        if(alreadyHandled.find(p) != alreadyHandled.end())
            continue;
        
        auto myLoc = getGridcoord(p->x, p->y);
        auto neighbors = getGridnbrs(myLoc.first, myLoc.second);
        for(auto nbr : neighbors)
        {
            //skip if this is current elem or an elem handled already
            if(nbr == p || alreadyHandled.find(nbr) != alreadyHandled.end())
                continue;

            float distance = pow((nbr->x - p->x), 2) + pow((nbr->y - p->y), 2);
            distance = pow(distance, 0.5);
            if(distance <= (nbr->radius + p->radius))
            {
                //collision case
                std::cout<<"Collision!!!\n";
                //elastic collision physics
                int m1 = p->mass;
                int vx1 = p->vx;
                int vy1 = p->vy;

                int m2 = nbr->mass;
                int vx2 = nbr->vx;
                int vy2 = nbr->vy;

                float alpha, beta, gamma;
                alpha = 2 * m1 / (m1 + m2);
                beta = (m1 - m2) / (m1 + m2);
                gamma = 2 * m2 * (m1 + m2);

                //new velocities
                p->vx = beta * vx1 + gamma * vx2;
                p->vy = beta * vy1 + gamma * vy2;
                nbr->vx = alpha * vx1 - beta * vx2;
                nbr->vy = alpha * vy1 - beta * vy2;
                
                alreadyHandled.insert(nbr);
            }
        }
    }
}

void Box::dbgMap()
{
    for(auto entry : gridMap)
    {
        std::cout<<entry.first.first<<" "<<entry.first.second<<" ";
        std::cout<<entry.second.size()<<"\n";
    }
}