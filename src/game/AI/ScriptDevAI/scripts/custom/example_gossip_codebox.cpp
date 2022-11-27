/* This file is part of the ScriptDev2 Project. See AUTHORS file for Copyright information
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Example_Item_Gossip_Codebox
SD%Complete: 100
SDComment: Show a codebox in gossip option
SDCategory: Script Examples
EndScriptData */

#include "AI/ScriptDevAI/include/sc_common.h"
#include <cstring>

void Hearthstone_Main_Menu(Player* pPlayer, Item* pItem)
{
    pPlayer->GetPlayerMenu()->ClearMenus(); // clear menus to avoid menu items duplicating
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Show me it works message on top screen!", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
    pPlayer->ADD_GOSSIP_ITEM_EXTENDED(0, "What is my name, correct or wrong?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2, "", 0, true);
    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Show me a submenu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
    pPlayer->GetPlayerMenu()->SendGossipMenu(907, pItem->GetObjectGuid());
}

void HackFix_StopVisualCastBug(Player* pPlayer, Item* pItem)
{
    /* Send equip error that shows no message
       This is a hack fix to stop spell casting visual bug when a spell is not cast on use
       by Rochet2
    */

    WorldPacket data(SMSG_INVENTORY_CHANGE_FAILURE, 18);
    data << uint8(EQUIP_ERR_NONE);
    data << pItem->GetObjectGuid();
    data << ObjectGuid(uint64(0));
    data << uint8(0);
    pPlayer->GetSession()->SendPacket(data);
}

bool Hearthstone_On_Use(Player* pPlayer, Item* pItem, SpellCastTargets const& /*scTargets*/)
{
    Hearthstone_Main_Menu(pPlayer, pItem);
    HackFix_StopVisualCastBug(pPlayer, pItem);
    return true;
}

bool Hearthstone_Gossip_Select(Player* pPlayer, Item* pItem, uint32 /*uiSender*/, uint32 uiAction)
{
    switch (uiAction)
    {
        case GOSSIP_ACTION_INFO_DEF + 1:
        {
            pPlayer->GetSession()->SendAreaTriggerMessage("It works");
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 3:
        {
            pPlayer->GetPlayerMenu()->ClearMenus(); // clear menus to avoid menu items duplicating
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Return to main menu", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Close gossip", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
            pPlayer->GetPlayerMenu()->SendGossipMenu(907, pItem->GetObjectGuid());
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 4:
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            Hearthstone_Main_Menu(pPlayer, pItem);
            break;
        }
        case GOSSIP_ACTION_INFO_DEF + 5:
        {
            pPlayer->CLOSE_GOSSIP_MENU();
            break;
        }
        default:
            break;
    }

    return true;
}

bool Hearthstone_Gossip_Select_With_code(Player* pPlayer, Item* pItem, uint32 uiSender, uint32 uiAction, const char* sCode)
{
    if (uiSender == GOSSIP_SENDER_MAIN)
    {
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+2:
            {
                if (std::strcmp(sCode, pPlayer->GetName()) != 0)
                {
                    pPlayer->GetSession()->SendAreaTriggerMessage("The typed name is not yours!");
                }
                else
                {
                    pPlayer->GetSession()->SendAreaTriggerMessage("The typed name is correct!");
                }
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
            }
        }
    }

    return false;
}

void AddSC_hearthstone_gossip()
{
    Script* pNewScript = new Script;
    pNewScript = new Script;
    pNewScript->Name = "Hearthstone_Gossip";
    pNewScript->pItemUse = &Hearthstone_On_Use;
    pNewScript->pGossipSelectItem = &Hearthstone_Gossip_Select;
    pNewScript->pGossipSelectItemWithCode = &Hearthstone_Gossip_Select_With_code;
    pNewScript->RegisterSelf(false);
}