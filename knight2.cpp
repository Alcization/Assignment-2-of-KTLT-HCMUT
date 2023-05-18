#include "knight2.h"

/* * * BEGIN implementation of class Events * * */

bool isPrime(int n){
    if (n < 2) return false;
    for (int i=2 ; i <= sqrt(n); i++){
        if (n%i==0) return false;
    }
    return true;
}

bool isPythago(int n){
    if (n==345 or n==354 or n== 435 or n== 453 or n==534 or n==543) return true;
    return false;
}

Events::Events(const string& file_events) {
    ifstream input{file_events};
    number_of_events = 0;
    eve = nullptr;
    input >> number_of_events;
    eve = new int[number_of_events];
    for (int i=0;i<number_of_events;i++){
        input >> eve[i];
    }
    input.close();
}

Events::~Events() {
    delete[] this->eve;
    this->eve = nullptr;
    number_of_events = 0;
}

int Events::count() const {
    return number_of_events;
}

int Events::get(int i) const {
    return this->eve[i];
}

/* * * END implementation of class Events * * */

/* * * BEGIN implementation of class BaseBag * * */

bool BaseBag::insertFirst(BaseItem* item) {
    if (count == 0){
        head = new BagNode(item);
        count++;
        return true;
    }
    else if (maxSlot == -1 or count<maxSlot){
        BagNode* tmp = new BagNode(item, head);
        head = tmp;
        count++;
        return true;
    }
    else return false;
}

BaseItem* BaseBag::get(ItemType itemType) {
    BagNode* tmp{ head };
    while (tmp){
        if (tmp->item->getType() == itemType){
            BaseItem* tmpItem{ tmp->item };
            return tmpItem;
        }
        tmp = tmp->next;
    }
    return nullptr;
}

string BaseBag::toString() const {
    string res{ "Bag[count=" };
    res = res + to_string(count) + ';';
    BagNode* tmp{ head };
    while (tmp){
        res = res + tmp->item->toString() + ',';
        tmp = tmp->next;
    }
    if (res[res.length() - 1] == ',') res[res.length() - 1] = ']';
    else res += ']';
    return res;
}

BaseItem* BaseBag::getAndCheck(ItemType itemType, BaseKnight* k9) {
    BagNode* tmp{ head };
    while (tmp){
        if (tmp->item->getType() == itemType and tmp->item->canUse(k9)){
            BaseItem* tmpItem{ tmp->item };
            tmp->item = head->item;
            head->item = tmpItem;
            return tmpItem;
        }
        tmp = tmp->next;
    }
    return nullptr;
}

bool Phoenix::canUse(BaseKnight* knight){
    int hp{knight->getHp()};
    int maxHp{knight->getMaxHp()};
    if (typeOfPhoenix==1){
        if (hp <= 0) return true;
        else return false;
    } else if (typeOfPhoenix==2){
        if (hp < (maxHp / 4)) return true;
        else return false;
    } else if (typeOfPhoenix==3){
        if (hp < (maxHp / 3)) return true;
        else return false;
    } else if (typeOfPhoenix==4){
        if (hp < (maxHp / 2)) return true;
        else return false;
    } 
    return false;
}

void Phoenix::use(BaseKnight* knight){
    int hp{knight->getHp()};
    int maxHp{knight->getMaxHp()};
    if (typeOfPhoenix==1){  
        knight->setHp(maxHp);
    } else if (typeOfPhoenix==2){
        knight->setHp(maxHp);
    } else if (typeOfPhoenix==3){
        if (hp <= 0) knight->setHp((maxHp/3));
        else knight->setHp((maxHp/4) + hp);
    } else if (typeOfPhoenix==4){
        if (hp <= 0) knight->setHp((maxHp/2));
        else knight->setHp((maxHp/5) + hp); 
    } 
}

bool BaseBag::useItem(ItemType itemType, BaseKnight* k9){
    BaseItem* itemToUse = getAndCheck(itemType, k9);
    if (itemToUse){
        itemToUse->use(k9);
        BagNode* tmp = head;
        head = head->next;
        --count;
        delete tmp;
        return true;
    }
    return false;
}

void BaseBag::dropLatestItem(){
    if (count>0){
        BagNode* tmp = head;
        head = head->next;
        --count;
    }
}

