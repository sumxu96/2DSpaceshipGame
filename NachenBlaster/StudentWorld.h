#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include "GameConstants.h"
#include "Actor.h"
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;


// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetDir);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    // If there's at least one alien that's collided with a, return
    // a pointer to one of them; otherwise, return a null pointer.
    Actor* getOneCollidingAlien(Actor* a) const;
    
    // If the player has collided with a, return a pointer to the player;
    // otherwise, return a null pointer.
    NachenBlaster* getCollidingPlayer(const Actor* a) const;
    
    // Is the player in the line of fire of a, which might cause a to attack?
    bool playerInLineOfFire(const Actor* a) const;
    
    // Add an actor to the world.
    void addActor(Actor* a, bool projectile = false);
    
    void updateDisplayText();
    
    void removeDead();
    
    std::string setDisplayText();
    
    int totalAliensDestroyedPerLevel() const;
    
    int getXpos() const;
    
    int getYpos() const;
 
    int getMaxAllowedAliens() const;
    // Record that one more alien on the current level has been destroyed.
    void recordAlienDestroyed();

private:
    int level;
    int score;
    int destroyed;
    int ticks;
    std::vector<Actor*> m_actors;
    std::vector<Actor*> m_projectiles;
    NachenBlaster* m_NachenBlaster;
};

#endif // STUDENTWORLD_H_
