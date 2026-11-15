#include "raylib.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>

struct Link {
    int from;
    int to;
};

// Exact ladders and snakes from your App.tsx
const std::vector<Link> LADDERS = {
    {4, 25}, {13, 46}, {33, 49}, {42, 63}, {50, 69}, {62, 81}, {74, 92}
};

const std::vector<Link> SNAKES = {
    {27, 5}, {40, 3}, {54, 31}, {66, 45}, {76, 58}, {89, 53}, {99, 78}
};

// Color palette matching your index.css
const Color COL_BG           = {173, 123, 214, 255}; // #ad7bd6
const Color COL_SHADOW_DARK  = {127, 67, 158, 255};  // #7f439e
const Color COL_TITLE_PINK   = {255, 212, 234, 255}; // #ffd4ea
const Color COL_TITLE_LAV    = {230, 204, 255, 255}; // #e6ccff
const Color COL_FRAME_PINK   = {247, 198, 234, 255}; // #f7c6ea
const Color COL_FRAME_PURPLE = {137, 83, 170, 255};  // #8953aa
const Color COL_FRAME_OUTER  = {212, 139, 206, 255}; // #d48bce
const Color COL_CELL_PINK    = {237, 186, 223, 255}; // #edbadf
const Color COL_CELL_PURPLE  = {198, 164, 231, 255}; // #c6a4e7
const Color COL_MILESTONE    = {213, 129, 195, 255}; // #d581c3
const Color COL_LADDER       = {113, 64, 152, 255};  // #714098
const Color COL_LADDER_SHAD  = {241, 200, 255, 255}; // #f1c8ff
const Color COL_SNAKE        = {186, 79, 143, 255};  // #ba4f8f
const Color COL_SNAKE_SHAD   = {113, 61, 142, 255};  // #713d8e
const Color COL_CARD_STATUS  = {198, 120, 193, 255}; // #c678c1
const Color COL_CARD_DICE_1  = {220, 169, 230, 255}; // #dca9e6
const Color COL_CARD_DICE_2  = {233, 158, 207, 255}; // #e99ecf
const Color COL_CARD_LEGEND  = {181, 107, 184, 255}; // #b56bb8
const Color COL_BTN          = {150, 86, 183, 255};  // #9656b7
const Color COL_BTN_HOVER    = {168, 98, 196, 255};  // #a862c4
const Color COL_BTN_SHADOW   = {104, 61, 132, 255};  // #683d84

// Board geometry constants
const int BOARD_X = 153;
const int BOARD_Y = 178;
const int CELL_SIZE = 60; // 10 x 60 = 600x600 board

// Convert square number (1..100) to center pixel coordinates on screen
Vector2 GetSquareCenter(int square) {
    int idx = (square < 1 ? 1 : (square > 100 ? 100 : square)) - 1;
    int rowFromBottom = idx / 10;
    int cellInRow = idx % 10;
    int col = (rowFromBottom % 2 == 0) ? cellInRow : (9 - cellInRow);
    int screenRow = 9 - rowFromBottom;

    return {
        BOARD_X + (col + 0.5f) * CELL_SIZE,
        BOARD_Y + (screenRow + 0.5f) * CELL_SIZE
    };
}

bool IsSpecialSquare(int sq) {
    for (const auto& l : LADDERS) if (l.from == sq || l.to == sq) return true;
    for (const auto& s : SNAKES)  if (s.from == sq || s.to == sq) return true;
    return false;
}

int GetLadderDest(int sq) {
    for (const auto& l : LADDERS) if (l.from == sq) return l.to;
    return 0;
}

int GetSnakeDest(int sq) {
    for (const auto& s : SNAKES) if (s.from == sq) return s.to;
    return 0;
}

// Helper to draw text with a crisp pixel drop shadow
void DrawShadowText(const char* text, int x, int y, int fontSize, Color color, Color shadow, int offset = 2) {
    DrawText(text, x + offset, y + offset, fontSize, shadow);
    DrawText(text, x, y, fontSize, color);
}

