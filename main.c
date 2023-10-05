#include <stdio.h>

#include "raylib.h"
#include "raymath.h"

Vector2 checkMovement(Vector2 position, int sprite_height, int sprite_width, float speed);
// Vector2 followerMovement(Vector2 follower_position, Vector2 position);

int width = 800;
int height = 450;
float velocity = 200;
int scale = 4;

int main(void)
{
    Texture2D sprite;
    Texture2D follower;
    Sound sound;
    // Music music;

    InitWindow(width, height, "title");
    InitAudioDevice();

    sprite = LoadTexture("tile_0121.png");
    follower = LoadTexture("tile_0123.png");
    sound = LoadSound("star.wav");
    // music = LoadMusicStream("dirediredocks.mp3");
    // PlayMusicStream(music);

    Vector2 pos = {0, 10};
    Vector2 follower_pos = {width - (follower.width * scale), 10};
    int sprite_width = sprite.width * scale;
    int sprite_height = sprite.height * scale;

    while (!WindowShouldClose())
    {
        // UpdateMusicStream(music);

        float speed = GetFrameTime() * velocity;

        pos = checkMovement(pos, sprite_height, sprite_width, speed);
        // follower_pos = followerMovement(follower_pos, pos);

        Rectangle spriteRect = {
            pos.x,
            pos.y,
            sprite.width * scale,
            sprite.height * scale
        };

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(GetMousePosition(), spriteRect))
            {
                PlaySound(sound);
            }
        }

        BeginDrawing();
            ClearBackground(GRAY);
            DrawTextureEx(sprite, pos, 0, scale, RED);
            DrawTextureEx(follower, follower_pos, 0, scale, GRAY);
        EndDrawing();
    }
    // StopMusicStream(music);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}

Vector2 checkMovement(Vector2 position, int sprite_height, int sprite_width, float speed)
{
    Vector2 min = {0, 0};
    Vector2 max = {width - sprite_width, height - sprite_height};

    Vector2 clamp = Vector2Clamp(position, min, max);

    if (IsKeyDown(KEY_D) && clamp.x != max.x)
        position.x += speed;
    if (IsKeyDown(KEY_A) && clamp.x != min.x)
        position.x -= speed;
    if (IsKeyDown(KEY_W) && clamp.y != min.y)
        position.y -= speed;
    if (IsKeyDown(KEY_S) && clamp.y != max.y)
        position.y += speed;

    printf("%f, %f\n", clamp.x, clamp.y);
    return position;
}