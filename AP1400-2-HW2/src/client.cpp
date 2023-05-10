#include "client.h"

#include "server.h"
Client::Client(std::string id, const Server& server) : id(id), server(&server) {
  std::string public_key{}, private_key{};
  crypto::generate_key(public_key, private_key);
  this->public_key = public_key, this->private_key = private_key;
}
std::string Client::get_id() const { return this->id; }
double Client::get_wallet() const { return this->server->get_wallet(this->id); }
std::string Client::get_publickey() const { return this->public_key; }
std::string Client::sign(std::string txt) const {
  std::string signature = crypto::signMessage(private_key, txt);
  return signature;
}
bool Client::transfer_money(std::string receiver, double value) {
  std::string trx = this->id + "-" + receiver + "-" + std::to_string(value);
  return server->add_pending_trx(trx, sign(trx));
}
size_t Client::generate_nonce() {
  // 初始化随机数生成器
  std::random_device rd;
  std::mt19937 gen(rd());

  // 定义随机数分布
  std::uniform_int_distribution<> dis(0, INT_MAX);
  size_t num = dis(gen);
  return num;
}