// Helper to draw a small pixel-art heart
void DrawPixelHeart(int cx, int cy, int p, Color col) {
    DrawShadowText("", 0, 0, 1, BLANK, BLANK, 0);
    DrawRectangle(cx - 2*p, cy - p, p, p, col);
    DrawRectangle(cx + p,   cy - p, p, p, col);
    DrawRectangle(cx - 3*p, cy, 3*p, p, col);
    DrawRectangle(cx + p,   cy, 3*p, p, col);
    DrawRectangle(cx - 3*p, cy + p, 7*p, p, col);
    DrawRectangle(cx - 2*p, cy + 2*p, 5*p, p, col);
    DrawRectangle(cx - p,   cy + 3*p, 3*p, p, col);
    DrawRectangle(cx,       cy + 4*p, p, p, col);
}

// Helper to draw a 4-point twinkle star
void DrawSparkle(float cx, float cy, float size, Color col) {
    DrawTriangle({cx, cy - size}, {cx - size * 0.28f, cy}, {cx + size * 0.28f, cy}, col);
    DrawTriangle({cx, cy + size}, {cx + size * 0.28f, cy}, {cx - size * 0.28f, cy}, col);
    DrawTriangle({cx - size, cy}, {cx, cy + size * 0.28f}, {cx, cy - size * 0.28f}, col);
    DrawTriangle({cx + size, cy}, {cx, cy - size * 0.28f}, {cx, cy + size * 0.28f}, col);
}

// Helper to draw a golden pixel crown
void DrawPixelCrown(int cx, int cy, int s, Color col, Color shadow) {
    auto block = [&](int gx, int gy, int gw, int gh) {
        DrawRectangle(cx + gx * s + 2, cy + gy * s + 2, gw * s, gh * s, shadow);
        DrawRectangle(cx + gx * s, cy + gy * s, gw * s, gh * s, col);
    };
    block(-4, -2, 1, 2);
    block(0,  -3, 1, 3);
    block(3,  -2, 1, 2);
    block(-4,  0, 8, 2);
}

// Draw the Pixel Princess player marker
void DrawPixelPerson(Vector2 pos, float time) {
    float bob = std::sin(time * 6.0f) * 3.0f;
    int x = (int)(pos.x - 23);
    int y = (int)(pos.y - 26 + bob);

    Color hair  = {124, 60, 130, 255};  // #7c3c82
    Color face  = {255, 217, 205, 255}; // #ffd9cd
    Color eye   = {100, 55, 109, 255};  // #64376d
    Color skirt = {196, 79, 156, 255};  // #c44f9c
    Color shad  = {91, 45, 117, 120};

    // Drop shadow
    DrawRectangle(x + 12, y + 5, 26, 44, shad);

    // Hair
    DrawRectangle(x + 13, y + 2, 20, 13, hair);
    DrawRectangle(x + 9,  y + 8, 7, 17, hair);
    DrawRectangle(x + 30, y + 8, 7, 17, hair);

    // Face & Eyes
    DrawRectangle(x + 14, y + 8, 18, 15, face);
    DrawRectangle(x + 17, y + 13, 3, 3, eye);
    DrawRectangle(x + 26, y + 13, 3, 3, eye);

    // White top & pink skirt
    DrawRectangle(x + 16, y + 22, 14, 11, WHITE);
    DrawRectangle(x + 13, y + 31, 20, 4, skirt);
    DrawRectangle(x + 11, y + 35, 24, 5, skirt);

    // Legs
    DrawRectangle(x + 16, y + 40, 5, 7, WHITE);
    DrawRectangle(x + 25, y + 40, 5, 7, WHITE);
}

