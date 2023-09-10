# Kutris - Computer Graphics Project

## Table of Contents
- [Introduction](#introduction)
- [Gameplay](#gameplay)
- [Rules](#rules)
- [Scoring](#scoring)
- [Concepts](#concepts)
  - [Basics](#basics)
  - [Transformation](#transformation)
  - [Interactions](#interactions)
  - [Texture](#texture)
  - [Shading](#shading)
- [Tools Used](#tools-used)
- [Installation](#installation)
- [How to Play](#how-to-play)
- [Contributors](#contributors)

## Introduction

Kutris is a Tetris-like game that challenges players to construct specific patterns by strategically placing blocks. Each game begins with the random assignment of one of two patterns: 'Home' or 'Heart' for the player to follow.

## Gameplay

There are five actions that can be performed by the player:
1. Moving the block one unit at a time using right and left arrow keys.
2. Rotating the object by pressing the space button.
3. Placing the current block by pressing the arrow down button.
4. Ending the game by pressing the 'E' key.
5. Deleting a particular cube from the current cube before placing it by clicking on the cube with the mouse.

## Rules

- Players are given 5 chances to delete a particular cube from the current cube before placing it.
- Score is calculated as follows: +1 point for each correctly placed block and -2 points for any wrongly placed one.

## Scoring

The player's score cannot go below 0.

## Concepts

### Basics

- Shader coding and 3D modeling are used to draw objects in the scene.
- A 10x10 grid is used as the game board.

### Transformation

- 4x4 transformation matrices are used to move and rotate the blocks.
- Animation is implemented for translation and rotation.

### Interactions

- Picking with the mouse is used to select which cube in the block to remove.
- Picking is also used to click the "Play Again" button.

### Texture

- Textures are applied to the blocks to represent different patterns.
- For example, a brick texture is used for the 'House' pattern, and small red hearts are used for the 'Heart' pattern.

### Shading

- Two different shading options, Gouraud and Phong, are implemented to create different visual themes.
- Modified Phong illumination is also used.
- Players can switch between shading options using the 'S' key.

## Tools Used

- [OpenGL](https://www.opengl.org/): An API used to render 2D and 3D vector graphics.
- [GLFW](https://www.glfw.org/): An open-source library for OpenGL, OpenGL ES, and Vulkan used for window creation, surface management, and input event handling.
- C++: The programming language used for implementation.

## Installation

To run Kutris on your local machine, you'll need to have OpenGL and GLFW set up. Follow these steps:

1. Clone this repository to your local machine.
2. Build the project using your preferred C++ compiler.
3. Run the executable to play Kutris.

## How to Play

- Use the right and left arrow keys to move the block.
- Press the space button to rotate the current block.
- Press the arrow down button to place the block.
- Press 'E' to end the game.
- Click on a cube with the mouse to delete it from the current block.

## Contributors

- Begüm Şen (72840)
- Berat Karayılan (72690)
- Sinan Cem Erdoğan (68912)
