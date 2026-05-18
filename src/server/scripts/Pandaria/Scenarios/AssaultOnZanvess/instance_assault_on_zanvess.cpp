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

#include "assault_on_zanvess.h"

class instance_assault_on_zanvess : public InstanceMapScript
{
public:
    instance_assault_on_zanvess() : InstanceMapScript("instance_assault_on_zanvess", 1050) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_assault_on_zanvess_InstanceMapScript(map);
    }

    struct instance_assault_on_zanvess_InstanceMapScript : public InstanceScript
    {
        instance_assault_on_zanvess_InstanceMapScript(InstanceMap* map) : InstanceScript(map) {}
    };
};

void AddSC_instance_assault_on_zanvess()
{
}
