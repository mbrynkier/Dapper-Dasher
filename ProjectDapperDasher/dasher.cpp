#include "raylib.h"


struct AnimData
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y >= windowHeight - data.rec.height;
}

AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)
{
    data.runningTime += deltaTime;
            
    if (data.runningTime >= data.updateTime)
    {
        data.rec.x = data.frame * data.rec.width;
        data.frame++;

        if (data.frame > maxFrame)
        {
            data.frame = 0;
        }

        data.runningTime = 0;
    }

    return data;
}

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

    const int sizeOfNebulae{3};

    AnimData nebulae[sizeOfNebulae]{};

    for (int i = 0; i < sizeOfNebulae; i++)
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
    
    float finishLine{nebulae[sizeOfNebulae-1].pos.x};

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
    
    Texture2D background = LoadTexture("textures/far-buildings.png");
    float bgX{};

    Texture2D midground = LoadTexture("textures/back-buildings.png");
    float mgX{};

    Texture2D foreground = LoadTexture("textures/foreground.png");
    float fgX{};

    bool collision{false};

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {

        //delta time
        const float dT{GetFrameTime()};        

        //Start Drawing

        BeginDrawing();
        ClearBackground(WHITE);

        //Background-------------------------------------------
        bgX -= 20 * dT;

        if (bgX <= -background.width*2)
        {
            bgX = 0;
        }

        //draw background
        Vector2 bg1Pos{bgX, 0.0};
        Vector2 bg2Pos{bgX + background.width*2, 0.0};
        DrawTextureEx(background, bg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(background, bg2Pos, 0.0, 2.0, WHITE);

        //Midground-------------------------------------------
        mgX -= 40 * dT;

        if (mgX <= -midground.width*2)
        {
            mgX = 0;
        }

        //draw midground
        Vector2 mg1Pos{mgX, 0.0};
        Vector2 mg2Pos{mgX + midground.width*2, 0.0};
        DrawTextureEx(midground, mg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(midground, mg2Pos, 0.0, 2.0, WHITE);


        //foreground-----------------------------------------
        fgX -= 80 * dT;

        if (fgX <= -foreground.width*2)
        {
            fgX = 0;
        }

        //draw foreground
        Vector2 fg1Pos{fgX, 0.0};
        Vector2 fg2Pos{fgX + foreground.width*2, 0.0};
        DrawTextureEx(foreground, fg1Pos, 0.0, 2.0, WHITE);
        DrawTextureEx(foreground, fg2Pos, 0.0, 2.0, WHITE);


        //perform ground check
        if (isOnGround(scarfyData, windowDimensions[1]))
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

        //update position scarfy
        scarfyData.pos.y += velocity * dT;

        
        //update animation frame scarfy
        if (!isInAir)
        {
            scarfyData = updateAnimData(scarfyData, dT, 5);
        }
        
        //Finish Line
        finishLine += nebulaVel * dT;        

        for (AnimData nebula : nebulae)
        {
            float pad{50};
            Rectangle nebRec{
                nebula.pos.x + pad,                 // Rectangle top-left corner position x
                nebula.pos.y + pad,                 // Rectangle top-left corner position y
                nebula.rec.width - pad*2,            // Rectangle width
                nebula.rec.height - pad*2            // Rectangle height
            };

            Rectangle scarfyRec{
                scarfyData.pos.x,
                scarfyData.pos.y,
                scarfyData.rec.width,
                scarfyData.rec.height
            };

            if (CheckCollisionRecs(nebRec, scarfyRec))
            {
                collision = true;
            }
            
        }
                

        for (int i = 0; i < sizeOfNebulae; i++)
        {
            //update position nebula
            nebulae[i].pos.x += nebulaVel * dT;

            //update animation frame nebula
            nebulae[i] = updateAnimData(nebulae[i], dT, 7);
        }

        
        if (collision)
        {
            //loose game
            DrawText("Game Over!", windowDimensions[0]/3, windowDimensions[1]/2, 20, RED);
        
        }
        else if (scarfyData.pos.x >= finishLine)
        {
            DrawText("You Win!", windowDimensions[0]/3, windowDimensions[1]/2, 20, GREEN);
        }else
        {
            
            for (int i = 0; i < sizeOfNebulae; i++)
            {
                DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, WHITE);
            }
            //draw scarfy
            DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        }

        //Stop Drawing
        EndDrawing();
    }
    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(background);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}