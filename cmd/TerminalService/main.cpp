#include "terminalServer.hpp"

int main(int argc, char* argv[]) {
  const boost::asio::ip::address address = net::ip::make_address("127.0.0.1");
  const unsigned short port = static_cast<unsigned short>(std::atoi("8181"));
  const std::shared_ptr<std::string> doc_root =
      std::make_shared<std::string>(".");
  const int threads = 1;

  net::io_context ioc{threads};

  std::make_shared<TerminalListener>(ioc, tcp::endpoint{address, port},
                                     doc_root)
      ->run();

  std::vector<std::thread> v;
  v.reserve(threads - 1);
  for (auto i = threads - 1; i > 0; --i) {
    v.emplace_back([&ioc] { ioc.run(); });
  }

  ioc.run();

  return EXIT_SUCCESS;
}