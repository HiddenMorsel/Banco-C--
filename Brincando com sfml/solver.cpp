#include<SFML/Graphics.hpp>
#include<iostream>

void setSize(sf::Sprite& sprite, int width, int height){
    auto dim = sprite.getLocalBounds();
    sprite.setScale((float) width/dim.width, (float) height/dim.height);
}

struct Board{
    int nc{0};
    int nl{0};
    int step{0};
    sf::Sprite sprite;
    sf::RectangleShape rect;

    Board(int nc, int nl, int step, sf::Texture& texture):
        nc{nc}, nl{nl}, step{step}, sprite(texture){
            setSize(this->sprite, nc*step, nl*step);
            this->sprite.setPosition(0, 0);
            this->rect.setSize(sf::Vector2f(step, step));
            this->rect.setFillColor(sf::Color::Transparent);
            this->rect.setOutlineColor(sf::Color::Black);
            this->rect.setOutlineThickness(2);

        }

    void draw(sf::RenderWindow& window){
        window.draw(this->sprite);
        for (int i=0; i<nc; i++)
            for (int j=0; j<nl; j++){
                rect.setPosition(i*step, j*step);
                window.draw(rect);
            }
    }

};

// struct Obstaculo{

//     int x{0};
//     int y{0};
//     int step{0};
//     sf::Sprite sprite;

//     Obstaculo(int x, int y, int step, sf::Texture& texture):
//         x{x}, y{y}, step{step}, sprite(texture){}

//     void draw(sf::RenderWindow& window){
//         this->sprite.setPosition(x*step, y*step);
//         setSize(this->sprite, step, step);
//         window.draw(this->sprite);
//     }
// };


struct Entity{
    int x{0};
    int y{0};
    int step{0};
    sf::Sprite sprite;

    Entity(int x, int y, int step, sf::Texture& texture):
        x{x}, y{y}, step{step}, sprite(texture){}

    void draw(sf::RenderWindow& window){
        this->sprite.setPosition(x*step, y*step);
        setSize(this->sprite, step, step);
        window.draw(this->sprite);
    }
};

void moveEntity(sf::Keyboard::Key key, Entity& entity, std::vector<sf::Keyboard::Key> move_keys){
    if (key == move_keys[0]){
        entity.x -= 1;
        if(entity.x < 0) entity.x = 0;
    } 
    if (key == move_keys[1]) {
        entity.y -= 1;
        if(entity.y < 0) entity.y = 0;
    }
    if (key == move_keys[2]) {
        entity.x += 1;
        if(entity.x > 6) entity.x = 6;
    }
    if (key == move_keys[3]){
        entity.y += 1;
        if(entity.y > 4) entity.y = 4;
    }
}

sf::Texture loadTexture(const std::string& path){
    sf::Texture texture;
    if (!texture.loadFromFile(path)){
        std::cout << "Error loading file" << std::endl;
    }
    return texture;
}

int main(){

    sf::Texture penpewpei = loadTexture("yohane.png");
    sf::Texture mumu = loadTexture("tsumugi.png");
    sf::Texture fundo = loadTexture("azul1.png");

    const int STEP{100};

    Entity yohane(1, 1, STEP, penpewpei);
    Entity mugi(3, 3, STEP, mumu);
    Board tabuleiro(7, 5, STEP, fundo);

    sf::RenderWindow window(sf::VideoMode(tabuleiro.nc*STEP, tabuleiro.nl*STEP ), "Yameroo!");

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed){
                moveEntity(event.key.code, yohane, {sf::Keyboard::Left, sf::Keyboard::Up, sf::Keyboard::Right, sf::Keyboard::Down});
                moveEntity(event.key.code, mugi, {sf::Keyboard::A, sf::Keyboard::W, sf::Keyboard::D, sf::Keyboard::S});
            }
        }
        window.clear();
        tabuleiro.draw(window);
        yohane.draw(window);
        mugi.draw(window);
        window.display();
    }
    return 0;
   
}
 // sf::Sprite yohane(penpewpei);
    // sf::FloatRect rect = yohane.getLocalBounds();
    // yohane.setScale(200/rect.width, 200/rect.height);
    // yohane.setScale(0.5, 0.5);
    // yohane.setPosition(100, 100);
// window.draw(yohane);