// Draw a ladder between two squares
void DrawLadder(int from, int to) {
    Vector2 start = GetSquareCenter(from);
    Vector2 end = GetSquareCenter(to);
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    float len = std::hypot(dx, dy);
    float nx = (-dy / len) * 9.5f;
    float ny = (dx / len) * 9.5f;

    // Shadow
    DrawLineEx({start.x + nx + 2, start.y + ny + 2}, {end.x + nx + 2, end.y + ny + 2}, 5.0f, COL_LADDER_SHAD);
    DrawLineEx({start.x - nx + 2, start.y - ny + 2}, {end.x - nx + 2, end.y - ny + 2}, 5.0f, COL_LADDER_SHAD);

    // Side rails
    DrawLineEx({start.x + nx, start.y + ny}, {end.x + nx, end.y + ny}, 5.0f, COL_LADDER);
    DrawLineEx({start.x - nx, start.y - ny}, {end.x - nx, end.y - ny}, 5.0f, COL_LADDER);

    // Rungs
    int rungs = (int)std::round(len / 42.0f);
    if (rungs < 3) rungs = 3;
    for (int i = 0; i < rungs; i++) {
        float t = (i + 0.6f) / rungs;
        float rx = start.x + dx * t;
        float ry = start.y + dy * t;
        DrawLineEx({rx + nx + 2, ry + ny + 2}, {rx - nx + 2, ry - ny + 2}, 4.5f, COL_LADDER_SHAD);
        DrawLineEx({rx + nx, ry + ny}, {rx - nx, ry - ny}, 4.5f, COL_LADDER);
    }
}

// Evaluate point on a cubic Bezier curve
Vector2 EvalBezier(Vector2 p0, Vector2 p1, Vector2 p2, Vector2 p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    return {
        uuu * p0.x + 3 * uu * t * p1.x + 3 * u * tt * p2.x + ttt * p3.x,
        uuu * p0.y + 3 * uu * t * p1.y + 3 * u * tt * p2.y + ttt * p3.y
    };
}

// Draw a curvy segmented pixel snake
void DrawSnake(int from, int to, int index) {
    Vector2 head = GetSquareCenter(from);
    Vector2 tail = GetSquareCenter(to);
    float dx = tail.x - head.x;
    float dy = tail.y - head.y;
    float curve = (index % 2 == 0) ? 66.0f : -66.0f;

    Vector2 p1 = {head.x + dx * 0.18f + curve, head.y + dy * 0.30f};
    Vector2 p2 = {head.x + dx * 0.76f - curve, head.y + dy * 0.62f};

    // Draw body as segmented pixel blocks along the Bezier curve
    const int steps = 28;
    for (int i = 0; i < steps; i++) {
        float t0 = (float)i / steps;
        float t1 = (float)(i + 0.78f) / steps; // slight gap for pixel-segment look
        Vector2 a = EvalBezier(head, p1, p2, tail, t0);
        Vector2 b = EvalBezier(head, p1, p2, tail, t1);
        DrawLineEx({a.x + 3, a.y + 3}, {b.x + 3, b.y + 3}, 14.0f, COL_SNAKE_SHAD);
        DrawLineEx(a, b, 14.0f, COL_SNAKE);
    }

    // Rotated pixel snake head
    float angle = std::atan2(dy, dx) * RAD2DEG + 90.0f;
    Rectangle headRect = {head.x, head.y, 28.0f, 26.0f};
    DrawRectanglePro({head.x + 2, head.y + 2, 28, 26}, {14, 13}, angle, COL_SNAKE_SHAD);
    DrawRectanglePro(headRect, {14, 13}, angle, COL_SNAKE);

    // Eyes and mouth rotated with the head
    float rad = angle * DEG2RAD;
    auto rotPt = [&](float lx, float ly) -> Vector2 {
        return {
            head.x + lx * std::cos(rad) - ly * std::sin(rad),
            head.y + lx * std::sin(rad) + ly * std::cos(rad)
        };
    };

    DrawCircleV(rotPt(-6, -5), 2.5f, WHITE);
    DrawCircleV(rotPt( 6, -5), 2.5f, WHITE);
    DrawLineEx(rotPt(-6, 3), rotPt(0, 7), 2.0f, WHITE);
    DrawLineEx(rotPt(0, 7),  rotPt(6, 3), 2.0f, WHITE);
}

