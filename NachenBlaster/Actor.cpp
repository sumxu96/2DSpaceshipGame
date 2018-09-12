#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
using namespace std;

class StudentWorld;

Actor::Actor(type e, int imageID, double startX, double startY, int dir, double size, int depth, StudentWorld* world): GraphObject(imageID, startX, startY, dir, size, depth), e(e)
{
    m_world = world;
    depth = 1;
}

bool Actor::isAlien() const
{
    return false;
}

bool Actor::isDead() const
{
    return m_dead;
}

void Actor::setDead()
{
    m_dead = true;
}

StudentWorld* Actor::getWorld() const
{
    return m_world;
}

//implement class Star
Star::Star(double x, double y, StudentWorld* world): Actor(STAR, IID_STAR, (x == VIEW_WIDTH-1 ? x:randInt(0, VIEW_WIDTH-1)), randInt(0, VIEW_HEIGHT-1), 0, fRand(0.05,0.50), 3, world)
{}

void Star::doSomething()
{
    moveTo(getX()-1, getY());
    if (getX() < 0)
        setDead();
}

//implement class DamageableObject
DamageableObject::DamageableObject(type e, int imageID, double startX, double startY, int startDir, double size, int depth, double hitPoints, StudentWorld* world): Actor(e, imageID, startX, startY, startDir, size, depth, world)
{
    m_HP = hitPoints;
}

double DamageableObject::hitPoints() const
{
    return m_HP;
}

void DamageableObject::increaseHitPoints(double amt)
{
    m_HP = m_HP + amt;
}

void DamageableObject::sufferDamage(double amt, int cause)
{
    if (cause == HIT_BY_SHIP)
        m_HP = m_HP - amt;
    if (cause == HIT_BY_PROJECTILE)
        m_HP = m_HP - amt;
}

//implement class NachenBlaster
NachenBlaster::NachenBlaster(StudentWorld* world):DamageableObject(NACHENBLASTER, IID_NACHENBLASTER, 0, 128, 0, 1.0, 0, 50, world)
{
    m_HP = 50;
    m_energy = 30;
    m_Torpedoes = 0;
    
    if (m_energy >= 30)
        m_energy = 30;
    
    if (m_HP >= 50)
        m_HP = 50;
}

