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

class Clock: public Thread {
public:
  Clock();
  Clock( long sec );
  Clock( const Clock & );
  ~Clock();

  long seconds() const
  { return clock / Hz; };

  void seconds( long seconds )
  { clock = seconds * Hz; };

  bool zero()
  { return clock <= 0; };

  void start();
  void stop();
  long elapsed_seconds() const { return elapsed / Hz; };
  bool is_running() const { return running; };

protected:
  // Ticks per second
  static const int Hz;
  void Run();

private:
  bool started;
  bool running;
  long clock;
  long last_start;
  long elapsed;
};

#endif // !_CLOCK_H
