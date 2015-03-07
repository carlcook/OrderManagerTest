#ifndef TYPES_H
#define TYPES_H

class IEmlServer
{
public:
  virtual ~IEmlServer() = default;
  virtual void InsertOrder(int volume, double price, int tag, bool side) = 0;
};

class IRiskChecker
{
public:
  virtual ~IRiskChecker() = default;
  virtual bool CheckInsertOrder(int volume, double price, int tag, bool side) = 0;
};

class IExecModuleOrderHandler
{
public:
  virtual ~IExecModuleOrderHandler() = default;
  virtual void OnOrderError(int tag) = 0;
};

class IExecModule
{
public:
  virtual ~IExecModule() = default;
  virtual IEmlServer& GetEmlServer() = 0;
  virtual void Initialise(IExecModuleOrderHandler*, IRiskChecker*) = 0;
  virtual void InsertOrder(int volume, double price, int tag, bool side) = 0;
};

#endif // TYPES_H
