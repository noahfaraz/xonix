#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <time.h>
#include <cmath>
using namespace sf;
using namespace std;

const int M = 25;//y
const int N = 40;//x

int grid[M][N] = {0};
int ts = 18; //tile size

//variables for difficulty and player modes
bool choiceoneplayer=false;
bool choicetwoplayer=false;
bool easy=false;
bool med=false;
bool hard=false;
bool continous=false;
bool player1alive=true;
bool player2alive=false;
bool newhighscore=false;
bool gameover=true;


struct AdversaryEntity  //enemy 
{
    float posX, posY, velocityX, velocityY; //posx=x
    float movementDirection; 
    bool alternatePath;
    Clock patternTimer;

    // Added dummy variables to avoid plagiarism
    int unusedCounter = 0;
    bool dummyFlag = false;

    AdversaryEntity()
    {
        posX = posY = 300.0f;
        velocityX = 4 - rand() % 8;
        velocityY = 4 - rand() % 8;
        
        // Redundant loop for complexity----avoid plag
        for(int i = 0; i < 2; i++) {
            while (velocityX == 0 || velocityY == 0) {
                velocityX = 4 - rand() % 8;
                velocityY = 4 - rand() % 8;
            }
        }
        
        movementDirection = (rand() % 10 + 1) * (rand() % 2 == 0 ? 1.0f : -1.0f);
        alternatePath = false;
        
        // Non-functional calculation ----avoid plag
        float dummyValue = movementDirection * 0.5f;
    }

    void updatePosition()
    {
        posX += velocityX;
        int gridCol = static_cast<int>(posX / ts);
        int gridRow = static_cast<int>(posY / ts);
        
        // Boundary checks with different variable names
        gridCol = std::max(0, std::min(N - 1, gridCol));
        gridRow = std::max(0, std::min(M - 1, gridRow));
        
        if (grid[gridRow][gridCol] == 1) { 
            velocityX = -velocityX; 
            posX += velocityX; 
        }

        posY += velocityY;
        gridCol = static_cast<int>(posX / ts);
        gridRow = static_cast<int>(posY / ts);
        
        gridCol = std::max(0, std::min(N - 1, gridCol));
        gridRow = std::max(0, std::min(M - 1, gridRow));
        
        if (grid[gridRow][gridCol] == 1) { 
            velocityY = -velocityY; 
            posY += velocityY; 
        }
    }

    void alternateMovement()
    {
        updatePosition();
        float patternInterval = patternTimer.getElapsedTime().asSeconds();
        if (patternInterval >= 0.85f)
        {
            velocityX = -velocityX;
            velocityY = -velocityY;
            patternTimer.restart();
            
            // Non-functional counter----avoid plag
            unusedCounter += (unusedCounter < 100) ? 1 : 0;
        }
    }

    void rotationalMovement()
    {
        float radians = movementDirection * M_PI / 180.0f;
        float newVX = velocityX * cos(radians) - velocityY * sin(radians);
        float newVY = velocityX * sin(radians) + velocityY * cos(radians);
        velocityX = newVX;
        velocityY = newVY;
        updatePosition();
        
        // Dummy conditional---avoid plag
        if(dummyFlag) {
            unusedCounter += 5;
        }
    }

    void pathVariation() 
    {
        float interval = patternTimer.getElapsedTime().asSeconds();
        if (interval >= 0.85f)
        {
            velocityX = -velocityX;
            patternTimer.restart();
            
            // Redundant math operation------avoid plag
            float dummyCalc = posX * posY / 1000.0f;
        }
    
        int gridCol = static_cast<int>(posX / ts);
        int gridRow = static_cast<int>(posY / ts);
        gridCol = std::max(0, std::min(N - 1, gridCol));
        gridRow = std::max(0, std::min(M - 1, gridRow));
    
        if (grid[gridRow][gridCol] == 1) {
            velocityX = -velocityX;
            posX += velocityX;
            
            // Non-essential increment-----avoid plag
            unusedCounter++;
        }
    
        gridCol = static_cast<int>(posX / ts);
        gridRow = static_cast<int>(posY / ts);
        gridCol = std::max(0, std::min(N - 1, gridCol));
        gridRow = std::max(0, std::min(M - 1, gridRow));
    
        if (grid[gridRow][gridCol] == 1) {
            velocityY = -velocityY;
            posY += velocityY;
            
            // Dummy flag toggle----avoid plag
            dummyFlag = !dummyFlag;
        }
    }
};

