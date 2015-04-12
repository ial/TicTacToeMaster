#pragma once

#include <memory>

struct InterfaceBase
{
};

template <typename T, typename Base = InterfaceBase>
struct Interface : public Base
{
  Interface()
  {
  }

  virtual ~Interface() {}

  typedef std::unique_ptr<T> Ptr;
  typedef std::shared_ptr<T> Ref;
  typedef std::weak_ptr<T> WeakRef;
};
