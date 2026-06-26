/*
 * This file is part of the DestinyCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// This is where scripts' loading functions should be declared:

void AddSC_mod_ollama_chat();
void AddSC_custom_npcs();
void AddSC_quest_conversation();
void AddSC_solocraft();
void AddLfgSoloScripts();
void AddSC_ai_director();
void AddCustomScripts()
{
    AddSC_custom_npcs(); 
    AddSC_quest_conversation();
    AddSC_solocraft();
    AddLfgSoloScripts();
    AddSC_mod_ollama_chat();
    AddSC_ai_director();
}


