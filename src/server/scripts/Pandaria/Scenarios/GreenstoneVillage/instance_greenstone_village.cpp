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

#include "greenstone_village.h"

class instance_greenstone_village : public InstanceMapScript
{
public:
    instance_greenstone_village() : InstanceMapScript("instance_greenstone_village", 1024) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_greenstone_village_InstanceMapScript(map);
    }

    struct instance_greenstone_village_InstanceMapScript : public InstanceScript
    {
        instance_greenstone_village_InstanceMapScript(InstanceMap* map) : InstanceScript(map) {}
    };
};

void AddSC_instance_greenstone_village()
{
}
