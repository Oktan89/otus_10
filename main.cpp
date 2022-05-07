#include <iostream>
#include <thread>
#include <cstdlib>
#include <boost/asio.hpp>
#include "server.h"

///Нужно сделать через future
void run_server(boost::asio::io_context& context)
{
  context.run();
}

int main(int argc, char** argv)
{
  try
  {
    if (argc != 3)
    {
      std::cerr << "Usage: bulk_server <port> <size_bulk>\n";
      return 1;
    }
    else
    {
      std::cout << argv[1] << "\n";
      std::cout << argv[2] << "\n";
    }

    boost::asio::io_context context;
    Server server(context, std::atoi(argv[1]), std::stoi(argv[2]));

    std::thread serv_thread = std::thread(run_server, std::ref(context));
    
    std::string answer;
    while(true)
    {
      std::cout << "Enter exit for quit\n";
      std::getline(std::cin, answer);
      if(answer == "exit")
      {
        context.stop();
        break;
      }
    }

    serv_thread.join();
  }
  catch (std::exception &exc)
  {
    std::cout << exc.what() << std::endl;
  }
}