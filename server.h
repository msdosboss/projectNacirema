#if !defined SERVER



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


struct equipableItemWeapon{
    int isWeapon;
    int type;           //type map 0 = dagger, 1 = sword, 2 = shield, 3 = staff, 4 = wand, 5 = gun
    int damage;

};


struct equipment{
    //struct item head;
    //struct item shoulder;
    //struct item chest;
    //struct item pants;
    //struct item boots;
    //struct item trinket1;
    //struct item trinket2;
    struct equipableItemWeapon weapon;
};

struct spell{
    char *name;
    int levelReq;
};

struct class{
    struct spell *spells;
    
};

struct equipableItem{
    int isEquiable;
    int slot;               //slot map 0 = head, 1 = shoulder, 3 = chest, 4 = pants, 5 = boots, 6 = trinkets
};

struct item {
    char *name;
    char *description;
    struct equipableItem equip;

};


struct character{
    //A lot of these atributes will be inherited from the SQL server
    struct item *items;
    struct class characterClass;
    struct equipment characterEquipment;
    struct characterStats stats;
    int level;
    int health;
    int canCombat;
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
#define SERVER 1