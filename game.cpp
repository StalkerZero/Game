#include <SFML/Graphics.hpp> //Библиотека для разработки игр
#include <time.h>

using namespace sf;

int main()
{
    srand(time(0));

    RenderWindow window(VideoMode(405, 720), "Running!"); //Размер рабочего окна

    Texture f; //Фон для игры
    f.loadFromFile("DOOM.png");
    Sprite fon(f);

    Texture e; //Земля по которой передвигается персонаж игры
    e.loadFromFile("earth.jpg");
    Sprite earth[3];
    for (int i = 0; i < 3; i++) {
        earth[i].setTexture(e);//Загрузка текстуры
        earth[i].setScale(1, 0.25);//Установка масштаба
        earth[i].setPosition(0, 150 + 150 * i);//Установка местоположения
    }

    Texture m; //Настройка персонажа
    m.loadFromFile("man.png");
    Sprite man(m);
    man.setScale(0.8, 0.8);//масштаб
    man.setTextureRect(IntRect(0, 0, 80, 130));//берем не всю картинку, а только ее кусочек
    float y = 200, frame = 0, py = 0, dy = 0, speed = 0.5;//y - где будет находиьься игрок, speed - скорость игрока, frame - переменная для создания анимации игрока
    bool up = false, down = false, anim = true;

    Texture t; //надпись "Click to continue"
    t.loadFromFile("continue.jpg");
    Sprite text(t);
    text.setPosition(300, 50);//местоположение
    float fx = 0;
    int game = 0;//целочисленный коэффицент для запуска игры

    Texture b;//блоки-преграды
    b.loadFromFile("block.jpg");
    Sprite block[2];
    int bx[2], by[2], bty[2];
    for (int i = 0; i < 2; i++) {
        block[i].setTexture(b);

        bx[i] = rand() % 3 + 1;//рандомная установка преград
        by[i] = rand() % 2 + 1;
        bty[i] = rand() % 3 + 1;
        block[i].setTextureRect(IntRect(100 * (bx[i] - 1), 100 * (by[i] - 1), 100, 100));
        block[i].setPosition(1000 + 500 * i, 50 + 150 * (bty[i] - 1));
    }

    Texture go;//надпись GAME OVER
    go.loadFromFile("died.png");
    Sprite gameover(go);
    gameover.setPosition(400, 50);

    Texture sc;//счет игры
    sc.loadFromFile("DOOM.png");
    Sprite score[5];
    float fs[5];
    for (int i = 0; i < 5; i++) {
        score[i].setTexture(sc);
        score[i].setTextureRect(IntRect(0, 0, 44, 55));
        score[i].setPosition(50 * i, 5);
        fs[i] = 0;
    }

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))//делать пока окно с игрой открыто
        {
            if (event.type == Event::Closed)//если программу закрыть
                window.close();

            if (event.type == Event::MouseButtonPressed)//если мы нажали на мышку
                if (event.key.code == Mouse::Left)//если на левую мышку нажали
                    if (game == 0)
                        game = 1;

            if (game == 1)
                if (event.type == Event::KeyPressed)
                    if (event.key.code == Keyboard::Up) { //нажатие ВВЕРХ на клавиатуре
                        dy = -150;

                        up = true;
                    }
                    else if (event.key.code == Keyboard::Down) {//нажатие ВНИЗ на клавиатуре
                        dy = 150;

                        down = true;
                    }
        }

        if (game == 0) {
            frame += 0.001;//анимация персонажа
            if (frame > 4)//если frame больше 4. то есть у нас будет всего 4 кадра
                frame -= 4;
            man.setTextureRect(IntRect(80 * int(frame), 0, 80, 130));//изменение кадра
        }
        else if (game == 1) {
            if (py != dy) {
                if (dy > 0) {
                    py = speed;
                    dy = dy - speed;
                }
                if (dy < 0) {
                    py = -speed;
                    dy = dy + speed;
                }

                y += py;

                anim = false;
            }

            else {
                up = false;
                down = false;

                anim = true;
            }

            if (y < 55)
                y = 55;
            else if (y > 355)
                y = 355;

            if (anim) {
                frame += 0.01;
                if (frame > 6)
                    frame -= 6;
                switch (int(frame))
                {
                    case 0:
                        man.setTextureRect(IntRect(0, 135, 80, 130));
                        break;
                    case 1:
                        man.setTextureRect(IntRect(80, 135, 90, 130));
                        break;
                    case 2:
                        man.setTextureRect(IntRect(172, 135, 100, 130));
                        break;
                    case 3:
                        man.setTextureRect(IntRect(0, 270, 88, 130));
                        break;
                    case 4:
                        man.setTextureRect(IntRect(88, 270, 88, 130));
                        break;
                    case 5:
                        man.setTextureRect(IntRect(172, 270, 90, 130));
                        break;
                }
            }

            else {
                if (up)
                    man.setTextureRect(IntRect(0, 400, 80, 150));//если передвижение идет вверх, то меняется картинка персонажа
                if (down)
                    man.setTextureRect(IntRect(82, 400, 100, 150));//если передвижение идет вниз то меняется картинка персонажа
            }

            Vector2f eh[3];
            for (int i = 0; i < 3; i++) {
                earth[i].move(-0.4, 0);

                eh[i] = earth[i].getPosition();
                if (eh[i].x < -800)
                    earth[i].setPosition(0, 150 + 150 * i);
            }

            Vector2f bk[2];
            for (int i = 0; i < 2; i++) {
                block[i].move(-0.4, 0);

                bk[i] = block[i].getPosition();//рандомная установка блоков
                if (bk[i].x < -100) {
                    bx[i] = rand() % 3 + 1;
                    by[i] = rand() % 2 + 1;
                    bty[i] = rand() % 3 + 1;
                    block[i].setTextureRect(IntRect(100 * (bx[i] - 1), 100 * (by[i] - 1), 100, 100));
                    block[i].setPosition(1000 + 500 * i, 50 + 150 * (bty[i] - 1));
                }

                if (man.getGlobalBounds().intersects(block[i].getGlobalBounds()))
                    game = 2;
            }

            fs[4] += 0.01;
            if (fs[4] > 10) {
                fs[4] -= 10;
                fs[3]++;
                if (fs[3] > 9) {
                    fs[3] -= 10;
                    fs[2]++;
                    if (fs[2] > 9) {
                        fs[2] -= 10;
                        fs[1]++;
                        if (fs[1] > 9) {
                            fs[1] -= 10;
                            fs[0]++;
                            if (fs[0] > 9)
                                fs[0] -= 10;
                            score[0].setTextureRect(IntRect(44 * int(fs[0]), 0, 44, 55));
                        }
                        score[1].setTextureRect(IntRect(44 * int(fs[1]), 0, 44, 55));
                    }
                    score[2].setTextureRect(IntRect(44 * int(fs[2]), 0, 44, 55));
                }
                score[3].setTextureRect(IntRect(44 * int(fs[3]), 0, 44, 55));
            }
            score[4].setTextureRect(IntRect(44 * int(fs[4]), 0, 44, 55));
        }

        else if (game == 2)
            man.setTextureRect(IntRect(190, 400, 100, 150));

        window.clear(Color::White);
        window.draw(fon);

        for (int i = 0; i < 3; i++)
            window.draw(earth[i]);

        man.setPosition(100, y);//игрок двигается по y
        window.draw(man);

        if (game == 0) {
            fx += 0.005;//запуск анимации надписи
            if (fx > 5)
                fx -= 5;
            if (fx < 3)
                window.draw(text);
        }

        for (int i = 0; i < 2; i++)
            window.draw(block[i]);

        if (game == 2)
            window.draw(gameover);

        for (int i = 0; i < 5; i++)
            window.draw(score[i]);

        window.display();
    }

    return 0;
}