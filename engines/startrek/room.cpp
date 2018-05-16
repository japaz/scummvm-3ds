/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * $URL: https://scummvm-startrek.googlecode.com/svn/trunk/graphics.h $
 * $Id: graphics.h 2 2009-09-12 20:13:40Z clone2727 $
 *
 */

#include "startrek/filestream.h"
#include "startrek/room.h"
#include "startrek/startrek.h"


namespace StarTrek {

Room::Room(StarTrekEngine *vm, Common::String name) : _vm(vm) {
	SharedPtr<FileStream> rdfFile = _vm->loadFile(name + ".RDF");

	int size = rdfFile->size();
	_rdfData = new byte[size];
	rdfFile->read(_rdfData, size);
}

Room::~Room() {
	delete[] _rdfData;
}

uint16 Room::readRdfWord(int offset) {
	return _rdfData[offset] | (_rdfData[offset+1]<<8);
}

Common::Point Room::getBeamInPosition(int crewmanIndex) {
	int base = 0xaa + crewmanIndex * 4;
	return Common::Point(readRdfWord(base), readRdfWord(base + 2));
}

}