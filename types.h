#ifndef TYPES_H
#define TYPES_H

class IEmlServer
{
public:
  virtual ~IEmlServer() = default;
  virtual void InsertOrder(int volume, double price, int tag, bool side) = 0;
};

struct InsertArgs
{
  const int& volume;
  const double& price;
  const bool& side;
};

class IOrderChecker
{
public:
  virtual ~IOrderChecker() = default;
  virtual bool CheckInsertOrder(InsertArgs, int tag) = 0;
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
  virtual void Initialise(IExecModuleOrderHandler*, IOrderChecker*) = 0;
  virtual void InsertOrder(int volume, double price, int tag, bool side) = 0;
};

// The "key passing idiom", with friend access to templated types
class AccessKey   {
private:
  AccessKey() = default;
  AccessKey(const AccessKey&) = default;
  template<typename Module> friend void SetupHandlers(Module*); // let handler setup have access
  template<typename Module> friend class EmlServer; // let server have access
};


#endif // TYPES_H
