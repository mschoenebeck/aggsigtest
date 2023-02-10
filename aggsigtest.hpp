#include <eosio/eosio.hpp>
#include <eosio/crypto_bls_ext.hpp>

using namespace eosio;

std::vector<uint8_t> message_1 = { 51, 23, 56, 93, 212, 129, 128, 27, 251, 12, 42, 129, 210, 9, 34, 98};
std::vector<uint8_t> message_2 = { 16, 38, 54, 125, 71, 214, 217, 78, 73, 23, 127, 235, 8, 94, 41, 53};
std::vector<uint8_t> message_3 = { 12, 4, 1, 64, 127, 86, 2, 8, 145, 25, 27, 5, 88, 4, 42, 58};

CONTRACT aggsigtest : public contract {
   public:
      using contract::contract;

      // cleos push action aggsigtest test1 '{"pk":"86243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1","sig":"adc93ff939b43eef9fcaf43e73670be1aca3f17e2ad82ccd60e2b1c5d263630e1e5f4ef11e9f54da86bcc0032fbc345114ad1662738ce5fdefdb2bc82285d4317dbd4e7339e0c9e6716a759e26931fb079db5a124163585d6ddd5ef8eaff10b5" }' -p aggsigtest@active
      ACTION test1(const bls_g1& pk, const bls_g2& sig);

      // cleos push action aggsigtest test2 '{"pks":["86243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1","8b431bb3675f354cf558281a67d3026075da83e2c36a932f60269c2d76dee96aa7ac6f3a7c8141acddb8e7ff1c122807","a2e9794d074c56874aa63ff3cc8062ba81f1405ebcf426bc4cb4b64a4a6fbd60cae0a71a12195b2ddd9677d01f6c2988"],"pk_agg":"a8d777ebb63a5406605497671c89135b063a2b29b8c819b4900ebfddcde211a8c4f7f3154bc67a6b18715d291ac885a4" }' -p aggsigtest@active
      ACTION test2(const std::vector<bls_g1>& pks, const bls_g1& pk_agg);

      // cleos push action aggsigtest test3 '{"sigs":["adc93ff939b43eef9fcaf43e73670be1aca3f17e2ad82ccd60e2b1c5d263630e1e5f4ef11e9f54da86bcc0032fbc345114ad1662738ce5fdefdb2bc82285d4317dbd4e7339e0c9e6716a759e26931fb079db5a124163585d6ddd5ef8eaff10b5","b6267017551eb805af9f717b5f886cd192f3fbcfcfaf161e7809b21ec59152d4ce4a40504e6b41e168f5e2d8b251d8e90e8fff3985a91d50362373820af2b503e51ac0ebccd45a97c6638ef7d8231b303b94acdf196caad8fb76f547ea16feae","852474a23847f905cb3741c2b02970c314f05c91651124280bff2702ef4338cec7cf2742ee11bb2bf61550bdb4a077000dfbe9e0bfd66b274de9ebdb62640937ecf5a74ffcaf456ecb8ec7dc97e355ba737eb0d8fa0e92272573608bb7b39b44"],"sig_agg":"834fc18dd712e74c2a43051fb6d4dc2ddf11ec17b728c227dbc18495c6387c6166d921c90c9002e13db1b25789300f35079a39014809c50ca8bebbd276111094d4625c4dd157ee3d310d7f95d4d63569ff493e74c1b10145555dcda26a69e123" }' -p aggsigtest@active
      ACTION test3(const std::vector<bls_g2>& sigs, const bls_g2& sig_agg);

      // cleos push action aggsigtest test4 '{"pks":["86243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1","8b431bb3675f354cf558281a67d3026075da83e2c36a932f60269c2d76dee96aa7ac6f3a7c8141acddb8e7ff1c122807","a2e9794d074c56874aa63ff3cc8062ba81f1405ebcf426bc4cb4b64a4a6fbd60cae0a71a12195b2ddd9677d01f6c2988"],"sig":"834fc18dd712e74c2a43051fb6d4dc2ddf11ec17b728c227dbc18495c6387c6166d921c90c9002e13db1b25789300f35079a39014809c50ca8bebbd276111094d4625c4dd157ee3d310d7f95d4d63569ff493e74c1b10145555dcda26a69e123" }' -p aggsigtest@active
      ACTION test4(const std::vector<bls_g1>& pks, const bls_g2& sig);
};
