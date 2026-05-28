# Walkthrough - Item System Implementation

We have successfully implemented the items (Dash, Kamikaze, Bow) and their synergy (Arcomikaze) exactly as detailed in the game's design report PDF, using an optimized design centered on your initial suggestion.

## Changes Made

### 1. Item Spawning & Pickups
* **[Items.h](file:///C:/Users/Sebastian/Documents/GitHub/TP-Programacion-2.-Extendible-map-RPG-in-C-/RPGGame/Items.h)**:
  * Expanded `PickupType` enum with `PICKUP_ITEM_BOW`, `PICKUP_ITEM_KAMIKAZE`, and `PICKUP_ITEM_DASH`.
* **[Items.cpp](file:///C:/Users/Sebastian/Documents/GitHub/TP-Programacion-2.-Extendible-map-RPG-in-C-/RPGGame/Items.cpp)**:
  * Assigned distinct rendering colors to ground items: **Green** for Bow, **Orange** for Kamikaze, and **Cyan** for Dash.
* **[Habitacion.cpp](file:///C:/Users/Sebastian/Documents/GitHub/TP-Programacion-2.-Extendible-map-RPG-in-C-/RPGGame/Habitacion.cpp)**:
  * Spawns these actual item pickups inside `TREASURE` rooms instead of placeholder coins/hearts.

### 2. Player Logic (Jugador)
* **[Jugador.h](file:///C:/Users/Sebastian/Documents/GitHub/TP-Programacion-2.-Extendible-map-RPG-in-C-/RPGGame/Jugador.h) & [Jugador.cpp](file:///C:/Users/Sebastian/Documents/GitHub/TP-Programacion-2.-Extendible-map-RPG-in-C-/RPGGame/Jugador.cpp)**:
  * **hasItem()**: Converted to a `const` member function to allow calling it safely from within `getAttackHitbox() const`.
  * **addItem()**: Automatically recalculates and applies a permanent speed boost (`+1.5f`) when the Bow or Arcomikaze is collected.
  * **setStats()**: Ensures that the speed boost is correctly applied even when loading a save file that includes the Bow/Arcomikaze.
  * **addPickup()**: Maps the collection of item pickups on the floor to the player's inventory list.
  * **updateMovement()**:
    * **Dash**: Restricts dash activation to players who actually possess the `ITEM_DASH` item, and adjusts the cooldown to 7.0 seconds.
    * **Kamikaze**: Triggers the explosion when the attack key is pressed, starting a 7.0-second cooldown and raising `pendingExplosionSpawn`.
    * **Bow / Arcomikaze**: Shoots arrows when attacking (cooldown of 0.4s), raising `pendingProjectileSpawn`.
  * **getAttackHitbox()**:
    * Disables the melee sword hitbox (`0,0,0,0`) if shooting arrows (Bow / Arcomikaze).
    * Extends the hitbox to a large square (200x200) centered on the player if using Kamikaze.
  * **render()**: Renders a temporary transparent orange/red circle centered on the player for real-time visual feedback when Kamikaze triggers.

### 3. Room & Collision Logic (Habitacion)
* **[Habitacion.h](file:///C:/Users/Sebastian/Documents/GitHub/TP-Programacion-2.-Extendible-map-RPG-in-C-/RPGGame/Habitacion.h) & [Habitacion.cpp](file:///C:/Users/Sebastian/Documents/GitHub/TP-Programacion-2.-Extendible-map-RPG-in-C-/RPGGame/Habitacion.cpp)**:
  * Added `VisualExplosion` container support inside `Habitacion`.
  * **update()**:
    * Listens for the player's `pendingProjectileSpawn` to create and fire a player projectile (colored orange/red and scaled up if using the explosive Arcomikaze synergy).
    * Listens for the player's `pendingExplosionSpawn` to generate a fire ring effect.
    * Updates visual explosions, removing them after a lifetime of 0.25 seconds.
    * Implements collision checks for player projectiles against enemies (applying damage and destroying the projectile).
    * Implements collision checks for player projectiles against walls (allowing explosive arrows to blow up on walls!).
    * **Arcomikaze Synergy Explosion**: When an explosive projectile hits an enemy or wall, deals double damage to all enemies within a radius of 100 pixels, and triggers a visual explosion.
  * **renderFondo()**: Renders all visual explosions as expanding, fading fire rings for premium aesthetics.

---

## Verification & Compilation
The project compiles successfully with the custom local compiler via the following command:
```powershell
& "Librerias/mingw64/bin/g++.exe" Character.cpp DungeonGenerator.cpp Enemigos.cpp Entity.cpp GameConfig.cpp Game_Class.cpp Game_MapTiles.cpp HUD.cpp Habitacion.cpp Items.cpp Jugador.cpp MainMenu.cpp Proyectil.cpp SaveManager.cpp main.cpp -o bin/Debug/RPGGame.exe -I"Librerias/SFML-2.6.2/include" -L"Librerias/SFML-2.6.2/lib" -lsfml-graphics-d -lsfml-window-d -lsfml-system-d -lsfml-audio-d -lsfml-network-d -std=c++17 -Wall
```

All source code files build without syntax or linking errors.

---

## Manual Verification Checklist for Gameplay
- [ ] Run `bin/Debug/RPGGame.exe` to open the game.
- [ ] Find a treasure room (Green door on minimap or marked TREASURE).
- [ ] Verify that there are three item pickups (Green = Bow, Orange = Kamikaze, Cyan = Dash).
- [ ] Pick up **Dash**: test dashing using Space and verify that you must wait 7 seconds to use it again.
- [ ] Pick up **Kamikaze**: test attacking with K, verify you perform a large orange area explosion instead of a sword swipe, and verify it has a 7-second cooldown.
- [ ] Pick up **Bow**: verify your walking speed increases, you no longer hit enemies with melee attacks, and you fire yellow arrows that damage enemies.
- [ ] Pick up **Bow + Kamikaze**: verify arrows are orange/larger and explode upon hitting an enemy or a wall, damaging all enemies in the area.
