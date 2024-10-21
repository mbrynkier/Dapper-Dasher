#include "raylib.h"


struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};


int main()
{
    //Window dimension  
    int windowDimensions[2];
    windowDimensions[0] = 512;
    windowDimensions[1] = 380;

    InitWindow(windowDimensions[0],windowDimensions[1],"Dapper Dasher");

    //Acceleration due to gravity
    const int gravity{1000};

    //Nebula variables
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

    AnimData nebulae[3]{};

    for (int i = 0; i < 3; i++)
    {
        nebulae[i].rec.x = 0.0;
        nebulae[i].rec.y = 0.0;
        nebulae[i].rec.width = nebula.width/8;
        nebulae[i].rec.height = nebula.height/8;
        nebulae[i].pos.x = windowDimensions[0] + i*300;
        nebulae[i].pos.y = windowDimensions[1] - nebula.height/8;
        nebulae[i].frame = 0;
        nebulae[i].updateTime = 1.0/16.0;
        nebulae[i].runningTime = 0.0;
    }
    
    //nebulae[0].pos.x = windowDimensions[0];
    //nebulae[1].pos.x = windowDimensions[0] + 300;

    int nebulaVel{-200};

    //Scarfy variables
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
    AnimData scarfyData;
    scarfyData.rec.width = scarfy.width/6;
    scarfyData.rec.height = scarfy.height;
    scarfyData.rec.x = 0;
    scarfyData.rec.y = 0;

    scarfyData.pos.x = windowDimensions[0]/2 - scarfyData.rec.width/2;
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height;

    scarfyData.frame = 0;
    scarfyData.updateTime = 1.0/12.0;
    scarfyData.runningTime = 0;

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
        if (scarfyData.pos.y + scarfyData.rec.height >= windowDimensions[1])
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
        
        //update position nebula
        nebulae[0].pos.x += nebulaVel * dT;

        //update position second nebula
        nebulae[1].pos.x += nebulaVel * dT;

        //update position scarfy
        scarfyData.pos.y += velocity * dT;

        
        //update animation frame scarfy
        if (!isInAir)
        {
            scarfyData.runningTime += dT;
            
            if (scarfyData.runningTime >= scarfyData.updateTime)
            {
                scarfyData.rec.x = scarfyData.frame * scarfyData.rec.width;
                scarfyData.frame++;

                if (scarfyData.frame > 5)
                {
                    scarfyData.frame = 0;
                }

                scarfyData.runningTime = 0;
            }
        }
        
        //update animation frame nebula
        nebulae[0].runningTime += dT;
            
        if (nebulae[0].runningTime >= nebulae[0].updateTime)
        {
            nebulae[0].rec.x = nebulae[0].frame * nebulae[0].rec.width;
            nebulae[0].frame++;

            if (nebulae[0].frame > 7)
            {
                nebulae[0].frame = 0;
            }

            nebulae[0].runningTime = 0;
        }

        //update animation frame nebula
        nebulae[1].runningTime += dT;
            
        if (nebulae[1].runningTime >= nebulae[1].updateTime)
        {
            nebulae[1].rec.x = nebulae[1].frame * nebulae[1].rec.width;
            nebulae[1].frame++;

            if (nebulae[1].frame > 7)
            {
                nebulae[1].frame = 0;
            }

            nebulae[1].runningTime = 0;
        }


        //draw nebula
        DrawTextureRec(nebula, nebulae[0].rec, nebulae[0].pos, WHITE);

        //draw second nubula
        DrawTextureRec(nebula, nebulae[1].rec, nebulae[1].pos, RED);

        //draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);

        //Stop Drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    CloseWindow();
}