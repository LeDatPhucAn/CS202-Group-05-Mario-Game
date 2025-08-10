#pragma once
#include "Structs.hpp"
#include <unordered_map>
#include <string>

class KeyBindingManager 
{
private:
    static std::unordered_map<std::string, std::unordered_map<Action, int>> playerKeyBindings;
    static const std::string KEYBINDINGS_FILE;

public:
    static void initializeDefaultBindings();
    static void resetToDefaults();
    static void setKeyBinding(const std::string& playerName, Action action, int key);
    static int getKeyBinding(const std::string& playerName, Action action);
    static std::unordered_map<Action, int> getPlayerBindings(const std::string& playerName);
    static void saveToFile();
    static void loadFromFile();
};
