#ifndef TYPES_H
#define TYPES_H

class IEmlServer
{
public:
  ~IEmlServer() = default;
  virtual void InsertOrder(int volume, double price, int tag, bool side) = 0;
};

class IRiskChecker
{
public:
  ~IRiskChecker() = default;
  virtual bool CheckInsertOrder(int volume, double price, int tag, bool side) = 0;
};

class IExecModuleOrderHandler
{
public:
  ~IExecModuleOrderHandler() = default;
  virtual void OnOrderError(int tag) = 0;
};

class IExecModule
{
public:
  ~IExecModule() = default;
  virtual IEmlServer& GetEmlServer() = 0;
  virtual void Initialise(IExecModuleOrderHandler*, IRiskChecker*) = 0;
  virtual void InsertOrder(int volume, double price, int tag, bool side) = 0;
};

#endif // TYPES_H
