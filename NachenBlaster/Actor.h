#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <cmath>
using namespace std;

const int HIT_BY_SHIP = 0;
const int HIT_BY_PROJECTILE = 1;

class StudentWorld;
class GameWorld;

enum type
{
    STAR,
    EXPLOSION,
    NACHENBLASTER,
    SMALLGON,
    SMOREGON,
    SNAGGLEGON,
    CABBAGE,
    TURNIP,
    PLAYERLAUNCHEDTORPEDO,
    ALIENLAUNCHEDTORPEDO,
    EXTRALIFEGOODIE,
    REPAIRGOODIE,
    FLATULENCETORPEDOGOODIE
};

//create class Actor
class Actor: public GraphObject
{
protected:
    type e;
public:
    Actor(type e, int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* world);
    
    type getType()
    {
        return e;
    }
    
    virtual ~Actor() {};
    virtual void doSomething() = 0;
    virtual void collide(Actor *other) {}
    // This actor suffers an amount of damage caused by being hit by either
    // a ship or a projectile (see constants above).
//    virtual void sufferDamage(double amt, int cause) = 0;
    
    //check for collision
//    bool checkCollision(Actor*a);
   
    // Is this actor dead?
    bool isDead() const;
    
    // Is this actor an alien?
    virtual bool isAlien() const;
    
    // Mark this actor as dead.
    void setDead();
    
    StudentWorld* getWorld() const;
private:
    StudentWorld* m_world;
    bool m_dead;
};

//create class Star
class Star: public Actor
{
public:
    Star(double x, double y, StudentWorld* world);
    virtual void doSomething();
private:
};

class DamageableObject : public Actor
{
public:
    DamageableObject(type e, int imageID, double startX, double startY, int startDir, double size, int depth, double hitPoints, StudentWorld* world);
    
    // How many hit points does this actor have left?
    double hitPoints() const;
    
    // Increase this actor's hit points by amt.
    virtual void increaseHitPoints(double amt);
    
    virtual void sufferDamage(double amt, int cause) = 0;
    
private:
    double m_HP;
};

//create class NachenBlaster
class NachenBlaster: public DamageableObject
{
public:
    NachenBlaster(StudentWorld* world);
    virtual void doSomething();
    virtual void increaseHitPoints(double amt);
    virtual void sufferDamage(double amt, int cause);
    virtual void collide(Actor *other);
    
    // Incease the number of torpedoes the player has by amt.
    void increaseTorpedoes(int amt);
    
    // Get the player's health percentage.
    int healthPct() const;
    
    // Get the player's cabbage energy percentage.
    int cabbagePct() const;
    
    // Get the number of torpedoes the player has.
    int numTorpedoes() const;
    
private:
    StudentWorld* m_world;
    double m_HP;
    double m_energy;
    int m_Torpedoes;
};

//create class Explosion
class Explosion: public Actor
{
public:
    Explosion(StudentWorld* world, double X, double Y);
    virtual void doSomething();
//    virtual ~Explosion();
};


class Alien : public DamageableObject
{
public:
    Alien(type e, StudentWorld* world, double startX, double startY, int imageID,
          double hitPoints, double damageAmt, double deltaX,
          double deltaY, double speed, unsigned int scoreValue);
    virtual bool isAlien() const;
    virtual void sufferDamage(double amt, int cause);
    
    void alienDoSomething();
    
    // Move the player by the current speed in the direction indicated
    // by the x and y deltas.
    void move();
    
    // Set the player's y direction.
    void setDeltaY(double dy);
    
    double getDeltaY() const;
    
    
    // Set the player's speed.
    void setSpeed(double speed);
    
    double getSpeed() const;
    
    void setLength(int l);
    
    int getLength() const;
    
    // If this alien drops goodies, drop one with the appropriate probability.
    virtual void possiblyDropGoodie() = 0;
    
private:
    double m_HP;
    double m_speed;
    double m_deltaY;
    double m_length;
};

//create Smallgon
class Smallgon: public Alien
{
public:
    Smallgon(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
    virtual void possiblyDropGoodie() {};
};

//create Smoregon
class Smoregon: public Alien
{
public:
    Smoregon(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
    virtual void possiblyDropGoodie();
};

//create Snagglegon
class Snagglegon: public Alien
{
public:
    Snagglegon(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
    virtual void possiblyDropGoodie();
};

//create class Projectile
class Projectile: public Actor
{
public:
    Projectile(type e, StudentWorld* world, double startX, double startY, int imageID,
               double damageAmt, double deltaX, bool rotates, int imageDir);
};

//create class Cabbage
class Cabbage: public Projectile
{
public:
    Cabbage(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
    virtual void collide(Actor* other);
};

//create class Turnip
class Turnip: public Projectile
{
public:
    Turnip(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
};

//create Flatulence Torpedo
class flatulenceTorpedo: public Projectile
{
public:
    flatulenceTorpedo(type e, StudentWorld* world, double startX, double startY, double deltaX,
                      int imageDir);
};

class PlayerLaunchedTorpedo : public flatulenceTorpedo
{
public:
    PlayerLaunchedTorpedo(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
    virtual void collide(Actor* other);
};

class AlienLaunchedTorpedo : public flatulenceTorpedo
{
public:
    AlienLaunchedTorpedo(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
};


class Goodie: public Actor
{
public:
    Goodie(type e, StudentWorld* world, double startX, double startY, int imageID);
    void doSomethingCheck();
};

//create Extra Life Goodie
class extraLifeGoodie: public Goodie
{
public:
    extraLifeGoodie(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
};

//create Repair Goodie
class repairGoodie: public Goodie
{
public:
    repairGoodie(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
};

//create Flatulence Torpedo Goodie
class flatulenceTorpedoGoodie: public Goodie
{
public:
    flatulenceTorpedoGoodie(StudentWorld* world, double startX, double startY);
    virtual void doSomething();
};


inline
double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

#endif // ACTOR_H_
