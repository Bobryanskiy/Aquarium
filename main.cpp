#include <stdio.h>
#include <vector>
#include <functional>
#include <iostream>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "menuButtons.h"
#include "choiceButtons.h"
extern "C" {
    #include "aquarium.h"
    #include "LinkedList.h"
}

class Button : public sf::Drawable, public sf::Transformable
{
public:

    Button() = default;

    Button(std::function<void()> action) : action_(action) {}

    void click() {
        if (isHovered)
            action_();
    }

    void init_button(sf::RenderWindow& window, sf::Vector2i position, sf::Vector2i addSize, sf::Text text, sf::Color color) {
        // sf::VertexArray fish (sf::Triangles, 6);
        // fish[0].position = sf::Vector2f(0.f, 5.f);
        // fish[1].position = sf::Vector2f(10.f, 0.f);
        // fish[2].position = sf::Vector2f(10.f, 10.f);
        // fish[3].position = sf::Vector2f(15.f, 2.f);
        // fish[4].position = sf::Vector2f(15.f, 8.f);
        // fish[5].position = sf::Vector2f(10.f, 5.f);
        // window.draw(fish);

        b_text = text;
        b_text.setPosition(position.x, position.y);
        sf::FloatRect boundingBox = b_text.getGlobalBounds();
        boundingBox.width += addSize.x;
        boundingBox.height += addSize.y;
        boundingBox.left -= addSize.x / 2;
        boundingBox.top -= addSize.y / 2;
        setPosition(boundingBox.left, boundingBox.top);
        setScale(boundingBox.width, boundingBox.height);
        sf::RectangleShape rectangle(sf::Vector2f(boundingBox.width, boundingBox.height));
        rectangle.setPosition(sf::Vector2f(boundingBox.left, boundingBox.top));
        rectangle.setFillColor(color);
        b_rec = rectangle;
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineThickness(2);
        rectangle.setOutlineColor(sf::Color::Black);
        b_outline_rec = rectangle;
    }
    
    void init_button(sf::RenderWindow& window, sf::Vector2i position, sf::Vector2i size, sf::Color color, std::string textureName, sf::IntRect intRect) {
        setPosition(position.x, position.y);
        setScale(size.x, size.y);
        sf::RectangleShape rectangle(sf::Vector2f(size.x, size.y));
        rectangle.setPosition(position.x, position.y);
        rectangle.setFillColor(color);
        b_rec = rectangle;
        rectangle.setFillColor(sf::Color::Transparent);
        rectangle.setOutlineThickness(2);
        rectangle.setOutlineColor(sf::Color::Black);
        b_outline_rec = rectangle;
        texture_name = textureName;
        if(b_texture.loadFromFile("" + textureName)) isTexture = true;
        int_rect = intRect;
    }

    void mouseCheck(sf::RenderWindow& window, int coordX, int coordY) {
        sf::FloatRect boundingBox (getPosition(), getScale());
        if (boundingBox.contains(window.mapPixelToCoords(sf::Vector2i(coordX, coordY)))) {
            isHovered = 1;
        }
        else isHovered = 0;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        // apply the entity's transform -- combine it with the one that was passed by the caller
        // states.transform *= getTransform(); // getTransform() is defined by sf::Transformable
        // apply the texture
        states.texture = &b_texture;

        // you may also override states.shader or states.blendMode if you want

        // draw the vertex array
        if (isTexture) {
            sf::Sprite sprite;
            sprite.setTexture(b_texture);
            sprite.setTextureRect(int_rect);
            sprite.setPosition(getPosition());
            sprite.setScale(getScale().x / int_rect.width, getScale().y / int_rect.height);
            target.draw(sprite);
        } else target.draw(b_rec);
        if (isHovered) {
            target.draw(b_outline_rec);
        }
        // target.draw(b_vertices, states);
        target.draw(b_text);
    }

    bool isHovered = 0;
    bool isTexture = false;
    sf::IntRect int_rect;
    std::string texture_name;
    sf::RectangleShape b_outline_rec;
    sf::RectangleShape b_rec;
    sf::Text b_text;
    sf::VertexArray b_vertices;
    sf::Texture b_texture;
    std::function<void()> action_ { nullptr };
};

std::vector <Button> buttons(1);

sf::VideoMode desktop;

sf::View view;

