/*
 *  Originally written  for TrinityCore by ShinDarth and GigaDev90 (www.trinitycore.org)
 *  Converted as module for AzerothCore by ShinDarth and Yehonal   (www.azerothcore.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "Player.h"
#include "Pet.h"
#include "SpellInfo.h"
#include "Configuration/Config.h"

class DuelResetScript : public PlayerScript
{
    public:
        DuelResetScript() : PlayerScript("DuelResetScript") { }

        // Called when a duel starts (after 3s countdown)
        void OnDuelStart(Player* player1, Player* player2) override
        {
            // Cooldowns reset
            if (sConfigMgr->GetBoolDefault("DuelResetCooldowns", true))
            {
                player1->RemoveArenaSpellCooldowns();
                player2->RemoveArenaSpellCooldowns();
                player1->RemoveAura(10278);             // Hand of Protection
                player2->RemoveAura(10278);
                player1->RemoveAura(498);               // Divine Protection
                player2->RemoveAura(498);
                player1->RemoveAura(642);               // Divine Shield
                player2->RemoveAura(642);
                player1->RemoveAura(41425);             // Remove Hypothermia Debuff
                player2->RemoveAura(41425);
                player1->RemoveAura(25771);             // Remove Forbearance Debuff
                player2->RemoveAura(25771);
                player1->RemoveAura(57724);             // Remove Sated Debuff
                player2->RemoveAura(57724);
                player1->RemoveAura(57723);             // Remove Exhaustion Debuff
                player2->RemoveAura(57723);
                player1->RemoveAura(66233);             // Remove Ardent Defender Debuff
                player2->RemoveAura(66233);
                player1->RemoveAura(61987);             // Avenging Wrath Marker
                player2->RemoveAura(61987);
                player1->RemoveAura(11196);             // Remove Recently Bandaged Debuff
                player2->RemoveAura(11196);
                player1->ClearDiminishings();
                player2->ClearDiminishings();
            }

            // Health, mana, runic power, rage, reset
            if (sConfigMgr->GetBoolDefault("DuelResetHealthMana", true))
            {
                player1->SetHealth(player1->GetMaxHealth());
                player1->SetPower(POWER_MANA, player1->GetMaxPower(POWER_MANA));
                player2->SetHealth(player2->GetMaxHealth());
                player2->SetPower(POWER_MANA,  player2->GetMaxPower(POWER_MANA));
                player1->SetPower(POWER_RAGE, 0);       // Remove Rage and Runic Power
                player2->SetPower(POWER_RAGE, 0);
                player1->SetPower(POWER_RUNIC_POWER, 0);
                player2->SetPower(POWER_RUNIC_POWER, 0);

            }
        }
};

class DuelResetWorld : public WorldScript
{
public:
    DuelResetWorld() : WorldScript("DuelResetWorld") { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            std::string conf_path = _CONF_DIR;
            std::string cfg_file = conf_path + "/duelreset.conf";
            std::string cfg_def_file = cfg_file +".dist";

            sConfigMgr->LoadMore(cfg_def_file.c_str());

            sConfigMgr->LoadMore(cfg_file.c_str());
        }
    }
};

void AddSC_DuelReset()
{
    new DuelResetWorld();
    new DuelResetScript();
}
