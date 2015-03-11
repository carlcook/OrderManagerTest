#include <iostream>

#include <boost/asio.hpp>
#include <boost/array.hpp>

#include "exampleModule.h"

// This method must be declared with the following signature, otherwise you'll get
// compilation errors due to private scope.
// This is where you can instantiate the OrderServer, with the correct callable
// methods (handlers).
template <typename MarketModule> static void SetupOrderHandlers(MarketModule* marketModule)
{
    // set up handlers
    auto insertHandler = [marketModule](AccessKey accessKey, const InsertArgs& args)
    {
      marketModule->SendInsertToMarket(accessKey, args);
    };

    // create order server
    marketModule->mOrderServer.reset(new OrderServer<decltype(insertHandler)>(*marketModule, insertHandler));
}

ExampleModule::ExampleModule()
{
  SetupOrderHandlers(this);
}

void ExampleModule::Initialise(IMarketModuleResponseHandler* execModuleOrderHandler, IOrderExecutor* orderExecutor)
{
  mOrderExecutor = orderExecutor;
  mMarketModuleResponseHandler = execModuleOrderHandler;
}

IOrderServer& ExampleModule::GetOrderServer()
{
  return *mOrderServer.get();
}

void ExampleModule::InsertOrder(int volume, double price, int tag, bool side)
{
  // check params
  if (volume == 0)
    return; // and probably error

  // order looks good, ask framework to check (expect callback to message handler on success), and
  // note that we have sent custom data along for the ride
  std::string message = "GET / HTTP/1.1\r\nHost: www.google.nl\r\n\r\n";
  ExtendedInsertArgs insertArgs(volume, price, side, 1, message);
  if (!mOrderExecutor->AttemptInsertOrder(insertArgs, tag))
    {
      // order insert failed
      mMarketModuleResponseHandler->OnOrderError(tag);
    }
}

// this is only called by the framework, it's impossible to call directly without deliberate hacking
// NOTE: just serialisation code (in this case, just send a request to google)
void ExampleModule::SendInsertToMarket(AccessKey, const InsertArgs& args)
{
  // connect to the remote server
  using boost::asio::ip::tcp;
  boost::asio::io_service io_service;
  tcp::resolver resolver(io_service);
  tcp::resolver::query query("www.google.com", "http");
  tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
  tcp::resolver::iterator end;
  tcp::socket socket(io_service);
  boost::system::error_code error = boost::asio::error::host_not_found;
  while (error && endpoint_iterator != end)
  {
    socket.close();
    socket.connect(*endpoint_iterator++, error);
  }
  if (error)
    throw boost::system::system_error(error);

  // transmit the data
  auto extendedArgs = static_cast<const ExtendedInsertArgs&>(args);
  std::string message = extendedArgs.mMessage;
  boost::system::error_code ignored_error;
  boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);

  // just out of interest, see what response we received from google
  boost::array<char, 2048> buf;
  size_t len = socket.read_some(boost::asio::buffer(buf), error);
  if (error && error != boost::asio::error::eof)
     throw boost::system::system_error(error); // Some other error.
  std::cout.write(buf.data(), len) << std::endl;
}
