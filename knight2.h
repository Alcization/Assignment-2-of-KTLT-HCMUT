#ifndef __KNIGHT2_H__
#define __KNIGHT2_H__

#include "main.h"
// #define DEBUG

class BaseItem;
class Phoenix;
class Antidote;
class BaseKnight;
class Events;

enum ItemType {ANTIDOTE = 0, PHOENIX};

class BaseItem {
protected:
    ItemType itemType;
public:
    virtual ItemType getType() { return itemType; }
    virtual string toString() = 0;
    virtual bool canUse(BaseKnight* knight) = 0;
    virtual void use(BaseKnight* knight) = 0;
    //student method
    virtual  ~BaseItem() {};
};

class Antidote : public BaseItem
{
public:
    Antidote() {
        this->itemType = ANTIDOTE;
    }
    ~Antidote(){}
    string toString() { return "Antidote";}
    bool canUse(BaseKnight* knight){
        return true;
    }
    void use(BaseKnight* knight){
        return;
    }
};

class Phoenix : public BaseItem{
protected:
    int typeOfPhoenix;
public:
    Phoenix(int num = 1){
        this->itemType = PHOENIX;
        typeOfPhoenix = num;
    }
    ~Phoenix(){}

    string toString() {
        string arr[4]{ "I", "II", "III", "IV" };
        return "Phoenix" + arr[typeOfPhoenix - 1];
    }

    bool canUse(BaseKnight* knight);

    void use(BaseKnight* knight);

};

//write inherit phoenix class
class BaseBag{
    struct BagNode{
        BaseItem* item;
        BagNode* next;
        BagNode(BaseItem* inp, BagNode* nextptr = nullptr) : item{ inp }, next{ nextptr } {}
        ~BagNode(){
            if (item) delete item;
        }
    };
    BagNode* head;
    int count;
    int maxSlot;
public:
    BaseBag(int phoenixdown, int antidote, int max=-1): maxSlot{max}, count{0}, head{nullptr} {
        for (int i{ 0 }; i < phoenixdown; ++i){
            //insert phoenix1 in bag
            BaseItem* tmp = new Phoenix();
            this->insertFirst(tmp);
        }

        for (int i{ 0 }; i < antidote; ++i){
            BaseItem* tmp = new Antidote();
            this->insertFirst(tmp);
        }
    }
    virtual bool insertFirst(BaseItem * item);
    virtual BaseItem * get(ItemType itemType);
    virtual string toString() const;

    ~BaseBag(){
        while (head){
            BagNode* tmp{ head };
            head = head->next;
            delete tmp;
        }
        head = nullptr;
        count = 0;
    }

protected:
    virtual BaseItem* getAndCheck(ItemType itemType, BaseKnight* k9);
public:
    virtual bool useItem(ItemType itemType, BaseKnight* k9);
    virtual void dropLatestItem();
};

class BaseOpponent{
protected:
    int level;
    int type;
    int gil;
    int baseDamage;
public:
    BaseOpponent(int lv, int num, int gil, int bd) : level{ lv }, type{ num }, gil{gil}, baseDamage{bd}{}
    int getType() { return type; }
    int getLevel() { return level; }
    int getGil() { return gil; }
    int getDame() { return baseDamage; }
};

enum KnightType {PALADIN=0, LANCELOT, DRAGON, NORMAL};
class BaseKnight {
protected:
    int id;
    int hp;
    int maxhp;
    int level;
    int gil;
    int antidote;
    BaseBag * bag;
    KnightType knightType;
    //student attri
public:
    static BaseKnight * create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI);
    virtual void fight(BaseOpponent * opponent) = 0;

    string toString() const;

    double dame_of_knight;
    //student method
    virtual ~BaseKnight(){
        if (bag) delete bag;
        bag = nullptr;
    }
    int getHp() const               {return hp;}
    void setHp(int newHp)           {this->hp = newHp;}
    int getMaxHp() const            {return maxhp;}
    int getLv() const               {return level;}
    void setLv(int lv)              {level = lv;}
    KnightType getType() const      {return knightType;}
    int getGil() const              {return gil;}
    void setGil(int newGil)         {gil = newGil;}
    BaseBag* getBag()               {return bag;}
    bool knightDead()               {return hp <= 0;}

    virtual bool process_HP_down();