void NachenBlaster::collide(Actor* other)
{
    switch(other->getType())
    {
        case SMALLGON:
            sufferDamage(5, HIT_BY_SHIP);
            other->setDead();
            getWorld()->playSound(SOUND_DEATH);
            getWorld()->increaseScore(250);
            getWorld()->recordAlienDestroyed();
//            getWorld()->addActor(new Explosion(m_world, other->getX(), other->getY()));
            break;
        case SMOREGON:
            sufferDamage(5, HIT_BY_SHIP);
            other->setDead();
            getWorld()->playSound(SOUND_DEATH);
            getWorld()->increaseScore(250);
            getWorld()->recordAlienDestroyed();
//            getWorld()->addActor(new Explosion(m_world, other->getX(), other->getY()));
            if (randInt(0,2) == 0)
            {
                if (randInt(0,1) == 0)
                    getWorld()->addActor(new repairGoodie(m_world, other->getX(), other->getY()));
                else getWorld()->addActor(new flatulenceTorpedoGoodie(m_world, other->getX(), other->getY()));
            }
            break;
        case SNAGGLEGON:
            sufferDamage(15, HIT_BY_SHIP);
            other->setDead();
            getWorld()->playSound(SOUND_DEATH);
            getWorld()->increaseScore(1000);
            getWorld()->recordAlienDestroyed();
//            getWorld()->addActor (new Explosion(m_world, other->getX(), other->getY()));
            if (randInt(0,5) == 0)
            {
                getWorld()->addActor(new extraLifeGoodie(m_world, other->getX(), other->getY()));
            }
            break;
        case TURNIP:
            sufferDamage(2, HIT_BY_PROJECTILE);
            getWorld()->playSound(SOUND_BLAST);
            other->setDead();
            break;
        case ALIENLAUNCHEDTORPEDO:
            sufferDamage(8, HIT_BY_PROJECTILE);
            getWorld()->playSound(SOUND_BLAST);
            other->setDead();
            break;
        case REPAIRGOODIE:
            getWorld()->increaseScore(100);
            other->setDead();
            getWorld()->playSound(SOUND_GOODIE);
            increaseHitPoints(10);
            break;
        case EXTRALIFEGOODIE:
            getWorld()->increaseScore(100);
            other->setDead();
            getWorld()->playSound(SOUND_GOODIE);
            getWorld()->incLives();
            break;
        case FLATULENCETORPEDOGOODIE:
            getWorld()->increaseScore(100);
            other->setDead();
            getWorld()->playSound(SOUND_GOODIE);
            increaseTorpedoes(5);
            break;
    }
}
void NachenBlaster::doSomething()
{
    if(isDead())
        return;
    int ch;
    if(getWorld()->getKey(ch))
    {
        switch(ch)
        {
            case KEY_PRESS_SPACE:
                if (m_energy > 0)
                {
                getWorld()->addActor(new Cabbage(m_world, getX()+12, getY()), true);
                getWorld()->playSound(SOUND_PLAYER_SHOOT);
                m_energy = m_energy - 5;
                }
                break;
            case KEY_PRESS_TAB:
                if (numTorpedoes() > 0)
                {
                    getWorld()->addActor(new PlayerLaunchedTorpedo(m_world, getX()+12, getY()), true);
                    m_Torpedoes = m_Torpedoes - 1;
                    getWorld()->playSound(SOUND_TORPEDO);
                }
                break;
            case KEY_PRESS_LEFT:
                if(getX() > 0)
                    moveTo(getX()-6,getY());
                break;
            case KEY_PRESS_RIGHT:
                if(getX() < VIEW_WIDTH-1)
                    moveTo(getX()+6,getY());
                break;
            case KEY_PRESS_UP:
                if(getY() < VIEW_HEIGHT-1)
                    moveTo(getX(),getY()+6);
                break;
            case KEY_PRESS_DOWN:
                if(getY() > 0)
                    moveTo(getX(),getY()-6);
                break;
        }
    }
   
    if (m_energy > 30)
    {
        m_energy = 30;
    }
    
    
    if (m_energy <= 0)
    {
        m_energy = 0;
    }
    
    m_energy++;
    if (m_HP >= 50)
        m_HP = 50;
    
    
    if (m_HP <= 0)
        setDead();
}

void NachenBlaster::increaseHitPoints(double amt)
{
    m_HP = m_HP + amt;
    if (m_HP > 50)
        m_HP = 50;
}

void NachenBlaster::sufferDamage(double amt, int cause)
{
    if (cause == HIT_BY_SHIP || cause == HIT_BY_PROJECTILE)
        m_HP -= amt;
    if (m_HP <= 0)
        setDead();
    
//todo
//turnip -2hp
//alien flatulence torpedo -8hp
//smallgon collision -5hp
//smoregon collision -5hp
//snagglegon collision -15hp
}

void NachenBlaster::increaseTorpedoes(int amt)
{
    m_Torpedoes = m_Torpedoes + amt;
}

int NachenBlaster::healthPct() const
{
    return (m_HP/50.0)*100;
}

int NachenBlaster::cabbagePct() const
{
    return (m_energy/30.0)*100;
}

int NachenBlaster::numTorpedoes() const
{
    return m_Torpedoes;
}

Explosion::Explosion(StudentWorld*world, double X, double Y): Actor(EXPLOSION, IID_EXPLOSION, 0, 0, 0, 1, 0, world)
{}

void Explosion::doSomething()
{
    //increase size *1.5
    //after 4 ticks, set to dead, destroy, and remove (do in StudentWorld class?)
}

Alien::Alien(type e, StudentWorld* world, double startX, double startY, int imageID,
             double hitPoints, double damageAmt, double deltaX,
             double deltaY, double speed, unsigned int scoreValue)
