#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;
using namespace sf;

int const n = 700;                      //X - Dimension of The Window
int const m = 700;                      //Y - Dimension of The Window
int const y = 100;                      //Score Board Y - Dimensions
float const l = (m - y) * (7.0/12);     //Y - Dimensions of The Goal
float const w = n * (1.0/7);            //X - Dimension of The Goal
bool  Ball_Power = false;               //Value for checking if the power is increased or not
float speed = 0;                        //The pixel value which the ball traverse in the x-direction
float speed_y = 0;                      //The pixel value which the ball traverse in the y-direction
float maxspeed = 2;                     //Max value for speed_x
float direc=1;                          //Value for checking if ball speed_y should by up or down the screen
int Ball_Power_inc = 0;                 //Increment for filling the ball power bar with red color
bool Ball_Stop=false;                   //Value to check whether ball is moving or not
float Player_Speed = 0.5;               //Player Speed
float theta = 0;                        //Angle
bool intro;                             //Intro Menu
bool End_Game;                          //End Game
int Score_Based = 5;                    //Score Based Game
float Time_Based = 60000;                //Time Based Game
bool Game_Base_Selection = true;        //Game Base is Score Base

class Start_Game
{
    public:
        void Begin(RenderWindow *window);
};

class Player_1
{
    public:
        void Load_Body(string);         //Load Texture and Sprite
        void Role_Move();               //Setting Movement by Checking If Player Is A Shooter Or A Goalkeeper
        bool kick = true;               //
        int Score = 0;                  //Score
        Texture Texture1;               //Creating Texture
        Sprite Player1;                 //Creating Sprite
        void Win();
        Text Win_text;
};

class Player_2
{
    public:
        void Load_Body(string);         //Load Texture and Sprite
        void Role_Move();                //Setting Movement by Checking If Player Is A Shooter Or A Goalkeeper
        bool kick = false;              //
        int Score = 0;                  //Score
        Texture Texture2;               //Creating Texture
        Sprite Player2;                 //Creating Sprite
        void Win();
        Text Win_text;
};

class Ball
{
    public:
        void Load_Body(string);             //Load Texture and Sprite
        Texture Texture3;                   //Creating Texture
        Sprite SBall;
        void Ball_Physics(Event*,RenderWindow*,Start_Game*,Ball*,Player_1*,Player_2*,RectangleShape*,RectangleShape*);

                     //Creating Sprite
};

class Button
{
    public:
        Button(string, float, float, Font *);
        void Mouse_Over(Vector2i *);
        bool position;
        Text text;

};

Button::Button(string name, float x, float y, Font *font)
{
        text.setString(name);
        text.setFont(*font);
        text.setCharacterSize(50);
        text.setStyle(Text::Bold);
        text.setColor(Color::White);
        text.setPosition(x,y);
}

