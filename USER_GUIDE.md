# Mario Game - User's Guide

## Table of Contents
1. [Getting Started](#getting-started)
2. [Main Menu Navigation](#main-menu-navigation)
3. [Game Setup](#game-setup)
4. [Controls](#controls)
5. [Level Selection](#level-selection)
6. [Gameplay](#gameplay)
7. [Characters](#characters)
8. [Enemies](#enemies)
9. [Items & Power-ups](#items--power-ups)
10. [Game Settings](#game-settings)
11. [Pause Menu](#pause-menu)
12. [Game Over & Winning](#game-over--winning)

## Getting Started

### System Requirements
- Windows operating system
- Graphics card with OpenGL support
- Keyboard and mouse
- Audio device for sound effects and music

### Launching the Game
1. Run the game executable
2. The game will start with the main menu

## Main Menu Navigation

### Menu Options
- **START GAME**: Begin a new game with current settings
- **CHOOSE LEVEL**: Select specific levels to play
- **SETTINGS**: Adjust audio and game preferences
- **CONTROLS**: View and customize key bindings
- **EXIT GAME**: Close the application

### Navigation Controls
- **Arrow Keys** or **WASD**: Navigate menu options
- **Enter** or **Space**: Select highlighted option
- **Mouse**: Click on buttons directly
- **Escape**: Exit the game from main menu

## Game Setup

### Character Selection
Choose your gameplay mode:
- **Single Player (Mario)**: Play as Mario alone
- **Single Player (Luigi)**: Play as Luigi alone  
- **Two Player (Dual)**: Both Mario and Luigi can play

### Difficulty Levels
- **EASY**: More lives to start with
- **HARD**: Fewer lives to start with
- **HARDCORE**: Minimum lives to start with

### Navigation in Setup
- **Arrow Keys** or **WASD**: Navigate between options
- **Up/Down**: Switch between character selection and difficulty
- **Left/Right**: Change selections within categories
- **Enter/Space**: Confirm selection
- **Play Button**: Start the game
- **Back Button**: Return to main menu
- **Escape/Backspace**: Quick return to main menu

## Controls

### Default Mario Controls
- **A**: Move Left
- **D**: Move Right
- **W**: Jump
- **S**: Crouch/Duck
- **Left Shift**: Run/Sprint
- **F**: Throw Fireball (when Fire Mario)

### Default Luigi Controls (Two Player Mode)
- **Left Arrow**: Move Left
- **Right Arrow**: Move Right
- **Up Arrow**: Jump
- **Down Arrow**: Crouch/Duck
- **Right Shift**: Run/Sprint
- **/ (Slash)**: Throw Fireball (when Fire Luigi)

### Universal Controls
- **P**: Pause game during gameplay
- **Escape**: Access pause menu
- **Backspace**: Go back in menus

### Customizing Controls
1. Go to **Controls** from the main menu
2. Navigate using **Arrow Keys** or **WASD**
3. Select the action you want to change
4. Press **Enter** or **Space** to edit
5. Press the new key you want to assign
6. **Tab**: Switch between table and back button
7. **Backspace**: Exit controls menu

**Note**: The game automatically handles key conflicts by swapping assignments.

## Level Selection

### Accessing Level Selection
- Choose **CHOOSE LEVEL** from the main menu
- Use **Arrow Keys** to move Mario between level portals
- Walk into level rectangles to select them
- Wait 0.7 seconds for automatic level start
- **P**: Return to main menu

### Available Levels
- **World 1-1**: Classic Mario level
- **World 1-2**: Underground/castle level
- **World 1-3**: Advanced challenge level

## Gameplay

### Basic Movement
- **Left/Right**: Walk in directions
- **Hold Run + Direction**: Sprint for faster movement
- **Jump**: Leap over obstacles and enemies
- **Crouch**: Duck under low obstacles (Big Mario/Luigi only)

### Advanced Techniques
- **Running Jump**: Hold run button while jumping for longer distance
- **Precision Landing**: Release direction keys for controlled landings
- **Skidding**: Quickly change direction while running to skid
- **Wall Jumping**: Jump off walls in certain situations

### Game Objectives
- Reach the end of each level
- Collect coins for points
- Avoid or defeat enemies
- Collect power-ups to grow stronger
- Maintain your lives (restart at 0 lives)

## Characters

### Mario/Luigi Forms

#### Small Mario/Luigi
- **Abilities**: Basic movement, jumping
- **Vulnerability**: Dies in one hit from enemies
- **Size**: Compact, fits through small spaces

#### Big Mario/Luigi (After Mushroom)
- **Abilities**: Can break blocks, crouch
- **Vulnerability**: Shrinks to small form when hit
- **Size**: Larger, cannot fit through small gaps

#### Fire Mario/Luigi (After Second Mushroom)
- **Abilities**: Throw fireballs, break blocks, crouch
- **Fireball Control**: Press fireball button to shoot projectiles
- **Vulnerability**: Shrinks to small form when hit
- **Combat**: Fireballs defeat most enemies instantly
- **Activation**: Collect a second mushroom while already Big Mario

## Enemies

### Enemy Types & Behaviors

#### Goomba
- **Appearance**: Brown mushroom-like creatures
- **Behavior**: Walks back and forth on platforms
- **Defeat Method**: Jump on top to squash
- **Danger**: Touching sides causes damage

#### Koopa Troopa
- **Appearance**: Turtle enemies with shells
- **Behavior**: Walks on platforms, retreats into shell when jumped on
- **Defeat Method**: 
  - Jump once to make them retreat into shell
  - Jump again or kick shell to defeat completely
  - Kicked shells can defeat other enemies
- **Danger**: Touching while active causes damage

#### Spiny
- **Appearance**: Spiky-shelled enemies
- **Behavior**: Walks on platforms with dangerous spikes
- **Defeat Method**: Cannot be safely jumped on! Use fireballs or kicked shells
- **Danger**: Damages Mario even when jumped on due to spikes

#### Piranha Plant
- **Appearance**: Green pipe-dwelling plants
- **Behavior**: Emerges and hides on a timer cycle
- **Defeat Method**: Use fireballs or avoid entirely
- **Danger**: Cannot be jumped on safely
- **Strategy**: Wait for them to retreat into pipes

#### Lakitu
- **Appearance**: Cloud-riding enemies above the action
- **Behavior**: Follows Mario and throws Spiny eggs as projectiles
- **Defeat Method**: Jump on them or use fireballs
- **Danger**: Creates additional Spiny enemies
- **Strategy**: Defeat quickly to prevent Spiny spawning

#### Bullet Bill
- **Appearance**: Black projectile enemies
- **Behavior**: Flies in straight lines at high speed
- **Defeat Method**: Jump on them or use fireballs
- **Danger**: Fast-moving and hard to avoid
- **Strategy**: Time jumps carefully or duck under them

## Items & Power-ups

### Collectible Items

#### Coins
- **Effect**: Increase score by 100 points
- **Location**: Floating in levels, inside question blocks

#### Question Blocks
- **Content**: May contain coins, mushrooms, or stars
- **Activation**: Jump and hit from below
- **Types**: 
  - Single-use blocks (turn into empty blocks)

### Power-ups

#### Mushroom (Red)
- **Effect**: 
  - First mushroom: Transforms Small Mario into Big Mario
  - Second mushroom: Transforms Big Mario into Fire Mario
- **Behavior**: Moves horizontally when released
- **Collection**: Touch to collect automatically
- **Benefit**: Allows breaking blocks, provides extra hit point, or grants fire powers


#### Star
- **Effect**: Temporary invincibility (star power)
- **Duration**: Limited time with special music
- **Behavior**: Bounces around when released
- **Benefits**: 
  - Invulnerable to all enemies
  - Enemies are defeated on contact
  - Increased movement speed
  - Glowing appearance

## Game Settings

### Audio Settings
- **Master Volume**: Overall game volume
- **Music Volume**: Background music level
- **Sound Effects Volume**: Action sounds level

### Accessing Settings
- Select **SETTINGS** from main menu
- Use **Arrow Keys** to navigate options
- **Enter/Space**: Adjust selected setting
- **Backspace**: Return to main menu

## Pause Menu

### Accessing Pause
- Press **P** during gameplay
- Game freezes until you make a selection

### Pause Options
- **RESUME**: Continue playing
- **CONTROLS**: View/change key bindings
- **SETTINGS**: Adjust audio settings
- **EXIT**: Return to main menu (progress lost)

### Navigation
- **Arrow Keys** or **WASD**: Select options
- **Enter/Space**: Confirm selection
- **Mouse**: Click on buttons

## Game Over & Winning

### Game Over Conditions
- Lose all lives (start with 3)
- Fall into pits or lava
- Get hit by enemies when Small Mario

### Game Over Screen
- **PLAY AGAIN**: Restart from beginning
- **BACK TO MENU**: Return to main menu
- Shows your final score and high score

### Winning Levels
- Reach the end goal of each level
- Automatic progression to next level
- Score bonus based on performance
- Time bonus for quick completion

### Level Completion
- **Map1 → Map2**: Automatic progression
- **Map2 → Map3**: Automatic progression  
- **Map3**: Returns to main menu upon completion

## Troubleshooting

### Performance Issues
- Close other applications for better performance
- Check graphics drivers are updated
- Ensure sufficient system memory

### Control Issues
- Reset controls to defaults in Controls menu
- Check keyboard functionality in other applications
- Verify key bindings are properly assigned

### Audio Issues
- Check system audio settings
- Adjust in-game volume settings
- Verify audio device is connected and working

---

