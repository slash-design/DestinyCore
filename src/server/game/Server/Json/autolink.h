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

#ifndef JSON_AUTOLINK_H_INCLUDED
# define JSON_AUTOLINK_H_INCLUDED

# include "jsonconfig.h"

# ifdef JSON_IN_CPPTL
#  include <cpptl/cpptl_autolink.h>
# endif

# if !defined(JSON_NO_AUTOLINK)  &&  !defined(JSON_DLL_BUILD)  &&  !defined(JSON_IN_CPPTL)
#  define CPPTL_AUTOLINK_NAME "json"
#  undef CPPTL_AUTOLINK_DLL
#  ifdef JSON_DLL
#   define CPPTL_AUTOLINK_DLL
#  endif
#  include "autolink.h"
# endif

#endif // JSON_AUTOLINK_H_INCLUDED
