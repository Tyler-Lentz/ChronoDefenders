#ifndef STATUS_H
#define STATUS_H

#include "colorstring.h"
#include "utilities.h"

class Creature;

// list of status afflictions that act as flags which moves look for
enum class StatusID
{
    OffBalance,
    Dodging,
    Burnt,
    Frostburnt,
    Spade,
    Diamond,
    Club,
    Heart,
    Dynamite,
    Zen,
    Storm,
    Invulnerable,
    Vulnerable,
    Bleeding,
    Poisoned,
    Thorns,
    Hexed,
    Zapped,
    Dragon,
    Jester,
    Elemental,
    Strangled,
    Stunned,
    Scorched
};

class Status
{
public:
    Status(StatusID theID, ColorString name, bool uni, bool ablethrow, bool totalRemove, bool unchange);

    virtual ColorString applyEndTurnEffect(Creature* target, int stackAmount) = 0;
    
    bool isThrowable();
    bool isUnique();
    bool shouldTotalRemove();
    bool isUnchanging();

    StatusID getID();
    ColorString getName();

    virtual Status* makeCopy() = 0;;
private:
    bool unchanging; // if true, the status number doesn't decrease naturally at the end of turn
    bool totalRemove; // if true, remove all of the status at the end of the turn, not just one tick
    bool unique; // tells if additional 
    bool throwable; // tells 
    StatusID id;
    ColorString name;
};

class NormalStatus : public Status
{
public:
    NormalStatus(StatusID theID, ColorString name);
};

class OffBalanceStatus : public NormalStatus
{
public:
    OffBalanceStatus();
    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;
};

class BurntStatus : public NormalStatus
{
public:
    BurntStatus();
    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    static const int BURN_DAMAGE = 2;
    static const int COLOR = ddutil::RED;
};

class ScorchedStatus : public NormalStatus
{
public:
    ScorchedStatus();
    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    static const int DAMAGE_PER_BURN = 2;
    static const int COLOR = ddutil::YELLOW;
};

class FrostBurntStatus : public NormalStatus
{
public:
    FrostBurntStatus();
    Status* makeCopy() override;

    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    static const int COLOR = ddutil::LIGHTCYAN;
};

class ZenStatus : public NormalStatus
{
public:
    ZenStatus();
    Status* makeCopy() override;

    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::LIGHTMAGENTA;
private:
};

class StormStatus : public NormalStatus
{
public:
    StormStatus();
    Status* makeCopy() override;

    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::BLUE;
private:
};

class InvulnerableStatus : public NormalStatus
{
public:
    InvulnerableStatus();
    Status* makeCopy() override;

    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::LIGHTGRAY;
};

class VulnerableStatus : public NormalStatus
{
public:
    VulnerableStatus();
    Status* makeCopy() override;

    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::LIGHTRED;
};

class PoisonedStatus : public NormalStatus
{
public:
    PoisonedStatus();
    Status* makeCopy() override;

    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::LIGHTGREEN;
};

class HexedStatus : public NormalStatus
{
public:
    HexedStatus();
    Status* makeCopy() override;

    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::LIGHTMAGENTA;
    const static int VITALITY_LOSS = 1;
};

class StrangledStatus : public NormalStatus
{
public:
    StrangledStatus();
    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;
    const static int COLOR = ddutil::WHITE;
    const static int STRANGLE_DAMAGE = 2;
};

class StunnedStatus : public NormalStatus
{
public:
    StunnedStatus();
    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;
    const static int COLOR = ddutil::YELLOW;
};

class ZappedStatus : public NormalStatus
{
public:
    ZappedStatus();
    Status* makeCopy() override;

    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::YELLOW;
};



// card status

class CardStatus : public Status
{
public:
    CardStatus(StatusID theID, ColorString name, int num);

protected:
    int number; // 2-10, 11 = jack, 12 = queen, 13 = king, 14 = ace
};

class SpadeStatus : public CardStatus
{
public:
    SpadeStatus(int num);

    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;
private:
};

class HeartStatus : public CardStatus
{
public: 
    HeartStatus(int num);

    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;
};

class ClubStatus : public CardStatus
{
public:
    ClubStatus(int num);

    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;
};

class DiamondStatus : public CardStatus
{
public:
    DiamondStatus(int num);

    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;
};


// Unique Status

class UniqueStatus : public Status
{
public:
    UniqueStatus(StatusID theID, ColorString name);
};

class Dynamite : public UniqueStatus
{
public:
    Dynamite();
    Status* makeCopy() override; 
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int DAMAGE = 40;
    const static int COLOR = ddutil::LIGHTRED;
};

class TotalRemoveStatus : public Status
{
public:
    TotalRemoveStatus(StatusID id, ColorString name);
};

// deals 1 damage per stack level, then at the end of the turn the entire stack is removed
class BleedingStatus : public TotalRemoveStatus
{
public:
    BleedingStatus();
    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::LIGHTRED;
};




// Unchanging status
class UnchangingStatus : public Status
{
public:
    UnchangingStatus(StatusID id, ColorString name);
};

class ThornsStatus : public UnchangingStatus
{
public:
    ThornsStatus();

    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::YELLOW;
};

class DragonStatus : public NormalStatus 
{
public:
    DragonStatus();

    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::RED;
};

class JesterStatus : public NormalStatus
{
public:
    JesterStatus();

    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;

    const static int COLOR = ddutil::GREEN;
};

class ElementalStatus : public NormalStatus
{
public:
    ElementalStatus();

    Status* makeCopy() override;
    ColorString applyEndTurnEffect(Creature* target, int stackAmount) override;
    
    const static int COLOR = ddutil::LIGHTMAGENTA;
    const static int ZAP_AMOUNT = 5;
};

#endif