:DamageableObject(e, imageID, VIEW_WIDTH, randInt(0, VIEW_HEIGHT-1), 0, 0, 0, hitPoints, world)
{
    m_HP = hitPoints;
    m_speed = speed;
    m_length = 0;
    m_deltaY = deltaY;
}

bool Alien::isAlien() const
{
    return true;
}

void Alien::setSpeed(double speed)
{
    m_speed = speed;
}

double Alien::getSpeed() const
{
    return m_speed;
}
void Alien::sufferDamage(double amt, int cause)
{
    m_HP = m_HP - amt;
    if (m_HP<=0)
    {
        setDead();
        getWorld()->playSound(SOUND_DEATH);
        getWorld()->increaseScore(250);
        getWorld()->recordAlienDestroyed();

    }
}

void Alien::setLength(int l)
{
    m_length = l;
}

int Alien::getLength() const
{
    return m_length;
}


void Alien::setDeltaY(double dy)
{
    m_deltaY = dy;
}

double Alien::getDeltaY() const
{
    return m_deltaY;
}

void Alien::alienDoSomething()
{
    if (isDead())
        return;
    if (getX() < 0)
        setDead();
}

void Alien::move()
{
    //if (traveldirection left)
    //        moveTo(getX()-2, getY());
    if (getDeltaY() == 0)
    {
        moveTo(getX()-getSpeed(), getY());
    }
    //    if (traveldirection up and left)
    //    moveTo(getX()-2, getY()+2);
    else if (getDeltaY() == 1)
    {
        moveTo(getX()-getSpeed(), getY()+getSpeed());
    }
    //    else if (traveldirection down and left)
    //    moveTo(getX()-2, getY()-2);
    else if (getDeltaY() == -1)
    {
        moveTo(getX()-getSpeed(), getY()-getSpeed());
    }
    
    // if top of screen, move down and left
    if (getY() >= VIEW_HEIGHT-1)
    {
        moveTo(getX()-getSpeed(), getY()-getSpeed());
        setDeltaY(-1);
    }
    // if bottom of screen, move up and left
    else if (getY() <= 0)
    {
        moveTo(getX()-getSpeed(), getY()+getSpeed());
        setDeltaY(1);
    }
    
    setLength(getLength()-1);
    
}

//void Alien::possiblyDropGoodie()
//{
//    //Smoregon 1/3 chance drop goodie (50/50 repair, torpedo)
//    //Snagglegon 1/6 chance drop extralife goodie
//}

//hp is 5 * (1 + (current_level_number – 1) * .1), direction 0, size 1.5, depth 1
Smallgon::Smallgon(StudentWorld* world, double startX, double startY): Alien(SMALLGON, world, startX, startY, IID_SMALLGON, 5*(1+(world->getLevel()-1)*0.1), 5, 0, 0, 2.0, 250)
{
    setSize(1.5);
    setDirection(0);
}

void Smallgon::doSomething()
{
    alienDoSomething();
    
    if (getLength() == 0)
    {
        double dy = randInt(-1, 1);
        setDeltaY(dy);
        //set direction to getX()-speed, getY()+(speed*dy);
        int n = randInt(1,32);
        setLength(n);
    }
    
    if (getWorld()->getXpos() < getX() && (getWorld()->getYpos()-4 <= getY() && getY() <=  getWorld()->getYpos()+4))
    {
    //    1 in ((20/CurrentLevelNumber)+5) chance:
        if (randInt(1, (20/getWorld()->getLevel())+5) == 1)
        {
            // fire turnip 14 pixels left of center of smallgon;
            getWorld()->addActor(new Turnip(getWorld(), getX()-14, getY()));
            // play sound effect
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
        }
    }
    move();
}

//hp is 5 * (1 + (current_level_number – 1) * .1), direction 0, size 1.5, depth 1
Smoregon::Smoregon(StudentWorld* world, double startX, double startY): Alien(SMOREGON, world, startX, startY, IID_SMOREGON, 5*(1 + (world->getLevel()-1)*0.1), 5, 0, 0, 2, 250)
{
    setSize(1.5);
}

