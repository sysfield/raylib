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

typedef struct npc
{
    struct sprite sp;
    float base_distance;
    Vector2 target_location;
} npc;

float sign(float z);
sp checkMovement(sp *sprite, float speed);
sp spriteClamp(sp *sprite);
npc followerMovement(npc *npc, sp *target, float speed);

int width = 800;
int height = 450;
float velocity = 200;
int scale = 4;
float distance = 150;

int main(void)
{    
    Sound sound;
    // Music music;

    SetConfigFlags(FLAG_VSYNC_HINT);

    InitWindow(width, height, "title");
    InitAudioDevice();

    SetTargetFPS(60);

    sound = LoadSound("star.wav");
    // music = LoadMusicStream("dirediredocks.mp3");
    // PlayMusicStream(music);

    sp sprite;
    npc follower;

    sprite.sprite = LoadTexture("tile_0121.png");
    sprite.pos = (Vector2){0, 10};
    sprite.sprite_height = sprite.sprite.height * scale;
    sprite.sprite_width = sprite.sprite.height * scale;
    
    // printf("clamp: %f, %f\nmax: %f, %f\nmin: %f, %f\n", sprite.clamp.x, sprite.clamp.y, sprite.max.x, sprite.max.y, sprite.min.x, sprite.min.y);

    follower.sp.sprite = LoadTexture("tile_0123.png");
    follower.sp.sprite_height = sprite.sprite.height * scale;
    follower.sp.sprite_width = sprite.sprite.width * scale;
    follower.sp.pos = (Vector2){width - follower.sp.sprite_width, 10};

    while (!WindowShouldClose())
    {
        // UpdateMusicStream(music);

        float speed = GetFrameTime() * velocity;

        checkMovement(&sprite, speed);
        spriteClamp(&sprite);

        follower.base_distance = Vector2Distance(follower.sp.pos, sprite.pos);
        follower.target_location = Vector2MoveTowards(sprite.pos, follower.sp.pos, 20);
        followerMovement(&follower, &sprite, speed / 2);
        // spriteClamp(&follower.sp);

        BeginDrawing();
            ClearBackground(GRAY);
            DrawFPS(10, height - 20);
            DrawTextureEx(sprite.sprite, sprite.pos, 0, scale, RED);
            DrawTextureEx(follower.sp.sprite, follower.sp.pos, 0, scale, GRAY);
        EndDrawing();
    }
    // StopMusicStream(music);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}

sp checkMovement(sp *sprite, float speed)
{
    if (IsKeyDown(KEY_D) && sprite->clamp.x != sprite->max.x)
        sprite->pos.x += speed;
    if (IsKeyDown(KEY_A) && sprite->clamp.x != sprite->min.x)
        sprite->pos.x -= speed;
    if (IsKeyDown(KEY_W) && sprite->clamp.y != sprite->min.y)
        sprite->pos.y -= speed;
    if (IsKeyDown(KEY_S) && sprite->clamp.y != sprite->max.y)
        sprite->pos.y += speed;
}

npc followerMovement(npc *npc, sp *target, float speed)
{
    Vector2 move_this_frame = {0, 0};
    Vector2 difference;

    if (npc->base_distance <= distance + npc->sp.sprite_width)
    {
        difference = Vector2Subtract(npc->target_location, npc->sp.pos);
        difference.x = sign(difference.x);
        difference.y = sign(difference.y);

        if (npc->base_distance != 0)
        {
            move_this_frame = Vector2Scale(difference, speed);
            // move_this_frame.x = roundf(move_this_frame.x);
            // move_this_frame.y = roundf(move_this_frame.y);
            npc->sp.pos = Vector2Add(npc->sp.pos, move_this_frame);
            printf("%f, %f\n", move_this_frame.x, move_this_frame.y);
        }

        printf("sprite: %f, %f\n", target->pos.x, target->pos.y);
        printf("target_location: %f, %f\n", npc->target_location.x, npc->target_location.y);
        printf("follower: %f, %f\n", npc->sp.pos.x, npc->sp.pos.y);
    }

}

sp spriteClamp(sp *sprite)
{
    sprite->min = (Vector2){0, 0};
    sprite->max = (Vector2){width - sprite->sprite_width, height - sprite->sprite_height};

    sprite->clamp = Vector2Clamp(sprite->pos, sprite->min, sprite->max);
}

float sign(float z)
{
    z = (z > 0) - (z < 0);

    return z;
}