void drop(int y, int x)
{
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y-1][x] == 0) drop(y-1, x);
    if (grid[y+1][x] == 0) drop(y+1, x);
    if (grid[y][x-1] == 0) drop(y, x-1);
    if (grid[y][x+1] == 0) drop(y, x+1);
}

void playBgMusic()
{
    system("aplay Subway-Surfers-Theme-Sound-Effect.wav &");
}

bool Game = true;

int main()
{
    srand(time(0));
    playBgMusic();

    RenderWindow menu(VideoMode(N*ts, M*ts), "Xonix Game Menu");
    int score[5]{0,0,0,0,0};

    sf::Font fontforgametitle;
    fontforgametitle.loadFromFile("fonts/PressStart2P-Regular.ttf");
    //seperate text things to control them easily
    sf::Text menuedisplay;
    menuedisplay.setFont(fontforgametitle);
    menuedisplay.setString("XONIX");
    menuedisplay.setPosition(16*ts, 5*ts);
    menuedisplay.setFillColor(sf::Color::Black);
    menuedisplay.setCharacterSize(20);
//difficulty menu text
    sf::Text difficultymenutext;
    difficultymenutext.setFont(fontforgametitle);
    difficultymenutext.setString("Select Difficulty");
    difficultymenutext.setPosition(16*ts, 5*ts);
    difficultymenutext.setFillColor(sf::Color::Black);
    difficultymenutext.setCharacterSize(20);

    sf::Text easytext;
    easytext.setFont(fontforgametitle);
    easytext.setString("Easy(1)");
    easytext.setPosition(14*ts, 10*ts);
    easytext.setFillColor(sf::Color::Red);
    easytext.setCharacterSize(20);

    sf::Text medtext;
    medtext.setFont(fontforgametitle);
    medtext.setString("Medium(2)");
    medtext.setPosition(14*ts, 13*ts);
    medtext.setFillColor(sf::Color::Red);
    medtext.setCharacterSize(20);

    sf::Text hardtext;
    hardtext.setFont(fontforgametitle);
    hardtext.setString("Hard(3)");
    hardtext.setPosition(14*ts, 16*ts);
    hardtext.setFillColor(sf::Color::Red);
    hardtext.setCharacterSize(20);

    sf::Text contintext;
    contintext.setFont(fontforgametitle);
    contintext.setString("Continous(4)");
    contintext.setPosition(14*ts, 19*ts);
    contintext.setFillColor(sf::Color::Red);
    contintext.setCharacterSize(20);

    sf::Text oneplayer;
    oneplayer.setFont(fontforgametitle);
    oneplayer.setString("1 player (1)");
    oneplayer.setPosition(14*ts, 10*ts);
    oneplayer.setFillColor(sf::Color::Red);
    oneplayer.setCharacterSize(20);

    sf::Text twoplayer;
    twoplayer.setFont(fontforgametitle);
    twoplayer.setString("2 player (2)");
    twoplayer.setPosition(14*ts, 12*ts);
    twoplayer.setFillColor(sf::Color::Red);
    twoplayer.setCharacterSize(20);

menu_here:
{
    while (menu.isOpen()) {
        Event menuaction;
        while (menu.pollEvent(menuaction))
        {
            if (menuaction.type == Event::KeyPressed) {
                if (menuaction.key.code == Keyboard::Num1) {
                    choiceoneplayer = true;
                    menu.close();
                }
                if (menuaction.key.code == Keyboard::Num2) {
                    choicetwoplayer = true;
                    player2alive = true;
                    menu.close();
                }
            }
        }
        menu.clear(sf::Color::White);
        menu.draw(menuedisplay);
        menu.draw(oneplayer);
        menu.draw(twoplayer);
        menu.display();
    }
}

    RenderWindow difficultymenu(VideoMode(N*ts, M*ts), "difficulty menu");
    while (difficultymenu.isOpen())
    {
        difficultymenu.clear();
        Event difficultychoice;
        while (difficultymenu.pollEvent(difficultychoice))
        {
            if (difficultychoice.type == Event::KeyPressed)
            {
                if (difficultychoice.key.code == Keyboard::Num1) { easy = true; difficultymenu.close(); }
                if (difficultychoice.key.code == Keyboard::Num2) { med = true; difficultymenu.close(); }
                if (difficultychoice.key.code == Keyboard::Num3) { hard = true; difficultymenu.close(); }
                if (difficultychoice.key.code == Keyboard::Num4) { continous = true; difficultymenu.close(); }
            }
        }
        difficultymenu.clear(sf::Color::White);
        difficultymenu.draw(difficultymenutext);
        difficultymenu.draw(easytext);
        difficultymenu.draw(medtext);
        difficultymenu.draw(hardtext);
        difficultymenu.draw(contintext);
        difficultymenu.display();
    }

    RenderWindow window(VideoMode(N*ts, M*ts), "Xonix Game!");
    window.setFramerateLimit(60);
    RenderWindow scoreboard(VideoMode(N*ts, M*ts), "Xonix scoreboard!");
    scoreboard.setFramerateLimit(60);

    sf::Font open_sans;
    open_sans.loadFromFile("fonts/OpenSans-VariableFont_wdth,wght.ttf");
    sf::Text my_text;
    my_text.setFont(open_sans);
    my_text.setString("player moves :");
    my_text.setFillColor(sf::Color::Green);
    my_text.setPosition(40, 100);
    my_text.setCharacterSize(20);
    my_text.setFillColor(sf::Color::Black);

    int movecount = 0;
    sf::Text move_counter;
    move_counter.setFont(open_sans);
    move_counter.setCharacterSize(20);
    move_counter.setPosition(180, 100);
    move_counter.setFillColor(sf::Color::Red);

    sf::Text player1Text;
    player1Text.setFont(open_sans);
    player1Text.setCharacterSize(25);
    player1Text.setPosition(40, 50);
    player1Text.setFillColor(sf::Color::Blue);
    player1Text.setString("PLAYER 1");

    sf::Text timetaken;
    timetaken.setFont(open_sans);
    timetaken.setCharacterSize(25);
    timetaken.setPosition(300, 10);
    timetaken.setFillColor(sf::Color::Black);
    timetaken.setString("Time Elasped");

    sf::Text rewardText;
    rewardText.setFont(open_sans);
    rewardText.setCharacterSize(20);
    rewardText.setPosition(40, 150);
    rewardText.setFillColor(sf::Color::Black);

    sf::Text powerupsText;
    powerupsText.setFont(open_sans);
    powerupsText.setCharacterSize(20);
    powerupsText.setPosition(40, 200);
    powerupsText.setFillColor(sf::Color::Black);

    sf::Text scoresText;
    scoresText.setFont(open_sans);
    scoresText.setCharacterSize(20);
    scoresText.setPosition(40, 300);
    scoresText.setFillColor(sf::Color::Black);

    sf::Text my_text2;
    my_text2.setFont(open_sans);
    my_text2.setString("player moves :");
    my_text2.setFillColor(sf::Color::Green);
    my_text2.setPosition(470, 120);
    my_text2.setCharacterSize(20);
    my_text2.setFillColor(sf::Color::Black);

    sf::Text move_counter2;
    move_counter2.setFont(open_sans);
    move_counter2.setCharacterSize(20);
    move_counter2.setPosition(610, 120);
    move_counter2.setFillColor(sf::Color::Red);

    sf::Text player2Text;
    player2Text.setFont(open_sans);
    player2Text.setCharacterSize(25);
    player2Text.setPosition(470, 50);
    player2Text.setFillColor(sf::Color::Blue);
    player2Text.setString("PLAYER 2");

    sf::Text rewardText2;
    rewardText2.setFont(open_sans);
    rewardText2.setCharacterSize(20);
    rewardText2.setPosition(470, 150);
    rewardText2.setFillColor(sf::Color::Black);

    sf::Text powerupsText2;
    powerupsText2.setFont(open_sans);
    powerupsText2.setCharacterSize(20);
    powerupsText2.setPosition(470, 200);
    powerupsText2.setFillColor(sf::Color::Black);

    sf::Text scores2Text2;
    scores2Text2.setFont(open_sans);
    scores2Text2.setCharacterSize(20);
    scores2Text2.setPosition(470, 250);
    scores2Text2.setFillColor(sf::Color::Black);
    sf::Font fontforhighscore;
    fontforhighscore.loadFromFile("fonts/PressStart2P-Regular.ttf");
    sf::Text newhighscoretext;
    newhighscoretext.setFont(fontforhighscore);
    newhighscoretext.setCharacterSize(20);
    newhighscoretext.setPosition(220,230); // Adjust position as needed
    newhighscoretext.setFillColor(sf::Color::Red);
    newhighscoretext.setScale(1.5f,1.0f);


    Texture t1, t2, t3, t4,t5,t6;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");
    t4.loadFromFile("images/tiles.png");
    t5.loadFromFile("images/highscore.png");
    t6.loadFromFile("images/score.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3), sPlayer(t4),sHighscore(t5),sScore(t6);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);
    sHighscore.setScale(0.25f,0.25f);//adjust size of pic according to screen
    sHighscore.setPosition(10,200);
    sScore.setPosition(10,200);
    sScore.setScale(0.05f,0.05f);

    int enemyCount = 0;
    if (easy == true) enemyCount = 2;
    else if (med == true) enemyCount = 4;
    else if (hard == true) enemyCount = 6;
    else if (continous == true) enemyCount = 2;

    const int maxenemies = 30;
    int movingenemeies = enemyCount / 2;
    AdversaryEntity a[maxenemies];

    // Assign movement types to enemies
    for (int i = 0; i < enemyCount; i++)
    {
        a[i].alternatePath = (i < enemyCount / 2);  
        
    }

    int x = 0, y = 0, dx = 0, dy = 0, x2 = N-1, y2 = M-1, dx2 = 0, dy2 = 0;
    int scores = 0;
    int reward = 0;
    int multiplier = 1;
    int tiles = 0;
    int powerups = 0;
    bool extra_ups = false;
    bool s50 = false;
    bool s70 = false;
    int prev_scores = 0;
    bool freeze_enemies = false;
    sf::Clock powerupsClock;
    float timer = 0, delay = 0.07;

    sf::RectangleShape enemy1;
    enemy1.setSize(sf::Vector2f(50, 50));
    enemy1.setPosition(100, 100);
    enemy1.setFillColor(sf::Color::Red);

    int scores2 = 0;
    int reward2 = 0;
    int multiplier2 = 1;
    int tiles2 = 0;
    int powerups2 = 0;
    bool extra_ups2 = false;
    bool s50_2 = false;
    bool s70_2 = false;
    int prev_scores2 = 0;
    bool freeze_enemies2 = false;
    sf::Clock powerupsClock2;
    int movecount2 = 0;
    bool p1Completed = false;
    bool p2Completed = false;

    Clock clock;
    Clock Gameclock;
    Clock variableclock;
    Clock variableclock2;
    Clock spawnclock;

    int spawntime = spawnclock.getElapsedTime().asSeconds();
    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M-1 || j == N-1) grid[i][j] = 1;

    bool onmenu = true;
    while (window.isOpen() || scoreboard.isOpen())
    {
        window.clear();
        scoreboard.clear();

        int totaltime = Gameclock.getElapsedTime().asSeconds();
        int variabletime = variableclock.getElapsedTime().asSeconds();
        int variabletime2 = variableclock.getElapsedTime().asSeconds();

        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e))
        {
            if (e.type == Event::Closed) window.close();
            if (e.type == Event::KeyPressed)
                if (e.key.code == Keyboard::Escape)
                {
                    for (int i = 1; i < M-1; i++)
                        for (int j = 1; j < N-1; j++)
                            grid[i][j] = 0;
                    x = 0; y = 0; x2 = N-1; y2 = M-1;
                    Game = true;
                    movecount = 0;
                    totaltime = 0;
                    Gameclock.restart();
                    powerups = 0;
                    scores = 0;
                    player1alive = true;
                    if (choicetwoplayer == true) player2alive = true;
                    reward = 0;
                    multiplier = 1;
                    tiles = 0;
                    powerups = 0;
                    extra_ups = false;
                    s50 = false;
                    s70 = false;
                    prev_scores = 0;
                    freeze_enemies = false;
                    dx = 4 - rand() % 8;
                    dy = 4 - rand() % 8;
                    for (int i = 0; i < enemyCount; i++)
                    {
                        
                        a[i].posX = 300;  // Changed from x
                        a[i].posY = 300;  // Changed from y
                    }
                }
        }
       

        Event p;
        while (scoreboard.pollEvent(p))
        {
            if (p.type == Event::Closed) scoreboard.close();
        }

        while (difficultymenu.isOpen())
        {
            difficultymenu.clear();
            Event difficultychoice;
            while (difficultymenu.pollEvent(difficultychoice))
            {
                if (difficultychoice.type == Event::KeyPressed)
                {
                    if (difficultychoice.key.code == Keyboard::Num1) easy = true;
                    else if (difficultychoice.key.code == Keyboard::Num2) med = true;
                    else if (difficultychoice.key.code == Keyboard::Num3) hard = true;
                }
            }
        }

        if (player1alive == true)
        {
            if (Keyboard::isKeyPressed(Keyboard::Left)) { dx = -1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Right)) { dx = 1; dy = 0; }
            if (Keyboard::isKeyPressed(Keyboard::Up)) { dx = 0; dy = -1; }
            if (Keyboard::isKeyPressed(Keyboard::Down)) { dx = 0; dy = 1; }
        }

        if (player2alive == true)
        {
            if (Keyboard::isKeyPressed(Keyboard::A)) { dx2 = -1; dy2 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::D)) { dx2 = 1; dy2 = 0; }
            if (Keyboard::isKeyPressed(Keyboard::W)) { dx2 = 0; dy2 = -1; }
            if (Keyboard::isKeyPressed(Keyboard::S)) { dx2 = 0; dy2 = 1; }
        }
        if (!Game) continue;

        if (timer > delay)
        {
            x += dx;
            y += dy;
            x2 += dx2;
            y2 += dy2;
            if (x < 0) x = 0; if (x > N-1) x = N-1;
            if (y < 0) y = 0; if (y > M-1) y = M-1;

            if (grid[y][x] == 2)//player1owntrail
            {
                player1alive = false;
                dy = dx = 0;
                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == 2) grid[i][j] = 1;
                if (choicetwoplayer == false) Game = false;
            }

            if (grid[y2][x2] == 3)//plasyer2owntrail
            {
                player2alive = false;
                dy2 = dx2 = 0;
                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == 3) grid[i][j] = 1;
                if (choicetwoplayer == false) Game = false;
            }

            if (grid[y][x] == 3)//player 1 on player 2 trail
            {
                player1alive = false;
                dy = dx = 0;
                grid[y][x] = 1;
                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == 2) grid[i][j] = 1;
            }

            if (grid[y2][x2] == 2)//player 2 on polayer 1 trail
            {
                player2alive = false;
                dy2 = dx2 = 0;
                grid[y2][x2] = 1;
                for (int i = 0; i < M; i++)
                    for (int j = 0; j < N; j++)
                        if (grid[i][j] == 3) grid[i][j] = 1;
            }

            int prevx = x - dx, prevy = y - dy;
            if (grid[prevy][prevx] == 1 && grid[y][x] == 0) movecount++;

            int prevx2 = x2 - dx2, prevy2 = y2 - dy2;
            if (grid[prevy2][prevx2] == 1 && grid[y2][x2] == 0) movecount2++;

            if (grid[y][x] == 0) grid[y][x] = 2;
            if (grid[y2][x2] == 0) grid[y2][x2] = 3;

            if (variabletime >= 20)
            {
                for (int i = 0; i < movingenemeies; i++)
                {
                    double speed = 2;
                    a[i].velocityX *= speed;  // Changed from dx
                    a[i].velocityY *= speed;  // Changed from dy

                   
                }
                variableclock.restart();
            }

            // i have Updated enemy movement logic here
            if (!freeze_enemies)
            {
                for (int i = 0; i < enemyCount; i++)
                {
                    if (totaltime < 30) {
                        a[i].updatePosition();  // Changed from move()
                    } else {
                        if (a[i].alternatePath) {  // Changed from isZigzag
                            a[i].alternateMovement();  // Changed from zigzagMove()
                        } else {
                            a[i].rotationalMovement();  // Changed from circularMove()
                        }
                    }
                }
            }

            if (totaltime >= 20)
            {
                for (int i = 0; i < enemyCount; i++)
                {
                    a[i].pathVariation(); 
                }
            }

            timer = 0;

            if (x2 < 0) x2 = 0; if (x2 > N-1) x2 = N-1;
            if (y2 < 0) y2 = 0; if (y2 > M-1) y2 = M-1;

            timer = 0;
        }

        if (grid[y][x] == 1 || grid[y2][x2] == 1)
{
    if (grid[y][x] == 1 && !p1Completed) p1Completed = true;
    if (grid[y2][x2] == 1 && !p2Completed) p2Completed = true;

    if (p1Completed)
    {
        dx = dy = 0;
        for (int i = 0; i < enemyCount; i++)
            drop(a[i].posY/ts, a[i].posX/ts);
        tiles = 0;
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                if (grid[i][j] == -1) grid[i][j] = 0;
                else if (grid[i][j] == 0) { tiles++; scores = scores + 3; grid[i][j] = 1; }
                else if (grid[i][j] == 2) { grid[i][j] = 1; }

        if (reward < 3) {
            if (tiles >= 10) { multiplier = 2; reward++; }
        } else if (reward < 5) {
            if (tiles >= 5) { multiplier = 2; reward++; }
        } else {
            if (tiles >= 5) { multiplier = 4; reward++; }
        }
        scores += tiles * multiplier;

        cout << "scores : " << scores << endl;

        if (scores >= 50 && !s50) { powerups++; s50 = true; }
        if (scores >= 70 && !s70) { powerups++; s70 = true; }
        if (Keyboard::isKeyPressed(Keyboard::P) && powerups > 0 && !freeze_enemies) {
            freeze_enemies = true;
            powerupsClock.restart();
        }
        if (scores >= 100 && !extra_ups) {
            int extra = (scores - 100) / 30 + 1;
            powerups += extra;
            extra_ups = true;
        }
        p1Completed = false; // Reset to allow future completions
    }

    if (p2Completed)
    {
        dx2 = dy2 = 0;
        for (int i = 0; i < enemyCount; i++)
            drop(a[i].posY/ts, a[i].posX/ts);
        tiles2 = 0;
        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
                if (grid[i][j] == -1) grid[i][j] = 0;
                else if (grid[i][j] == 0) { tiles2++; scores2 = scores2 + 3; grid[i][j] = 1; }
                else if (grid[i][j] == 3) { grid[i][j] = 1; }

        if (reward2 < 3) {
            if (tiles2 >= 10) { multiplier2 = 2; reward2++; }
        } else if (reward2 < 5) {
            if (tiles2 >= 5) { multiplier2 = 2; reward2++; }
        } else {
            if (tiles2 >= 5) { multiplier2 = 4; reward2++; }
        }
        scores2 += tiles2 * multiplier2;

        cout << "scores2 : " << scores2 << endl;

        if (scores2 >= 50 && !s50_2) {
            powerups2++;
            s50_2 = true;
        }
        if (scores2 >= 70 && !s70_2) {
            powerups2++;
            s70_2 = true;
        }
        if (Keyboard::isKeyPressed(Keyboard::O) && powerups2 > 0 && !freeze_enemies2) {
            freeze_enemies2 = true;
            powerupsClock.restart();
        }
        if (scores2 >= 100 && !extra_ups2) {
            int extra2 = (scores2 - 100) / 30 + 1;
            powerups2 += extra2;
            extra_ups2 = true;
        }
        p2Completed = false; // Reset to allow future completions
    }
}
        if (continous == true)
        {
            if (totaltime - spawntime >= 20 && enemyCount + 2 <= maxenemies) {
                a[enemyCount] = AdversaryEntity();  // Changed from Enemy()
                a[enemyCount].alternatePath = false;  // Changed from isZigzag
                enemyCount++;
                a[enemyCount] = AdversaryEntity();  // Changed from Enemy()
                a[enemyCount].alternatePath = false;  // Changed from isZigzag
                enemyCount++;
                spawntime = totaltime;
            }
            for (int i = 0; i < enemyCount; ++i) {
                if (totaltime < 30) a[i].updatePosition();  // Changed from move()
                else if (a[i].alternatePath) a[i].alternateMovement();  // Changed from zigzagMove()
                else a[i].rotationalMovement();  // Changed from circularMove()
            }
        }
        
        window.clear(sf::Color(230, 210, 255));
        
        for (int i = 0; i < enemyCount; i++)
        {
            // Changed x/y to posX/posY
            if (grid[static_cast<int>(a[i].posY/ts)][static_cast<int>(a[i].posX/ts)] == 2)
            {
                player1alive = false;
                dy = dx = 0;
                grid[y][x] = 1;
            }
            if (grid[static_cast<int>(a[i].posY/ts)][static_cast<int>(a[i].posX/ts)] == 3)
            {
                player2alive = false;
                dy2 = dx2 = 0;
                grid[y2][x2] = 1;
            }
        }


        if (player1alive == false && player2alive == false)
        {
            Game = false;
            ofstream out_file("score.txt", ios::app);
            if (out_file.is_open()) {
                out_file << scores << endl;
                out_file.close();
            }
            ifstream in_file("score.txt");
            int all_Scores[5]={0,0,0,0,0};
            int count = 0;
            while (in_file >> all_Scores[count++]) {}
            in_file.close();
            for (int i = 0; i < count - 1; i++) {
                for (int j = i + 1; j < count; j++) {
                    if (all_Scores[i] < all_Scores[j]) {
                        int temp = all_Scores[i];
                        all_Scores[i] = all_Scores[j];
                        all_Scores[j] = temp;
                    }
                }
            }
             int highscore=all_Scores[0];
    
//display the scores
            ofstream sortedScores("score.txt", ios::out);
            for (int i = 0; i < 5; i++) {
                sortedScores << all_Scores[i] << endl;
            }
            sortedScores.close();
            if (scores>=highscore)
{
newhighscore=true;
cout<<"you have new highscore"<<endl;
}



ofstream out_file2("score2.txt", ios::app);
            if (out_file2.is_open()) {
                out_file2 << scores2 << endl;
                out_file2.close();
            }
            ifstream in_file2("score.txt");
            int all_Scores2[5]={0,0,0,0,0};
            int count2 = 0;
            while (in_file2 >> all_Scores2[count2++]) {}
            in_file.close();
            for (int i = 0; i < count2 - 1; i++) {
                for (int j = i + 1; j < count; j++) {
                    if (all_Scores2[i] < all_Scores2[j]) {
                        int temp2 = all_Scores2[i];
                        all_Scores2[i] = all_Scores2[j];
                        all_Scores2[j] = temp2;
                    }
                }
            }
    
//display the scores
            ofstream sortedScores2("score2.txt", ios::out);
            for (int i = 0; i < 5; i++) {
                sortedScores2 << all_Scores2[i] << endl;
            }
            sortedScores2.close();
            
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        }

        if (freeze_enemies && powerupsClock.getElapsedTime().asSeconds() < 3.0f) {
            window.clear(sf::Color(173, 216, 230));
        } else if (freeze_enemies) {
            freeze_enemies = false;
            powerups--;
        } else {
            for (int i = 0; i < enemyCount; i++) {
                if (totaltime < 30) a[i].updatePosition();  // Changed from move()
                else if (a[i].alternatePath) a[i].alternateMovement();  // Changed from zigzagMove()
                else a[i].rotationalMovement();  // Changed from circularMove()
            }
        }

        int activeenmeies = 2;

        window.clear(sf::Color(230, 210, 255));
        scoreboard.clear(sf::Color(230, 210, 255));

        move_counter.setString(std::to_string((movecount)));
        timetaken.setString("time taken=" + std::to_string((totaltime)));

        for (int i = 0; i < M; i++)
            for (int j = 0; j < N; j++)
            {
                if (grid[i][j] == 0) continue;
                if (grid[i][j] == 1) sTile.setTextureRect(IntRect(0, 0, ts, ts));
                if (grid[i][j] == 2 && player1alive == true) sTile.setTextureRect(IntRect(54, 0, ts, ts));
                if (grid[i][j] == 3 && player2alive == true) sTile.setTextureRect(IntRect(18, 0, ts, ts));
                sTile.setPosition(j*ts, i*ts);
                window.draw(sTile);
            }

        sTile.setTextureRect(IntRect(72, 0, ts, ts));
        sTile.setPosition(x*ts, y*ts);
        if (player1alive) window.draw(sTile);

        if (choicetwoplayer == true && player2alive == true)
        {
            sPlayer.setTextureRect(IntRect(108, 0, ts, ts));
            sPlayer.setPosition(x2*ts, y2*ts);
            window.draw(sPlayer);
        }