void Smoregon::doSomething()
{
    alienDoSomething();
    
   
    if (getLength() == 0)
    {
        double dy = randInt(-1, 1);
        setDeltaY(dy);
        //set direction to getX()-speed, getY()+(speed*dy);
        int n = randInt(1,32);
        setLength(n);
    }
    
    if (getWorld()->getXpos() < getX() && (getWorld()->getYpos()-4 <= getY() && getY() <=  getWorld()->getYpos()+4))
    {
        //    1 in ((20/CurrentLevelNumber)+5) chance:
        if (randInt(1, (20/getWorld()->getLevel())+5) == 1)
        {
            // fire turnip 14 pixels left of center of smallgon;
            getWorld()->addActor(new Turnip(getWorld(), getX()-14, getY()));
            // play sound effect
            getWorld()->playSound(SOUND_ALIEN_SHOOT);
        }

        if (randInt(1, (20/getWorld()->getLevel())+5) == 1)
        {
            setDeltaY(0);
            setLength(VIEW_WIDTH);
            setSpeed(5);
        }
    }
    
    move();
    
    if(hitPoints() <= 0)
    {
        setDead();
        possiblyDropGoodie();
    }
    
}

void Smoregon::possiblyDropGoodie()
{
    if (randInt(0,2) == 0)
    {
        if (randInt(0,1) == 0)
            getWorld()->addActor(new repairGoodie(getWorld(), this->getX(), this->getY()));
        else getWorld()->addActor(new flatulenceTorpedoGoodie(getWorld(), this->getX(), this->getY()));
    }
}

//HP: 10 * (1 + (current_level_number – 1) * .1)

Snagglegon::Snagglegon(StudentWorld*world, double startX, double startY): Alien(SNAGGLEGON, world, startX, startY, IID_SNAGGLEGON, 10*(1+(world->getLevel()-1)*0.1), 10, -1, -1, 1.75, 1000)
{
    setSize(1.5);
    setDirection(0);
}

void Snagglegon::doSomething()
{
    alienDoSomething();
    
    if (getWorld()->getXpos() < getX() && (getWorld()->getYpos()-4 <= getY() && getY() <=  getWorld()->getYpos()+4))
    {
        //    1 in ((15/CurrentLevelNumber)+10) chance:
        if (randInt(1, (15/getWorld()->getLevel())+10) == 1)
        {
            // fire torpedo 14 pixels left of center of smallgon;
            getWorld()->addActor(new AlienLaunchedTorpedo(getWorld(), getX()-14, getY()));
            // play sound effect
            getWorld()->playSound(SOUND_TORPEDO);
        }
    }
    
    //    if (traveldirection up and left)
    //    moveTo(getX()-2, getY()+2);
    if (getDeltaY() == 1)
    {
        moveTo(getX()-getSpeed(), getY()+getSpeed());
    }
    //    else if (traveldirection down and left)
    //    moveTo(getX()-2, getY()-2);
    else if (getDeltaY() == -1)
    {
        moveTo(getX()-getSpeed(), getY()-getSpeed());
    }
    
    // if top of screen, move down and left
    if (getY() >= VIEW_HEIGHT-1)
    {
        moveTo(getX()-getSpeed(), getY()-getSpeed());
        setDeltaY(-1);
    }
    // if bottom of screen, move up and left
    else if (getY() <= 0)
    {
        moveTo(getX()-getSpeed(), getY()+getSpeed());
        setDeltaY(1);
    }
    
    if (hitPoints() <= 0)
    {
        setDead();
        possiblyDropGoodie();
    }
}

void Snagglegon::possiblyDropGoodie()
{
    if (randInt(0,5) == 0)
    {
        getWorld()->addActor(new extraLifeGoodie(getWorld(), this->getX(), this->getY()));
    }
}

Projectile::Projectile(type e, StudentWorld* world, double startX, double startY, int imageID,
                       double damageAmt, double deltaX, bool rotates, int imageDir): Actor(e, imageID, startX, startY, 0, 0, 0, world)
{}

Cabbage::Cabbage(StudentWorld* world, double startX, double startY): Projectile(CABBAGE, world, startX, startY, IID_CABBAGE, 2, 0, true, 0)
{
    setSize(0.5);
}
//size 0.5, depth 1

