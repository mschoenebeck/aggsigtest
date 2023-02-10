#include <eosio/eosio.hpp>
#include <eosio/crypto_bls_ext.hpp>

using namespace eosio;

std::vector<uint8_t> message_1 = { 51, 23, 56, 93, 212, 129, 128, 27, 251, 12, 42, 129, 210, 9, 34, 98};
std::vector<uint8_t> message_2 = { 16, 38, 54, 125, 71, 214, 217, 78, 73, 23, 127, 235, 8, 94, 41, 53};
std::vector<uint8_t> message_3 = { 12, 4, 1, 64, 127, 86, 2, 8, 145, 25, 27, 5, 88, 4, 42, 58};

CONTRACT aggsigtest : public contract {
   public:
      using contract::contract;

      ACTION test1(const bls_g1& pk, const bls_g2& sig);
      ACTION test2(const std::vector<bls_g1>& pks, const bls_g1& pk_agg);
      ACTION test3(const std::vector<bls_g2>& sigs, const bls_g2& sig_agg);
      ACTION test4(const std::vector<bls_g1>& pks, const bls_g2& sig);
};
