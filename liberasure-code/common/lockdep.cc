// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab
/*
 * Ceph - scalable distributed file system
 *
 * Copyright (C) 2008-2011 New Dream Network
 *
 * This is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software
 * Foundation.  See file COPYING.
 *
 */
#include "lockdep.h"
#include "valgrind.h"
#include <pthread.h>
#include "unordered_map.h"
#include <string.h> 
#include <map>
#include <iostream>
/******* Constants **********/
#define MAX_LOCKS 4096 // increase me as needed
#define BACKTRACE_SKIP 2

/******* Globals **********/
bool g_lockdep;
struct lockdep_stopper_t
{
  // disable lockdep when this module destructs.
  ~lockdep_stopper_t()
  {
    g_lockdep = 0;
  }
};

static pthread_mutex_t lockdep_mutex = PTHREAD_MUTEX_INITIALIZER;
static lockdep_stopper_t lockdep_stopper;
static ceph::unordered_map<std::string, int> lock_ids;
static std::map<int, std::string> lock_names;
static std::map<int, int> lock_refs;
static char free_ids[MAX_LOCKS / 8]; // bit set = free
// static ceph::unordered_map<pthread_t, std::map<int, ceph::BackTrace *>> held;
static char follows[MAX_LOCKS][MAX_LOCKS / 8]; // follows[a][b] means b taken after a
// static ceph::BackTrace *follows_bt[MAX_LOCKS][MAX_LOCKS];
unsigned current_maxid;
int last_freed_id = -1;
static bool free_ids_inited;

int lockdep_dump_locks()
{
  pthread_mutex_lock(&lockdep_mutex);
  if (!g_lockdep)
    goto out;

  // for (auto p = held.begin(); p != held.end(); ++p)
  // {
  //   //lockdep_dout(0) << "--- thread " << p->first << " ---" << std::endl;
  //   for (auto q = p->second.begin();
  //        q != p->second.end();
  //        ++q)
  //   {
  //     //lockdep_dout(0) << "  * " << lock_names[q->first] << "\n";
  //     if (q->second)
  //       std::cout << *(q->second);
  //     std::cout << std::endl;
  //   }
  // }
out:
  pthread_mutex_unlock(&lockdep_mutex);
  return 0;
}

int lockdep_get_free_id(void)
{
  // if there's id known to be freed lately, reuse it
  if ((last_freed_id >= 0) &&
      (free_ids[last_freed_id / 8] & (1 << (last_freed_id % 8))))
  {
    int tmp = last_freed_id;
    last_freed_id = -1;
    free_ids[tmp / 8] &= 255 - (1 << (tmp % 8));
    //lockdep_dout(1) << "lockdep reusing last freed id " << tmp << std::endl;
    return tmp;
  }

  // walk through entire array and locate nonzero char, then find
  // actual bit.
  for (int i = 0; i < MAX_LOCKS / 8; ++i)
  {
    if (free_ids[i] != 0)
    {
      for (int j = 0; j < 8; ++j)
      {
        if (free_ids[i] & (1 << j))
        {
          free_ids[i] &= 255 - (1 << j);
          //lockdep_dout(1) << "lockdep using id " << i * 8 + j << std::endl;
          return i * 8 + j;
        }
      }
    }
  }

  // not found
  //lockdep_dout(0) << "failing miserably..." << std::endl;
  return -1;
}

static int _lockdep_register(const char *name)
{
  int id = -1;

  if (!g_lockdep)
    return id;
  ceph::unordered_map<std::string, int>::iterator p = lock_ids.find(name);
  if (p == lock_ids.end())
  {
    id = lockdep_get_free_id();
    if (id < 0)
    {
      //lockdep_dout(0) << "ERROR OUT OF IDS .. have 0"
      // << " max " << MAX_LOCKS << std::endl;
      for (auto &p : lock_names)
      {
        //lockdep_dout(0) << "  lock " << p.first << " " << p.second << std::endl;
      }
      //ceph_abort();
    }
    if (current_maxid <= (unsigned)id)
    {
      current_maxid = (unsigned)id + 1;
    }
    lock_ids[name] = id;
    lock_names[id] = name;
    //lockdep_dout(10) << "registered '" << name << "' as " << id << std::endl;
  }
  else
  {
    id = p->second;
    //lockdep_dout(20) << "had '" << name << "' as " << id << std::endl;
  }

  ++lock_refs[id];

  return id;
}

int lockdep_register(const char *name)
{
  int id;

  pthread_mutex_lock(&lockdep_mutex);
  id = _lockdep_register(name);
  pthread_mutex_unlock(&lockdep_mutex);
  return id;
}

void lockdep_unregister(int id)
{
  if (id < 0)
  {
    return;
  }

  pthread_mutex_lock(&lockdep_mutex);

  std::string name;
  auto p = lock_names.find(id);
  if (p == lock_names.end())
    name = "unknown";
  else
    name = p->second;

  int &refs = lock_refs[id];
  if (--refs == 0)
  {
    if (p != lock_names.end())
    {
      // reset dependency ordering
      // FIPS zeroization audit 20191115: this memset is not security related.
      memset((void *)&follows[id][0], 0, MAX_LOCKS / 8);
      for (unsigned i = 0; i < current_maxid; ++i)
      {
        // delete follows_bt[id][i];
        // follows_bt[id][i] = NULL;

        // delete follows_bt[i][id];
        // follows_bt[i][id] = NULL;
        follows[i][id / 8] &= 255 - (1 << (id % 8));
      }

      //lockdep_dout(10) << "unregistered '" << name << "' from " << id << std::endl;
      lock_ids.erase(p->second);
      lock_names.erase(id);
    }
    lock_refs.erase(id);
    free_ids[id / 8] |= (1 << (id % 8));
    last_freed_id = id;
  }
  else if (g_lockdep)
  {
    //lockdep_dout(20) << "have " << refs << " of '" << name << "' " <<
    // "from " << id << std::endl;
  }
  pthread_mutex_unlock(&lockdep_mutex);
}

