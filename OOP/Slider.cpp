#include <iostream>
#include <cmath>
#include <list>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

float distance(sf::Vector2f start, sf::Vector2f finish)
{
    return sqrtf((start.x - finish.x)*(start.x - finish.x) + (start.y - finish.y)*(start.y - finish.y));
}

void drawLine(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f finish, sf::Color color = sf::Color::White)
{
    sf::Vertex line_vertices[2] = {sf::Vertex(start, color), sf::Vertex(finish, color)};
    window.draw(line_vertices, 2, sf::Lines);
}

bool in_box(sf::Vector2f mouse, sf::Vector2f pos, sf::Vector2f siz ){
    if ((mouse.x > pos.x) && (mouse.x < pos.x + siz.x))
        if ((mouse.y > pos.y) && (mouse.y < pos.y + siz.y))
            return true;
    return false;
}
struct Ball
{
    sf::Vector2f position;
    float radius;
    bool isChoosen;
    vector<int> clr = {255, 255, 255};

    Ball(sf::Vector2f position, float radius) : position(position), radius(radius)
    {
        isChoosen = true;
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

    }
};

struct Slider
{
private:
    int min_val;
    int max_val;
    public:
    sf::Vector2f position;
    sf::Vector2f slider_position;
    sf::Font font;
    string name;
    int value;
    bool isChoosen = false;
    Slider(int min_val,int max_val, sf::Vector2f position, string name, float value): min_val(min_val), max_val(max_val), position(position), slider_position({position.x + value/max_val * 200, position.y - 15}), name(name), value(value)
    {
        if (!font.loadFromFile("consolas.ttf")) {
        std::cout << "Can't load button font" << std::endl;
        }
    }

    void draw(sf::RenderWindow& window)
    {
        value = (slider_position.x - position.x)/200 * max_val + min_val;
        sf::Text Name;
        sf::Text Value;
        sf::RectangleShape slider;
        sf::RectangleShape rectangle;
        Name.setString(name);
        Value.setString(to_string(value));
        Name.setFont(font);
        Value.setFont(font);
        Name.setCharacterSize(30);
        Name.setFillColor(sf::Color::White);
        Name.setPosition(position-sf::Vector2f({70, 10}));
        Value.setCharacterSize(30);
        Value.setFillColor(sf::Color::White);
        Value.setPosition(position+sf::Vector2f({220, -10}));
        slider.setSize(sf::Vector2f(10, 40));
        rectangle.setSize(sf::Vector2f(200, 10));
        rectangle.setFillColor(sf::Color{255, 255, 255});
        slider.setFillColor(sf::Color{200, 200, 200});
        rectangle.setPosition(position);
        slider.setPosition(slider_position);
        window.draw(rectangle);
        window.draw(slider);
        window.draw(Name);
        window.draw(Value);
    }
};

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Slider!", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    vector<Slider> sliders;
    sliders.push_back(Slider(0,255,{100, 400}, "B", 255));
    sliders.push_back(Slider(0,255,{100, 300}, "G", 255));
    sliders.push_back(Slider(0,255,{100, 200}, "R", 255));
    sliders.push_back(Slider(10,90,{100, 100}, "Size", 0));
    // Создаём связный список из шариков
    // Связный список -- потому что нам нужно будет постоянно удалять и добавлять в этот список
    std::list<Ball> balls;
    balls.push_back(Ball({550, 250}, 20));

    // Создаём прямоугольник выделения
    // Обратите внимание на четвёртый параметр sf::Color(150, 150, 240, 50)
    // Это alpha - прозрачность      0 = полностью прозрачный     255 = непрозрачный
    sf::RectangleShape selectionRect;
    selectionRect.setFillColor(sf::Color(150, 150, 240, 50));
    selectionRect.setOutlineColor(sf::Color(200, 200, 255));
    selectionRect.setOutlineThickness(1);

    // Специальная переменная, которая будет показывать, что мы находимся в режиме выделения
    bool isSelecting = false;
    bool moving_slider = false;
    sf::Vector2f pos;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});

                // Если мы находимся в режиме выделения, то меняем прямоугольник выделения
                if (moving_slider){
                    for (Slider& s: sliders){
                            if (s.isChoosen)
                                s.slider_position.x = min(max(mousePosition.x, s.position.x), s.position.x+200);
                            }

                    }
            }

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePosition = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                if (event.mouseButton.button == sf::Mouse::Left) {
                    for (Slider& s : sliders) {
                        if (in_box(mousePosition, s.position, {200, 10}) && !in_box(mousePosition, s.slider_position, {10, 40})) {
                            s.slider_position.x = mousePosition.x;
                            break;
                        }
                    }
                    if (!moving_slider){
                        for (Slider& s : sliders) {
                        if (in_box(mousePosition, s.slider_position, {10, 40})) {
                            s.isChoosen = true;
                            moving_slider = true;
                            break;
                        }
                    }}
                }
            }
            // При отпускании кнопки мыши выходим из режима выделения
            if (event.type == sf::Event::MouseButtonReleased) {
                moving_slider = false;
                for (Slider& s : sliders) {
                    s.isChoosen = false;

                }

            }
        }

        window.clear(sf::Color::Black);
        // Рисуем все шарики
        for (Ball& b : balls) {
            for (int i = 0; i < 3; i ++)
                b.clr[i] = sliders[2-i].value;
            b.radius = sliders[3].value;
            b.draw(window);
        }
        for (Slider& s: sliders)
            s.draw(window);
        window.display();
    }

    return 0;
}
