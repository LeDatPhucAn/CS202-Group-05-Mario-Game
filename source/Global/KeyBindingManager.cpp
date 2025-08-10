#include "KeyBindingManager.hpp"
#include "raylib.h"
#include <fstream>
#include <iostream>

std::unordered_map<std::string, std::unordered_map<Action, int>> KeyBindingManager::playerKeyBindings;
const std::string KeyBindingManager::KEYBINDINGS_FILE = "assets/keybindings.dat";

void KeyBindingManager::initializeDefaultBindings()
{
    // Mario default bindings
    playerKeyBindings["Mario"] = {
        {Action::BUTTON_LEFT, KEY_A},
        {Action::BUTTON_RIGHT, KEY_D},
        {Action::BUTTON_UP, KEY_W},
        {Action::BUTTON_DOWN, KEY_S},
        {Action::BUTTON_RUN, KEY_LEFT_SHIFT},
        {Action::BUTTON_THROW_FIREBALL, KEY_F}
    };

    // Luigi default bindings
    playerKeyBindings["Luigi"] = {
        {Action::BUTTON_LEFT, KEY_LEFT},
        {Action::BUTTON_RIGHT, KEY_RIGHT},
        {Action::BUTTON_UP, KEY_UP},
        {Action::BUTTON_DOWN, KEY_DOWN},
        {Action::BUTTON_RUN, KEY_RIGHT_SHIFT},
        {Action::BUTTON_THROW_FIREBALL, KEY_SLASH}
    };
}

void KeyBindingManager::setKeyBinding(const std::string& playerName, Action action, int key)
{
    // Check if this key is already bound to another action for this player
    auto playerIt = playerKeyBindings.find(playerName);
    if (playerIt != playerKeyBindings.end())
    {
        Action conflictingAction = static_cast<Action>(-1); // Invalid action
        int oldKey = 0;
        
        // Find if the new key is already bound to another action
        for (const auto& binding : playerIt->second)
        {
            if (binding.second == key && binding.first != action)
            {
                conflictingAction = binding.first;
                break;
            }
        }
        
        // Get the old key for the current action
        auto actionIt = playerIt->second.find(action);
        if (actionIt != playerIt->second.end())
        {
            oldKey = actionIt->second;
        }
        
        // If there's a conflict, swap the keys
        if (conflictingAction != static_cast<Action>(-1))
        {
            std::cout << "Key conflict detected! Swapping keys between actions." << std::endl;
            // Set the conflicting action to use the old key
            playerKeyBindings[playerName][conflictingAction] = oldKey;
        }
    }
    
    // Set the new key binding
    playerKeyBindings[playerName][action] = key;
    saveToFile(); // Auto-save when changed
}

int KeyBindingManager::getKeyBinding(const std::string& playerName, Action action)
{
    auto playerIt = playerKeyBindings.find(playerName);
    if (playerIt != playerKeyBindings.end())
    {
        auto actionIt = playerIt->second.find(action);
        if (actionIt != playerIt->second.end())
        {
            return actionIt->second;
        }
    }
    return 0; // Return 0 if not found
}

std::unordered_map<Action, int> KeyBindingManager::getPlayerBindings(const std::string& playerName)
{
    auto playerIt = playerKeyBindings.find(playerName);
    if (playerIt != playerKeyBindings.end())
    {
        return playerIt->second;
    }
    return {}; // Return empty map if player not found
}

void KeyBindingManager::saveToFile()
{
    std::ofstream file(KEYBINDINGS_FILE, std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Failed to save key bindings to file" << std::endl;
        return;
    }

    // Save number of players
    size_t playerCount = playerKeyBindings.size();
    file.write(reinterpret_cast<const char*>(&playerCount), sizeof(playerCount));

    // Save each player's bindings
    for (const auto& player : playerKeyBindings)
    {
        // Save player name length and name
        size_t nameLength = player.first.length();
        file.write(reinterpret_cast<const char*>(&nameLength), sizeof(nameLength));
        file.write(player.first.c_str(), nameLength);

        // Save number of bindings
        size_t bindingCount = player.second.size();
        file.write(reinterpret_cast<const char*>(&bindingCount), sizeof(bindingCount));

        // Save each binding
        for (const auto& binding : player.second)
        {
            file.write(reinterpret_cast<const char*>(&binding.first), sizeof(binding.first));
            file.write(reinterpret_cast<const char*>(&binding.second), sizeof(binding.second));
        }
    }

    file.close();
}

void KeyBindingManager::loadFromFile()
{
    std::ifstream file(KEYBINDINGS_FILE, std::ios::binary);
    if (!file.is_open())
    {
        // File doesn't exist, use default bindings
        initializeDefaultBindings();
        return;
    }

    playerKeyBindings.clear();

    // Load number of players
    size_t playerCount;
    file.read(reinterpret_cast<char*>(&playerCount), sizeof(playerCount));

    // Load each player's bindings
    for (size_t i = 0; i < playerCount; i++)
    {
        // Load player name
        size_t nameLength;
        file.read(reinterpret_cast<char*>(&nameLength), sizeof(nameLength));
        
        std::string playerName(nameLength, '\0');
        file.read(&playerName[0], nameLength);

        // Load number of bindings
        size_t bindingCount;
        file.read(reinterpret_cast<char*>(&bindingCount), sizeof(bindingCount));

        // Load each binding
        std::unordered_map<Action, int> bindings;
        for (size_t j = 0; j < bindingCount; j++)
        {
            Action action;
            int key;
            file.read(reinterpret_cast<char*>(&action), sizeof(action));
            file.read(reinterpret_cast<char*>(&key), sizeof(key));
            bindings[action] = key;
        }

        playerKeyBindings[playerName] = bindings;
    }

    file.close();
}

void KeyBindingManager::resetToDefaults()
{
    std::cout << "Resetting to default key bindings..." << std::endl;
    initializeDefaultBindings();
    saveToFile(); // Save the defaults to file
    std::cout << "Reset to defaults complete" << std::endl;
}
