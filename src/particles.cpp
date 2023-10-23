#include "../include/particles.h"


Particle::Particle()
{
    //todo
}

std::pair<float,float> Particle::getCoords()
{
    return std::pair<float,float>(this->x, this->y);
}

std::pair<float,float> Particle::getVelocities()
{
    return std::pair<float,float>(this->vx, this->vy);
}

void Particle::updatePosition()
{
    //todo
}

void Particle::drawToScreen(sf::RenderWindow)
{
    //todo
}
