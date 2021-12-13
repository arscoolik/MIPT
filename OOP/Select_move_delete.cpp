#include <iostream>
#include <cmath>
#include <list>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "context_menu.hpp"

using namespace std;

// Вспомогательные функции, вычисляет расстояние между двумя точками
float distance(sf::Vector2f start, sf::Vector2f finish)
{
    return sqrtf((start.x - finish.x)*(start.x - finish.x) + (start.y - finish.y)*(start.y - finish.y));
}

// Вспомогательные функции, рисует линию на холсте окна window
void drawLine(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f finish, sf::Color color = sf::Color::White)
{
    sf::Vertex line_vertices[2] = {sf::Vertex(start, color), sf::Vertex(finish, color)};
    window.draw(line_vertices, 2, sf::Lines);
}


// Вспомагательный класс, описывет шарик
// position - положение шарика; radius - радиус
// is_chosen - говорит о том, выбран ли шарик или нет
struct Ball
{
    sf::Vector2f position;
    float radius;
    bool isChoosen;
    vector<int> clr = {255, 255, 255};

    Ball(sf::Vector2f position, float radius) : position(position), radius(radius)
    {
        isChoosen = false;
    }

    // Метод, который рисует шарик на холстек окна window
    void draw(sf::RenderWindow& window) const
    {
        // Тут рисуем белый кружочек
        sf::CircleShape circle(radius);
        circle.setFillColor(sf::Color{clr[0], clr[1], clr[2]});
        circle.setOrigin({radius, radius});
        circle.setPosition(position);
        window.draw(circle);

        // Если шарик выбран
        if (isChoosen) {
            // То рисуем "уголки"
            const float fraction = 0.7;
            drawLine(window, {position.x - radius, position.y + radius}, {position.x - radius, position.y + radius*fraction});
            drawLine(window, {position.x - radius, position.y + radius}, {position.x - fraction * radius, position.y + radius});

            drawLine(window, {position.x + radius, position.y + radius}, {position.x + radius, position.y + radius*fraction});
            drawLine(window, {position.x + radius, position.y + radius}, {position.x + radius*fraction, position.y + radius});

            drawLine(window, {position.x + radius, position.y - radius}, {position.x + radius*fraction, position.y - radius});
            drawLine(window, {position.x + radius, position.y - radius}, {position.x + radius, position.y - radius*fraction});

            drawLine(window, {position.x - radius, position.y - radius}, {position.x - radius*fraction, position.y - radius});
            drawLine(window, {position.x - radius, position.y - radius}, {position.x - radius, position.y - radius*fraction});
        }
    }
};