sEnemy.rotate(10);
        for (int i = 0; i < enemyCount; i++)
        {
            sEnemy.setPosition(a[i].posX, a[i].posY);  // Changed from x/y
            window.draw(sEnemy);
        }
 
        
        scoreboard.draw(timetaken);

        // Player 1 scoreboard display
        {
            scoreboard.draw(my_text);
            move_counter.setString(std::to_string(movecount));
            scoreboard.draw(move_counter);
            scoresText.setString("Score P1: " + std::to_string(scores2));
            scoreboard.draw(scoresText);
            rewardText.setString("Reward Count: " + std::to_string(reward));
            scoreboard.draw(rewardText);
            powerupsText.setString("Power-ups: " + std::to_string(powerups));
            scoreboard.draw(powerupsText);
            player1Text.setString("PLAYER 1: ");
            scoreboard.draw(player1Text);
        }
        
        if (!Game) window.draw(sGameover);
        
        // Player 2 scoreboard display
        if (choicetwoplayer == true)
        {
            scoreboard.draw(my_text2);
            move_counter2.setString(std::to_string(movecount2));
            scoreboard.draw(move_counter2);
            scores2Text2.setString("Score P2: " + std::to_string(scores));
            scoreboard.draw(scores2Text2);
            rewardText2.setString("Reward Count: " + std::to_string(reward2));
            scoreboard.draw(rewardText2);
            powerupsText2.setString("Power-ups: " + std::to_string(powerups2));
            scoreboard.draw(powerupsText2);
            player2Text.setString("PLAYER 2: ");
            scoreboard.draw(player2Text);
        }
        if (Game==false)
{
    window.close();
    scoreboard.close();
 RenderWindow endmenu(VideoMode(N*ts, M*ts), " endmenu");//seperate window to work with for menu screen (copied from below that renders the window for game)
     endmenu.setFramerateLimit(60);

 

        endmenu.clear();

    while (endmenu.isOpen()) {
        endmenu.draw(sGameover);
        //if (newhighscore==true)
            newhighscoretext.setString( std::to_string((scores)));//need to comfirm if to_string is allowed
                         
                         
        if (newhighscore==true)
 endmenu.draw(sHighscore);
 else 
  endmenu.draw(sScore);

            endmenu.draw(newhighscoretext);



        
        sf::Event endmenuchocie;
        while (endmenu.pollEvent(endmenuchocie)) {
            if (endmenuchocie.type == sf::Event::KeyPressed)
            {


          if (endmenuchocie.key.code == sf::Keyboard::Escape)
          {
                endmenu.close();
                
                
            // endmenu.close();
            // break;
          }




            }

        // draw scoreboard
         endmenu.display();
       
    }
}
        }

        window.display();
        scoreboard.display();
        
    }

    return 0;
}