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

#ifndef _NICSCORE_H
#define _NICSCORE_H

class NICSNetwork;
class NICSDatabase;
class NICSSessionManager;
class NICSGameManager;
class NICSLog;

class NICSCore {
private:
  static int exists;

 public:

  // Ensure that only one instance ever exists
  NICSCore() { if (exists) throw; else exists = 1; };
  ~NICSCore() { exists = 0; }

  int Run(void);

  // Use forward declarations to make the header dependencies more
  // manageable This way, modules only pull in the NICS*.h that they
  // need, and we don't need to recompile the whole world whenever any
  // of them change

  //static class NICSLoader *loader;
  static class NICSNetwork *network;
  static class NICSDatabase *database;
  static class NICSSessionManager *session_manager;
  static class NICSGameManager *game_manager;
  static class NICSLog *log;
  //static class NICSConfig *config;
};

#endif // !_NICSCORE_H
