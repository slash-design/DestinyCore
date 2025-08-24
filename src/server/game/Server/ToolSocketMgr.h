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

#ifndef __TOOLSOCKETMGR_H
#define __TOOLSOCKETMGR_H

class ToolSocket;

#include "SocketMgr.h"

/// Manages all sockets connected to peers and network threads
class TC_GAME_API ToolSocketMgr : public SocketMgr<ToolSocket>
{
	typedef SocketMgr<ToolSocket> BaseSocketMgr;

public:
	static ToolSocketMgr& Instance();

	/// Start network, listen at address:port .
	bool StartNetwork(Trinity::Asio::IoContext& ioContext, std::string const& bindIp, uint16 port, int networkThreads) override;

	/// Stops all network threads, It will wait for all running threads .
	void StopNetwork() override;

	void OnSocketOpen(tcp::socket&& sock, uint32 threadIndex) override;

protected:
	ToolSocketMgr();

	NetworkThread<ToolSocket>* CreateThreads() const override;

private:
	int32 _socketSendBufferSize;
	int32 m_SockOutUBuff;
	bool _tcpNoDelay;
};

#define sToolSocketMgr ToolSocketMgr::Instance()

#endif // __TOOLSOCKETMGR_H