protected:
    virtual void winningMinor(BaseOpponent* opponent){
        this->gil += opponent->getGil();
    }
    virtual void losingMinor(BaseOpponent* opponent){
        int damage = opponent->getDame() * (opponent->getLevel() - this->level);
        this->hp -= damage;
        process_HP_down();
    }
    virtual void winningTornbery(){
        this->level += 1;
        if (this->level > 10) this->level = 10;
    }
    virtual void losingTornbery(){
        if (antidote > 0){
            antidote--;
            bag->useItem(ANTIDOTE, this);
        }
        else{
            for (int i{ 0 }; i < 3; ++i) bag->dropLatestItem();
            this->hp -= 10;
            process_HP_down();
        }
    }
    virtual void winningQueen(){
        this->gil *= 2;
    }
    virtual void losingQueen(){
        this->gil /= 2;
    }
};

class PaladinKnight : public BaseKnight {
public:
    PaladinKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixI)
    {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new BaseBag(phoenixI, antidote);
        this->knightType = PALADIN;
        dame_of_knight = 0.06;
    }
    ~PaladinKnight()
    {
        if (bag) delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent* opponent);
};

class LancelotKnight : public BaseKnight {
public:
    LancelotKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixI)
    {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new BaseBag(phoenixI, antidote,16);
        this->knightType = LANCELOT;
        dame_of_knight = 0.05;
    }
    ~LancelotKnight()
    {
        if (bag) delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent* opponent);
};

class DragonKnight : public BaseKnight {
public:
    DragonKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixI)
    {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new BaseBag(phoenixI, antidote,14);
        this->knightType = DRAGON;
        dame_of_knight = 0.075;
    }
    ~DragonKnight()
    {
        if (bag) delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent* opponent);
};

class NormalKnight : public BaseKnight {
public:
    NormalKnight(int id, int maxhp, int level, int gil, int antidote, int phoenixI)
    {
        this->id = id;
        this->maxhp = maxhp;
        this->hp = maxhp;
        this->level = level;
        this->gil = gil;
        this->antidote = antidote;
        bag = new BaseBag(phoenixI, antidote,19);
        this->knightType = NORMAL;
        dame_of_knight = 0;
    }
    ~NormalKnight()
    {
        if (bag) delete bag;
        bag = nullptr;
    }
    void fight(BaseOpponent* opponent);
};

struct checker {
    bool shield;
    bool spear;
    bool hair;
    bool excalibur;
    bool omegaweapon;
    bool hades;
    checker() : 
        shield{false}, spear{false}, hair{false}, excalibur{false}, omegaweapon{false}, hades{false} {}       
};

class ArmyKnights {
    BaseKnight** army;
    int max_knights;
    int number_of_knights;
    checker condition;

public:
    ArmyKnights (const string & file_armyknights);
    ~ArmyKnights();
    bool adventure (Events * events);
    int count() const;
    BaseKnight * lastKnight() const;

    bool hasFightUlti;
    bool hasPaladinShield()  const  {return condition.shield;}
    bool hasLancelotSpear()  const  {return condition.spear;}
    bool hasGuinevereHair()  const  {return condition.hair;} 
    bool hasExcaliburSword() const  {return condition.excalibur;}

    void printInfo() const;
    void printResult(bool win) const;

private:
    void delete_knight(int index);
    bool process_event(int event_order, int eve);
    bool fight_normal_monster(int event_order, int monsterType);
    bool fight_tornbery(int event_order);
    bool fight_queen(int event_order);
    void meet_nina();
    bool fight_omega_weapon();
    bool fight_hades();
    bool fight_ultimecia();
    void collect_phoenix(int type);
    void throw_gil_back(int, int);
    void destroyArmyKnight();
    BaseKnight* lastKnightAfterFightUlti() const;
};

class Events {
    int* eve;
    int number_of_events;
public:
    Events(const string& file_events);
    ~Events();
    int count() const;
    int get(int i) const;
};

class KnightAdventure {
private:
    ArmyKnights * armyKnights;
    Events * events;

public:
    KnightAdventure();
    ~KnightAdventure(); // TODO:

    void loadArmyKnights(const string &);
    void loadEvents(const string &);
    void run();
};

#endif // __KNIGHT2_H__