int main()
{
    RenderWindow window(VideoMode(n, m), "Game V1.0!");

    //Game_Field
    Start_Game Field;                                               //Initializing Game Field

    //Player One Messi
    Player_1 M10;                                                   //Initializing Player 2
    M10.Load_Body("M10.png");                                       //Loading Player Shape
    M10.Player1.setPosition((n/2) + (n/4), y + (m - y)/2 - 25);     //Setting new Position

    //Player Two Cristiano Ronaldo
    Player_2 CR7;                                                   //Initializing Player 2
    CR7.Load_Body("CR7.png");                                       //Loading Player Shape
    CR7.Player2.setPosition(w/2, y + (m - y)/2 - 25);               //Setting new Position

    //Ball
    Ball Ball;                                                      //Creating Texture
    Ball.Load_Body("Ball.png");                                     //Loading Ball Shape
    Ball.SBall.setPosition(n/2, y + (m - y)/2);                     //Setting new Position

    //create a font
    Font font;
    font.loadFromFile("Candara.ttf");                               //Load it from a file

    //Scoreboard
    ostringstream output;
    output << M10.Score <<" - " << CR7.Score;
    Text score;
    score.setFont(font);
    score.setCharacterSize(50);
    score.setPosition(310,20);
    score.setString(output.str());
    score.setFillColor(Color::White);

    RectangleShape Score_Board(Vector2f(n - 6,y - 6));
    Score_Board.setFillColor(Color(47,79,79));                     //Score Board Colors
    Score_Board.setOutlineThickness(3);                            //Score Board Outline Thickness
    Score_Board.setPosition(3,3);                                  //Setting (x,y)
    Score_Board.setOutlineColor(Color(169,169,169));               //Score Board Color


    //Initiates the power bar fill shape
    RectangleShape power(Vector2f(50, 10));
    power.setFillColor(Color::Transparent);
    power.setPosition((n/2) + (n/4) - 13, y + (m - y)/2 + 25);

    RectangleShape powerFill(Vector2f(0, 5));
    powerFill.setFillColor(Color::Green);
    powerFill.setPosition((n/2) + (n/4) - 13, y + (m - y)/2 + 27.5);

    //Sound for Goal!
    SoundBuffer goal;
    if (!goal.loadFromFile("Goal_Japanese.wav"))
        cout << "Failed to load sound file";
    Sound goalSound;
    goalSound.setBuffer(goal);

    //Sound for Out
    SoundBuffer out;
    if (!out.loadFromFile("Out_Japanese.wav"))
        cout << "Failed to load sound file";
    Sound outSound;
    outSound.setBuffer(out);

    //Intro
    intro = true;
    Button Start    ("Start Game", 240, 300, &font);
    Button Exit     ("Exit", 310 , 500, &font);
    Button Game_Base("Score Based 5 Points", 150, 400, &font);

    //End Game Window
    End_Game = false;

    //Clock
    Time Game_Time;
    Clock clock;
    ostringstream clock_time;
    Text Timer;
    Timer.setFont(font);
    Timer.setCharacterSize(50);
    Timer.setPosition(50,20);
    score.setFillColor(Color::White);

    //Return Back Button
    Button Back ("Main Menu", 430, 20, &font);

    while(window.isOpen())
    {
        Event event;
        if (intro == true && End_Game == false)
        {
            Vector2i localPosition = Mouse::getPosition(window);
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                window.close();

                Start.Mouse_Over(&localPosition);
                Game_Base.Mouse_Over(&localPosition);
                Exit.Mouse_Over(&localPosition);

                if (Start.position && Mouse::isButtonPressed(Mouse::Left))
                    {
                        intro = false;
                        M10.Score = 0;
                        CR7.Score = 0;
                        ostringstream output;
                        output << M10.Score <<" - " << CR7.Score;
                        score.setString(output.str());
                        if (Game_Base_Selection == false)
                        {
                            clock.restart();
                        }
                    }
                else if (Exit.position && Mouse::isButtonPressed(Mouse::Left))
                    window.close();
                else if (Game_Base.position && Mouse::isButtonPressed(Mouse::Left))
                {
                    if (Game_Base_Selection == true)
                    {
                        Game_Base_Selection = false;
                        Game_Base.text.setString("Time Based 60 Sec.");
                    }
                    else
                    {
                        Game_Base_Selection = true;
                        Game_Base.text.setString("Score Based 5 Points");
                    }
                }

            }
            window.clear();
            window.draw(Start.text);
            window.draw(Game_Base.text);
            window.draw(Exit.text);
            window.display();
        }
        else if (intro == false && End_Game == false)
        {
            Vector2i localPosition = Mouse::getPosition(window);
            Back.Mouse_Over(&localPosition);
            if (Back.position && Mouse::isButtonPressed(Mouse::Left))
            {
                intro = true;
            }
            //Setting Goal Keeper Movement
            if(M10.kick == true)
            {
               CR7.Role_Move();
            }
            else if (M10.kick == false)
            {
                M10.Role_Move();
            }

            // Sound Play for Out
            if((Ball.SBall.getPosition().x < -5 || Ball.SBall.getPosition().y < (y+7) || Ball.SBall.getPosition().y > (n-7)) && !((Ball.SBall.getPosition().x <= -5 && Ball.SBall.getPosition().y >= (y+ ((m - y)/2) - l/2) && Ball.SBall.getPosition().y <= (((m - y)/2) + l/2 + y))))
            {
                outSound.play();
            }

            //When Goal is entered update Sound and Score
            if((Ball.SBall.getPosition().x <= -5 && Ball.SBall.getPosition().y >= (y+ ((m - y)/2) - l/2) && Ball.SBall.getPosition().y <= (((m - y)/2) + l/2 + y)))
            {
                if(M10.kick == true)
                {
                    M10.Score++;
                    goalSound.play();
                    ostringstream output;
                    output << M10.Score <<" - " << CR7.Score;
                    score.setString(output.str());
                }

                else if(M10.kick == false)
                {
                    CR7.Score++;
                    goalSound.play();
                    ostringstream output;
                    output << M10.Score <<" - " << CR7.Score;
                    score.setString(output.str());
                }
            }

            //When ball is stopped, players are re-assigned to shooter and goalkeeper positions.
            if(Ball_Stop)
            {
                if(M10.kick == true)
                {
                    CR7.Player2.setPosition((n/2) + (n/4), y + (m - y)/2 - 25);
                    M10.Player1.setPosition(w/2, y + (m - y)/2 - 25);
                    Ball.SBall.setPosition(n/2, y + (m - y)/2);
                    power.setFillColor(Color::Transparent);
                    power.setPosition((n/2) + (n/4) - 16.5, y + (m - y)/2 + 25);
                    powerFill.setPosition((n/2) + (n/4) - 16.5, y + (m - y)/2 + 27.5);
                    CR7.kick = true;
                    M10.kick = false;
                    powerFill.setSize(Vector2f(0, 20));
                    speed = 0;
                    theta= 0;
                    Ball_Power_inc = 0;
                    Ball_Stop = false;

                    window.clear();
                    Field.Begin(&window);
                    window.draw(Score_Board);
                    window.draw(score);
                    window.draw(M10.Player1);
                    window.draw(CR7.Player2);
                    window.draw(Ball.SBall);
                    if(Ball_Power)
                    {
                        window.draw(powerFill);
                    }
                    window.display();
                }
                else if(M10.kick == false)
                {
                    M10.Player1.setPosition((n/2) + (n/4), y + (m - y)/2 - 25);
                    CR7.Player2.setPosition(w/2, y + (m - y)/2 - 25);
                    Ball.SBall.setPosition(n/2, y + (m - y)/2);
                    power.setPosition((n/2) + (n/4) - 13, y + (m - y)/2 + 25);
                    powerFill.setPosition((n/2) + (n/4) - 13, y + (m - y)/2 + 27.5);
                    power.setFillColor(Color::Transparent);
                    CR7.kick = false;
                    M10.kick = true;
                    powerFill.setSize(Vector2f(0, 20));
                    speed = 0;
                    theta = 0;
                    Ball_Power_inc = 0;
                    Ball_Stop = false;

                    window.clear();
                    Field.Begin(&window);
                    window.draw(Score_Board);
                    window.draw(score);
                    window.draw(M10.Player1);
                    window.draw(CR7.Player2);
                    window.draw(Ball.SBall);
                    if(Ball_Power)
                    {
                        window.draw(powerFill);
                    }
                    window.display();
                }
            }

            while(window.pollEvent(event))
            {
                Ball.Ball_Physics(&event,&window,&Field,&Ball,&M10,&CR7,&powerFill,&power);    //Game physics
                if(event.type == Event::Closed)                                                //Close Button
                window.close();
            }

            if (Game_Base_Selection == true && (M10.Score == Score_Based || CR7.Score == Score_Based) && M10.kick == true)
            {
                End_Game = true;
            }

            if (Game_Base_Selection == false)
            {
                ostringstream clock_time;
                int t = clock.getElapsedTime().asSeconds();
                clock_time << 60 - t << " Sec.";
                Timer.setString(clock_time.str());
                Game_Time = clock.getElapsedTime();
            }

            if (Game_Base_Selection== false && M10.kick == true && Game_Time.asMilliseconds() >= Time_Based)
            {
                End_Game = true;
            }

            window.clear();
            Field.Begin(&window);
            window.draw(Score_Board);
            window.draw(score);
            if (Game_Base_Selection == false)
                window.draw(Timer);
            window.draw(Back.text);
            window.draw(M10.Player1);
            window.draw(CR7.Player2);
            window.draw(Ball.SBall);
            window.draw(power);
            if(Ball_Power)
                window.draw(powerFill);
            window.display();
        }
        else if (intro == false && End_Game == true)
        {
            window.clear();
            if (M10.Score > CR7.Score)
            {
                M10.Win();
                M10.Win_text.setFont(font);
                window.draw(M10.Win_text);
            }
            else if (M10.Score < CR7.Score)
            {
                CR7.Win();
                CR7.Win_text.setFont(font);
                window.draw(CR7.Win_text);
            }
            else if (CR7.Score == M10.Score)
            {
                Text Tie_Text;
                Tie_Text.setString("Draw!!");
                Tie_Text.setFont(font);
                Tie_Text.setCharacterSize(50);
                Tie_Text.setPosition(270,200);
                Tie_Text.setColor(Color::Red);
                window.draw(Tie_Text);
            }

                Text Play_again;
                Play_again.setString("play again (Enter), exit(Esc).");
                Play_again.setFont(font);
                Play_again.setCharacterSize(50);
                Play_again.setPosition(50,300);
                Play_again.setColor(Color::Red);
                window.draw(Play_again);

                if(Keyboard::isKeyPressed(Keyboard::Return))
                {
                    End_Game = false;
                    intro = true;
                }
                else if (Keyboard::isKeyPressed(Keyboard::Escape))
                    window.close();
                    window.display();
        }
    }
    return 0;
}

