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

#include "brewing_storm.h"

class instance_brewing_storm : public InstanceMapScript
{
public:
    instance_brewing_storm() : InstanceMapScript("instance_brewing_storm", 1005) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_brewing_storm_InstanceMapScript(map);
    }

    struct instance_brewing_storm_InstanceMapScript : public InstanceScript
    {
        instance_brewing_storm_InstanceMapScript(InstanceMap* map) : InstanceScript(map) {}
    };
};

void AddSC_instance_brewing_storm()
{
}
