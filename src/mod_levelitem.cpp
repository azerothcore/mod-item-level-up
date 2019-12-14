

#include "Configuration/Config.h"
#include "Player.h"
#include "Creature.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Define.h"
#include "GossipDef.h"

uint32 Increase_Level;
uint32 MaxItemLevel = 70;
bool LevelItemEnable = true;
bool LevelItemAnnounce = true;

// Configuration
class mod_levelitem_Conf : public WorldScript
{
public:
    mod_levelitem_Conf() : WorldScript("mod_levelitem_Conf") { }

    // Initialize Configuration
    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            std::string conf_path = _CONF_DIR;
            std::string cfg_file = conf_path + "/mod_levelitem.conf";
            std::string cfg_def_file = cfg_file + ".dist";
            sConfigMgr->LoadMore(cfg_def_file.c_str());
            sConfigMgr->LoadMore(cfg_file.c_str());
        }
    }

    // Load Configuration Settings
    void SetInitialWorldSettings()
    {
        LevelItemEnable = sConfigMgr->GetBoolDefault("LevelItem.Enable", true);
        LevelItemAnnounce = sConfigMgr->GetBoolDefault("LevelItem.Announce", true);
        MaxItemLevel = sConfigMgr->GetIntDefault("LevelItem.MaxItemLevel", 70);
    }
};

// Announce
class mod_levelitem_Announce : public PlayerScript
{

public:
    mod_levelitem_Announce() : PlayerScript("mod_levelitem_Announce") {}

    void OnLogin(Player* player) override
    {
        if (sConfigMgr->GetBoolDefault("Arena1v1Announcer.Enable", true))
        {
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Level Item |rmodule.");
        }
    }
};

class LevelItem : public ItemScript
{
public:
    LevelItem() : ItemScript("LevelItem") { }

    bool OnUse(Player* p, Item* i, const SpellCastTargets &) override
    {
        if (!LevelItemEnable)
            return false;

        if (p->IsInCombat() || p->IsInFlight() || p->GetMap()->IsBattlegroundOrArena())
        {
            ChatHandler(p->GetSession()).PSendSysMessage("You can't use that right now!");
            return false;
        }

        if (p->getLevel() >= MaxItemLevel)
        {
            ChatHandler(p->GetSession()).PSendSysMessage("You're already at level 70!");
            return false;
        }

        uint8 newLevel = p->getLevel() + 1;
        p->GiveLevel(newLevel);
        p->SetUInt32Value(PLAYER_XP, 0);
        p->DestroyItemCount(i->GetEntry(), 1, true);
        ChatHandler(p->GetSession()).PSendSysMessage("You have used one Level-Up Token!");

        return true;
    }
};



void AddLevelItemScripts()
{
    new mod_levelitem_Conf();
    new mod_levelitem_Announce();
    new LevelItem();
}
