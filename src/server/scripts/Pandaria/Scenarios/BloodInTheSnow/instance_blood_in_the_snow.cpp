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

#include "blood_in_the_snow.h"

class instance_blood_in_the_snow : public InstanceMapScript
{
public:
    instance_blood_in_the_snow() : InstanceMapScript("instance_blood_in_the_snow", 1130) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_blood_in_the_snow_InstanceMapScript(map);
    }

    struct instance_blood_in_the_snow_InstanceMapScript : public InstanceScript
    {
        instance_blood_in_the_snow_InstanceMapScript(InstanceMap* map) : InstanceScript(map) {}
    };
};

void AddSC_instance_blood_in_the_snow()
{
}
