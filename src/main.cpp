//#include "../include/particles.h"
#include <SFML/Graphics.hpp>


#define HEIGHT 1000
#define WIDTH 1000

void loop()
{
    sf::RenderWindow window(sf::VideoMode(HEIGHT, WIDTH), "Particles");
    //window.setFramerateLimit(120);
    window.setVerticalSyncEnabled(1);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();



            window.display();
            
        }

    }    
}


int main(){

    loop();
    return 0;
}