int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Select, Move, Delete!", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::Font font;

    if (!font.loadFromFile("consolas.ttf")) {
        std::cout << "Can't load button font" << std::endl;
    }

    std::vector<sf::String> contextMenuStrings {"Cut", "Copy", "Paste", "Increase", "Decrease", "Delete", "Create", "Random Color"};
    ContextMenu contextMenu(window, font);
    for (const auto& el : contextMenuStrings) {
        contextMenu.addButton(el);
    }
    vector<Ball> buffer;
    sf::Vector2f buffer_pos;

    // Создаём связный список из шариков
    // Связный список -- потому что нам нужно будет постоянно удалять и добавлять в этот список
    std::list<Ball> balls;
    balls.push_back(Ball({200, 200}, 26));
    balls.push_back(Ball({400, 300}, 20));
    balls.push_back(Ball({500, 100}, 16));
    balls.push_back(Ball({200, 400}, 18));
    balls.push_back(Ball({350, 150}, 22));
    balls.push_back(Ball({750, 400}, 21));

    // Создаём прямоугольник выделения
    // Обратите внимание на четвёртый параметр sf::Color(150, 150, 240, 50)
    // Это alpha - прозрачность      0 = полностью прозрачный     255 = непрозрачный
    sf::RectangleShape selectionRect;
    selectionRect.setFillColor(sf::Color(150, 150, 240, 50));
    selectionRect.setOutlineColor(sf::Color(200, 200, 255));
    selectionRect.setOutlineThickness(1);

    // Специальная переменная, которая будет показывать, что мы находимся в режиме выделения
    bool isSelecting = false;
    bool moving_balls = false;
    sf::Vector2f pos;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            int result = contextMenu.handleEvent(event);
            if (result == 0){
                    buffer.clear();
                buffer_pos = window.mapPixelToCoords(sf::Mouse::getPosition());
                for (Ball& b : balls) {
                    if (b.isChoosen){
                        buffer.push_back(b);
                    }
                }
                for (auto b = balls.begin(); b != balls.end(); b++) {
                    if ((*b).isChoosen){
                        b = balls.erase(b);
                    }
                }
            }
            if (result == 1){
                    buffer.clear();
                buffer_pos = window.mapPixelToCoords(sf::Mouse::getPosition());
                for (Ball& b : balls) {
                    if (b.isChoosen){
                        buffer.push_back(b);
                    }
                }
            }
            if (result == 2){
                cout << buffer.size();
               sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition()) - buffer_pos;
               for (Ball& b : buffer) {
                    b.position += mousePosition;
                    balls.push_back(b);
                }
            }
            if (result == 3) {
                 for (Ball& b: balls){
                    if (b.isChoosen)
                        b.radius *= 1.25;
                 }
            }
            if (result == 4){
                 for (Ball& b: balls){
                    if (b.isChoosen)
                        b.radius *= 0.75;
                 }
            }
            if (result == 5) {
                for (auto b = balls.begin(); b != balls.end(); b++) {
                    if ((*b).isChoosen){
                        b = balls.erase(b);
                    }
                }
            }
            if (result == 6){
                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition());
                mousePosition.x -= 600;
                mousePosition.y -= 400;
                 balls.push_back(Ball(mousePosition, 5 + rand() % 40));
            }
            if (result == 7){
                 for (Ball& b : balls) {
                    if (b.isChoosen){
                        for (int i = 0; i <3; i++)
                            b.clr[i] = rand()%255;
                    }

                }
            }
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});

                // Если мы находимся в режиме выделения, то меняем прямоугольник выделения
                if (isSelecting) {
                    selectionRect.setSize(mousePosition - selectionRect.getPosition());
                    for (Ball& b: balls){
                        if (((b.position.x -b.radius > min(mousePosition.x , selectionRect.getPosition().x)) &&(b.position.x + b.radius < max(mousePosition.x , selectionRect.getPosition().x))) && ((b.position.y -b.radius > min(mousePosition.y , selectionRect.getPosition().y)) &&(b.position.y + b.radius < max(mousePosition.y , selectionRect.getPosition().y))))
                            b.isChoosen = true;
                        else
                            b.isChoosen = false;
                    }
                }
                if (moving_balls){
                    for (Ball& b : balls){
                            if (b.isChoosen){
                                b.position.x += mousePosition.x - pos.x;
                                b.position.y += mousePosition.y - pos.y;
                            }

                    }
                     pos = mousePosition;
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                for (Ball& b : balls) {
                    if (b.isChoosen){
                        for (int i = 0; i <3; i++)
                            b.clr[i] = rand()%255;
                    }

                }
            }

            if (((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))) && sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
                       buffer.clear();
                buffer_pos = window.mapPixelToCoords(sf::Mouse::getPosition());
                for (Ball& b : balls) {
                    if (b.isChoosen){
                        buffer.push_back(b);
                    }
                }
            }
            if (((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))) && sf::Keyboard::isKeyPressed(sf::Keyboard::X)){
                buffer.clear();
                buffer_pos = window.mapPixelToCoords(sf::Mouse::getPosition());
                for (Ball& b : balls) {
                    if (b.isChoosen){
                        buffer.push_back(b);
                    }
                }
                for (auto b = balls.begin(); b != balls.end(); b++) {
                    if ((*b).isChoosen){
                        b = balls.erase(b);
                    }
                }
            }
            if (((sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))) && sf::Keyboard::isKeyPressed(sf::Keyboard::V)){
                sf::Vector2f mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition()) - buffer_pos;
               for (Ball& b : buffer) {
                    b.position += mousePosition;
                    balls.push_back(b);
                }
            }
             if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete)) {
                for (auto b = balls.begin(); b != balls.end(); b++) {
                    if ((*b).isChoosen){
                        b = balls.erase(b);
                    }

                }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!moving_balls){
                        for (Ball& b : balls) {
                        if ((distance(mousePosition, b.position) < b.radius) && (b.isChoosen)) {
                            pos = mousePosition;
                            moving_balls = true;
                            break;
                        }
                    }}
                    // Если не зажат левый Ctrl, то все выделения снимаются
                    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && !moving_balls) {
                        for (Ball& b : balls) {
                            b.isChoosen = false;
                        }
                    }
                    isSelecting = true;
                    // Проверяем попал ли курсор в какой-нибудь шарик, если попал - выделяем
                    for (Ball& b : balls) {
                        if (distance(mousePosition, b.position) < b.radius) {
                            b.isChoosen = true;
                            isSelecting = false;
                            break;
                        }
                    }
                    // ЛКМ + левый Alt - добавляем новый случайный шарик
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) {
                        balls.push_back(Ball(mousePosition, 5 + rand() % 40));
                    }
                    // Задаём новое положения прямоугольника выделения
                    selectionRect.setPosition(mousePosition);
                    selectionRect.setSize({0, 0});
                }
            }
            // При отпускании кнопки мыши выходим из режима выделения
            if (event.type == sf::Event::MouseButtonReleased) {
                isSelecting = false;
                moving_balls = false;
            }
        }

        window.clear(sf::Color::Black);
        // Рисуем все шарики
        for (Ball& b : balls) {
            b.draw(window);
        }
        // Рисуем прямоугольник выделения
        if (isSelecting) {
            window.draw(selectionRect);
        }
        contextMenu.draw();
        window.display();
    }

    return 0;
}