void planktonAmountUp() {
    planktonsM.size++;
}

void planktonAmountDown() {
    planktonsM.size--;
}

void planktonSatietyUp() {
    plankton.foodWeight++;
}

void planktonSatietyDown() {
    plankton.foodWeight--;
}

void startBut() {
    windowNumber = PLAY;
}

sf::Vector2f screenScale;

sf::Vector2u windowSize;

void choice(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("BloggerSans-wMAZ.ttf"))
        exit(EXIT_FAILURE);
    sf::Text titleT("Aquarium", font, 70);
    titleT.setPosition(windowSize.x / 2 - titleT.getString().getSize() / 4 * titleT.getCharacterSize(), windowSize.y / 20 - titleT.getCharacterSize() / 2);
    sf::Text planktonsT("Planktons", font, 50);
    planktonsT.setPosition(280 * screenScale.x - planktonsT.getString().getSize() / 4 * planktonsT.getCharacterSize(), windowSize.y / 6 - planktonsT.getCharacterSize() / 2);
    sf::Text fishT("Fish", font, 50);
    fishT.setPosition(640 * screenScale.x - fishT.getString().getSize() / 4 * fishT.getCharacterSize(), windowSize.y / 6 - fishT.getCharacterSize() / 2);
    sf::Text sharkT("Sharks", font, 50);
    sharkT.setPosition(1000 * screenScale.x - sharkT.getString().getSize() / 4 * sharkT.getCharacterSize(), windowSize.y / 6 - sharkT.getCharacterSize() / 2);
    sf::Text amountT("Amount", font, 32);
    amountT.setPosition(60 * screenScale.x - amountT.getString().getSize() / 4 * amountT.getCharacterSize(), windowSize.y / 3.5 - amountT.getCharacterSize() / 2);
    sf::Text amountNum("", font, 24);
    amountNum.setPosition(240 * screenScale.x - amountNum.getString().getSize() / 4 * amountNum.getCharacterSize(), amountT.getPosition().y + amountNum.getCharacterSize() / 4);
    sf::Text satietyT("Satiety%", font, 32);
    satietyT.setPosition(90 * screenScale.x - satietyT.getString().getSize() / 4 * satietyT.getCharacterSize(), windowSize.y / 2.5 - satietyT.getCharacterSize() / 2);
    sf::Text satietyNum("", font, 24);
    satietyNum.setPosition(240 * screenScale.x - satietyNum.getString().getSize() / 4 * satietyNum.getCharacterSize(), satietyT.getPosition().y + satietyNum.getCharacterSize() / 4);

    buttons.clear();
    
    Button PAU (planktonAmountUp);
    sf::Text UP("^", font, 32);
    // PAU.init_button(window, sf::Vector2f(320 * screenScale.x - UP.getString().getSize() / 4 * UP.getCharacterSize(), amountT.getPosition().y - UP.getCharacterSize() / 8.f), sf::Vector2i(10, 10), UP, sf::Color (240, 186, 175, 255));
    // buttons.push_back(PAU);
    
    // Button PAD (planktonAmountDown);
    // sf::Text DOWN("^", font, 32);
    // DOWN.setOrigin(15, 15);
    // DOWN.rotate(180);
    // PAD.init_button(window, sf::Vector2f(317 * screenScale.x - DOWN.getString().getSize() / 4 * DOWN.getCharacterSize(), amountT.getPosition().y + 30 * screenScale.y + DOWN.getCharacterSize() / 2.f), sf::Vector2i(10, 10), DOWN, sf::Color (240, 186, 175, 255));
    // buttons.push_back(PAD);
    
    // Button PSU (planktonSatietyUp);
    // PSU.init_button(window, sf::Vector2f(320 * screenScale.x - UP.getString().getSize() / 4 * UP.getCharacterSize(), satietyT.getPosition().y - UP.getCharacterSize() / 8.f), sf::Vector2i(10, 10), UP, sf::Color (240, 186, 175, 255));
    // buttons.push_back(PSU);
    
    // Button PSD (planktonSatietyDown);
    // PSD.init_button(window, sf::Vector2f(317 * screenScale.x - DOWN.getString().getSize() / 4 * DOWN.getCharacterSize(), satietyT.getPosition().y + 30 * screenScale.y + DOWN.getCharacterSize() / 2.f), sf::Vector2i(10, 10), DOWN, sf::Color (240, 186, 175, 255));
    // buttons.push_back(PSD);
    
    Button startButton (startBut);
    sf::Text StartT("Start", font, 32);
    startButton.init_button(window, window.mapCoordsToPixel(sf::Vector2f(windowSize.x - 150, windowSize.y - 100)), sf::Vector2i(60, 20), StartT, sf::Color (240, 186, 175, 255));
    buttons.push_back(startButton);

    sf::Clock clock; // привязка ко времени
    int fps;

    // Start the game loop
    while (windowNumber == CHOICE) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseMoved) {
                for (int i = 0; i < buttons.size(); ++i) {
                    buttons[i].mouseCheck(window, event.mouseMove.x, event.mouseMove.y);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < buttons.size(); ++i) {
                        buttons[i].click();
                    }
                }
            }
        }
        // Clear screen
        window.clear(sf::Color(1, 96, 143, 1));
        // Draw
        for (int i = 0; i < buttons.size(); ++i)
            window.draw(buttons[i]);
        // Draw the string
        window.draw(titleT);
        window.draw(planktonsT);
        window.draw(fishT);
        window.draw(sharkT);
        window.draw(amountT);
        amountNum.setString(std::to_string(planktonsM.size));
        window.draw(amountNum);
        window.draw(satietyT);
        satietyNum.setString(std::to_string(plankton.foodWeight));
        window.draw(satietyNum);

        // Update the window
        window.display();
    }
    return;
}

