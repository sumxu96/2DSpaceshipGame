#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>
using namespace std;

double euclidean_dist(double x1, double y1, double x2, double y2)
{
    double x = x1 - x2; //calculating number to square in next step
    double y = y1 - y2;
    double dist;
    
    dist = pow(x, 2) + pow(y, 2);       //calculating Euclidean distance
    dist = sqrt(dist);
    
    return dist;
}

bool checkCollision(Actor*a, Actor*b)
{
    if (euclidean_dist(a->getX(), a->getY(), b->getX(), b->getY()) < 0.75*(a->getRadius() + b->getRadius()))
        return true;
    
    else return false;
}

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

StudentWorld::StudentWorld(string assetDir)
: GameWorld(assetDir)
{
    m_NachenBlaster = nullptr;
    getLevel();
    destroyed = 0;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    m_NachenBlaster = new NachenBlaster(this);
    destroyed = 0;
    
    for (int i = 0; i < 30; i++)
    {
        m_actors.push_back(new Star(randInt(0, VIEW_WIDTH-1), randInt(0, VIEW_HEIGHT-1), this));
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setGameStatText(setDisplayText());
    
    if (randInt(0, 14) == 0)
    {
        addActor(new Star(VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1), this));
    }
    
    m_NachenBlaster->doSomething();

    int count = 0;
    int remaining = totalAliensDestroyedPerLevel() - destroyed;
    
    for (vector <Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        
        if ((*it)->getType() == SMALLGON || (*it)->getType() == SMOREGON || (*it)->getType() == SNAGGLEGON)
            count++;
    }

    double S1 = 60;
    double S2 = 20 + (getLevel()*5);
    double S3 = 5 + (getLevel()*10);
    double S = S1+S2+S3;

    double smallgon = fRand(0,1);
    double smoregon = fRand(0,1);
    double snagglegon = fRand(0,1);

    int limit = min(getMaxAllowedAliens(), remaining);
    if (smallgon <= (S1/S) && count < limit)
    {
        addActor(new Smallgon(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
        count++;
    }
    if (smoregon <= (S2/S) && count < limit)
    {
        addActor(new Smoregon(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
        count++;
    }
    if (snagglegon <= (S3/S) && count < limit)
    {
        addActor(new Snagglegon(this, VIEW_WIDTH-1, randInt(0, VIEW_HEIGHT-1)));
        count++;
    }
    
    for(vector <Actor*>::iterator it = m_projectiles.begin(); it != m_projectiles.end(); it++)
    {
        (*it)->doSomething();
    }
    
    for(vector <Actor*>::iterator it = m_actors.begin(); it != m_actors.end(); it++)
    {
        (*it)->doSomething();
        for (vector <Actor*>::iterator it_p = m_projectiles.begin(); it_p != m_projectiles.end(); it_p++)
        {
            if(checkCollision((*it), (*it_p)))
                (*it_p)->collide((*it));

        }

        if (checkCollision(m_NachenBlaster, (*it)))
            m_NachenBlaster->collide(*it);
    }
    
    removeDead();
    
    if (m_NachenBlaster->isDead())
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    if (destroyed >= totalAliensDestroyedPerLevel())
        return GWSTATUS_FINISHED_LEVEL;
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::removeDead()
{
    for (int i = m_actors.size()-1; i>=0; i--)
    {
        if(m_actors[i]->isDead())
        {
            delete m_actors[i];
            m_actors.erase(m_actors.begin()+i);
        }
    }
    for (int i = m_projectiles.size()-1;i>=0; i--)
    {
        if (m_projectiles[i]->isDead())
        {
            delete m_projectiles[i];
            m_projectiles.erase(m_projectiles.begin()+i);
        }
    }
}

void StudentWorld::recordAlienDestroyed()
{
    destroyed++;
}

std::string StudentWorld::setDisplayText()
{
    ostringstream oss;
    oss << setw(5) << "Lives: " << getLives() << "  " << "Health: " << m_NachenBlaster->healthPct() << "%" << "  " << "Score: " << getScore() << "  " << "Level: " << getLevel() << "  " << "Cabbages: " << m_NachenBlaster->cabbagePct() << "%" << "  " << "Torpedoes: " << m_NachenBlaster->numTorpedoes();
    string s = oss.str();
    return s;
}

void StudentWorld::addActor(Actor* a, bool projectile)
{
    if (projectile)
        m_projectiles.push_back(a);
    else
    m_actors.push_back(a);
}

int StudentWorld::totalAliensDestroyedPerLevel() const
{
    return (6+(4*getLevel()));
}

int StudentWorld::getMaxAllowedAliens() const
{
    return (4+(.5*getLevel()));
}

int StudentWorld::getXpos() const
{
    return m_NachenBlaster->getX();
}

int StudentWorld::getYpos() const
{
    return m_NachenBlaster->getY();
}

void StudentWorld::cleanUp()
{
    vector <Actor*>::iterator it;
    it = m_actors.begin();
    while (it != m_actors.end())
    {
        delete (*it);
        it++;
    }
    
    vector <Actor*>::iterator itp;
    itp = m_projectiles.begin();
    while (it != m_projectiles.end())
    {
        delete (*itp);
        itp++;
    }
    
    delete m_NachenBlaster;
}