void    Start_Game::Begin(RenderWindow *window)
{
        RectangleShape ScoreBoard(Vector2f(n ,y));
        //Field
        RectangleShape Field(Vector2f(n, m - y));                   //Field
        Field.setFillColor(Color::Green);                           //Field Color
        Field.setPosition(0,y);                                     //Position
        //Boarders of The PlayGround
        RectangleShape Boarders(Vector2f(n - 10, m - y - 10));      //Boarders
        Boarders.setFillColor(Color::Green);                        //Boarders Colors
        Boarders.setOutlineThickness(2);                            //Boarders Outline Thickness
        Boarders.setPosition(5, y + 5);                             //Setting (x,y)
        Boarders.setOutlineColor(Color::White);                     //Boarder Color
        //Goal_Boarders
        RectangleShape Goal(Vector2f(w, l));                        //Goal
        Goal.setFillColor(Color::Green);                            //Goal Color
        Goal.setOutlineThickness(2);                                //Goal outline Thickness
        Goal.setPosition(5, y + (m - y)/2 - l/2) ;                  //Setting (x,y)
        Goal.setOutlineColor(Color::White);                         //Boarder Color



        window -> draw(ScoreBoard);
        window -> draw(Field);
        window -> draw(Boarders);
        window -> draw(Goal);
}

