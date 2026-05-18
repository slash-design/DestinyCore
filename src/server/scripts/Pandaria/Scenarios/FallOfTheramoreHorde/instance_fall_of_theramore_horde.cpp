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

#include "fall_of_theramore_horde.h"

class instance_fall_of_theramore_horde : public InstanceMapScript
{
public:
    instance_fall_of_theramore_horde() : InstanceMapScript("instance_fall_of_theramore_horde", 999) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_fall_of_theramore_horde_InstanceMapScript(map);
    }

    struct instance_fall_of_theramore_horde_InstanceMapScript : public InstanceScript
    {
        instance_fall_of_theramore_horde_InstanceMapScript(InstanceMap* map) : InstanceScript(map) {}
    };
};

void AddSC_instance_fall_of_theramore_horde()
{
}
