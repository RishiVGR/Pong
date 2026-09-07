#include <iostream>
#include <raylib.h>

class Ball{
    public:
        float x,y;                  // basic asset features
        int speed_x, speed_y;
        int radius;
        Color color = WHITE;

    public:
        void drawBall(){ // draw the ball
            DrawCircle(x, y, radius, color);
        }

        void update(){ // update balls location (dependent on fps) - this case 420px per s
            x += speed_x;
            y += speed_y;
            
            if(y + radius >= GetScreenHeight() || y-radius <= 0){ // check if ball is on or beyond the screen boundaries
                // then reverse direction
                speed_y *= -1;
            }
            if(x+radius >= GetScreenWidth() || x-radius <= 0){ // check if ball is on or beyond the screen boyndaries
                // then reverse direction
                speed_x*=-1;
            }
        }

};

class Paddle{
    public:
        float width, height;
        float speed_y;
        float y;
        float x = 10;
        Color color;

    public:
        void drawPaddle(){
            DrawRectangle(x, y, width, height, color); // draw player paddle
        }
        void updatePaddle(){
            if (y <= 0){
                y = 0; // if y value is equal or less than 0 (top of screen) - set y to top
            }
            if ((y+height) >= GetScreenHeight()){
                y = GetScreenHeight() - height; // if y value is greater than screen height - set bottom of paddle equal to screen height
            }
            if(IsKeyDown(KEY_UP)){ // check if Up key is pressed
                speed_y = 5;
                y -= speed_y;
            }
            else if (IsKeyDown(KEY_DOWN)){ // check if Down key is pressed
                speed_y = 5;
                y += speed_y;
            }
                       
        }
    

};

class cpuPaddle: public Paddle{
    public:
        void updateCPU(int ball_y){
            if(y+height/2 > ball_y){ // check if center of ball y is less than half of paddle
                y = y -speed_y;
            }
            if(y+height/2 <= ball_y){ // check if center of ball y is greater than half of paddle 
                y=y+speed_y;
            }
            
        }
};



int main () {
    const int scrWidth {1280}; // defined constant screen dimensions - top left = 0,0 - bottom right = 1280,800
    const int scrHeight  {800};
    int centerScrx = scrWidth/2, centerScry = scrHeight/2;

    std::cout << "Starting the game" << std::endl;
    InitWindow(scrWidth, scrHeight, "My Pong Game!"); // create window
    SetTargetFPS(60); // set fps of game window - if not defined 
                     //- computer will run as fast as possible

    // ball instance
    Ball gameBall;
    gameBall.x = centerScrx;
    gameBall.y = centerScry;
    gameBall.radius = 15;
    gameBall.speed_x = 7;
    gameBall.speed_y = 7;
    gameBall.color = RED;

    // player instance
    Paddle player;
    player.y = (scrHeight/2) - 60;
    player.height = 120;
    player.width = 25;
    player.speed_y = 5;
    player.color = GREEN;

    // cpu instance
    cpuPaddle ai;
    ai.y = (scrHeight/2) - 60;
    ai.height = 120;
    ai.speed_y = 6;
    ai.color = GREEN;
    ai.width = 25;
    ai.x = (scrWidth-10) - ai.width;


    while(WindowShouldClose() == false){ // checks if esc key pressed - or window exited
        BeginDrawing(); // create blank canvas

        //update
        player.updatePaddle();
        gameBall.update(); // updates balls location
        ai.updateCPU(gameBall.y);
        if(CheckCollisionCircleRec(Vector2{gameBall.x, gameBall.y}, gameBall.radius, Rectangle{player.x, player.y, player.width, player.height})){ // check if ball collides with player paddle
            gameBall.speed_x *= -1;
        }
        if(CheckCollisionCircleRec(Vector2{gameBall.x, gameBall.y}, gameBall.radius, Rectangle{ai.x, ai.y, ai.width, ai.height})){ // check if ball collides with cpu paddle
            gameBall.speed_x *= -1;
        }

        //assets
        ClearBackground(WHITE); // clears background before drawing assets - gets rid of trace
        DrawLine(scrWidth/2, 0, scrWidth/2, scrHeight, DARKBLUE);
        player.drawPaddle();
        gameBall.drawBall();
        ai.drawPaddle();


        EndDrawing(); // ends canvas drawing
    }

    CloseWindow();
    return 0;
}