void Player_1::Load_Body(string s)
{
        Texture1.loadFromFile(s);                            //Reading Texture
        Texture1.setSmooth(true);                            //Smoothing Texture
        Player1.setTextureRect(IntRect(0, 0, 24, 50));       //Setting Position and Size
        Player1.setTexture(Texture1);                        //Create the Spirit
}

void Player_1::Role_Move()
{
    if(Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Down) && !Keyboard::isKeyPressed(Keyboard::Right))
    {
        if(Player1.getPosition().x > 7)
        Player1.move(-Player_Speed, 0);
    }

    else if(Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Down) && !Keyboard::isKeyPressed(Keyboard::Left))
    {
        if(Player1.getPosition().x < w + 5 - 24)
        Player1.move(Player_Speed, 0);
    }
    else if(Keyboard::isKeyPressed(Keyboard::Up) && !Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Down))
    {
        if(Player1.getPosition().y > y + (m - y)/2 - l/2)
        Player1.move(0, -Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::Down) && !Keyboard::isKeyPressed(Keyboard::Right) && !Keyboard::isKeyPressed(Keyboard::Left) && !Keyboard::isKeyPressed(Keyboard::Up))
    {
        if(Player1.getPosition().y < y + (m - y)/2 + l/2 - 50)
        Player1.move(0, Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::Right) && Keyboard::isKeyPressed(Keyboard::Up))
    {
        if(Player1.getPosition().x < w + 5 - 24 && Player1.getPosition().y > y + (m - y)/2 - l/2)
        Player1.move(Player_Speed, -Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::Right) && Keyboard::isKeyPressed(Keyboard::Down))
    {
        if(Player1.getPosition().x < w + 5 - 24 && Player1.getPosition().y < y + (m - y)/2 + l/2 - 50)
        Player1.move(Player_Speed, Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::Left) && Keyboard::isKeyPressed(Keyboard::Up))
    {
        if(Player1.getPosition().x > 7 && Player1.getPosition().y > y + (m - y)/2 - l/2)
        Player1.move(-Player_Speed, -Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::Left) && Keyboard::isKeyPressed(Keyboard::Down))
    {
        if(Player1.getPosition().x > 7 && Player1.getPosition().y < y + (m - y)/2 + l/2 - 50)
        Player1.move(-Player_Speed, Player_Speed);
    }
}

