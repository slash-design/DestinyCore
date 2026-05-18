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

#include "crypt_of_forgotten_kings.h"

class instance_crypt_of_forgotten_kings : public InstanceMapScript
{
public:
    instance_crypt_of_forgotten_kings() : InstanceMapScript("instance_crypt_of_forgotten_kings", 1030) {}

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_crypt_of_forgotten_kings_InstanceMapScript(map);
    }

    struct instance_crypt_of_forgotten_kings_InstanceMapScript : public InstanceScript
    {
        instance_crypt_of_forgotten_kings_InstanceMapScript(InstanceMap* map) : InstanceScript(map) {}
    };
};

void AddSC_instance_crypt_of_forgotten_kings()
{
}
