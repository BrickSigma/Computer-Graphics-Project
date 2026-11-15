# Snakes & Ladders

A self-contained, single-file C++17 desktop game built with [raylib](https://www.raylib.com/). It features a pastel retro-pixel aesthetic with animated sprites, curved snakes, angled ladders, a status tracker, and a victory modal—all drawn procedurally in code with **zero external image or font files required**.

---

## Gameplay & Controls

- **Objective:** Start on square **1** (`START`) and reach square **100** (the golden crown) using an exact dice roll. If a roll exceeds 100, your character stays put until the next turn.
- **Ladders (Climb Up):** `4 → 25`, `13 → 46`, `33 → 49`, `42 → 63`, `50 → 69`, `62 → 81`, `74 → 92`
- **Snakes (Slide Down):** `27 → 5`, `40 → 3`, `54 → 31`, `66 → 45`, `76 → 58`, `89 → 53`, `99 → 78`

### Controls
| Action | Mouse | Keyboard |
| :--- | :--- | :--- |
| **Roll Dice** | Click **ROLL DICE** | `Space` or `Enter` |
| **Restart Game** | Click **START OVER** / **PLAY AGAIN** | `R` (or `Space` / `Enter` on Win screen) |
| **Quit Game** | Click window close button (`X`) | `Esc` |

---

## How the Code Works

The entire game lives in `snakesandladders.cpp` and runs inside a `1200 × 880` window at **60 FPS** using an immediate-mode game loop (`Update` → `Draw`).

### 1. Game State Machine
Instead of blocking timers, gameplay transitions smoothly across five states (`enum State`):
1. **`IDLE`**: Waiting for the player to roll the dice.
2. **`ROLLING`**: Shakes the die and cycles through 7 random faces (`0.07s` per tick) before locking in a final value (`1–6`).
3. **`STEPPING`**: Moves the player forward one square at a time (`0.15s` per step) while updating the square readout and progress bar.
4. **`PAUSE_BEFORE_JUMP`**: Pauses for `0.65s` when landing on a snake head or ladder base so the player can read the status message before teleporting to the destination.
5. **`WON`**: Displays the **Pixel Princess** victory overlay with the total turn count.

### 2. Zig-Zag Board Mapping (`GetSquareCenter`)
Screen coordinates start at `(0, 0)` in the **top-left**, whereas Snakes & Ladders starts at square `1` in the **bottom-left** and alternates direction every row. `GetSquareCenter(square)` converts any square number (`1–100`) into exact screen `(x, y)` pixel coordinates by:
- Calculating the row from the bottom (`(square - 1) / 10`).
- Reversing the column index on odd rows (`9 - cellInRow`).
- Flipping the vertical axis (`9 - rowFromBottom`) and scaling by `CELL_SIZE` (`60px`).

### 3. Procedural Graphics (No Assets Needed)
Every visual element is constructed from basic raylib shapes (`DrawRectangle`, `DrawTriangle`, `DrawLineEx`) every frame:
- **Painter's Algorithm & Drop Shadows:** Elements are drawn back-to-front (grid background → board frame → squares → ladders/snakes → player → UI cards → win modal). Text and cards draw a dark offset copy first to create crisp 16-bit drop shadows.
- **Pixel Art Sprites:** `DrawPixelPerson`, `DrawPixelCrown`, `DrawPixelHeart`, and `DrawSparkle` assemble tiny colored blocks at relative grid offsets.
- **Dynamic Ladders (`DrawLadder`):** Uses perpendicular vector math (`(-dy, dx)`) to draw parallel rails and evenly spaced rungs between any two squares.
- **Curved Snakes (`DrawSnake` & `EvalBezier`):** Evaluates a cubic Bezier curve between the snake's head and tail, rendering 28 segmented strokes with slight gaps and a rotated head (`atan2` + `DrawRectanglePro`).
- **Wave Animations:** Uses `std::sin()` and `std::cos()` driven by `GetTime()` to smoothly bob the player character, pulse the background stars, and shake the rolling die.
- **Immediate-Mode UI:** `CheckCollisionPointRec()` checks mouse hover and click states each frame to animate button elevation and trigger rolls or resets.

---

## How to Install & Run

### Windows (Official Raylib Installer + Notepad++)
1. Download and install the Windows `.exe` installer from the [raylib Releases page](https://github.com/raysan5/raylib/releases) (installs to `C:\raylib` by default, including `w64devkit` and Notepad++).
2. Open `snakesandladders.cpp` in Notepad++ (`C:\raylib\npp\notepad++.exe`).
3. Press **`F6`** to open the **Execute** dialog.
4. Make sure the build command uses **`g++`** and **`-std=c++17`** (the default script uses `gcc -std=c99`, which only compiles C):
   ```bat
   g++ -o snakesandladders.exe snakesandladders.cpp C:\raylib\raylib\src\raylib.rc.data -s -static -O3 -std=c++17 -Wall -Wshadow -Wunused-parameter -IC:\raylib\raylib\src -Iexternal -DPLATFORM_DESKTOP -L. -lraylib -lopengl32 -lgdi32 -lwinmm -lshcore