/* * * END implementation of class BaseBag * * */

/* * * BEGIN implementation of class BaseKnight * * */
string BaseKnight::toString() const {
    string typeString[4] = {"PALADIN", "LANCELOT", "DRAGON", "NORMAL"};
    stringstream ss;  
    ss << "[Knight:id:" << id << ",hp:" << hp << ",maxhp:" << maxhp << ",level:" << level << ",gil:" << gil << "," << bag->toString() << ",knight_type:" << typeString[knightType] << "]";
    string str = ss.str();
    return str;
}

BaseKnight* BaseKnight::create(int id, int maxhp, int level, int gil, int antidote, int phoenixdownI){
    BaseKnight* res{nullptr};
    if (maxhp == 888){
        res = new LancelotKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    }
    else if (isPrime(maxhp)){
        res = new PaladinKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    }
    else if (maxhp > 99 and maxhp <= 999 and isPythago(maxhp)){
        res = new DragonKnight(id, maxhp, level, gil, 0, phoenixdownI);
    }
    else{
        res = new NormalKnight(id, maxhp, level, gil, antidote, phoenixdownI);
    }
    return res;
}

void ArmyKnights::destroyArmyKnight() {
    for (int i = number_of_knights - 1; i >= 0; --i) {
        delete_knight(i);
    }
}

bool BaseKnight::process_HP_down(){
    if (bag->useItem(PHOENIX, this)) return true;
    else if (this->hp <= 0 and this->gil >= 100){
        this->hp = this->maxhp/2;
        this->gil -= 100;
        return true;
    }
    return false;
}

void PaladinKnight::fight(BaseOpponent* opponent){
    int monsterType = opponent->getType();
    if (monsterType <= 5){
        winningMinor(opponent);
    }
    else if (this->level >= opponent->getLevel()){
        if (monsterType == 6){
            winningTornbery();
        }
        else winningQueen();
    }
    else if (monsterType==6){
        losingTornbery();
    }
}

void LancelotKnight::fight(BaseOpponent* opponent){
    int monsterType = opponent->getType();
    if (monsterType <= 5){
        winningMinor(opponent);
    }
    else if (this->level >= opponent->getLevel()){
        if (monsterType == 6){
            winningTornbery();
        }
        else winningQueen();
    }
    else if (monsterType == 6){
        losingTornbery();
    }
    else losingQueen();
}

void DragonKnight::fight(BaseOpponent* opponent){
    int monsterType = opponent->getType();
    if (this->level >= opponent->getLevel()){
        if (monsterType == 5) winningMinor(opponent);
        else if (monsterType == 6){
            winningTornbery();
        }
        else winningQueen();
    }
    else if (monsterType <= 5){
        losingMinor(opponent);
    }
    else if (monsterType == 7) losingQueen();
}

void NormalKnight::fight(BaseOpponent* opponent){
    int monsterType = opponent->getType();
    if (this->level >= opponent->getLevel()){
        if (monsterType <= 5) winningMinor(opponent);
        else if (monsterType == 6){
            winningTornbery();
        }
        else winningQueen();
    }
    else if (monsterType <= 5){
        losingMinor(opponent);
    }
    else if (monsterType == 6) losingTornbery();
    else losingQueen();
}

/* * * END implementation of class BaseKnight * * */

/* * * BEGIN implementation of class ArmyKnights * * */

ArmyKnights::ArmyKnights(const string & file_armyknights): condition{}, hasFightUlti{false} {
    ifstream input{ file_armyknights };
    army = nullptr;
    max_knights = 0;
    number_of_knights = 0;
    input >> number_of_knights;
    max_knights = number_of_knights;
    army = new BaseKnight*[number_of_knights];
    for (int i=0;i<number_of_knights;i++){
        int hp, level, phoenix, gil, antidote;
        input >> hp >> level >> phoenix >> gil >> antidote;
        army[i] = BaseKnight::create(i+1,hp,level,gil,antidote,phoenix);
    }
    input.close();
}

ArmyKnights::~ArmyKnights(){
    for (int i=0;i < max_knights;i++){
        if (army[i]) delete army[i];
    }
    delete[] army;
    army = nullptr;
    number_of_knights = 0;
}

