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

#include "dark_heart_of_pandaria.h"

class instance_dark_heart_of_pandaria : public InstanceMapScript
{
public:
    instance_dark_heart_of_pandaria() : InstanceMapScript("instance_dark_heart_of_pandaria", 1144) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_dark_heart_of_pandaria_InstanceMapScript(map);
    }

    struct instance_dark_heart_of_pandaria_InstanceMapScript : public InstanceScript
    {
        instance_dark_heart_of_pandaria_InstanceMapScript(InstanceMap* map) : InstanceScript(map) {}
    };
};

void AddSC_instance_dark_heart_of_pandaria()
{
}