// does b follow a?
static bool does_follow(int a, int b)
{
  if (follows[a][b / 8] & (1 << (b % 8)))
  {
    //lockdep_dout(0) << "\n";
    std::cout << "------------------------------------"
           << "\n";
    std::cout << "existing dependency " << lock_names[a] << " (" << a << ") -> "
           << lock_names[b] << " (" << b << ") at:\n";
    // if (follows_bt[a][b])
    // {
    //   follows_bt[a][b]->print(std::cout);
    // }
    std::cout << std::endl;
    return true;
  }

  for (unsigned i = 0; i < current_maxid; i++)
  {
    if ((follows[a][i / 8] & (1 << (i % 8))) &&
        does_follow(i, b))
    {
      //lockdep_dout(0) << "existing intermediate dependency " << lock_names[a]
      // << " (" << a << ") -> " << lock_names[i] << " (" << i << ") at:\n";
      // if (follows_bt[a][i])
      // {
      //   follows_bt[a][i]->print(std::cout);
      // }
      std::cout << std::endl;
      return true;
    }
  }

  return false;
}

int lockdep_will_lock(const char *name, int id, bool force_backtrace,
                      bool recursive)
{
  pthread_t p = pthread_self();

  pthread_mutex_lock(&lockdep_mutex);
  if (!g_lockdep)
  {
    pthread_mutex_unlock(&lockdep_mutex);
    return id;
  }

  if (id < 0)
    id = _lockdep_register(name);

  //lockdep_dout(20) << "_will_lock " << name << " (" << id << ")" << std::endl;

  // check dependency graph
  // auto &m = held[p];
  // for (auto p = m.begin(); p != m.end(); ++p)
  // {
  //   if (p->first == id)
  //   {
  //     if (!recursive)
  //     {
  //       //lockdep_dout(0) << "\n";
  //       std::cout << "recursive lock of " << name << " (" << id << ")\n";
  //       // auto bt = new ceph::BackTrace(BACKTRACE_SKIP);
  //       // bt->print(std::cout);
  //       if (p->second)
  //       {
  //         std::cout << "\npreviously locked at\n";
  //         // p->second->print(std::cout);
  //       }
  //       // delete bt;
  //       std::cout << std::endl;
  //       //ceph_abort();
  //     }
  //   }
  //   else if (!(follows[p->first][id / 8] & (1 << (id % 8))))
  //   {
  //     // new dependency

  //     // did we just create a cycle?
  //     if (does_follow(id, p->first))
  //     {
  //       // auto bt = new ceph::BackTrace(BACKTRACE_SKIP);
  //       // //lockdep_dout(0) << "new dependency " << lock_names[p->first]
  //       // << " (" << p->first << ") -> " << name << " (" << id << ")"
  //       // << " creates a cycle at\n";
  //       // bt->print(std::cout);
  //       std::cout << std::endl;

  //       //lockdep_dout(0) << "btw, i am holding these locks:" << std::endl;
  //       for (auto q = m.begin(); q != m.end(); ++q)
  //       {
  //         //lockdep_dout(0) << "  " << lock_names[q->first] << " (" << q->first << ")" << std::endl;
  //         if (q->second)
  //         {
  //           //lockdep_dout(0) << " ";
  //           // q->second->print(std::cout);
  //           std::cout << std::endl;
  //         }
  //       }

  //       //lockdep_dout(0) << "\n" << std::endl;

  //       // don't add this dependency, or we'll get aMutex. cycle in the graph, and
  //       // does_follow() won't terminate.

  //       //ceph_abort(); // actually, we should just die here.
  //     }
  //     else
  //     {
  //       // ceph::BackTrace *bt = NULL;
  //       // if (force_backtrace || lockdep_force_backtrace())
  //       // {
  //       //   // bt = new ceph::BackTrace(BACKTRACE_SKIP);
  //       // }
  //       follows[p->first][id / 8] |= 1 << (id % 8);
  //       // follows_bt[p->first][id] = bt;
  //       //lockdep_dout(10) << lock_names[p->first] << " -> " << name << " at" << std::endl;
  //       //bt->print(std::cout);
  //     }
  //   }
  // }
  pthread_mutex_unlock(&lockdep_mutex);
  return id;
}

int lockdep_locked(const char *name, int id, bool force_backtrace)
{
  pthread_t p = pthread_self();

  pthread_mutex_lock(&lockdep_mutex);
  if (!g_lockdep)
    goto out;
  if (id < 0)
    id = _lockdep_register(name);

  //lockdep_dout(20) << "_locked " << name << std::endl;
  // if (force_backtrace || lockdep_force_backtrace())
  //   held[p][id] = new ceph::BackTrace(BACKTRACE_SKIP);
  // else
  //   held[p][id] = 0;
out:
  pthread_mutex_unlock(&lockdep_mutex);
  return id;
}

int lockdep_will_unlock(const char *name, int id)
{
  pthread_t p = pthread_self();

  if (id < 0)
  {
    //id = lockdep_register(name);
    //ceph_assert(id == -1);
    return id;
  }

  pthread_mutex_lock(&lockdep_mutex);
  if (!g_lockdep)
    goto out;
  //lockdep_dout(20) << "_will_unlock " << name << std::endl;

  // don't assert.. lockdep may be enabled at any point in time
  //assert(held.count(p));
  //assert(held[p].count(id));

  // delete held[p][id];
  // held[p].erase(id);
out:
  pthread_mutex_unlock(&lockdep_mutex);
  return id;
}
