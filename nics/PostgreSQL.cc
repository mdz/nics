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

#include "PostgreSQL.h"
#include "NICSCore.h"
#include "NICSLog.h"
#include "Chess.h"
#include <libpq++.h>
#include <sstream>
#include <iomanip>
#include <cstdlib>

PostgreSQL::PostgreSQL(const char *connect):
  NICSDatabase(),
  database(connect) {
  if (database.ConnectionBad()) {

    NICSCore::log->log( NICSLog::Database, NICSLog::Critical,
			string("Database connection failed: ")
			+ database.ErrorMessage() );
    throw 0;
  }
}

void PostgreSQL::get_user(const string &name, User &user) throw(UserNotFound) {
  string query;

  query = "SELECT * FROM users WHERE name = '" + name + "'";

  lock();
  if ( database.Exec(query.c_str()) != PGRES_TUPLES_OK) {

    NICSCore::log->log( NICSLog::Database, NICSLog::Error,
			string("Database query failed: ")
			+ database.ErrorMessage() );
    
  } else if (database.Tuples() > 0) {

    int uid = atoi( database.GetValue(0, database.FieldNum("uid")) );
    string n( database.GetValue(0, database.FieldNum("name")) );
    string p( database.GetValue(0, database.FieldNum("password")) );

    if (n == name) {
      user.uid(uid);
      user.name(n);
      user.password(p);

      unlock();
      return;
    }
  }

  unlock();

  throw UserNotFound("No such user: " + name);
}

void PostgreSQL::get_user( int uid, User &user ) throw(UserNotFound) {
  ostringstream query;

  query << "SELECT * FROM users WHERE uid = " << uid;

  lock();
  if ( database.Exec(query.str().c_str()) != PGRES_TUPLES_OK) {

    NICSCore::log->log( NICSLog::Database, NICSLog::Error,
			string("Database query failed: ")
			+ database.ErrorMessage() );
    
  } else if (database.Tuples() > 0) {

    int uid = atoi( database.GetValue(0, database.FieldNum("uid")) );
    string n( database.GetValue(0, database.FieldNum("name")) );
    string p( database.GetValue(0, database.FieldNum("password")) );

    user.uid(uid);
    user.name(n);
    user.password(p);
    
    unlock();
    return;
  }

  unlock();

  throw UserNotFound("No such user with uid " + uid);
}

void PostgreSQL::add_user(const User &user) throw() {
  string query = "INSERT INTO users (uid, name, password) ";
  query += "VALUES (NEXTVAL('uid'), '" + user.name()
    + "', '" + user.password() + "')";

  lock();

  if ( database.Exec(query.c_str()) != PGRES_COMMAND_OK ) {

    NICSCore::log->log( NICSLog::Database, NICSLog::Error,
			string("Database query failed: ")
      + database.ErrorMessage() );

    return;
  }

  unlock();

  NICSCore::log->log( NICSLog::Database, NICSLog::Info,
		      "Added new user: " + user.name() );
}

void PostgreSQL::save_game( const Chess &game ) throw() {
  return;
//    int serial = next_game();

//    User white_user;
//    User black_user;

//    try {
//      get_user( game.white_name(), white_user );
//      get_user( game.black_name(), black_user );
//    } catch (UserNotFound e) {
//      NICSCore::log->log( NICSLog::Database, NICSLog::Error,
//  			string("User not found while saving game: ")
//  			+ e.what() );
//    }

//    time_t now;
//    char timestamp[1024];
//    time(&now);
//    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S -0:00",
//  	   gmtime(&now));

//    ostrstream query;
//    query << "INSERT INTO games (id, type, white, white_rating,"
//  	<< " black, black_rating,"
//  	<< " winner, result,"
//  	<< " initial_time, increment,"
//  	<< " timestamp)"
//  	<< " VALUES ("
//  	<< serial << ", "
//  	<< Game::Chess << ", "
//  	<< white_user.uid() << ", "
//  	<< 0 << ", "
//  	<< black_user.uid() << ", "
//  	<< 0 << ", "
//  	<< game.get_winner() << ", "
//  	<< game.get_result() << ", "
//  	<< game.initial_time() << ", " << game.increment() << ", "
//  	<< '\'' << timestamp << '\''
//  	<< ")" << ends;
  
//    lock();
//    if ( database.Exec(query.str()) != PGRES_COMMAND_OK ) {
//      NICSCore::log->log( NICSLog::Database, NICSLog::Error,
//  			string("Database query failed: ")
//  			       + database.ErrorMessage() );

//      unlock();
//      return;
//    }

//    unlock();
//    for( unsigned int i = 0 ; i < game.moves() ; ++i ) {
//      const ChessMove &move = game.move(i);
//      ostrstream query2;
//      query2 << "INSERT INTO moves_chess (game_id, movenum,
//  start_x, start_y, end_x, end_y) VALUES (" << serial << ", "
//  	   << i << ", "
//  	   << move.get_start_x() << ", "
//  	   << move.get_start_y() << ", "
//  	   << move.get_end_x() << ", "
//  	   << move.get_end_y() << ")" << ends;

//      lock();
//      if ( database.Exec(query2.str()) != PGRES_COMMAND_OK ) {
//        NICSCore::log->log( NICSLog::Database, NICSLog::Error,
//  			  string("Database query failed: ")
//  			  + database.ErrorMessage() );
//        unlock();
//        return;
//      }
//      unlock();
//    }
}