float scaleEverything = 1;
int isEsc = 0;

void resumeButton() {
    isEsc = 0;
    scaleEverything = 1;
}

void menuButton() {
    deleteArrays();
    isEsc = 0;
    scaleEverything = 1;
    windowNumber = MENU;
}

void play(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("BloggerSans-wMAZ.ttf"))
        exit(EXIT_FAILURE);
    sf::Texture planktonT, fishT, sharkT;
    if (!planktonT.loadFromFile("plankton.png"))
        exit(EXIT_FAILURE);
    if (!fishT.loadFromFile("fish2.png"))
        exit(EXIT_FAILURE);
    if (!sharkT.loadFromFile("shark.png"))
        exit(EXIT_FAILURE);
    sf::Sprite planktonS(planktonT);
    planktonS.setOrigin(420, 420);
    planktonS.setScale(0.05, 0.05);
    planktonS.scale(desktop.width / view.getSize().x, desktop.height / view.getSize().y);
    sf::Sprite fishS(fishT);
    fishS.setOrigin(102.5, 66.5);
    fishS.setScale(0.2, 0.2);
    fishS.scale(desktop.width / view.getSize().x, desktop.height / view.getSize().y);
    sf::Sprite sharkS(sharkT);
    sharkS.setOrigin(785.5, 395.5);
    sharkS.setScale(0.05, 0.05);
    sharkS.scale(desktop.width / view.getSize().x, desktop.height / view.getSize().y);
    createArrays();

    buttons.clear();

    sf::Clock clock; // привязка ко времени
    sf::RenderTexture background;
    if (!background.create(window.getSize().x, window.getSize().y))
        exit(EXIT_FAILURE);
    background.clear(sf::Color(0, 0, 0, 120));
    sf::Sprite backgroundS(background.getTexture());
    Button buttonPlay (resumeButton);
    sf::Text b_t("Resume", font, 70);
    buttonPlay.init_button(window, window.mapCoordsToPixel(sf::Vector2f(540, 120), view), sf::Vector2i(60, 20), b_t, sf::Color (240, 186, 175, 255));
    buttons.push_back(buttonPlay);
    Button buttonMenu(menuButton);
    sf::Text menu_t("To menu", font, 70);
    buttonMenu.init_button(window, window.mapCoordsToPixel(sf::Vector2f(540, 340), view), sf::Vector2i(60, 20), menu_t, sf::Color (89, 3, 37));
    buttons.push_back(buttonMenu);
    Button buttonExit(exitButton);
    sf::Text exit_t("Exit", font, 70);
    buttonExit.init_button(window, window.mapCoordsToPixel(sf::Vector2f(540, 540), view), sf::Vector2i(60, 20), exit_t, sf::Color (89, 3, 37));
    buttons.push_back(buttonExit);

    // Start the game loop
    while (windowNumber == PLAY) {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseMoved) {
                for (int i = 0; i < buttons.size(); ++i) {
                    buttons[i].mouseCheck(window, event.mouseMove.x, event.mouseMove.y);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < buttons.size(); ++i) {
                        buttons[i].click();
                    }
                }
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    isEsc = 1;
                    scaleEverything = 0.01;
                }
            }
        }
        // Clear screen
        window.clear(sf::Color(1, 96, 143, 1));
        // Draw
        NODE* node = planktonsM.head;
        for (int i = 0; i < planktonsM.size; ++i) {
            PLANKTON p = node->data.plankton;
            sf::Vector2i v = window.mapCoordsToPixel(sf::Vector2f(p.coordX, p.coordY), view);
            planktonS.setPosition(v.x, v.y);
            if (p.coordXWant > p.coordX) planktonS.scale(-1, 1);
            if (p.coordYWant > p.coordY) planktonS.scale(1, -1);
            window.draw(planktonS);
            if (p.coordXWant > p.coordX) planktonS.scale(-1, 1);
            if (p.coordYWant > p.coordY) planktonS.scale(1, -1);
            node = node->next;
        }
        node = fishM.head;
        for (int i = 0; i < fishM.size; ++i) {
            FISH f = node->data.fish;
            sf::Vector2i v = window.mapCoordsToPixel(sf::Vector2f(f.base.coordX, f.base.coordY), view);
            fishS.setPosition(v.x, v.y);
            if (f.base.coordXWant > f.base.coordX) fishS.scale(-1, 1);
            window.draw(fishS);
            if (f.base.coordXWant > f.base.coordX) fishS.scale(-1, 1);
            node = node->next;
        }
        node = sharksM.head;
        for (int i = 0; i < sharksM.size; ++i) {
            SHARK s = node->data.shark;
            sf::Vector2i v = window.mapCoordsToPixel(sf::Vector2f(s.brainBase.base.coordX, s.brainBase.base.coordY), view);
            sharkS.setPosition(v.x, v.y);
            if (s.brainBase.base.coordXWant > s.brainBase.base.coordX) sharkS.scale(-1, 1);
            window.draw(sharkS);
            if (s.brainBase.base.coordXWant > s.brainBase.base.coordX) sharkS.scale(-1, 1);
            node = node->next;
        }

        updateEntities(time, scaleEverything);

        if (isEsc) {
            window.draw(backgroundS);
            for (int i = 0; i < buttons.size(); ++i)
                window.draw(buttons[i]);
        }

        // Update the window
        window.display();
    }
    return;
}

