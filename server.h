#if !defined SEVER



struct characterStats{
    int iq;
    int agility;
    int luck;
    int hp;
    int empathy;
};

struct armorStats{
    int armor;
    int magicResist;
    int critRate;
    int spellPower;
    int attackPower;
    int hitRate;
    int critReduction;
};

struct equipment{
    struct item head;
    struct item shoulder;
    struct item chest;
    struct item pants;
    struct item boots;
    struct item trinket1;
    struct item trinket2;
};

struct spell{
    char *name;
    int levelReq;
}

struct class{
    struct spell *spells;
    
};

struct equipableItemWeapon{
    int isWeapon;
    int type;           //type map 0 = dagger, 1 = sword, 2 = shield, 3 = staff, 4 = wand, 5 = gun

};

struct equipableItem{
    int isEquiable;
    int slot;               //slot map 0 = head, 1 = shoulder, 3 = chest, 4 = pants, 5 = boots, 6 = trinkets
};

struct item {
    char *name;
    char *description;
    struct equipableItem;
    struct equipableItemWeapon;

};


struct character{
    //A lot of these atributes will be inherited from the SQL server
    struct item *items;
    struct class;
    struct equipment;
    struct stats;
    int level;
};

struct square {
    struct characters *characters;
};

struct zone {
    struct square **squares;
};

struct world {
    struct zone **zones;
};


#endif
#define SEVER 1