int PostgreSQL::next_game() {
  string query = "SELECT NEXTVAL('game')";

  lock();
  if ( database.Exec(query.c_str()) != PGRES_TUPLES_OK ||
       database.Tuples() < 1 ) {
    NICSCore::log->log( NICSLog::Database, NICSLog::Error,
			string("Database query failed: ")
			+ database.ErrorMessage() );
    unlock();
    throw runtime_error("Unable to retrieve next sequence number");
  }

  int nextval = atoi( database.GetValue(0, 0) );

  unlock();

  return nextval;
}

void PostgreSQL::dump_history( const User &user, ostream &out,
			       unsigned int howmany ) throw(UserNotFound) {
  ostringstream query;

  query << "SELECT white.name, white.uid, game.white_rating,"
	<< " black.name, black.uid, game.black_rating,"
	<< " game.winner, game.initial_time, game.increment,"
	<< " game.timestamp"
	<< " FROM users white, users black, games game"
	<< " WHERE white.uid = game.white AND black.uid=game.black"
	<< " AND ( "
	<< "white.uid = " << user.uid()
	<< " OR "
	<< "black.uid = " << user.uid()
	<< " ) ORDER BY game.timestamp";

  lock();
  if ( database.Exec(query.str().c_str()) != PGRES_TUPLES_OK ) {
    NICSCore::log->log( NICSLog::Database, NICSLog::Error,
			string("Database query failed: ")
			+ database.ErrorMessage() );

    unlock();
    return;
  }

  for( unsigned int i = 0 ;
       i < howmany && i < static_cast<unsigned int>(database.Tuples()) ;
       ++i ) {
    const char *white = database.GetValue(i, 0);
    int white_uid = atoi( database.GetValue(i, 1) );
    int white_rating = atoi( database.GetValue(i, 2) );
    const char *black = database.GetValue(i, 3);
    //int black_uid = atoi( database.GetValue(i, 4) );
    int black_rating = atoi( database.GetValue(i, 5) );

    int winner = atoi( database.GetValue(i, 6) );
    int initial_time = atoi( database.GetValue(i, 7) );
    int increment = atoi( database.GetValue(i, 8) );
    const char *timestamp = database.GetValue(i, 9);

    int she_won = 0;
    int played_white = ( user.uid() == white_uid );

    if ( winner == Chess::White && played_white
	 ||
	 winner == Chess::Black && !played_white )
      she_won = 1;

    out << setfill(' ') 
	<< setw(2) << i << ": "
	<< ( she_won ? '+' : '-' ) << ' '
	<< setw(4) << white_rating << ' '
	<< ( played_white ? "W " : "B " )
	<< setw(4) << black_rating << ' ';

    out.setf(ios::left);

    out << setw(8) << ( played_white ? black : white ) << ' '
	<< "[ cu ";

    out.setf(ios::right);

    if ( initial_time < 60 )
      out << setw(2) << initial_time << 's';
    else
      out << setw(2) << initial_time / 60;

    out << ' '
	<< setw(2) << increment
	<< "] "
	<< timestamp
	<< endl;
  }

  unlock();
}

string PostgreSQL::last_opponent( const User &user ) throw() {
  ostringstream query;

  query << "SELECT opponent.name"
	<< " FROM users opponent, games game"
	<< " WHERE ( game.white = " << user.uid()
	<< "   AND opponent.uid = game.black )"
	<< "  OR ( game.black = " << user.uid()
	<< "   AND opponent.uid = game.white )"
	<< " ORDER BY game.timestamp DESC"
	<< " LIMIT 1";

  lock();
  if ( database.Exec(query.str().c_str()) != PGRES_TUPLES_OK ) {
    NICSCore::log->log( NICSLog::Database, NICSLog::Error,
			string("Query failed: ") + database.ErrorMessage() );
    return "";
  }
  
  if ( database.Tuples() > 0 ) {
    unlock();
    return string( database.GetValue(0, 0) );
  }
  
  unlock();
  return "";
}
