#include "graphics.h"

using namespace std;
using namespace mssm;

class Ship{
public:
    Vec2d position;
    Vec2d velocity;
    double angle;
    int lives;

    double radius = 25;
    void draw(Graphics& g, Vec2d position, double scale, double angle);
    void update(Graphics& g);

};

class Asteroid{
public:
    Color color;
    Vec2d velocity;
    Vec2d position;
    double scale;
    double angle;
    int split;

    void draw(Graphics& g, Vec2d position, double scale, double angle);
    void update(Graphics& g);
    double radius();
    //  void break(Graphics& g, )
};

class Bullet{
public:
    Vec2d velocity;
    Vec2d position;

    double scale;
    double radius = 10;

    void draw(Graphics& g, Vec2d position);
    void update(Graphics& g);
    bool hit(Asteroid asteroid);
};

double vecDist(Vec2d a, Vec2d b){
    double xDist = (a.x - b.x) * (a.x - b.x);
    double yDist = (a.y - b.y) * (a.y - b.y);
    double dist = xDist + yDist;
    return sqrt(dist);
}

void welcome(Graphics& g)
{
    while (g.draw()) {
        if (!g.isDrawable()) {
            continue;
        }

        Vec2d center{g.width()/2, g.height()/2};

        g.text(center, 30, "Welcome! Press Enter to Start", YELLOW, HAlign::center);

        if (g.onKeyPress(Key::ENTER)) {
            return;
        }
    }
}

void bye(Graphics& g)
{
   while (g.draw()) {
        if (!g.isDrawable()) {
           continue;
       }

      Vec2d center{g.width()/2, g.height()/2};

       g.text(center, 30, "Game over:( ---> Press ENTER to Play Again", YELLOW, HAlign::center);

       if (g.onKeyPress(Key::ENTER)) {
           return;
       }
    }
}

void Ship::draw(Graphics& g, Vec2d position, double scale, double angle){
    Array<Vec2d> pts{ {-50, -50}, {50, -50}, {0, 50} };

    for (int i = 0; i < pts.size(); i++) {
        pts[i].rotate(angle-1.5708);
        pts[i] *= scale;     // scale the points
        pts[i] += position;  // translate the points by position
    }

    g.polygon(pts, GREEN, BLUE);
}

void Asteroid::draw(Graphics& g, Vec2d position, double scale, double angle){
    Array<Vec2d> pts {{5,15}, {15,25}, {26,19}, {31,17}, {30,5}, {21.2,1.5}, {11,-3}, {3,5}};

    for (int i = 0; i < pts.size(); i++){
        pts[i] += {-15,-10};
        pts[i].rotate(angle);
        pts[i] *= scale;
        pts[i] += position;
    }
    g.polygon(pts, WHITE, GREY);
    // g.ellipse(position, )
    // g.ellipse(position,radius(),radius()); use later when diffeent sizes
}

double Asteroid::radius()
{
    return 50;    //make smarter
}

void Bullet::draw(Graphics& g, Vec2d position){
    g.ellipse(position, 20, 20, GREEN, GREEN);
}

bool Bullet::hit(Asteroid asteroid){
    double dist = vecDist(asteroid.position, position);
    if (10 + asteroid.radius() >= dist){
        return true;
    }
    else{
        return false;
    }
}

void Ship::update(Graphics& g){  //ship key controls, figure out how to get nose to point in correct direction


    if (g.isKeyPressed(Key::Left)){
        angle = angle - 0.05;
    }
    if (g.isKeyPressed(Key::Right)){
        angle = angle + 0.05;
    }

    Vec2d acceleration;
    if (g.isKeyPressed(Key::Up)){

        acceleration = {0.1 * cos(angle), 0.1 * sin(angle)};
    }
    position = position + velocity;
    velocity = velocity + acceleration;

    //wrap ship around screen
    if (position.x > g.width()){
        position.x = 1;
    }
    if (position.x < 0){
        position.x = g.width();
    }
    if (position.y > g.height()){
        position.y = 1;
    }
    if (position.y < 0){
        position.y = g.height();
    }
}


void Asteroid::update(Graphics& g){
    position += velocity;
    if (position.x < 0) {
        position.x = g.width();

    }

    if (position.y < 0) {
        position.y = g.height();

    }

    if (position.x > g.width()) {
        position.x = 0;

    }

    if (position.y > g.height()) {
        position.y = 0;

    }
}

