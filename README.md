# Super Mario Bros - C++ Game Engine

**CS202 - Programming Systems Project**

**Team Members**:
* Lê Đạt Phúc An
* Bùi Trọng Thành
* Trần Cảnh Anh Tuấn
* Trần Tôn Minh Kỳ

A modern C++ implementation of the classic Super Mario Bros game with advanced game development patterns and object-oriented programming principles.

## 🎮 Features

- **Dual Character Support**: Mario & Luigi with independent controls
- **Multiple Game Modes**: Single/Dual player, 3 difficulty levels
- **Physics-Based Gameplay**: Box2D integration for realistic movement and collisions
- **Advanced Audio System**: Dynamic music and sound effects
- **Progressive Levels**: Multiple maps with increasing difficulty
- **Classic Power-ups**: Mushrooms, Fire Flowers, Star power
- **Customizable Controls**: Rebindable key mappings for personalized gameplay

## 🛠️ Technologies

- **C++17** + **Raylib** + **Box2D** + **CMake/Ninja**
- JSON parsing, Tiled map support, Audio system

## 🚀 Quick Start

### Windows
```batch
# Build and run release version
setup.bat
run.bat


## 🏗️ Object-Oriented Architecture

The project demonstrates advanced OOP principles taught in CS202:

### Design Patterns Implementation
- **Singleton Pattern**: 
  - `SoundController` - Centralized audio management
  - `GameInfo` - Global game state management
  - `KeyBindingManager` - Input configuration management

- **State Pattern**: 
  - Player behavior management (idle, running, jumping, etc.)
  - Enemy AI state transitions

- **Factory Pattern**: 
  - Scene creation and management
  - Dynamic enemy generation

- **Observer Pattern**: 
  - Physics collision event handling
  - Game event notifications

- **Strategy Pattern**: 
  - Block interaction behaviors
  - Different enemy AI strategies

### Class Hierarchy
- **Base Classes**: GameObject, Entity, Collider
- **Inheritance**: Player → Mario/Luigi, Enemy → Goomba/Koopa
- **Polymorphism**: Dynamic behavior through virtual functions
- **Encapsulation**: Private member variables with public interfaces
- **Abstraction**: Clean interfaces for complex game systems

## 📚 CS202 Learning Objectives Demonstrated

This project showcases key object-oriented programming concepts:

- **Inheritance & Polymorphism**: Character classes inherit from base Entity class
- **Encapsulation**: Game state and logic properly encapsulated in classes
- **Abstraction**: Clean interfaces hide complex implementation details
- **Composition**: Game objects composed of smaller, reusable components
- **SOLID Principles**: Single responsibility, open/closed, and dependency inversion
- **Memory Management**: Smart pointers and RAII for safe resource handling

## 🎯 Technical Achievements

- **Real-time Physics Simulation**: Integration with Box2D for authentic game feel
- **Efficient Rendering Pipeline**: Optimized graphics rendering with Raylib
- **Modular Audio System**: Dynamic sound management with multiple channels
- **Scalable Architecture**: Easy to extend with new characters, levels, and features
- **Cross-platform Compatibility**: Runs on Windows, Linux, and macOS

## 🤝 Team Contributions

This project represents collaborative work in:
- **Code Architecture**: Designing maintainable OOP structure
- **Game Development**: Implementing classic gameplay mechanics
- **Physics Integration**: Realistic collision detection and response
- **Audio/Visual Systems**: Creating engaging multimedia experience

## 📝 License

Educational project developed for CS202 - Programming Systems course.

Original Super Mario Bros game and assets © Nintendo. This implementation is for educational purposes only.

## 🙏 Acknowledgments

- CS202 course instructors for guidance on OOP principles
- Raylib community for excellent game development framework
- Box2D developers for robust physics engine
- Nintendo for the timeless Super Mario Bros game design