void Player_1::Win()
{
    Win_text.setString("Messi Has Won");
    Win_text.setCharacterSize(50);
    Win_text.setPosition(200,200);
    Win_text.setColor(Color::Red);
}

void Player_2::Load_Body(string s)
{
        Texture2.loadFromFile(s);                            //Reading Texture
        Texture2.setSmooth(true);                            //Smoothing Texture
        Player2.setTextureRect(IntRect(0, 0, 17, 50));       //Setting Position and Size
        Player2.setTexture(Texture2);                        //Create the Spirit
}

void Player_2::Role_Move()
{
    if(Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::D))
    {
        if(Player2.getPosition().x > 7)
        Player2.move(-Player_Speed, 0);
    }

    else if(Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::A))
    {
        if(Player2.getPosition().x < w + 5 - 17)
        Player2.move(Player_Speed, 0);
    }
    else if(Keyboard::isKeyPressed(Keyboard::W) && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::S))
    {
        if(Player2.getPosition().y > y + (m - y)/2 - l/2)
        Player2.move(0, -Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::S) && !Keyboard::isKeyPressed(Keyboard::D) && !Keyboard::isKeyPressed(Keyboard::A) && !Keyboard::isKeyPressed(Keyboard::W))
    {
        if(Player2.getPosition().y < y + (m - y)/2 + l/2 - 50)
        Player2.move(0, Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::W))
    {
        if(Player2.getPosition().x < w + 5 - 17 && Player2.getPosition().y > y + (m - y)/2 - l/2)
        Player2.move(Player_Speed, -Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::D) && Keyboard::isKeyPressed(Keyboard::S))
    {
        if(Player2.getPosition().x < w + 5 - 17 && Player2.getPosition().y < y + (m - y)/2 + l/2 - 50)
        Player2.move(Player_Speed, Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::W))
    {
        if(Player2.getPosition().x > 7 && Player2.getPosition().y > y + (m - y)/2 - l/2)
        Player2.move(-Player_Speed, -Player_Speed);
    }
    else if(Keyboard::isKeyPressed(Keyboard::A) && Keyboard::isKeyPressed(Keyboard::S))
    {
        if(Player2.getPosition().x > 7 && Player2.getPosition().y < y + (m - y)/2 + l/2 - 50)
        Player2.move(-Player_Speed, Player_Speed);
    }
}

void Player_2::Win()
{
    Win_text.setString("Cristiano Has Won");
    Win_text.setCharacterSize(50);
    Win_text.setPosition(150,200);
    Win_text.setColor(Color::Red);
}

