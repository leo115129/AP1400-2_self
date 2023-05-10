#include "server.h"

#include "client.h"
std::vector<std::string> pending_trxs;
Server::Server() {}
void show_wallets(const Server& server) {
  std::cout << std::string(20, '*') << std::endl;
  for (const auto& client : server.clients)
    std::cout << client.first->get_id() << " : " << client.second << std::endl;
  std::cout << std::string(20, '*') << std::endl;
}

std::shared_ptr<Client> Server::add_client(std::string id) {
  // 初始化随机数生成器
  std::random_device rd;
  std::mt19937 gen(rd());

  // 定义随机数分布
  std::uniform_int_distribution<> dis(1000, 9999);
  int num = dis(gen);
  std::string tail = std::to_string(num);
  for (const auto& i : clients) {
    if (i.first->get_id() == id) {
      id += tail;
      break;
    }
  }
  auto new_client = std::make_shared<Client>(id, *this);
  clients[new_client] = 5;
  return new_client;
}
std::shared_ptr<Client> Server::get_client(std::string id) const {
  for (const auto& i : clients) {
    if (i.first->get_id() == id) {
      return i.first;
    }
  }
  return nullptr;
}
double Server::get_wallet(std::string id) const {
  for (const auto& i : clients) {
    if (i.first->get_id() == id) {
      return i.second;
    }
  }
  return 0;
}
bool Server::add_pending_trx(std::string trx, std::string signature) const {
  std::string sender, receiver;
  double value;
  if (parse_trx(trx, sender, receiver, value)) {
    auto p_receiver = get_client(receiver);
    auto p_sender = get_client(sender);
    if (p_receiver == nullptr) {
      return false;
    }
    bool sign =
        crypto::verifySignature(p_sender->get_publickey(), trx, signature);
    if (sign && value <= clients.at(p_sender)) {
      pending_trxs.push_back(trx);
      return true;
    }
  }
  return false;
}
bool Server::parse_trx(std::string trx, std::string& sender,
                       std::string& receiver, double& value) {
  std::vector<std::string> tokens;
  std::stringstream ss(trx);
  std::string token;
  while (getline(ss, token, '-')) {
    tokens.push_back(token);
  }
  if (tokens.size() != 3) {
    throw std::runtime_error("error");
    return false;
  }
  sender = tokens[0], receiver = tokens[1], value = std::stod(tokens[2]);
  return true;
}
size_t Server::mine() {
  std::string all = "";
  for (const auto& i : pending_trxs) {
    all += i;
  }
  all += "nonce";
  size_t num;
  for(auto&i:clients){
    num=i.first->generate_nonce();
    //  std::cout<<num<<'\n';
    if(crypto::sha256(all+std::to_string(num)).find("0")!=std::string::npos){
      //std::cout<<crypto::sha256(all)<<'\n';
      i.second+=6.25;
      for(const auto&trx:pending_trxs){
        std::string sender,receiver;
        double value;
        Server::parse_trx(trx,sender,receiver,value);
        clients[get_client(sender)]-=value;
        clients[get_client(receiver)]+=value;
      }
      pending_trxs.clear();
      std::cout<<i.first->get_id();
      return num;
    }
  }
  return  num;
}