void ArmyKnights::throw_gil_back(int number_of_gil, int start_index){
    while (start_index>0 and number_of_gil>0){
        int current_gil = army[start_index]->getGil();
        current_gil += number_of_gil;
        number_of_gil = 0;
        if (current_gil > 999)
        {
            number_of_gil = current_gil - 999;
            current_gil = 999;
        }
        army[start_index]->setGil(current_gil);
        start_index--;
    }
}

bool ArmyKnights::fight_normal_monster(int event_order, int monsterType){
    int gil_of_monster[5]{100,150,450,750,800};
    int basedamage_of_monster[5]{10,15,45,75,95};
    int monster_level = (event_order + monsterType)%10 + 1;
    BaseOpponent* monster = new BaseOpponent(monster_level, monsterType, gil_of_monster[monsterType - 1], basedamage_of_monster[monsterType - 1]);
    for (int i=number_of_knights - 1;i>=0;i--){
        army[i]->fight(monster);
        if (army[i]->getHp() <= 0) delete_knight(i);
        else{
            int gil = army[i]->getGil();
            if (gil > 999){
                army[i]->setGil(999);
                throw_gil_back(gil - 999, i - 1);
            }
            break;
        }
    }
    delete monster;
    if (number_of_knights < 0) return false;
    return true;
}

bool ArmyKnights::fight_tornbery(int event_order){
    int monster_level = (event_order + 6)%10 + 1;
    BaseOpponent* monster = new BaseOpponent(monster_level, 6, 0, 0);
    for (int i{ number_of_knights - 1 }; i >= 0; --i){
        army[i]->fight(monster);
        if (army[i]->getHp() <= 0) delete_knight(i);
        else break;
    }
    delete monster;
    if (number_of_knights < 0) return false;
    return true;
}

bool ArmyKnights::fight_queen(int event_order){
    int monster_level = (event_order + 7)%10 + 1;
    BaseOpponent* monster = new BaseOpponent(monster_level, 7, 0, 0);
    BaseKnight* last_knight = this->lastKnight();
    last_knight->fight(monster);
    int gil = last_knight->getGil();
    if (gil > 999){
        last_knight->setGil(999);
        throw_gil_back(gil - 999, number_of_knights - 2);
    }
    delete monster;
    return true;
}

void ArmyKnights::meet_nina(){
    for (int i=0;i<number_of_knights;i++){
        BaseKnight* tmp = this->army[i];
        int current_gil = tmp->getGil();
        int current_hp = tmp->getHp();
        int maxHp = tmp->getMaxHp();
        if (tmp->getType() == PALADIN and current_hp < (maxHp/3)){
            tmp->setHp(current_hp + maxHp/5);
        } else if (current_gil >= 50 and current_hp < (maxHp/3)){
            tmp->setGil(current_gil - 50);
            tmp->setHp(current_hp + maxHp/5);
        }
    }
}

bool ArmyKnights::fight_omega_weapon(){
    if (condition.omegaweapon) return true;
    int i=(number_of_knights - 1);
        if (army[i]->getType() == DRAGON or army[i]->getLv()==10 and (army[i]->getHp() == army[i]->getMaxHp())){
            condition.omegaweapon = true;
            army[i]->setLv(10);
            army[i]->setGil(999);
            return true;
        }
        else{
            army[i]->setHp(0);
            army[i]->process_HP_down();
            if (army[i]->getHp() <= 0) delete_knight(i);
            else return true;
        }
    if (number_of_knights==0) return false;
    return true;
}

bool ArmyKnights::fight_hades(){
    if (condition.hades) return true;
    int i=number_of_knights - 1;
    if (army[i]->getLv() == 10 or (army[i]->getType() == PALADIN and army[i]->getLv() >= 8 )){
        condition.hades = true;
        condition.shield = true;
        return true;
    }
    else {
        army[i]->setHp(0);
        army[i]->process_HP_down();
        if (army[i]->getHp() <= 0) delete_knight(i);
        else return true;
    }
    if (number_of_knights<=0) return false;
    return true;
}

