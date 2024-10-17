#include "raylib.h"

int main()
{
    //Window dimension    
    const int windowWidth{512};
    const int windowHeight{380};

    InitWindow(windowWidth,windowHeight,"Dapper Dasher");

    //Acceleration due to gravity
    const int gravity{1000};

    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;

    Vector2 scarfyPos;
    scarfyPos.x = windowWidth/2 - scarfyRec.width/2;
    scarfyPos.y = windowHeight - scarfyRec.height;

    //Jump Velocity pixel per second
    const int jumpVel{-600};

    // Rectangle is in the air?
    bool isInAir{false};

    int velocity{0};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

        //delta time
        const float dT{GetFrameTime()};
        //Start Drawing

        BeginDrawing();
        ClearBackground(WHITE);

        //perform ground check
        if (scarfyPos.y + scarfyRec.height >= windowHeight)
        {
            velocity = 0;
            isInAir = false;
        }
        else
        {
            //apply gravity
            velocity += gravity * dT;
            isInAir = true;
        }

        //Jump by pressing space
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
        }
        

        //update position
        scarfyPos.y += velocity * dT;
        DrawTextureRec(scarfy, scarfyRec, scarfyPos, WHITE);

        //Stop Drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    CloseWindow();
}