#include <iostream>

#include "eurexModule.h"

// This method must be declared with the following signature, otherwise you'll get
// compilation errors due to private scope.
// This is where you can instantiate the EmlServer, with the correct callable
// methods (handlers).
template <typename Module> static void SetupHandlers(Module* module)
{
    // set up handlers
    auto insertFunction = [module](AccessKey key, InsertArgs args)
    {
      module->SendInsertToMarket(key, args);
    };

    // create eml server
    module->mEmlServer.reset(new EmlServer<decltype(insertFunction)>(*module, insertFunction));
}

void EurexModule::Initialise(IExecModuleOrderHandler* execModuleOrderHandler, IOrderChecker* orderChecker)
{
  mOrderChecker = orderChecker;
  mExecModuleOrderHandler = execModuleOrderHandler;
}

void EurexModule::InsertOrder(int volume, double price, int tag, bool side)
{
  // order looks good, ask framework to check (expect callback on success)
  InsertArgs args { volume, price, side };
  if (!mOrderChecker->CheckInsertOrder(args, tag))
    {
      // order insert failed
      mExecModuleOrderHandler->OnOrderError(tag);
    }
}

// this is only called by the framework, it's impossible to call directly
void EurexModule::SendInsertToMarket(AccessKey, InsertArgs args)
{
  std::cout << args.volume << "@" << args.price << " " << args.side << std::endl;
}

IEmlServer& EurexModule::GetEmlServer()
{
  return *mEmlServer.get();
}

EurexModule::EurexModule()
{
  SetupHandlers(this);
}