// Draw the dice card's die with pips
void DrawDice(int value, bool rolling, float time, int cx, int cy, int size) {
    float shakeX = rolling ? std::sin(time * 45.0f) * 4.0f : 0.0f;
    float shakeY = rolling ? std::cos(time * 38.0f) * 3.0f : 0.0f;
    int x = (int)(cx - size / 2 + shakeX);
    int y = (int)(cy - size / 2 + shakeY);

    // Drop shadow & white border
    DrawRectangle(x + 7, y + 7, size, size, {138, 81, 168, 255});
    DrawRectangle(x, y, size, size, WHITE);

    // Two-tone diagonal interior
    int pad = 5;
    int inSize = size - pad * 2;
    Vector2 tl = {(float)(x + pad), (float)(y + pad)};
    Vector2 tr = {(float)(x + pad + inSize), (float)(y + pad)};
    Vector2 bl = {(float)(x + pad), (float)(y + pad + inSize)};
    Vector2 br = {(float)(x + pad + inSize), (float)(y + pad + inSize)};

    DrawTriangle(tl, bl, tr, {244, 196, 223, 255}); // #f4c4df
    DrawTriangle(tr, bl, br, {203, 177, 237, 255}); // #cbb1ed

    // Pips
    auto pip = [&](float px, float py) {
        int pw = 12;
        int ix = (int)(x + size * px - pw / 2);
        int iy = (int)(y + size * py - pw / 2);
        DrawRectangle(ix + 2, iy + 2, pw, pw, {126, 74, 158, 110});
        DrawRectangle(ix, iy, pw, pw, WHITE);
    };

    if (value == 1 || value == 3 || value == 5) pip(0.50f, 0.50f);
    if (value >= 2) { pip(0.28f, 0.28f); pip(0.72f, 0.72f); }
    if (value >= 4) { pip(0.72f, 0.28f); pip(0.28f, 0.72f); }
    if (value == 6) { pip(0.28f, 0.50f); pip(0.72f, 0.50f); }
}

// Helper to draw centered wrapped text inside a card
void DrawCenteredMessage(const std::string& msg, int centerX, int y, int maxWidth, int fontSize) {
    if (MeasureText(msg.c_str(), fontSize) <= maxWidth) {
        int w = MeasureText(msg.c_str(), fontSize);
        DrawShadowText(msg.c_str(), centerX - w / 2, y, fontSize, WHITE, {147, 80, 147, 255}, 1);
        return;
    }
    // Split near middle space
    size_t mid = msg.length() / 2;
    size_t split = msg.find(' ', mid);
    if (split == std::string::npos) split = msg.rfind(' ', mid);
    if (split != std::string::npos) {
        std::string line1 = msg.substr(0, split);
        std::string line2 = msg.substr(split + 1);
        int w1 = MeasureText(line1.c_str(), fontSize);
        int w2 = MeasureText(line2.c_str(), fontSize);
        DrawShadowText(line1.c_str(), centerX - w1 / 2, y - 8, fontSize, WHITE, {147, 80, 147, 255}, 1);
        DrawShadowText(line2.c_str(), centerX - w2 / 2, y + 12, fontSize, WHITE, {147, 80, 147, 255}, 1);
    }
}

