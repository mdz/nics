// -*- C++ -*-

#ifndef _REFERENCECOUNTED_H
#define _REFERENCECOUNTED_H

#include <cc++/thread.h>

namespace ReferenceCounted {

  template<class T> class object;

  template<class T>
  class pointer {
  public:
    inline explicit pointer(object<T>& _parent):
      parent(_parent) {

      parent.ReadLock();
    };

    inline pointer(object<T>* _parent):
      parent(*_parent) {

      parent.ReadLock();
    };

    inline pointer(const pointer<T>& copy):
      parent(copy.parent) {

      parent.ReadLock();
    };

    inline ~pointer() {
      parent.Unlock();
    };

    inline bool operator== (const pointer& other) const {
      return (parent == other.parent);
    };

    inline bool operator!= (const pointer& other) const {
      return (parent != other.parent);
    };

    inline T& operator* (void) const {
      return parent.instance;
    };

    inline T* operator-> (void) const {
      return &parent.instance;
    };

    inline T* ptr(void) const {
      return &parent.instance;
    };

  private:
    object<T>& parent;
  };

  template<class T>
  class object: private ThreadLock {
  public:

    friend class pointer<T>;

    typedef ReferenceCounted::pointer<T> pointer;

//      inline ReferenceCounted::pointer<T> operator& (void) {
//        return ReferenceCounted::pointer<T>(*this);
//      };

    inline object(T& _instance): instance(_instance) {};

    inline ~object(void) { WriteLock(); };

  private:
    T &instance;
  };
}

#endif // !_REFERENCECOUNTED_H
