#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<functional>


struct Pincel{
    sf::Font font;
    sf::Text texto;
    sf::RenderWindow& window;

    Pincel(sf::RenderWindow& window) : window(window){
        if(!font.loadFromFile("Honeybeeregular-MG0p.ttf")){
            std::cout << "Erro ao carregar fonte\n";
        }
        texto.setFont(font);
    }

    void write(std::string str, int x, int y, int size, sf::Color color){
        texto.setString(str);
        texto.setCharacterSize(size);
        texto.setPosition(x, y);
        texto.setFillColor(color);
        window.draw(texto);
    }

};

struct Bolha{
    int x, y;
    char letter;
    int speed;
    bool alive{true};

    static const int radius{ 10 };

    Bolha( int x, int y, char letter, int speed) : x(x), y(y), letter(letter), speed(speed){
    }

    void uptade(){
        y += speed;
    }

    void draw(sf::RenderWindow& window){
        sf::CircleShape circle(Bolha :: radius);
        circle.setPosition(x, y);
        circle.setFillColor(sf::Color::Red);
        window.draw(circle);

        static Pincel pincel(window);
        pincel.write(std::string(1, letter), x + 0.2* Bolha::radius , y + 0.2* Bolha::radius, Bolha::radius * 1.5, sf::Color::White);

    }
};

struct Board{
    sf::RenderWindow& window; 
    std::vector<Bolha> bolhas;
    Pincel  pencil;
    
    int hits{ 0 };
    int misses{ 0 };

    Board(sf::RenderWindow& window) : window(window), pencil(window){
        // bolhas.push_back(Bolha(100, 100, 'A', 1));
        // bolhas.push_back(Bolha(200, 100, 'B', 1));
        // bolhas.push_back(Bolha(300, 100, 'C', 1));
    }
   
    void check_hits(char letter){
        for(Bolha& bolha : bolhas) {
            if(letter == bolha.letter && bolha.alive){
                bolha.alive = false;
                hits++;
                break;
            }
        }
    }
    bool check_new_bolha(){
        static const int new_bolha_timeout{ 30 };
        static int new_bolha_timer{ 0 };

        new_bolha_timer--;
        if(new_bolha_timer <= 0){
            new_bolha_timer = new_bolha_timeout;
            return true;
        }
        return false;
    }

    void add_new_bolha(){
        int x = rand() % ((int) this->window.getSize().x - 2* Bolha::radius);
        int y = -2* Bolha::radius;
        char letter = 'A' + rand() % 26;
        int speed = rand() % 5 + 1;
        bolhas.push_back(Bolha(x, y, letter, speed));
    }

    void mark_outside(){
        for(auto& bolha : bolhas){
            if(bolha.y + 2* Bolha::radius > window.getSize().y){
                if(bolha.alive){
                    bolha.alive = false;
                    misses++;
                }
            }
        }
    }

    void remove_mortas(){
        std::vector<Bolha> vivas;
        for(auto& bolha : bolhas){
            if(bolha.alive){
                vivas.push_back(bolha);
            }
        }
        bolhas = vivas;
    }

    void uptade(){
        if(check_new_bolha()){
            add_new_bolha();
        }

        for(auto& bolha : bolhas){
            bolha.uptade();
        }

        mark_outside();
        remove_mortas();
    }

    void draw(){
        pencil.write("Hits: " + std::to_string(this->hits), 10, 10, 20, sf::Color::Green);
        pencil.write("Misses: " + std::to_string(this->misses), 10, 40, 20, sf::Color::Red);
        pencil.write("Size: " + std::to_string(this->bolhas.size()), 10, 70, 20, sf::Color::Blue);
        for(auto& bolha : bolhas){
            bolha.draw(window);
        }
    }
};

struct Game{
    sf::RenderWindow window;
    Board board;
    std::function<void()> uptade;
    
    Game() : window(sf::VideoMode(800, 600), "Bolhas!") , board(window){
        window.setFramerateLimit(60);
        this->uptade = [&](){ this->gameplay(); };
    }

    void process_events(){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            else if(event.type == sf::Event::TextEntered){
                char code = static_cast<char>(event.text.unicode);
                code = toupper(code);
                board.check_hits(code); 
            }
        }
    }

    void gameplay(){
        board.uptade();

        window.clear(sf::Color::White);
        board.draw();
        window.display();

        if(board.misses > 10){
            this->uptade = [&](){
                this->gameover(); 
            };
        }
    }

    void gameover(){
        static Pincel p(window);
        window.clear(sf::Color::Red);
        p.write("Game Over", 100, 100, 60, sf::Color::White);
        p.write("Press 'Enter' to play again!", 100, 200, 20, sf::Color::White);
        window.display();
        
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)){
            this->uptade = [&](){
                this->gameplay();
            };
        }
    }

    void main_loop(){
        while(window.isOpen()){
            process_events();
            uptade();
        }
    }

};


int main(){
    sf::RenderWindow window(sf::VideoMode(800, 600), "Bolhas!");

    Game game;
    game.main_loop();
}
