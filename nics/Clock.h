// -*- C++ -*-

// NICS - the Next Internet Chess Server
// Copyright (C) 2001  Matt Zimmerman <mdz@csh.rit.edu>
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation; either version 2 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//

#ifndef _CLOCK_H
#define _CLOCK_H

#include <cc++/thread.h>

class Clock: protected ost::Mutex {
public:
  Clock() {
    started = running = false;
  };

  Clock( long sec ) {
    started = running = false;
    seconds(sec);
  };

  long seconds() const
  { return ticks / Hz; };

  void seconds( long sec )
  { ticks = sec * Hz; };

  void start() {
    EnterMutex();
    if (!running) {
      gettimeofday(&last_start, NULL);
      running = started = true;
    }
    LeaveMutex();
  }

  void stop() {
    EnterMutex();
    if (running) {
      gettimeofday(&last_stop, NULL);
      running = false;
      ticks -= elapsed();
    }
    LeaveMutex();
  };

  long remaining_seconds() const {
    return remaining() / Hz;
  };

  bool zero() {
    return remaining() <= 0;
  };

  long elapsed_seconds() const {
    if (running)
      return 0;
    else
      return elapsed() / Hz;
  };

  bool is_running() const { return running; };

protected:
  // Ticks per second
  static const int Hz;
  void Run();
  long elapsed() const {
    if (!started)
      return 0;

    struct timeval when;
    struct timeval diff;
    if (running) {
      gettimeofday(&when, NULL);
    } else {
      when = last_stop;
    }

    diff.tv_sec = when.tv_sec - last_start.tv_sec;
    diff.tv_usec = when.tv_usec - last_start.tv_usec;
    if (diff.tv_usec < 0) {
      --diff.tv_sec;
      diff.tv_usec += 1000000;
    }

    return diff.tv_sec * Hz + diff.tv_usec * Hz / 1000000;
  };

  long remaining() const {
    if (running)
      return ticks - elapsed();
    else
      return ticks;
  };

private:
  // Not counting what has elapsed while we are running
  long ticks;

  bool running;
  bool started;
  struct timeval last_start;
  struct timeval last_stop;
};

#endif // !_CLOCK_H
