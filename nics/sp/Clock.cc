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

#include "Clock.h"

// Ticks per second
const int Clock::Hz = 2;

Clock::Clock() {
  clock = last_start = elapsed = 0;
  started = running = false;
}

Clock::Clock(long sec) {
  clock = last_start = elapsed = 0;
  started = running = false;
  seconds(sec);
}

Clock::Clock( const Clock &copy ) {
  clock = copy.clock;
  last_start = copy.last_start;
  elapsed = copy.elapsed;
  started = running = false;
}

Clock::~Clock() {
  Resume(); // So that the thread exits correctly
}

void Clock::start() {
  last_start = clock;

  if (!started)
    Start();
  else
    Resume();
  started = running = true;
}

void Clock::stop() {
  Suspend();
  running = false;
  elapsed = last_start - clock;
}

void Clock::Run() {
  unsigned long tick = 1000 / Hz;

  for(;;) {
    Sleep(tick);
    --clock;
  }
}