void Ball::Load_Body(string s)
{
        Texture3.loadFromFile(s);                            //Reading Texture
        Texture3.setSmooth(true);                            //Smoothing Texture
        SBall.setTextureRect(IntRect(0, 0, 15, 15));         //Setting Position and Size
        SBall.setTexture(Texture3);                          //Create the Spirit
}

void Ball::Ball_Physics(Event *event,RenderWindow *window,Start_Game *Field,Ball *Ball ,Player_1 *M10,Player_2 *CR7,RectangleShape *powerFill,RectangleShape *power)
{
    //Power Bar filling and increasing ball speed as user presses space
    if(Keyboard::isKeyPressed(Keyboard::Space) && speed < maxspeed && (M10->Player1.getPosition().x > (Ball->SBall.getPosition().x + 15) || CR7->Player2.getPosition().x > (Ball->SBall.getPosition().x + 15)))
    {
        power->setFillColor(Color::Black);
        speed += 0.1;
        Ball_Power_inc += 5;
        powerFill->setSize(Vector2f(Ball_Power_inc/2, 5));
        powerFill->setFillColor(sf::Color((speed/2) < 0.5 ? (0 + 25.5*(speed)*10):255,(speed/2) > 0.6 ? (255 - 25.5*(speed)*10):255, 0));
        Ball_Power = true;
    }

    //Ball mechanics if Messi shoots
    if(event->type == Event::KeyPressed)
    {
        if(event->key.code == Keyboard::Left && M10->Player1.getPosition().x == (n/2 + n/4))
        {
            //Fail safe statement if user do not presses space before moving
            if(speed == 0)
            {
                speed = 0.2;
            }

            //Player move automation in the beginning of the shoot if user presses left button
            while (M10->Player1.getPosition().x > (Ball->SBall.getPosition().x + 15))
            {
                M10->Player1.move(-0.2, 0);
                power->move(-0.2,0);
                powerFill->move(-0.2,0);

                //Increasing the angle of the shoot
                if(Keyboard::isKeyPressed(Keyboard::Up) && theta < 70)
                {
                    theta = (theta + 0.05);
                    direc = -1.0;
                }
                else if(Keyboard::isKeyPressed(Keyboard::Down) && theta < 70)
                {
                    theta = (theta + 0.05);
                    direc = 1.0;
                }

                    CR7->Role_Move();

                    window->clear();
                    Field->Begin(window);
                    window->draw(M10->Player1);
                    window->draw(CR7->Player2);
                    window->draw(Ball->SBall);
                    window->draw(*power);
                    window->draw(*powerFill);
                    window->display();
            }
        }
    }

    //Ball movement intiation as user presses space at the vicinity of the ball
    if(M10->kick)
    {

        if(event->type == Event::KeyReleased)
        {
            if(event->key.code == Keyboard::Space && (Ball->SBall.getPosition().x + 15) > M10->Player1.getPosition().x && Ball->SBall.getPosition().x < (M10->Player1.getPosition().x+24) && (Ball->SBall.getPosition().y + 15) <= (M10->Player1.getPosition().y + 50) && Ball->SBall.getPosition().y >= M10->Player1.getPosition().y)
            {
                Ball_Stop = false;

                while(Ball->SBall.getPosition().x > -7 && Ball_Stop == false)
                {
                    if (Ball->SBall.getPosition().y < (y+7) || Ball->SBall.getPosition().y > (n-7))
                    {
                        Ball_Stop = true;
                    }
                    if(Ball->SBall.getPosition().x <= (CR7->Player2.getPosition().x + 17) && (Ball->SBall.getPosition().x+15) >= (CR7->Player2.getPosition().x) && Ball->SBall.getPosition().y <= (CR7->Player2.getPosition().y + 50) && (Ball->SBall.getPosition().y+15) >= CR7->Player2.getPosition().y)
                    {
                        Ball_Stop = true;
                    }
                    if(Ball->SBall.getPosition().x < -5)
                    {
                        Ball_Stop = true;
                    }

                    Ball->SBall.move(-speed*cos((3.1415926536/180)*theta), direc*speed*sin((3.1415926536/180)*theta));

                    CR7->Role_Move();

                    window->clear();
                    Field->Begin(window);
                    window->draw(M10->Player1);
                    window->draw(CR7->Player2);
                    window->draw(Ball->SBall);
                    window->draw(*power);
                    window->draw(*powerFill);
                    window->display();
                }
            }
        }
    }

//Ball mechanics if CR7 shoots

    if(event->type == Event::KeyPressed)
    {
        if(event->key.code == Keyboard::A && CR7->Player2.getPosition().x == (n/2+n/4))
        {
            //Fail safe statement if user do not presses space before moving
            if(speed == 0)
            {
                speed = 0.2;
            }

            //Player move automation in the beginning of the shoot if user presses left button
            while (CR7->Player2.getPosition().x > (Ball->SBall.getPosition().x + 15))
            {
                CR7->Player2.move(-0.2, 0);
                power->move(-0.2,0);
                powerFill->move(-0.2,0);

                //Increasing the angle of the shoot
                if(Keyboard::isKeyPressed(Keyboard::W) && theta < 70)
                {
                    theta = (theta + 0.05);
                    direc = -1.0;
                }
                else if(Keyboard::isKeyPressed(Keyboard::S) && theta < 70)
                {
                    theta = (theta + 0.05);
                    direc = 1.0;
                }

                M10->Role_Move();

                window->clear();
                Field->Begin(window);
                window->draw(M10->Player1);
                window->draw(CR7->Player2);
                window->draw(Ball->SBall);
                window->draw(*power);
                window->draw(*powerFill);
                window->display();
            }
        }
    }

    //Ball movement intiation as user presses space at the vicinity of the ball
    if(!M10->kick)
    {
        if(event->type == Event::KeyReleased)
        {
            if(event->key.code == Keyboard::Space && (Ball->SBall.getPosition().x + 15) > CR7->Player2.getPosition().x && Ball->SBall.getPosition().x < (CR7->Player2.getPosition().x+17) && (Ball->SBall.getPosition().y + 15) <= (CR7->Player2.getPosition().y + 50) && Ball->SBall.getPosition().y >= CR7->Player2.getPosition().y)
            {
                Ball_Stop = false;

                while(Ball->SBall.getPosition().x > -7 && Ball_Stop == false)
                {
                    if (Ball->SBall.getPosition().y < (y+7) || Ball->SBall.getPosition().y > (n-7))
                    {
                        Ball_Stop = true;
                    }
                    if(Ball->SBall.getPosition().x <= (M10->Player1.getPosition().x + 17) && (Ball->SBall.getPosition().x+15) >= M10->Player1.getPosition().x && Ball->SBall.getPosition().y <= (M10->Player1.getPosition().y + 50) && (Ball->SBall.getPosition().y+15) >= M10->Player1.getPosition().y)
                    {
                        Ball_Stop = true;
                    }
                    if(Ball->SBall.getPosition().x < -5)
                    {
                        Ball_Stop = true;
                    }

                    Ball->SBall.move(-speed*cos((3.1415926536/180)*theta), direc*speed*sin((3.1415926536/180)*theta));

                    M10->Role_Move();

                    window->clear();
                    Field->Begin(window);
                    window->draw(M10->Player1);
                    window->draw(CR7->Player2);
                    window->draw(Ball->SBall);
                    window->draw(*power);
                    window->draw(*powerFill);
                    window->display();

                }
            }
        }
    }

}

void Button::Mouse_Over(Vector2i *localPosition)
{
    if (localPosition -> x >= text.getGlobalBounds().left && localPosition -> y >= text.getGlobalBounds().top && localPosition -> x < (text.getGlobalBounds().left + text.getGlobalBounds().width) && localPosition -> y < (text.getGlobalBounds().top + text.getGlobalBounds().height))
        {
            text.setColor(Color::Red);
            position = true;
        }
    else
    {
        text.setColor(Color::White);
        position = false;
    }
}
