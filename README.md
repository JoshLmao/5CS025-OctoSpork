<p align="center">
  <h1>Octo-Spork</h1>
  <a href="https://twitter.com/JoshLmao">
    <img src="https://img.shields.io/badge/twitter-JoshLmao-blue.svg?style=flat-square.svg"/>
  </a>
</p>

### About

Octo-Spork is a text-based adventure game I created for my assignment at University (Level 5). The story is that you are trapped inside an unknown building with three spirits blocking the path. You must search around the building to find a way to get out.

### Code Explanation

Octo-Spork is coded in C++ and meant to be run inside a console. The game, it's rooms and it's items are all loaded from "Octo-Spork/Octo-Spork/resources/game.json". [RapidJSON](https://rapidjson.org/) is being used to parse the json and extract the data into memory for being used. [OctoSpork.cpp](https://github.com/JoshLmao/Octo-Spork/blob/master/Octo-Spork/Octo-Spork/OctoSpork.cpp) also contains specific code for triggering the specific ending. However, you can change the game.json file to have extra or less rooms/items/npc's
