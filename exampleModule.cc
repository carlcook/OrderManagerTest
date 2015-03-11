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
    auto insertHandler = [marketModule](AccessKey accessKey, InsertArgs args)
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

// TODO pass the message data as a templated type to avoid casts?
struct MessageData
{
  int mId = 0;
  std::string mMessage = "";
};

void ExampleModule::InsertOrder(int volume, double price, int tag, bool side)
{
  // order looks good, ask framework to check (expect callback to message handler on success)
  MessageData messageData;
  messageData.mId = 1;
  messageData.mMessage = "GET / HTTP/1.1\r\nHost: www.google.nl\r\n\r\n";
  InsertArgs args { volume, price, side, &messageData };
  if (!mOrderExecutor->AttemptInsertOrder(args, tag))
    {
      // order insert failed
      mMarketModuleResponseHandler->OnOrderError(tag);
    }
}

// this is only called by the framework, it's impossible to call directly
void ExampleModule::SendInsertToMarket(AccessKey, InsertArgs args)
{
  // NOTE: just serialisation code
  std::cout << args.mVolume << "@" << args.mPrice << " " << args.mSide << std::endl;
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

  auto messageData = static_cast<const MessageData*>(args.mData);

  std::string message = messageData->mMessage;
  boost::system::error_code ignored_error;
  boost::asio::write(socket, boost::asio::buffer(message), boost::asio::transfer_all(), ignored_error);

  boost::array<char, 128> buf;
  size_t len = socket.read_some(boost::asio::buffer(buf), error);

  if (error && error != boost::asio::error::eof)
     throw boost::system::system_error(error); // Some other error.
  std::cout.write(buf.data(), len) << std::endl;
}

IOrderServer& ExampleModule::GetOrderServer()
{
  return *mOrderServer.get();
}
