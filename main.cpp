#include <SFML/Graphics.hpp>
#include <iostream>
#include <thread>
#include <list>

using namespace sf;
using namespace std;

class Player{
    Texture texture;
    Sprite sprites[5];
    int nowSprite = 0;
    short int position = 0;

public:
    explicit Player(){
        texture.loadFromFile("../Textures/doomguy.png");
        for (auto & sprite : sprites) {
            sprite.setTexture(texture);
            sprite.setPosition(20, 90);
        }
        sprites[0].setTextureRect(IntRect(0,0,49,88));
        sprites[1].setTextureRect(IntRect(49,0,64,88));
        sprites[2].setTextureRect(IntRect(201,0,49,88));
        sprites[3].setTextureRect(IntRect(160,0,41,88));
        sprites[4].setTextureRect(IntRect(113,0,47,88));
    }

    void move(){
        if (nowSprite == 4) nowSprite = 1;
        else nowSprite++;
        sprites[nowSprite].setPosition(20, 90+position*90);
    }

    Sprite getCurrentSprite(){
        return sprites[nowSprite];
    }

    short getPosition() const {
        return position;
    }

    void setPosition(short position) {
        Player::position = position;
    }
};

bool gameOver = false;
bool pause = false;
bool removeEnemy = false;
Player player;

class Enemy{
    Texture texture;
    Sprite sprite;
    int position;

public:
    explicit Enemy(int position) {
        this->position = position;
        texture.loadFromFile("../Textures/cacodemon.png");
        sprite.setTexture(texture);
        sprite.setPosition(486, 90+position*90);
    }

    const Sprite &getSprite() const {
        return sprite;
    }

    void move(){
        sprite.setPosition(sprite.getPosition().x-8, 90+position*90);
        if(player.getPosition()==position && sprite.getPosition().x<=70)
            gameOver = pause = true;
        if(sprite.getPosition().x<=-80)
            removeEnemy = true;
    }
};

list<Enemy*> enemies = {new Enemy(rand()%3-1)};

void moving(){
    while(!pause) {
        for (int i = 0; i < 3; i++) {
            this_thread::sleep_for(chrono::milliseconds(50));
            for(Enemy* e : enemies)
                e->move();
            if(removeEnemy){
                enemies.pop_front();
                removeEnemy = false;
            }
        }
        if(enemies.empty() || enemies.back()->getSprite().getPosition().x<=266)
            enemies.push_back(new Enemy(rand() % 3 - 1));
        if(rand()%3==1) srand(time(nullptr));
        player.move();
    }
    return;
}

int main() {
    RenderWindow window(VideoMode(480, 270), "Running");

    Texture backT;
    backT.loadFromFile("../Textures/background.png");
    Sprite backS(backT);

    Texture endT;
    endT.loadFromFile("../Textures/died.jpg");
    Sprite endS(endT);

    Thread keepMoving(&moving);
    keepMoving.launch();

    while(window.isOpen()){
        Event event;
        while(window.pollEvent(event)){
            if(event.type == Event::Closed) {
                pause = true;
                window.close();
            } else if (event.type == Event::KeyPressed){
                switch (event.key.code){
                    case Keyboard::Up:
                        if (player.getPosition()!=-1)
                            player.setPosition(player.getPosition()-1);
                        break;
                    case Keyboard::Down:
                        if (player.getPosition()!=1)
                            player.setPosition(player.getPosition()+1);
                        break;
                    case Keyboard::Escape:
                        if(gameOver)
                            enemies.clear();
                        gameOver = false;
                        pause = !pause;
                        if(!pause)
                            keepMoving.launch();
                        else
                            keepMoving.terminate();
                        break;
                }
            }
        }

        window.clear();
        if(gameOver) {
            keepMoving.terminate();
            window.draw(endS);
        }
        else {
            window.draw(backS);
            window.draw(player.getCurrentSprite());
            if(!enemies.empty())
                for (const Enemy *e: enemies)
                    window.draw(e->getSprite());
        }
        window.display();
    }
    return 0;
}