bool ArmyKnights::fight_ultimecia(){
    hasFightUlti=true;
    if (condition.excalibur) return true;
    if (!hasPaladinShield() or !hasLancelotSpear() or !hasGuinevereHair()){
        destroyArmyKnight();
        return false;
    }
    int ultimecia_hp = 5000;
    for (int i = (number_of_knights-1);i>=0;i--){
        if (army[i]->getType() != NORMAL){
            int damage = army[i]->getLv() * army[i]->getHp() * army[i]->dame_of_knight;
            ultimecia_hp -= damage;
            if (ultimecia_hp <= 0) return true;
            delete_knight(i);
        }
    }
    destroyArmyKnight();
    return false;
}

void ArmyKnights::collect_phoenix(int type){
    BaseItem* tmp = new Phoenix(type);
    for (int i= (number_of_knights - 1);i>=0;i--){
        if (army[i]->getBag()->insertFirst(tmp)) return;
    }
    delete tmp;
    return;
}

bool ArmyKnights::process_event(int event_order, int eve){
    if (eve <= 5){
        return fight_normal_monster(event_order, eve);
    }
    else if (eve == 6) return fight_tornbery(event_order);
    else if (eve == 7) return fight_queen(event_order);
    else if (eve == 8){
        meet_nina();
        return true;
    }
    else if (eve == 9){
        BaseKnight* tmp = this->army[0];
        tmp->setHp(tmp->getMaxHp());
        return true;
    }
    else if (eve == 10) return fight_omega_weapon();
    else if (eve == 11) return fight_hades();
    else if (eve == 95 or eve == 96 or eve == 97){
        if (eve == 95) condition.shield = true;
        if (eve == 96) condition.spear = true;
        if (eve == 97) condition.hair = true;
        return true;
    }
    else if (eve == 98){
        if (condition.shield and condition.spear and condition.hair) condition.excalibur = true;
        else condition.excalibur = false;
        return true;
    }
    else if (eve == 99) return fight_ultimecia();
    else {
        collect_phoenix(eve - 110);
        return true;
    }
    return true;
}

bool ArmyKnights::adventure(Events* events){
    int number_of_eve = events->count();
    bool result = true;
    for (int i=0;i<number_of_eve;i++){
        int eve = events->get(i) ;
        if (number_of_knights <=0){
            result = false;
            return result;
        }
        bool res = process_event(i, eve);
        printInfo();
        if (!res) result = false;
    }
    return result;
}

int ArmyKnights::count() const {
    return number_of_knights;
}

BaseKnight* ArmyKnights::lastKnight() const {
    if (hasFightUlti) return lastKnightAfterFightUlti();
    return army[number_of_knights-1];
}

BaseKnight* ArmyKnights::lastKnightAfterFightUlti() const  {
    for (int i=number_of_knights-1;i>=0;i--) {
        if (army[i] != nullptr) return army[i];
    }
    return army[1];
}
void ArmyKnights::printInfo() const {
    cout << "No. knights: " << this->count();
    if (this->count() > 0) {
        BaseKnight * lknight = lastKnight(); // last knight
        cout << ";" << lknight->toString();
    }
    cout << ";PaladinShield:" << this->hasPaladinShield()
        << ";LancelotSpear:" << this->hasLancelotSpear()
        << ";GuinevereHair:" << this->hasGuinevereHair()
        << ";ExcaliburSword:" << this->hasExcaliburSword()
        << endl
        << string(50,'-') << endl;
}

void ArmyKnights::printResult(bool win) const {
    if (win) cout << "WIN" << endl;
    else cout << "LOSE" << endl;
}

void ArmyKnights::delete_knight(int index){
    if (army && army[index]){
        delete army[index];
        army[index] = nullptr;
        number_of_knights--;
    }
}
/* * * END implementation of class ArmyKnights * * */

/* * * BEGIN implementation of class KnightAdventure * * */
KnightAdventure::KnightAdventure(){
    armyKnights = nullptr;
    events = nullptr;
}

KnightAdventure::~KnightAdventure(){
    if (armyKnights) delete armyKnights;
    if (events) delete events;
}

void KnightAdventure::loadArmyKnights(const string& input){
    armyKnights = new ArmyKnights(input);
}

void KnightAdventure::loadEvents(const string& input){
    events = new Events(input);
}

void KnightAdventure::run(){
    if (armyKnights and events){
        bool res = armyKnights->adventure(events);
        armyKnights->printResult(res);
    }
}

/* * * END implementation of class KnightAdventure * * */


