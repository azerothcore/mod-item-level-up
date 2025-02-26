#include "AccountMgr.h"
#include "Chat.h"
#include "Configuration/Config.h"
#include "Creature.h"
#include "Define.h"
#include "GossipDef.h"
#include "Player.h"
#include "ScriptMgr.h"

uint32 Increase_Level;
uint32 MaxItemLevel = 80;
bool LevelItemEnable = true;
bool LevelItemAnnounce = true;

// Configuration
class mod_levelitem_Conf : public WorldScript
{
public:
    mod_levelitem_Conf() : WorldScript("mod_levelitem_Conf", {
        WORLDHOOK_ON_BEFORE_CONFIG_LOAD
    }) { }

    // Load Configuration Settings
    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        LevelItemEnable = sConfigMgr->GetOption<bool>("LevelItem.Enable", true);
        LevelItemAnnounce = sConfigMgr->GetOption<bool>("LevelItem.Announce", true);
        MaxItemLevel = sConfigMgr->GetOption<int>("LevelItem.MaxItemLevel", 80);
    }
};

// Announce
class mod_levelitem_Announce : public PlayerScript
{

public:
    mod_levelitem_Announce() : PlayerScript("mod_levelitem_Announce", {
        PLAYERHOOK_ON_LOGIN
    }) {}

    void OnPlayerLogin(Player* player) override
    {
        if (LevelItemAnnounce)
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00Level Item |rmodule.");
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

        if (p->GetLevel() >= MaxItemLevel)
        {
            ChatHandler(p->GetSession()).PSendSysMessage("You're already at level %u!", MaxItemLevel);
            return false;
        }

        uint8 newLevel = p->GetLevel() + 1;
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
