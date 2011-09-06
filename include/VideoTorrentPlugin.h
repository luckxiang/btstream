/*
 * Copyright 2011 Gabriel Mendonça
 *
 * This file is part of BIVoD.
 * BIVoD is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BIVoD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with BIVoD.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * VideoTorrentPlugin.h
 *
 *  Created on: 24/08/2011
 *      Author: gabriel
 */

#ifndef VIDEOTORRENTPLUGIN_H_
#define VIDEOTORRENTPLUGIN_H_

#include <boost/shared_ptr.hpp>

#include <libtorrent/extensions.hpp>

using namespace libtorrent;

namespace libtorrent {
class torrent;
}

namespace bivod {

class VideoTorrentPlugin: public torrent_plugin {
public:
	VideoTorrentPlugin(torrent* t);

	/**
	 * Returns a VideoPeerPlugin.
	 * Called when a new peer is connected to the torrent.
	 */
	virtual boost::shared_ptr<peer_plugin> new_connection(peer_connection* pc);

	/**
	 * Informs torrent that the received piece will be read.
	 * Called when a piece is received and pass the hash check.
	 */
	virtual void on_piece_pass(int index);

private:
	torrent* m_torrent;
};

boost::shared_ptr<torrent_plugin> create_video_plugin(torrent* t, void* params);

} /* namespace bivod */

#endif /* VIDEOTORRENTPLUGIN_H_ */
