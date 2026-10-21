#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <cassert>
#include <cstddef>
#include <deque>
#include <string>
#include <unordered_map>
#include <utility>

#include "sprite.h"
#include <SFML/Graphics.hpp>

using SpriteID = size_t;

// Centralized, append-only storage for textures and sprites (no deletion needed for this lab).
class SpriteManager {
public:
    // Loads the texture at filepath into the manager's storage (only the first time) and
    // returns a reference to it. Idempotent: the same filepath always returns the same object.
    sf::Texture& registerTexture(const std::string& filepath) {
        // try_emplace only constructs sf::Texture(filepath) if the key isn't already present;
        // otherwise it returns the existing entry untouched.
        auto [it, inserted] = mTextures.try_emplace(filepath, filepath);
        return it->second;
    }

    // Constructs a Sprite in place from any of Sprite's constructor argument sets and
    // returns the slot (index) it occupies.
    template <typename... Args>
    SpriteID createSprite(Args&&... args) {
        mSprites.emplace_back(std::forward<Args>(args)...);
        return mSprites.size() - 1;
    }

    // Returns a reference to the sprite stored in slot id.
    Sprite& getSprite(SpriteID id) {
        assert(id < mSprites.size());
        return mSprites[id];
    }

private:
    // unordered_map: keyed by filepath (gives idempotency), and references to its elements
    // stay valid even when it rehashes. This matters because Sprite stores a raw
    // const sf::Texture*, so a texture must never move in memory.
    std::unordered_map<std::string, sf::Texture> mTextures;

    // deque: emplace_back never moves existing elements, so references returned by
    // getSprite stay valid after more sprites are created (a vector would reallocate).
    std::deque<Sprite> mSprites;
};

#endif  // SPRITE_MANAGER_H