void menu(sf::RenderWindow& window) {
    // Load a sprite to display
    sf::Texture texture;
    if (!texture.loadFromFile("cute_image.png"))
        exit(EXIT_FAILURE);
    sf::Sprite sprite(texture);
    sf::Vector2u textureSize = texture.getSize();
    sprite.setScale((float)windowSize.x / textureSize.x, (float)windowSize.y / textureSize.y);
    // Create a graphical text to display
    sf::Font font;
    if (!font.loadFromFile("BloggerSans-wMAZ.ttf"))
        exit(EXIT_FAILURE);
    sf::Text text("Aquarium", font, 70);
    text.setPosition(windowSize.x / 2 - text.getString().getSize() / 4 * text.getCharacterSize(), windowSize.y / 4 - text.getCharacterSize() / 2);
    // Load a music to play
    sf::Music music;
    if (!music.openFromFile("nice_music.ogg"))
        exit(EXIT_FAILURE);
    // Play the music
    music.setVolume(5);
    // music.play();

    // sf::Cursor cursor;
    // if (cursor.loadFromSystem(sf::Cursor::Arrow))
        // window.setMouseCursor(cursor);

    std::string string = "0";
    int i = 0;

    // sf::Vertex vertices[] =
    // {
    //     sf::Vertex(sf::Vector2f(  0,   0), sf::Color::Red),
    //     sf::Vertex(sf::Vector2f(  0, 100), sf::Color::Transparent),
    //     sf::Vertex(sf::Vector2f(100, 100), sf::Color::Green),
    //     sf::Vertex(sf::Vector2f(100,   0), sf::Color::Transparent)
    // };

    buttons.clear();
    Button buttonPlay (playButton);
    sf::Text b_t("Play", font, 70);
    buttonPlay.init_button(window, window.mapCoordsToPixel(sf::Vector2f(540, 360), view), sf::Vector2i(60, 20), b_t, sf::Color (240, 186, 175, 255));
    buttons.push_back(buttonPlay);
    
    Button buttonSwitchScreen (switchScreen);
    // sprite.setScale((float)windowSize.x / textureSize.x, (float)windowSize.y / textureSize.y);
    buttonSwitchScreen.init_button(window, window.mapCoordsToPixel(sf::Vector2f(windowSize.x - 125, 25)), sf::Vector2i(100, 100), sf::Color (43, 20, 4), "Layer 6.png", sf::IntRect(currentScreenStatus == FULLSCREEN ? 220 : 0, 0, 220, 220));
    buttons.push_back(buttonSwitchScreen);
    
    Button buttonExit (exitButton);
    sf::Text exit_t("Exit", font, 70);
    buttonExit.init_button(window, window.mapCoordsToPixel(sf::Vector2f(50.f, windowSize.y - 100)), sf::Vector2i(60, 20), exit_t, sf::Color (89, 3, 37));
    buttons.push_back(buttonExit);

    sf::Clock clock; // привязка ко времени
    int fps;

    // Start the game loop
    while (windowNumber == MENU) {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();
        time = time / 800;
        // string = std::to_string(std::stoi(string) + 1);
        std::string c1;
        std::string c2;
        c1 = std::to_string(sf::Mouse::getPosition(window).x);
        c2 = std::to_string(sf::Mouse::getPosition(window).y);
        sf::Text t1(c1, font, 30);
        t1.setPosition(50.f, 100.f);
        sf::Text t2(c2, font, 30);
        t2.setPosition(150.f, 100.f);

        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::MouseMoved) {
                for (int i = 0; i < buttons.size(); ++i) {
                    buttons[i].mouseCheck(window, event.mouseMove.x, event.mouseMove.y);
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (int i = 0; i < buttons.size(); ++i) {
                        buttons[i].click();
                    }
                }
            }
        }
        // Clear screen
        window.clear(sf::Color(1, 96, 143, 1));
        // Draw
        for (int i = 0; i < buttons.size(); ++i)
            window.draw(buttons[i]);
        // Draw the string
        window.draw(text);

        fps = time;
        sf::Text ttt(std::to_string(fps), font, 70);
        window.draw(ttt);

        // Draw the sprite
        // window.draw(sprite);

        window.draw(t1);
        window.draw(t2);

        // window.draw(vertices, 4, sf::Quads);

        // Update the window
        window.display();

        if (currentScreenStatus != screenStatusNeeded) return;
    }
    return;
}

