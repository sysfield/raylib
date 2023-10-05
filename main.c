#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

typedef struct sprite
{
    Texture2D sprite;
    Vector2 pos;
    int sprite_height;
    int sprite_width;
    Vector2 min;
    Vector2 max;
    Vector2 clamp;
} sp;

Vector2 checkMovement(sp *sprite, float speed);
sp spriteClamp(sp *sprite);
// Vector2 followerMovement(Vector2 follower_position, Vector2 position);

int width = 800;
int height = 450;
float velocity = 200;
int scale = 4;

int main(void)
{
    sp *sprite = malloc(sizeof(sp));
    // sp follower;

    sprite->sprite = LoadTexture("tile_0121.png");
    sprite->pos = (Vector2){0, 10};
    sprite->sprite_height = sprite->sprite.height * scale;
    sprite->sprite_width = sprite->sprite.height * scale;
    
    spriteClamp(sprite);

    // follower.sprite = LoadTexture("tile_0123.png");
    // follower.sprite_height = sprite.sprite.height * scale;
    // follower.sprite_width = sprite.sprite.width * scale;
    // follower.pos = (Vector2){width - follower.sprite_width, 10};
    // spriteClamp(follower);

    Sound sound;
    // Music music;

    InitWindow(width, height, "title");
    InitAudioDevice();

    sound = LoadSound("star.wav");
    // music = LoadMusicStream("dirediredocks.mp3");
    // PlayMusicStream(music);

    while (!WindowShouldClose())
    {
        // UpdateMusicStream(music);

        float speed = GetFrameTime() * velocity;

        sprite.pos = checkMovement(sprite, speed);
        // follower_pos = followerMovement(follower_pos, pos);

        Rectangle spriteRect = {
            sprite.pos.x,
            sprite.pos.y,
            sprite.sprite_width,
            sprite.sprite_height
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
            DrawTextureEx(sprite.sprite, sprite.pos, 0, scale, RED);
            // DrawTextureEx(follower.sprite, follower.pos, 0, scale, GRAY);
        EndDrawing();
    }
    // StopMusicStream(music);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}

Vector2 checkMovement(sp *sprite, float speed)
{

    if (IsKeyDown(KEY_D) && sprite.clamp.x != sprite.max.x)
        sprite.pos.x += speed;
    if (IsKeyDown(KEY_A) && sprite.clamp.x != sprite.min.x)
        sprite.pos.x -= speed;
    if (IsKeyDown(KEY_W) && sprite.clamp.y != sprite.min.y)
        sprite.pos.y -= speed;
    if (IsKeyDown(KEY_S) && sprite.clamp.y != sprite.max.y)
        sprite.pos.y += speed;

    printf("%f, %f\n", sprite.clamp.x, sprite.clamp.y);
    return sprite.pos;
}

sp spriteClamp(sp *sprite)
{
    sprite.min = (Vector2){0, 0};
    sprite.max = (Vector2){width - sprite.sprite_width, height - sprite.sprite_height};

    sprite.clamp = Vector2Clamp(sprite.pos, sprite.min, sprite.max);
}