void Bullet::update(Graphics& g){
    position = position + velocity; //keeps bullet moving
}

//add score in top left
//add 100 when bullet hits asteroid
int main()
{
    Graphics g("Testing", 1024, 768);
    welcome(g);

    int score = 0;
    Ship ship;
    ship.position = {200,200};
    ship.angle = 0;
    ship.velocity = {0,0};
    ship.lives = 3;

    Array<Asteroid> asteroids;
//creating asteroids
    for (int i = 0; i < 12; i++){
        Asteroid asteroid;
        asteroid.position = {i * 100, 30};
        asteroid.color = GREY;
        asteroid.velocity = {randomDouble(-2, 2), randomDouble(-2, 2)};
        asteroid.angle = 0;
        asteroid.split = 0;
        asteroid.scale = 3.5;

        asteroids.append(asteroid);
    }
//creating bullets
    Array<Bullet> bullets;

    while (g.draw()) {
        if (!g.isDrawable()) {
            continue;
        }

        g.text({10,50}, 50, "Lives:" + to_string(ship.lives), WHITE);
        g.text({g.width()-300, 50}, 50, "Score:" + to_string(score), WHITE);
        //loop for asteroid collisons with ship
        for (int i = 0; i < asteroids.size(); i++){


            double d = vecDist(ship.position, asteroids[i].position);
            double r1= asteroids[i].radius();
            double r2 = ship.radius;

            if (d< r1+r2){

                g.text({500,500}, 100, "Boom", WHITE, HAlign::center);
                ship.lives = ship.lives - 1;
                ship.position = {g.width()/2, g.height()/2};
                for (int k = 0; k < asteroids.size(); k++){
                    asteroids[k].position = {k*100, 0};


                }
//move asteroids to outside of the screen


                break;
            }


            //if asteroid hits ship game over screen
            //ship.postion
            //aestroid[i].postiton
            //3 lives, lose one when hit and reset ship position to center screen
        }

        if (ship.lives <= 0){
            bye(g);
            welcome(g);
            score = 0;
            ship.position = {200,200};
            ship.angle = 0;
            ship.velocity = {0,0};
            ship.lives = 3;
            asteroids.clear();
            for (int i = 0; i < 12; i++){
                Asteroid asteroid;
                asteroid.position = {i * 100, 30};
                asteroid.color = GREY;
                asteroid.velocity = {randomDouble(-2, 2), randomDouble(-2, 2)};
                asteroid.angle = 0;
                asteroid.split = 0;
                asteroid.scale = 3.5;

                asteroids.append(asteroid);
            }
        }

        for (int i = 0; i < asteroids.size(); i++){
            asteroids[i].draw(g, asteroids[i].position, asteroids[i].scale, 0.5);
            asteroids[i].update(g);
        }

        ship.draw(g, ship.position, 0.25, ship.angle);
        ship.update(g);

        if (g.onKeyPress(Key::Space)){
            Bullet b;
            b.position = ship.position;
            b.velocity = {10 * cos(ship.angle), 10 * sin(ship.angle)};
            bullets.append(b);

        }
        for (int i = 0; i < bullets.size(); i++){

            for (int j = 0; j < asteroids.size(); j++){

                if (bullets[i].hit(asteroids[j])){
                    bullets.removeAtIndex(i);
                    score = score + 100;

                    if (asteroids[j].split == 0){
                        asteroids[j].scale = 1.75;
                        asteroids[j].split = 1;
                        Asteroid newA;
                        newA.scale = 1.75;
                        newA.velocity = {randomDouble(-2, 2), randomDouble(-2, 2)};
                        newA.position = asteroids[j].position;
                        newA.angle = 0;
                        newA.split = 1;
                        asteroids.append(newA);
                    }
                    else if (asteroids[j].split == 1){
                        asteroids.removeAtIndex(j);
                    }

                    break;
                }



            }

        }

        for (int i = 0; i < bullets.size(); i++){
            bullets[i].draw(g, bullets[i].position);
            bullets[i].update(g);
            if (bullets[i].position.x < 0 || bullets[i].position.x > g.width() || bullets[i].position.y < 0 || bullets[i].position.y > g.height()){

                bullets.removeAtIndex(i);

                if (bullets.size() == 0){
                    i = 0;
                }
                else{
                    i--;
                }

            }

        }


    }

    return 0;
}


//int lives= 3;
//if (lives <= 0){ //print loss screen
//    bye(g); //"gameover" function
//    lives = 3; //reset losses
