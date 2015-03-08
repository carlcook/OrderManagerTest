#include <iostream>

#include "exampleModule.h"

// This method must be declared with the following signature, otherwise you'll get
// compilation errors due to private scope.
// This is where you can instantiate the OrderServer, with the correct callable
// methods (handlers).
template <typename Module> static void SetupHandlers(Module* module)
{
    // set up handlers
    auto insertHandler = [module](AccessKey accessKey, InsertArgs args)
    {
      module->SendInsertToMarket(accessKey, args);
    };

    // create eml server
    module->mOrderServer.reset(new OrderServer<decltype(insertHandler)>(*module, insertHandler));
}

void ExampleModule::Initialise(IMarketModuleResponseHandler* execModuleOrderHandler, IOrderChecker* orderChecker)
{
  mOrderChecker = orderChecker;
  mMarketModuleResponseHandler = execModuleOrderHandler;
}

void ExampleModule::InsertOrder(int volume, double price, int tag, bool side)
{
  // order looks good, ask framework to check (expect callback on success)
  InsertArgs args { volume, price, side, nullptr };
  if (!mOrderChecker->CheckInsertOrder(args, tag))
    {
      // order insert failed
      mMarketModuleResponseHandler->OnOrderError(tag);
    }
}

// this is only called by the framework, it's impossible to call directly
void ExampleModule::SendInsertToMarket(AccessKey, InsertArgs args)
{
  std::cout << args.mVolume << "@" << args.mPrice << " " << args.mSide << std::endl;
}

IOrderServer& ExampleModule::GetOrderServer()
{
  return *mOrderServer.get();
}

ExampleModule::ExampleModule()
{
  SetupHandlers(this);
}
