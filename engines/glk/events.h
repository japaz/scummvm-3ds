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
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

#ifndef GLK_EVENTS_H
#define GLK_EVENTS_H

#include "common/events.h"
#include "graphics/surface.h"
#include "glk/utils.h"

namespace Glk {

#define GAME_FRAME_RATE 100
#define GAME_FRAME_TIME (1000 / GAME_FRAME_RATE)

class Window;

/**
 * Event types
 */
enum EvType {
	evtype_None         = 0,
	evtype_Timer        = 1,
	evtype_CharInput    = 2,
	evtype_LineInput    = 3,
	evtype_MouseInput   = 4,
	evtype_Arrange      = 5,
	evtype_Redraw       = 6,
	evtype_SoundNotify  = 7,
	evtype_Hyperlink    = 8,
	evtype_VolumeNotify = 9,

	// ScummVM custom events
	evtype_Quit         = 99
};

/**
 * Keycodes
 */
enum Keycode {
	keycode_Unknown  = 0xffffffffU,
	keycode_Left     = 0xfffffffeU,
	keycode_Right    = 0xfffffffdU,
	keycode_Up       = 0xfffffffcU,
	keycode_Down     = 0xfffffffbU,
	keycode_Return   = 0xfffffffaU,
	keycode_Delete   = 0xfffffff9U,
	keycode_Escape   = 0xfffffff8U,
	keycode_Tab      = 0xfffffff7U,
	keycode_PageUp   = 0xfffffff6U,
	keycode_PageDown = 0xfffffff5U,
	keycode_Home     = 0xfffffff4U,
	keycode_End      = 0xfffffff3U,
	keycode_Func1    = 0xffffffefU,
	keycode_Func2    = 0xffffffeeU,
	keycode_Func3    = 0xffffffedU,
	keycode_Func4    = 0xffffffecU,
	keycode_Func5    = 0xffffffebU,
	keycode_Func6    = 0xffffffeaU,
	keycode_Func7    = 0xffffffe9U,
	keycode_Func8    = 0xffffffe8U,
	keycode_Func9    = 0xffffffe7U,
	keycode_Func10   = 0xffffffe6U,
	keycode_Func11   = 0xffffffe5U,
	keycode_Func12   = 0xffffffe4U,

	// non standard keycodes
	keycode_Erase          = 0xffffef7fU,
	keycode_MouseWheelUp   = 0xffffeffeU,
	keycode_MouseWheelDown = 0xffffefffU,
	keycode_SkipWordLeft   = 0xfffff000U,
	keycode_SkipWordRight  = 0xfffff001U,

	// The last keycode is always = 0x100000000 - keycode_MAXVAL)
	keycode_MAXVAL = 28U
};

/**
 * List of cursors
 */
enum CursorId {
	CURSOR_NONE = 0,
	CURSOR_ARROW = 1,
	CURSOR_IBEAM = 2,
	CURSOR_HAND = 3
};

/**
 * Event structure
 */
struct Event {
	EvType type;
	Window *window;
	uint val1, val2;

	/**
	 * Constructor
	 */
	Event() {
		clear();
	}

	/**
	 * Constructor
	 */
	Event(EvType evType, Window *evWindow, uint evVal1, uint evVal2) {
		type = evType;
		window = evWindow;
		val1 = evVal1;
		val2 = evVal2;
	}

	/**
	 * Clear
	 */
	void clear() {
		type = evtype_None;
		window = nullptr;
		val1 = val2 = 0;
	}

	/**
	 * Boolean cast to allow checking whether event is filled out
	 */
	operator bool() const {
		return type != evtype_None;
	}
};
typedef Event event_t;

class EventQueue : public Common::Queue<Event> {
public:
	/**
	 * Retrieve a pending event, if any
	 */
	Event retrieve() {
		return empty() ? Event() : pop();
	}
};

/**
 * Events manager
 */
class Events {
	struct Surface : public Graphics::Surface {
		Common::Point _hotspot;
	};
private:
	EventQueue _eventsPolled;       ///< User generated events
	EventQueue _eventsLogged;       ///< Custom events generated by game code
	Event *_currentEvent;           ///< Event pointer passed during event retrieval
	uint32 _priorFrameTime;         ///< Time of prior game frame
	uint32 _frameCounter;           ///< Frame counter
	bool _redraw;                   ///< Screen needed redrawing
	CursorId _cursorId;             ///< Current cursor Id
	Surface _cursors[4];            ///< Cursor pixel data
	uint _timerMilli;               ///< Time in milliseconds between timer events
	uint _timerTimeExpiry;          ///< When to trigger next timer event
private:
	/**
	 * Initialize the cursor graphics
	 */
	void initializeCursors();

	/**
	 * Checks for whether it's time for the next game frame
	 */
	void checkForNextFrameCounter();

	/**
	 * Dispatches an event
	 */
	void dispatchEvent(Event &ev, bool polled);

	/**
	 * Poll for user events
	 */
	void pollEvents();

	/**
	 * Handle a key down event
	 */
	void handleKeyDown(const Common::KeyState &ks);

	/**
	 * Handle scroll events
	 */
	void handleScroll(bool wheelUp);

	/**
	 * Handle mouse move events
	 */
	void handleMouseMove(const Point &pos);

	/**
	 * Handle mouse down events
	 */
	void handleButtonDown(bool isLeft, const Point &pos);

	/**
	 * Handle mouse up events
	 */
	void handleButtonUp(bool isLeft, const Point &pos);
public:
	bool _forceClick;
public:
	/**
	 * Constructor
	 */
	Events();

	/**
	 * Destructor
	 */
	~Events();

	/**
	  * Get any pending event
	  */
	void getEvent(event_t *event, bool polled);

	/**
	 * Store an event for retrieval
	 */
	void store(EvType type, Window *win, uint val1 = 0, uint val2 = 0);

	/**
	 * Wait for a keyboard or mouse press
	 */
	void waitForPress();

	/**
	 * Get the total number of frames played
	 */
	uint32 getTotalPlayTicks() const {
		return _frameCounter;
	}

	/**
	 * Set the total number of frames played
	 */
	void setTotalPlayTicks(uint frames) {
		_frameCounter = frames;
	}

	/**
	 * Flags the screen for redrawing
	 */
	void redraw() {
		_redraw = true;
	}

	/**
	 * Sets the current cursor
	 */
	void setCursor(CursorId cursorId);

	/**
	 * Set a timer interval
	 * @param   milli       Time in millieseconds for intervals, or 0 for off
	 */
	void setTimerInterval(uint milli);

	/**
	 * Returns true if it's time for a timer event
	 */
	bool isTimerExpired() const;
};

} // End of namespace Glk

#endif