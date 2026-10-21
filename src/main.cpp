#include <cassert>
#include <iostream>

#include "basic_transform.h"
#include "sprite_manager.h"

int main() {
    // Quick sanity checks for compose / inverse (values chosen to be exact in float).
    {
        BasicTransform f({2.f, -4.f}, {5.f, 7.f});
        BasicTransform g({0.5f, 2.f}, {-3.f, 1.f});
        sf::Vector2f p{3.f, -2.f};

        assert(BasicTransform::compose(f, g).applyPoint(p) == f.applyPoint(g.applyPoint(p)));
        assert(BasicTransform::compose(f, g).applyVector(p) == f.applyVector(g.applyVector(p)));
        assert(BasicTransform::compose(f, BasicTransform::inverse(f)) == BasicTransform{});
        assert(BasicTransform::compose(BasicTransform::inverse(f), f) == BasicTransform{});
        std::cout << "compose/inverse checks passed\n";
    }

    SpriteManager spriteManager;
    auto& texture = spriteManager.registerTexture("assets/sword_diamond.png");
    assert(&texture == &spriteManager.registerTexture("assets/sword_diamond.png"));  // idempotent
    auto spriteID = spriteManager.createSprite(texture);
    auto& sprite = spriteManager.getSprite(spriteID);
    sprite.origin() = {114, 14};
    sprite.position() = {128, 128};
    sprite.scale().x = -1;

    sf::RenderWindow window(sf::VideoMode({256, 256}), "Sprites lab", sf::State::Windowed);
    // window.setMouseCursorVisible(false); // you can use this to hide the mouse
    while (window.isOpen()) {
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                return 0;
            }
        }
        sprite.position() = sf::Vector2f(sf::Mouse::getPosition(window));
        sf::RenderStates states{};
        window.clear(sf::Color::White);
        sprite.draw(window, states);
        window.display();
    }
    return 0;
}