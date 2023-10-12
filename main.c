#include <stdio.h>
#include <stdlib.h>

#include "raylib.h"
#include "raymath.h"

typedef struct tmr
{
    float lifetime;
} tmr;

// clamping struct
typedef struct boundary
{
    Vector2 min;
    Vector2 max;
    Vector2 clamp;
} bounds;

typedef struct position
{
    Vector2 pos;
    int sprite_height;
    int sprite_width;
} pos;

// invisibility struct
typedef struct invisibility
{
    bool visible;
    tmr visible_tmr;
    float visible_lifetime;
} invis;

typedef struct sprite
{
    Texture2D sprite;
    pos pos;
    bounds bounds;
    invis invis;
    Color colour;
} sp;

typedef struct npc
{
    Texture2D sprite;
    pos pos;
    float base_distance;
    Vector2 target_location;
} npc;

void startTimer(tmr *tmr, float lifetime);
void updateTimer(tmr *tmr);
bool timerDone(tmr *tmr);
float sign(float z);
sp checkMovement(sp *sprite, float speed);
sp checkVisible(invis *invis, Color *colour);
sp spriteClamp(sp *sprite);
npc followerMovement(npc *npc, sp *target, float speed);

int width = 640;
int height = 360;
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
    sprite.pos.pos = (Vector2){0, 10};
    sprite.pos.sprite_height = sprite.sprite.height * scale;
    sprite.pos.sprite_width = sprite.sprite.height * scale;
    sprite.invis.visible = true;
    sprite.invis.visible_tmr = (tmr){0};
    sprite.invis.visible_lifetime = 2;
    sprite.colour = RED;
    
    follower.sprite = LoadTexture("tile_0123.png");
    follower.pos.sprite_height = sprite.sprite.height * scale;
    follower.pos.sprite_width = sprite.sprite.width * scale;
    follower.pos.pos = (Vector2){width - follower.pos.sprite_width, 10};

    while (!WindowShouldClose())
    {
        // UpdateMusicStream(music);

        float speed = GetFrameTime() * velocity;

        checkMovement(&sprite, speed);
        checkVisible(&sprite.invis, &sprite.colour);
        spriteClamp(&sprite);

        follower.base_distance = Vector2Distance(follower.pos.pos, sprite.pos.pos);
        follower.target_location = Vector2MoveTowards(sprite.pos.pos, follower.pos.pos, 20);
        followerMovement(&follower, &sprite, speed);

        BeginDrawing();
            ClearBackground(GRAY);
            DrawFPS(10, height - 20);
            DrawTextureEx(sprite.sprite, sprite.pos.pos, 0, scale, sprite.colour);
            DrawTextureEx(follower.sprite, follower.pos.pos, 0, scale, GRAY);
        EndDrawing();
    }
    // StopMusicStream(music);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}

sp checkMovement(sp *sprite, float speed)
{
    if (IsKeyDown(KEY_D) && sprite->bounds.clamp.x != sprite->bounds.max.x)
        sprite->pos.pos.x += speed;
    if (IsKeyDown(KEY_A) && sprite->bounds.clamp.x != sprite->bounds.min.x)
        sprite->pos.pos.x -= speed;
    if (IsKeyDown(KEY_W) && sprite->bounds.clamp.y != sprite->bounds.min.y)
        sprite->pos.pos.y -= speed;
    if (IsKeyDown(KEY_S) && sprite->bounds.clamp.y != sprite->bounds.max.y)
        sprite->pos.pos.y += speed;
}

sp checkVisible(invis *invis, Color *colour)
{
    if (IsKeyPressed(KEY_SPACE))
    {
        startTimer(&invis->visible_tmr, invis->visible_lifetime);
    }
    
    if (IsKeyDown(KEY_SPACE))
    {
        updateTimer(&invis->visible_tmr);
    }
    else
    {
        invis->visible_tmr.lifetime = 0;
    }

    printf("%f\n", invis->visible_tmr.lifetime);

    if (!timerDone(&invis->visible_tmr))
    {
        invis->visible = false;
        *colour = ORANGE;
    }
    else
    {
        invis->visible = true;
        *colour = RED;
    }
}

npc followerMovement(npc *npc, sp *target, float speed)
{
    Vector2 move_this_frame = {0, 0};
    Vector2 difference;

    if (npc->base_distance <= distance + npc->pos.sprite_width && target->invis.visible)
    {
        difference = Vector2Subtract(npc->target_location, npc->pos.pos);
        difference.x = sign(difference.x);
        difference.y = sign(difference.y);

        if (npc->base_distance != 0) 
        {
                move_this_frame = Vector2Scale(difference, speed);
                npc->pos.pos = Vector2Add(npc->pos.pos, move_this_frame);
        }   
    }

}

sp spriteClamp(sp *sprite)
{
    sprite->bounds.min = (Vector2){0, 0};
    sprite->bounds.max = (Vector2){width - sprite->pos.sprite_width, height - sprite->pos.sprite_height};

    sprite->bounds.clamp = Vector2Clamp(sprite->pos.pos, sprite->bounds.min, sprite->bounds.max);
}

float sign(float z)
{
    z = (z > 0) - (z < 0);

    return z;
}

// start *tmr with a specific (float) lifetime
void startTimer(tmr *tmr, float lifetime)
{
    if (tmr != NULL)
    {
        tmr->lifetime = lifetime;
    }
}

void updateTimer(tmr *tmr)
{
    // subtract time since last frame from tmr if not ended
    if (tmr != NULL && tmr->lifetime > 0)
    {
        tmr->lifetime -= GetFrameTime();
    }
}

bool timerDone(tmr *tmr)
{
    if (tmr != NULL)
    {
        return tmr->lifetime <= 0;
    }
}