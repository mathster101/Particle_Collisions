#include "../include/particles.h"
#include<iostream>
#include<cstdio>

float magnitude(sf::Vector2f vec)
{
    float mag = pow(vec.x, 2) + pow(vec.y, 2);
    mag = pow(mag, 0.5);
    return mag;
}

float distanceCalc(Particle *p, Particle *nbr)
{
    // float d = pow((nbr->x - p->x), 2) + pow((nbr->y - p->y), 2);
    // return pow(d + 0.1, 0.5);

    sf::Vector2f difference = p->get_pos() - nbr->get_pos();
    return (float)pow(pow(difference.x, 2) + pow(difference.y, 2), 0.5);
}

float keCalc(Particle *p)
{
    float ke = 0.5 * p->get_mass() * (pow(p->get_vel().x, 2) + pow(p->get_vel().y, 2));
    return ke;
}

sf::Vector2f unitVector(const sf::Vector2f vec)
{
    return vec/magnitude(vec);
}

/********************************************************************/

Particle::Particle(int screen_height, int screen_width)
{
    pos.x = rand()%screen_width;
    pos.y = rand()%screen_height;
    vel.x = -150 + rand()%300;
    vel.y = -150 + rand()%300;
    accel.x = accel.y = 0;
    //accel.y = 300; 
    radius = 5 + rand()%10;
    mass = pow(radius, 3) * DENSITY;

    box_h = screen_height;
    box_w = screen_width;

    circle = sf::CircleShape(radius);//need to set color
    circle.setOrigin(radius, radius);

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
    std::pair<int, int> coords = getGridcoord(p->get_pos());

    std::vector<std::pair<int, int>> nbrs = {
        coords,
        {coords.first + 1, coords.second},
        {coords.first - 1, coords.second},
        {coords.first, coords.second + 1},
        {coords.first, coords.second - 1},
        {coords.first + 1, coords.second + 1},
        {coords.first - 1, coords.second + 1},
        {coords.first + 1, coords.second - 1},
        {coords.first - 1, coords.second - 1} 
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

void Box::collisionUpdate(float dt)
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
        
        auto myLoc = getGridcoord(p->get_pos());
        auto neighbors = getGridnbrs(myLoc.first, myLoc.second);
        for(auto nbr : neighbors)
        {
            //skip if this is current elem or an elem handled already
            if(nbr == p)
                continue;
            

            if(distanceCalc(p, nbr) <= (nbr->get_radius() + p->get_radius()))
            {
                int iter = 0;
                float distance = distanceCalc(p, nbr);
                if(distance < (nbr->get_radius() + p->get_radius()))
                {
                    float intersectAmt = (nbr->get_radius() + p->get_radius()) - distance;
                    sf::Vector2f intersectDir(nbr->get_pos().x - p->get_pos().x, nbr->get_pos().y - p->get_pos().y);
                    intersectDir = intersectAmt * unitVector(intersectDir);
                    p->set_pos(p->get_pos() - (intersectDir / 1.7F));
                    nbr->set_pos(nbr->get_pos() + (intersectDir / 1.7F));
                }

                //elastic collision physics
                float m1 = p->get_mass();
                float m2 = nbr->get_mass();
                float alpha, beta, gamma;

                alpha = 2 * m1 / (m1 + m2);
                beta = (m1 - m2) / (m1 + m2);
                gamma = 2 * m2 / (m1 + m2);

                //new velocities
                auto newNbrVel = DAMPING * ((alpha * p->get_vel()) - (beta * nbr->get_vel()));                
                auto newMyVel = DAMPING * ((beta * p->get_vel()) + (gamma * nbr->get_vel()));
                p->set_vel(newMyVel);
                nbr->set_vel(newNbrVel);
                if(magnitude(p->get_vel()) < 10 || magnitude(nbr->get_vel()) < 10)
                {
                    //code to add a small normal vel
                    sf::Vector2f intersectDir(nbr->get_pos().x - p->get_pos().x, nbr->get_pos().y - p->get_pos().y);
                    p->set_vel(p->get_vel() - 20.0F*unitVector(intersectDir));
                    nbr->set_vel(nbr->get_vel() + 20.0F*unitVector(intersectDir));
                } 
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