void Cabbage::doSomething()
{
    if (isDead())
        return;
    if (getX() >= VIEW_WIDTH)
        setDead();
 
    moveTo(getX()+8, getY());
    
    setDirection(getDirection() + 20);
    
}

void Cabbage::collide(Actor*other)
{
    switch(other->getType())
    {
        case SMALLGON:
        case SNAGGLEGON:
        case SMOREGON:
            setDead();
            Alien* a = static_cast<Alien*>(other);
            a->sufferDamage(2, HIT_BY_PROJECTILE);
            break;
    }
}

//size 0.5, depth 1
Turnip::Turnip(StudentWorld* world, double startX, double startY): Projectile(TURNIP, world, startX, startY, IID_TURNIP, 2, 20, true, 0)
{
    setSize(0.5);
}

void Turnip::doSomething()
{
    if (isDead())
        return;
    if (getX() <= 0)
        setDead();
    //check for collsion
    moveTo(getX()-6, getY());
  
    setDirection(getDirection() + 20);
    //check for collision again
}

flatulenceTorpedo::flatulenceTorpedo(type e, StudentWorld* world, double startX, double startY, double deltaX, int imageDir): Projectile(e, world, startX, startY, IID_TORPEDO, 0, 8, false, 0)
{
    setSize(0.5);
}

PlayerLaunchedTorpedo::PlayerLaunchedTorpedo(StudentWorld* world, double startX, double startY):flatulenceTorpedo(PLAYERLAUNCHEDTORPEDO, world, startX, startY, 0, 0)
{}

void PlayerLaunchedTorpedo::collide(Actor*other)
{
    switch(other->getType())
    {
        case SMALLGON:
        case SNAGGLEGON:
        case SMOREGON:
            setDead();
            Alien* a = static_cast<Alien*>(other);
            a->sufferDamage(8, HIT_BY_PROJECTILE);
        break;
    }
}


void PlayerLaunchedTorpedo::doSomething()
{
    if (isDead())
        return;
    if (getX() >= VIEW_WIDTH)
        setDead();
    moveTo(getX()+8, getY());
}

AlienLaunchedTorpedo::AlienLaunchedTorpedo(StudentWorld* world, double startX, double startY): flatulenceTorpedo(ALIENLAUNCHEDTORPEDO, world, startX, startY, 0, 180)
{}

void AlienLaunchedTorpedo::doSomething()
{
    if (isDead())
        return;
    if (getX() < 0)
        setDead();
    moveTo(getX()-8, getY());
}

Goodie::Goodie(type e, StudentWorld* world, double startX, double startY, int imageID): Actor(e, imageID, startX, startY, 0, 0, 0, world)
{}

void Goodie::doSomethingCheck()
{
    if(isDead())
        return;
    if (getX() < 0)
        setDead();
}

extraLifeGoodie::extraLifeGoodie(StudentWorld* world, double startX, double startY): Goodie(EXTRALIFEGOODIE, world, startX, startY, IID_LIFE_GOODIE)
{
    setDirection(0);
    setSize(0.5);
}

void extraLifeGoodie::doSomething()
{
    doSomethingCheck();
    
    moveTo(getX()-0.75, getY()-0.75);
    
}

repairGoodie::repairGoodie(StudentWorld* world, double startX, double startY): Goodie(REPAIRGOODIE, world, startX, startY, IID_REPAIR_GOODIE)
{
    setDirection(0);
    setSize(0.5);
}

void repairGoodie::doSomething()
{
    doSomethingCheck();
  
    moveTo(getX()-0.75, getY()-0.75);
}

flatulenceTorpedoGoodie::flatulenceTorpedoGoodie(StudentWorld* world, double startX, double startY): Goodie(FLATULENCETORPEDOGOODIE, world, startX, startY, IID_TORPEDO_GOODIE)
{
    setDirection(0);
    setSize(0.5);
}

void flatulenceTorpedoGoodie::doSomething()
{
    doSomethingCheck();
    
    moveTo(getX()-0.75, getY()-0.75);
}
