#include "../include/particles.h"
#include<iostream>
#include<cstdio>

Particle::Particle(int screen_height, int screen_width)
{
    x = rand()%screen_width;
    y = rand()%screen_height;
    vx = -150 + rand()%300;
    vy = -150 + rand()%300; 
    radius = 5 + rand()%5;
    mass = pow(radius, 3) * DENSITY;

    box_h = screen_height;
    box_w = screen_width;

    circle = sf::CircleShape(radius);//need to set color

    //std::cout<<x<<" "<<y<<"\n";
}

void Particle::updatePosition(float dt)
{
    float x_new = x + vx * dt;
    float y_new = y + vy * dt;

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
    circle.setPosition((int)x, (int)y);
    window.draw(circle);
}

/********************************************************************/

float distanceCalc(Particle *p, Particle *nbr)
{
    float d = pow((nbr->x - p->x), 2) + pow((nbr->y - p->y), 2);
    return pow(d + 0.1, 0.5);
}

float keCalc(Particle *p)
{
    float ke = 0.5 * p->mass * (pow(p->vx, 2) + pow(p->vy, 2));
    return ke;
}

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
    std::pair<int, int> coords = getGridcoord(p->x, p->y);

    std::vector<std::pair<int, int>> boxes= {
        coords,
        std::pair<int, int> (coords.first + 1, coords.second),
        std::pair<int, int> (coords.first - 1, coords.second),
        std::pair<int, int> (coords.first, coords.second + 1),
        std::pair<int, int> (coords.first, coords.second - 1)
        
    };

    for(auto box : boxes)
    {
        if(box.first >= 0 && box.first <= GRIDSEG && box.second >= 0 && box.second <= GRIDSEG)
        {
            if(gridMap.find(box) == gridMap.end())
                gridMap[box] = {};
            gridMap[box].push_back(p);
        }
    }    
    // if(gridMap.find(coords) == gridMap.end())
    //     gridMap[coords] = {};
    // gridMap[coords].push_back(p);
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
            

            if(distanceCalc(p, nbr) <= (nbr->radius + p->radius))
            {

                int iter = 0;
                while(distanceCalc(p, nbr) <= (nbr->radius + p->radius))
                {
                    ++iter;
                    //move both particles out of each other in small steps
                    p->x -= 0.01*p->vx;
                    p->y -= 0.01*p->vy;

                    nbr->x -= 0.01*nbr->vx;
                    nbr->y -= 0.01*nbr->vy;

                    //std::cout<<"fixer "<<iter<<"\n";

                }


                //elastic collision physics
                float m1 = p->mass;
                float vx1 = p->vx;
                float vy1 = p->vy;

                float m2 = nbr->mass;
                float vx2 = nbr->vx;
                float vy2 = nbr->vy;

                float alpha, beta, gamma;
                alpha = 2 * m1 / (m1 + m2);
                beta = (m1 - m2) / (m1 + m2);
                gamma = 2 * m2 / (m1 + m2);


                float coeffRestit = 1;
                //new velocities
                p->vx = coeffRestit * (beta * vx1 + gamma * vx2);
                p->vy = coeffRestit * (beta * vy1 + gamma * vy2);
                nbr->vx = coeffRestit * (alpha * vx1 - beta * vx2);
                nbr->vy = coeffRestit * (alpha * vy1 - beta * vy2);

                // while(distanceCalc(p, nbr) <= (nbr->radius + p->radius))
                // {
                //     //move both particles out of each other in small steps
                //     p->x += 0.01*p->vx;
                //     p->y += 0.01*p->vy;

                //     nbr->x += 0.01*nbr->vx;
                //     nbr->y += 0.01*nbr->vy;

                // }
                alreadyHandled.insert(nbr);
                alreadyHandled.insert(p);
                //break;
            }
        }
    }
}

void Box::calcKinetic()
{
    float ke = 0;
    for(auto p : particleList)
    {
        ke += keCalc(p);
    }
    std::cout<<ke<<"\n";

}

void Box::dbgMap()
{
    for(auto entry : gridMap)
    {
        std::cout<<entry.first.first<<" "<<entry.first.second<<" ";
        std::cout<<entry.second.size()<<"\n";
    }
}