void (*windows[3])(sf::RenderWindow&) = {menu, choice, play};

void runner(sf::RenderWindow& window) {
    windows[windowNumber](window);
}

int main() {
    view = sf::View(sf::FloatRect(0 , 0, 1280, 720));
    defaultSettings();
    desktop = sf::VideoMode::getDesktopMode();
    currentScreenStatus = WINDOW;
    screenStatusNeeded = WINDOW;
    windowNumber = MENU;
    // windowNumber = CHOICE;
    sf::RenderWindow* curwin = nullptr;
    while (windowNumber != EXIT) {
        if(curwin && curwin->isOpen()) curwin->close();
        switch (screenStatusNeeded) {
            case WINDOW:
                curwin = new sf::RenderWindow (sf::VideoMode(1280, 720), "Aquarium", sf::Style::Default);
                windowSize = curwin->getSize();
                curwin->setPosition(sf::Vector2i(desktop.width / 2 - windowSize.x / 2, desktop.height / 2 - windowSize.y / 2));
                break;
            case FULLSCREEN:
                curwin = new sf::RenderWindow (sf::VideoMode(desktop.width, desktop.height), "Aquarium", sf::Style::Fullscreen);
                break;
            default:
                // curwin = &window;
                break;
        }
        windowSize = curwin->getSize();
        screenScale.x = windowSize.x / 1280;
        screenScale.y = windowSize.y / 720;
        currentScreenStatus = screenStatusNeeded;
        // curwin->setFramerateLimit(100);
        runner(*curwin);
    }
    return EXIT_SUCCESS;
}