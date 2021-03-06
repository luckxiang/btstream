/*
 * Copyright (C) 2011-2013 Gabriel Mendonça
 *
 * This file is part of BTStream.
 * BTStream is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * BTStream is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with BTStream.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * VideoBuffer.h
 *
 *  Created on: 29/09/2011
 *      Author: gabriel
 */

#ifndef VIDEOBUFFER_H_
#define VIDEOBUFFER_H_

#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/thread.hpp>

#include "exception.h"

namespace btstream {

/**
 * Data object that represents a torrent piece.
 */
struct Piece {
	Piece(int index, boost::shared_array<char> data, int size) :
			index(index), data(data), size(size) {}

	int index;
	boost::shared_array<char> data;
	int size;
};

/**
 * Stores references to downloaded video pieces.
 * VideoBuffer is a thread-safe container in which piece references can
 * be added and read by different threads at the same time.
 * VideoBuffer keeps track of current video playback position when the
 * get_next_piece method is used.
 */
class VideoBuffer {
public:

	/**
	 * Constructor.
	 * @param num_pieces Number of pieces in the video file.
	 */
	VideoBuffer(int num_pieces = 1) throw (Exception);

	/**
	 * Destructor.
	 * Unlocks any blocked calls to get_next_piece().
	 */
	~VideoBuffer();

	/**
	 * Adds a piece reference to the buffer. If there is no space left
	 * on the buffer then the method will block until the method
	 * get_next_piece is called.
	 *
	 * This method implements mutual exclusion and will block until
	 * resources are available.
	 * @param index the piece index.
	 * @param data a char array with piece data.
	 * @param size the size of the data array.
	 */
	void add_piece(int index, boost::shared_array<char> data, int size);

	/**
	 * Returns a pointer to the next piece that should be played.
	 *
	 * If all pieces have already been returned or the unlock() method
	 * was called, returns a default constructed (NULL) shared_ptr.
	 *
	 * This method will block (sleep) until the piece is available or
	 * the unlock() method is called.
	 */
	boost::shared_ptr<Piece> get_next_piece();

	/**
	 * Returns the index of the next piece that should be played.
	 */
	int get_next_piece_index();

	/**
	 * Unlocks any blocked calls to get_next_piece().
	 */
	void unlock();

	/**
	 * Returns true if unlock() was called and false otherwise.
	 * If false, a call to get_next_piece() may be blocked waiting for
	 * the next piece to be downloaded.
	 */
	bool unlocked();

private:
	const int m_buffer_size;

	std::queue<boost::shared_ptr<Piece> > m_pieces;
	int m_num_pieces;
	int m_next_piece_index;
	bool m_unlocked;

	mutable boost::mutex m_mutex;
	mutable boost::condition_variable m_next_piece_available;
	mutable boost::condition_variable m_buffer_not_full;
};

} /* namespace btstream */

#endif /* VIDEOBUFFER_H_ */