int main() {
    std::srand((unsigned int)std::time(nullptr));

    const int screenW = 1200;
    const int screenH = 880;
    InitWindow(screenW, screenH, "Snakes & Ladders - A Magical Pixel Adventure");
    SetTargetFPS(60);

    // Game state
    enum State { IDLE, ROLLING, STEPPING, PAUSE_BEFORE_JUMP, WON };
    State state = IDLE;

    int position = 1;
    int dice = 1;
    int turns = 0;
    std::string message = "Roll the dice to begin!";

    float timer = 0.0f;
    int rollTicks = 0;
    int stepsRemaining = 0;
    int pendingJumpDest = 0;

    auto restartGame = [&]() {
        state = IDLE;
        position = 1;
        dice = 1;
        turns = 0;
        message = "Roll the dice to begin!";
    };

    auto triggerRoll = [&]() {
        if (state != IDLE) return;
        state = ROLLING;
        message = "Rolling...";
        timer = 0.0f;
        rollTicks = 0;
    };

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        float totalTime = (float)GetTime();

        // --- UPDATE GAME LOGIC ---
        if (state == ROLLING) {
            timer += dt;
            if (timer >= 0.07f) {
                timer = 0.0f;
                dice = (std::rand() % 6) + 1;
                rollTicks++;
                if (rollTicks >= 7) {
                    turns++;
                    if (position + dice > 100) {
                        message = "You rolled " + std::to_string(dice) + ". Need exact roll!";
                        state = IDLE;
                    } else {
                        stepsRemaining = dice;
                        state = STEPPING;
                    }
                }
            }
        } else if (state == STEPPING) {
            timer += dt;
            if (timer >= 0.15f) {
                timer = 0.0f;
                position++;
                stepsRemaining--;

                if (stepsRemaining <= 0) {
                    int ladderTo = GetLadderDest(position);
                    int snakeTo = GetSnakeDest(position);
                    if (ladderTo > 0) {
                        message = "A ladder! Climb, cutie!";
                        pendingJumpDest = ladderTo;
                        state = PAUSE_BEFORE_JUMP;
                    } else if (snakeTo > 0) {
                        message = "Oh no, a sneaky snake!";
                        pendingJumpDest = snakeTo;
                        state = PAUSE_BEFORE_JUMP;
                    } else if (position == 100) {
                        message = "You reached the castle!";
                        state = WON;
                    } else {
                        message = "You rolled " + std::to_string(dice) + ". Square " + std::to_string(position) + ".";
                        state = IDLE;
                    }
                }
            }
        } else if (state == PAUSE_BEFORE_JUMP) {
            timer += dt;
            if (timer >= 0.65f) {
                timer = 0.0f;
                bool climbed = pendingJumpDest > position;
                position = pendingJumpDest;
                message = climbed
                    ? "Sweet! You climbed to " + std::to_string(position) + "."
                    : "Whoops! You slipped to " + std::to_string(position) + ".";
                state = (position == 100) ? WON : IDLE;
            }
        }

        // Keyboard shortcuts (Space / Enter to roll, R to restart)
        if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER)) {
            if (state == IDLE) triggerRoll();
            else if (state == WON) restartGame();
        }
        if (IsKeyPressed(KEY_R)) restartGame();

        // --- RENDERING ---
        BeginDrawing();
        ClearBackground(COL_BG);

        // Background 20x20 grid + scanlines
        for (int gx = 0; gx < screenW; gx += 20) DrawLine(gx, 0, gx, screenH, {255, 255, 255, 18});
        for (int gy = 0; gy < screenH; gy += 20) DrawLine(0, gy, screenW, gy, {255, 255, 255, 18});
        for (int sy = 0; sy < screenH; sy += 4)  DrawLine(0, sy, screenW, sy, {68, 28, 98, 22});

        // Twinkling side sparkles
        float pulse1 = 6.0f + std::sin(totalTime * 3.0f) * 2.5f;
        float pulse2 = 6.0f + std::cos(totalTime * 3.0f) * 2.5f;
        DrawSparkle(60, 140, pulse1, WHITE);
        DrawSparkle(95, 440, pulse2, WHITE);
        DrawSparkle(40, 700, pulse1, WHITE);
        DrawSparkle(1140, 110, pulse2, WHITE);
        DrawSparkle(1160, 400, pulse1, WHITE);
        DrawSparkle(1110, 730, pulse2, WHITE);

        // Header
        const char* eyebrow = "A MAGICAL PIXEL ADVENTURE";
        int ew = MeasureText(eyebrow, 14);
        DrawShadowText(eyebrow, screenW / 2 - ew / 2, 24, 14, WHITE, {138, 76, 172, 255}, 2);

        // Title: "Snakes & Ladders"
        int titleY = 50;
        int wSnakes  = MeasureText("Snakes", 54);
        int wAmp     = MeasureText(" & ", 44);
        int wLadders = MeasureText("Ladders", 54);
        int titleX   = screenW / 2 - (wSnakes + wAmp + wLadders) / 2;

        DrawShadowText("Snakes",  titleX, titleY, 54, COL_TITLE_PINK, COL_SHADOW_DARK, 4);
        DrawShadowText(" & ",     titleX + wSnakes, titleY + 6, 44, WHITE, COL_SHADOW_DARK, 4);
        DrawShadowText("Ladders", titleX + wSnakes + wAmp, titleY, 54, COL_TITLE_LAV, COL_SHADOW_DARK, 4);

        // Three header hearts
        DrawPixelHeart(screenW / 2 - 42, 120, 2, {255, 227, 242, 255});
        DrawPixelHeart(screenW / 2,      126, 2, {255, 227, 242, 255});
        DrawPixelHeart(screenW / 2 + 42, 120, 2, {255, 227, 242, 255});

        // --- BOARD FRAME ---
        int fx = BOARD_X - 18;
        int fy = BOARD_Y - 18;
        int fSize = 636;
        DrawRectangle(fx + 14, fy + 16, fSize, fSize, {91, 48, 117, 75}); // Drop shadow
        DrawRectangle(fx - 10, fy - 10, fSize + 20, fSize + 20, COL_FRAME_OUTER);
        DrawRectangle(fx - 5,  fy - 5,  fSize + 10, fSize + 10, COL_FRAME_PURPLE);
        DrawRectangle(fx,      fy,      fSize,      fSize,      WHITE);
        DrawRectangle(fx + 5,  fy + 5,  fSize - 10, fSize - 10, COL_FRAME_PINK);
        DrawRectangle(BOARD_X - 4, BOARD_Y - 4, 608, 608, COL_FRAME_PURPLE);

        // 4 White pixel corners
        auto drawCorner = [&](int cx, int cy) {
            DrawRectangle(cx, cy, 18, 18, COL_FRAME_PURPLE);
            DrawRectangle(cx + 4, cy + 4, 10, 10, WHITE);
        };
        drawCorner(fx - 8, fy - 8);
        drawCorner(fx + fSize - 10, fy - 8);
        drawCorner(fx - 8, fy + fSize - 10);
        drawCorner(fx + fSize - 10, fy + fSize - 10);

        // --- 100 BOARD SQUARES ---
        for (int sq = 1; sq <= 100; sq++) {
            Vector2 c = GetSquareCenter(sq);
            int cx = (int)(c.x - CELL_SIZE / 2);
            int cy = (int)(c.y - CELL_SIZE / 2);

            Color cellCol = (sq % 4 == 2 || sq % 4 == 3) ? COL_CELL_PURPLE : COL_CELL_PINK;
            if (sq == 1 || sq == 100) cellCol = COL_MILESTONE;

            DrawRectangle(cx, cy, CELL_SIZE, CELL_SIZE, cellCol);
            DrawRectangleLines(cx, cy, CELL_SIZE, CELL_SIZE, {255, 255, 255, 95});

            // Polka-dot texture on special squares
            if (IsSpecialSquare(sq)) {
                for (int dx = 10; dx < CELL_SIZE; dx += 14) {
                    for (int dy = 10; dy < CELL_SIZE; dy += 14) {
                        DrawPixel(cx + dx, cy + dy, {255, 255, 255, 110});
                        DrawPixel(cx + dx + 1, cy + dy, {255, 255, 255, 110});
                    }
                }
            }

            // Square number
            DrawShadowText(std::to_string(sq).c_str(), cx + 6, cy + 6, 12, WHITE, {140, 80, 145, 255}, 1);

            if (sq == 1) {
                DrawShadowText("START", cx + 12, cy + 44, 10, WHITE, {140, 80, 145, 255}, 1);
            } else if (sq == 100) {
                DrawPixelCrown(cx + 30, cy + 34, 3, {255, 242, 173, 255}, {164, 92, 146, 255});
            }
        }

        // --- LADDERS & SNAKES ---
        for (const auto& l : LADDERS) DrawLadder(l.from, l.to);
        for (int i = 0; i < (int)SNAKES.size(); i++) DrawSnake(SNAKES[i].from, SNAKES[i].to, i);

        // --- PLAYER CHARACTER ---
        DrawPixelPerson(GetSquareCenter(position), totalTime);

        // --- RIGHT SIDE PANEL ---
        int px = 825;
        int pw = 265;
        Vector2 mouse = GetMousePosition();

        // 1. Status Card
        int sY = fy;
        int sH = 145;
        DrawRectangle(px + 6, sY + 7, pw, sH, {128, 80, 162, 255});
        DrawRectangle(px, sY, pw, sH, WHITE);
        DrawRectangle(px + 4, sY + 4, pw - 8, sH - 8, COL_CARD_STATUS);

        DrawShadowText("YOUR TURN", px + 18, sY + 18, 11, WHITE, COL_SHADOW_DARK, 1);
        std::string turnTxt = "TURN " + std::to_string(turns + 1);
        DrawShadowText(turnTxt.c_str(), px + pw - 18 - MeasureText(turnTxt.c_str(), 11), sY + 18, 11, WHITE, COL_SHADOW_DARK, 1);

        std::string posStr = std::to_string(position);
        int posW = MeasureText(posStr.c_str(), 36);
        int posX = px + pw / 2 - posW / 2;
        DrawShadowText("Square", posX - 52, sY + 68, 12, WHITE, COL_SHADOW_DARK, 1);
        DrawShadowText(posStr.c_str(), posX, sY + 52, 36, WHITE, {130, 72, 142, 255}, 3);
        DrawShadowText("of 100", posX + posW + 10, sY + 68, 12, WHITE, COL_SHADOW_DARK, 1);

        // Progress bar
        DrawRectangle(px + 18, sY + 108, pw - 36, 12, {133, 80, 164, 255});
        int fillW = (int)((pw - 40) * (position / 100.0f));
        if (fillW < 6) fillW = 6;
        DrawRectangle(px + 20, sY + 110, fillW, 8, COL_TITLE_PINK);

        // 2. Dice Card
        int dY = sY + sH + 18;
        int dH = 345;
        DrawRectangle(px + 6, dY + 7, pw, dH, {128, 80, 162, 255});
        DrawRectangle(px, dY, pw, dH, WHITE);
        DrawRectangleGradientV(px + 4, dY + 4, pw - 8, dH - 8, COL_CARD_DICE_1, COL_CARD_DICE_2);

        // Glow and corner stars inside dice card
        DrawSparkle((float)(px + pw / 2), (float)(dY + 95), 65.0f, {255, 255, 255, 45});
        DrawSparkle((float)(px + 26), (float)(dY + 30), 7.0f, WHITE);
        DrawSparkle((float)(px + pw - 26), (float)(dY + 55), 7.0f, WHITE);

        DrawDice(dice, state == ROLLING, totalTime, px + pw / 2, dY + 95, 92);
        DrawCenteredMessage(message, px + pw / 2, dY + 172, pw - 28, 15);

        // Roll Button
        Rectangle rollBtn = {(float)(px + 20), (float)(dY + 225), (float)(pw - 40), 46.0f};
        bool canRoll = (state == IDLE);
        bool hoverRoll = canRoll && CheckCollisionPointRec(mouse, rollBtn);
        bool pressRoll = hoverRoll && IsMouseButtonDown(MOUSE_LEFT_BUTTON);
        int btnOffset = pressRoll ? 4 : (hoverRoll ? -2 : 0);

        DrawRectangle((int)rollBtn.x, (int)rollBtn.y + 5, (int)rollBtn.width, (int)rollBtn.height, COL_BTN_SHADOW);
        DrawRectangle((int)rollBtn.x, (int)rollBtn.y + btnOffset, (int)rollBtn.width, (int)rollBtn.height, WHITE);
        Color btnFill = !canRoll ? Color{150, 86, 183, 180} : (hoverRoll ? COL_BTN_HOVER : COL_BTN);
        DrawRectangle((int)rollBtn.x + 3, (int)rollBtn.y + 3 + btnOffset, (int)rollBtn.width - 6, (int)rollBtn.height - 6, btnFill);

        const char* btnLabel = (state == ROLLING) ? "ROLLING..." : (position == 100 ? "YOU WON!" : "ROLL DICE");
        int lblW = MeasureText(btnLabel, 14);
        DrawText(btnLabel, (int)(rollBtn.x + rollBtn.width / 2 - lblW / 2), (int)(rollBtn.y + 16 + btnOffset), 14, WHITE);

        if (hoverRoll && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) triggerRoll();

        // Start Over Link Button
        const char* resetLabel = "START OVER";
        int resetW = MeasureText(resetLabel, 12);
        Rectangle resetRect = {(float)(px + pw / 2 - resetW / 2), (float)(dY + 296), (float)resetW, 18.0f};
        bool hoverReset = CheckCollisionPointRec(mouse, resetRect);
        DrawText(resetLabel, (int)resetRect.x, (int)resetRect.y, 12, hoverReset ? COL_TITLE_PINK : WHITE);
        DrawLine((int)resetRect.x, (int)resetRect.y + 15, (int)(resetRect.x + resetW), (int)resetRect.y + 15, WHITE);
        if (hoverReset && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) restartGame();

        // 3. Legend Card ("MAGIC MOVES")
        int lY = dY + dH + 18;
        int lH = (fy + fSize) - lY;
        DrawRectangle(px + 6, lY + 7, pw, lH, {128, 80, 162, 255});
        DrawRectangle(px, lY, pw, lH, WHITE);
        DrawRectangle(px + 4, lY + 4, pw - 8, lH - 8, COL_CARD_LEGEND);

        const char* magicTitle = "MAGIC MOVES";
        DrawShadowText(magicTitle, px + pw / 2 - MeasureText(magicTitle, 12) / 2, lY + 16, 12, WHITE, COL_SHADOW_DARK, 1);

        // Mini Ladder Icon + Label
        int lx = px + 26;
        int ly = lY + 48;
        DrawRectangle(lx, ly, 3, 28, COL_TITLE_LAV);
        DrawRectangle(lx + 11, ly, 3, 28, COL_TITLE_LAV);
        DrawRectangle(lx, ly + 6, 14, 3, COL_TITLE_LAV);
        DrawRectangle(lx, ly + 14, 14, 3, COL_TITLE_LAV);
        DrawRectangle(lx, ly + 22, 14, 3, COL_TITLE_LAV);
        DrawShadowText("Ladder", lx + 22, ly + 1, 13, WHITE, COL_SHADOW_DARK, 1);
        DrawText("Climb up!", lx + 22, ly + 16, 11, {255, 255, 255, 220});

        // Mini Snake Icon + Label
        int sx = px + 145;
        DrawShadowText("S", sx, ly + 2, 26, {242, 149, 202, 255}, {124, 61, 119, 255}, 2);
        DrawShadowText("Snake", sx + 22, ly + 1, 13, WHITE, COL_SHADOW_DARK, 1);
        DrawText("Slide down!", sx + 22, ly + 16, 11, {255, 255, 255, 220});

        // --- WIN MODAL OVERLAY ---
        if (state == WON) {
            DrawRectangle(0, 0, screenW, screenH, {93, 49, 117, 190});
            int mw = 420;
            int mh = 310;
            int mx = screenW / 2 - mw / 2;
            int my = screenH / 2 - mh / 2;

            DrawRectangle(mx + 12, my + 12, mw, mh, {116, 69, 144, 255});
            DrawRectangle(mx, my, mw, mh, WHITE);
            DrawRectangleGradientV(mx + 6, my + 6, mw - 12, mh - 12, {233, 156, 204, 255}, {185, 148, 229, 255});

            DrawPixelCrown(screenW / 2, my + 58, 6, {255, 243, 177, 255}, {156, 87, 156, 255});
            DrawShadowText("YOU DID IT!", screenW / 2 - MeasureText("YOU DID IT!", 18) / 2, my + 95, 18, WHITE, COL_SHADOW_DARK, 2);
            DrawShadowText("Pixel Princess", screenW / 2 - MeasureText("Pixel Princess", 30) / 2, my + 126, 30, WHITE, {128, 80, 157, 255}, 3);

            std::string winStats = "You reached the castle in " + std::to_string(turns) + " turns.";
            DrawShadowText(winStats.c_str(), screenW / 2 - MeasureText(winStats.c_str(), 15) / 2, my + 176, 15, WHITE, COL_SHADOW_DARK, 1);

            Rectangle playAgainBtn = {(float)(screenW / 2 - 95), (float)(my + 222), 190.0f, 46.0f};
            bool hoverAgain = CheckCollisionPointRec(mouse, playAgainBtn);
            DrawRectangle((int)playAgainBtn.x, (int)playAgainBtn.y + 5, 190, 46, {100, 59, 126, 255});
            DrawRectangle((int)playAgainBtn.x, (int)playAgainBtn.y, 190, 46, WHITE);
            DrawRectangle((int)playAgainBtn.x + 3, (int)playAgainBtn.y + 3, 184, 40, hoverAgain ? COL_BTN_HOVER : Color{143, 82, 176, 255});
            DrawText("PLAY AGAIN", screenW / 2 - MeasureText("PLAY AGAIN", 14) / 2, my + 238, 14, WHITE);

            if (hoverAgain && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) restartGame();
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
