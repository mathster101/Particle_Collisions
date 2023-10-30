#include "../include/particles.h"
#include<iostream>
#include<cstdio>

Particle::Particle(int screen_height, int screen_width)
{
    pos.x = rand()%screen_width;
    pos.y = rand()%screen_height;
    vel.x = -150 + rand()%300;
    vel.y = -150 + rand()%300;
    accel.x = accel.y = 0;
    accel.x = -300; 
    radius = 20 + rand()%3;
    mass = pow(radius, 3) * DENSITY;

    box_h = screen_height;
    box_w = screen_width;

    circle = sf::CircleShape(radius);//need to set color

    //std::cout<<x<<" "<<y<<"\n";
}

void Particle::updatePosition(float dt)
{
    vel.x += accel.x*dt;
    vel.y += accel.y*dt;
    sf::Vector2f posNew = pos + vel*dt;
    //wall checks
    if(posNew.x < 0)
    {
        vel.x = -1 * vel.x * DAMPING;
        posNew.x *= -1;
    }
    if(posNew.y < 0)
    {
        vel.y = -1 * vel.y * DAMPING;
        posNew.y *= -1;
    }
    if(posNew.x > box_w)
    {
        vel.x = -1 * vel.x * DAMPING;
        posNew.x = box_w - (posNew.x - box_w);
    }
    if(posNew.y > box_h)
    {
        vel.y = -1 * vel.y * DAMPING;
        posNew.y = box_h - (posNew.y - box_h);
    }
    pos = posNew;

}

void Particle::drawToScreen(sf::RenderWindow &window)
{
    circle.setPosition(pos);
    window.draw(circle);
}

/********************************************************************/

float distanceCalc(Particle *p, Particle *nbr)
{
    // float d = pow((nbr->x - p->x), 2) + pow((nbr->y - p->y), 2);
    // return pow(d + 0.1, 0.5);

    sf::Vector2f difference = p->pos - nbr->pos;
    return pow(pow(difference.x, 2) + pow(difference.y, 2), 0.5);
}

float keCalc(Particle *p)
{
    float ke = 0.5 * p->mass * (pow(p->vel.x, 2) + pow(p->vel.y, 2));
    return ke;
}

Box::Box(int screen_height, int screen_width)
{
    height = screen_height;
    width = screen_width;
}

std::pair<int, int> Box::getGridcoord(sf::Vector2f position)
{
    //maps pixel value to grid box;
    float segSizew = width/GRIDSEG;
    float segSizeh = height/GRIDSEG;

    int xCoord = position.x/segSizew;
    int yCoord = position.y/segSizew;
    //std::cout<<xCoord<<" "<<yCoord<<"\n";
    return std::pair<int, int>(xCoord, yCoord);
}

void Box::updateGridmap(Particle* p)
{
    std::pair<int, int> coords = getGridcoord(p->pos);

    std::vector<std::pair<int, int>> nbrs = {
        coords,
        {coords.first + 1, coords.second},
        {coords.first - 1, coords.second},
        {coords.first, coords.second + 1},
        {coords.first, coords.second - 1}       
    };

    for(auto &nbr : nbrs)
    {
        if(nbr.first >= 0 && nbr.first <= GRIDSEG && nbr.second >= 0 && nbr.second <= GRIDSEG)
        {
            if(gridMap.find(nbr) == gridMap.end())
                gridMap[nbr] = {};
            gridMap[nbr].push_back(p);            
        }
    }
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
        
        auto myLoc = getGridcoord(p->pos);
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
                    p->pos -= 0.01f*p->vel;
                    nbr->pos -= 0.01f*nbr->vel;

                    // if(iter > 5 && iter < 100)
                    // {
                    //     std::cout<<"fixer "<<iter<<"\n";
                    //     p->circle.setFillColor(sf::Color::Magenta);
                    //     nbr->circle.setFillColor(sf::Color::Magenta);
                    // }

                }


                //elastic collision physics
                float m1 = p->mass;
                float m2 = nbr->mass;
                float alpha, beta, gamma;

                alpha = 2 * m1 / (m1 + m2);
                beta = (m1 - m2) / (m1 + m2);
                gamma = 2 * m2 / (m1 + m2);

                //new velocities
                auto newNbrVel = DAMPING * ((alpha * p->vel) - (beta * nbr->vel));                
                auto newMyVel = DAMPING * ((beta * p->vel) + (gamma * nbr->vel));
                p->vel = newMyVel;
                nbr->vel = newNbrVel;
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