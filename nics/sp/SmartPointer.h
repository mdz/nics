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

#ifndef _SMARTPOINTER_H
#define _SMARTPOINTER_H

#include <cc++/thread.h>

template<class T>
class SmartPointer: private Mutex {

public:
  inline explicit SmartPointer( T* ptr ) {
    EnterMutex();
    shared = new Shared( ptr );
    LeaveMutex();
  };

  inline ~SmartPointer( void ) {
    release();
  };

  inline SmartPointer( SmartPointer<T>& ptr ) {
    EnterMutex();
    shared = ptr.acquire();
    LeaveMutex();
  };

  inline bool operator== ( SmartPointer<T>& ptr ) {
    return (this->shared == ptr.shared);
  }

  inline bool operator!= ( SmartPointer<T>& ptr ) {
    return (this->shared != ptr.shared);
  }

  inline SmartPointer<T>& operator= ( SmartPointer<T>& ptr ) {
    if ( *this != ptr ) {
      release();
      EnterMutex();
      shared = ptr.acquire();
      LeaveMutex();
    }
    
    return *this;
  };

  inline T& operator* ( void ) const {
    return *shared->ptr;
  };

  inline T* operator-> ( void ) const {
    return shared->ptr;
  };

  inline T* get( void ) const {
    if ( shared ) {
      return shared->ptr;
    } else {
      return 0;
    }
  };

protected:
  class Shared;

  inline Shared* acquire() {
    Shared* ret = NULL;

    EnterMutex();

    if ( shared ) {
      shared->EnterMutex();
      ++shared->count;
      ret = shared;			
      shared->LeaveMutex();
    }

    LeaveMutex();
    
    return ret;
  };

  inline void release( void ) {

    EnterMutex();

    if ( shared ) {
      bool nuked = false;
      shared->EnterMutex();
      if ( shared->count ) {
	if ( --shared->count == 0 ) {
	  shared->LeaveMutex();
	  delete shared;
	  nuked = true;
	}
      }

      if ( !nuked )
	shared->LeaveMutex();

      shared = NULL;
    }

    LeaveMutex();
  };

protected:

  class Shared: public Mutex {
  public:
    explicit Shared( T* _ptr ) {
      ptr = _ptr;
      count = 1;
    };

    ~Shared() {
      delete ptr;
    };
    
    T* ptr;
    unsigned int count;
  } *shared;

};

#endif // !_SMARTPOINTER_H
