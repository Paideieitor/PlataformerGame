# Ninja jumps and throws shurikens, he kinda climbs too
## by Full de Mango

## Description

 Pretty good game, shouldn't be to hard to beat. The title is pretty self-explanatory just be a ninja, jump around and throw shurikens.

## Key Features

 - Shuriken attack (they kill enemies and get stuck on walls)
 - Can jump on shurikens stuck to the walls
 - 2 shurikens colliding results in bought getting destroyed
 - 2 types of enemies:
	- Bats: flying creatures that chase you when near
	- Guards: shout when they see you and will chase you, they won't hesitate droping from a ledge to catch you and will wait for you if they can't catch you
 - Game loading from an xml file
 - 2 Levels divided in checkpoints
 - Save and load function (EVERYTHING IS SAVED)*almost*
 - All movement is normalized
 - Entity system
 - UI with buttons, scrollbars, checkboxes and rotating button
 - Timer for each level
 - Time based highest score system 

## Controls

	Movement				Usefull							Debug
		W(jump)				R(respawn in last checkpoint)				F1(start level 1)
	A(move left)	D(move right)		Escape(exit / return to main menu)			F2(start level 2)
	Shift(throw shuriken [1sec coldown])	Enter(skip logo scene and exit menus)			F3(restart current level)
													F5(save game [save current checkpoint and level])
													F6(load game [load current checkpoint and level])
													F7(ui debug info)
													F8(pathfinding debug info)
													F9(draw colliders)
													F10(god mode)
													F11(cap/uncap FPS)
													Right arrow(advance to next checkpoint)
													Left arrow(reset checkpoint to the start of the level)
													
## Developer

 - Rami Rodríguez, David   - [Paideieitor](https://github.com/Paideieitor)

## License

This project is licensed under an unmodified MIT license, which is an OSI-certified license that allows static linking with closed source software. Check [LICENSE](LICENSE) for further details.

Music from:
https://www.youtube.com/user/8BitUniverseMusic
Guard Alert sound:
Sound #531750 by Phil Strahl from https://freesound.org/people/PhilStrahl/sounds/531750/

For extra licenses read COPYING.txt or readme.txt in every folder at ../Game/Source/Externals
