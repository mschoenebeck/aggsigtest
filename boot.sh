/home/mschoenebeck/dev/leap/build/bin/cleos wallet unlock --password PW5KDgeah3RPd6aZfcTzbAVfUxrjprP8iCCvAHehBhTUvKwKYuDnw
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.bpay EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.msig EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.names EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.ram EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.ramfee EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.saving EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.stake EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.token EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.vpay EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio eosio.rex EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
/home/mschoenebeck/dev/leap/build/bin/cleos create account eosio aggsigtest EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV
echo "eosio accounts created"
/home/mschoenebeck/dev/leap/build/bin/cleos set contract eosio.token /home/mschoenebeck/dev/reference-contracts/build/contracts/eosio.token/
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio.token create '[ "eosio", "10000000000.0000 EOS" ]' -p eosio.token@active
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio.token issue '[ "eosio", "1000000000.0000 EOS", "initial issuance" ]' -p eosio

curl --request POST --url http://127.0.0.1:8888/v1/producer/schedule_protocol_feature_activations     -d '{"protocol_features_to_activate": ["0ec7e080177b2c02b278d5088611686b49d739925a92d9bfcacd7fc6b74053bd"]}'
sleep 1
/home/mschoenebeck/dev/leap/build/bin/cleos set contract eosio /home/mschoenebeck/dev/reference-contracts/build/contracts/eosio.boot/
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["f0af56d2c5a48d60a4a5b5c903edfb7db3a736a94ed589d0b797df33ff9d3e1d"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["2652f5f96006294109b3dd0bbde63693f55324af452b799ee137a81a905eed25"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["8ba52fe7a3956c5cd3a656a3174b931d3bb2abb45578befc59f283ecd816a405"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["ad9e3d8f650687709fd68f4b90b41f7d825a365b02c23a636cef88ac2ac00c43"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["68dcaa34c0517d19666e6b33add67351d8c5f69e999ca1e37931bc410a297428"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["e0fb64b1085cc5538970158d05a009c24e276fb94e1a0bf6a528b48fbc4ff526"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["ef43112c6543b88db2283a2e077278c315ae2c84719a8b25f25cc88565fbea99"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["4a90c00d55454dc5b059055ca213579c6ea856967712a56017487886a4d4cc0f"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["1a99a59d87e06e09ec5b028a9cbb7749b4a5ad8819004365d02dc4379a8b7241"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["4e7bf348da00a945489b2a681749eb56f5de00b900014e137ddae39f48f69d67"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["4fca8bd82bbd181e714e283f83e1b45d95ca5af40fb89ad3977b653c448f78c2"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["299dcb6af692324b899b39f16d5a530a33062804e41f09dc97e9f156b4476707"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["d528b9f6e9693f45ed277af93474fd473ce7d831dae2180cca35d907bd10cb40"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["5443fcf88330c586bc0e5f3dee10e7f63c76c00249c87fe4fbf7f38c082006b4"]' -p eosio
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio activate '["8763b136fd679e253f0fabc4c5fca57bb7c822fce89b8288d98776bc31a49401"]' -p eosio
echo "features activated"
/home/mschoenebeck/dev/leap/build/bin/cleos set contract eosio /home/mschoenebeck/dev/reference-contracts/build/contracts/eosio.system/
/home/mschoenebeck/dev/leap/build/bin/cleos push action eosio init '["0", "4,EOS"]' -p eosio@active

echo "deploy aggsigtest contract"
/home/mschoenebeck/dev/leap/build/bin/cleos set contract aggsigtest . aggsigtest.wasm aggsigtest.abi
echo "test bls_verify (test1)"
/home/mschoenebeck/dev/leap/build/bin/cleos push action aggsigtest test1 '{"pk":"86243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1","sig":"adc93ff939b43eef9fcaf43e73670be1aca3f17e2ad82ccd60e2b1c5d263630e1e5f4ef11e9f54da86bcc0032fbc345114ad1662738ce5fdefdb2bc82285d4317dbd4e7339e0c9e6716a759e26931fb079db5a124163585d6ddd5ef8eaff10b5" }' -p aggsigtest@active
echo "test bls_aggregate_public_keys (test2)"
/home/mschoenebeck/dev/leap/build/bin/cleos push action aggsigtest test2 '{"pks":["86243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1","8b431bb3675f354cf558281a67d3026075da83e2c36a932f60269c2d76dee96aa7ac6f3a7c8141acddb8e7ff1c122807","a2e9794d074c56874aa63ff3cc8062ba81f1405ebcf426bc4cb4b64a4a6fbd60cae0a71a12195b2ddd9677d01f6c2988"],"pk_agg":"a8d777ebb63a5406605497671c89135b063a2b29b8c819b4900ebfddcde211a8c4f7f3154bc67a6b18715d291ac885a4" }' -p aggsigtest@active
echo "test bls_aggregate_signatures (test3)"
/home/mschoenebeck/dev/leap/build/bin/cleos push action aggsigtest test3 '{"sigs":["adc93ff939b43eef9fcaf43e73670be1aca3f17e2ad82ccd60e2b1c5d263630e1e5f4ef11e9f54da86bcc0032fbc345114ad1662738ce5fdefdb2bc82285d4317dbd4e7339e0c9e6716a759e26931fb079db5a124163585d6ddd5ef8eaff10b5","b6267017551eb805af9f717b5f886cd192f3fbcfcfaf161e7809b21ec59152d4ce4a40504e6b41e168f5e2d8b251d8e90e8fff3985a91d50362373820af2b503e51ac0ebccd45a97c6638ef7d8231b303b94acdf196caad8fb76f547ea16feae","852474a23847f905cb3741c2b02970c314f05c91651124280bff2702ef4338cec7cf2742ee11bb2bf61550bdb4a077000dfbe9e0bfd66b274de9ebdb62640937ecf5a74ffcaf456ecb8ec7dc97e355ba737eb0d8fa0e92272573608bb7b39b44"],"sig_agg":"834fc18dd712e74c2a43051fb6d4dc2ddf11ec17b728c227dbc18495c6387c6166d921c90c9002e13db1b25789300f35079a39014809c50ca8bebbd276111094d4625c4dd157ee3d310d7f95d4d63569ff493e74c1b10145555dcda26a69e123" }' -p aggsigtest@active
echo "test bls_aggregate_verify (test4)"
/home/mschoenebeck/dev/leap/build/bin/cleos push action aggsigtest test4 '{"pks":["86243290bbcbfd9ae75bdece7981965350208eb5e99b04d5cd24e955ada961f8c0a162dee740be7bdc6c3c0613ba2eb1","8b431bb3675f354cf558281a67d3026075da83e2c36a932f60269c2d76dee96aa7ac6f3a7c8141acddb8e7ff1c122807","a2e9794d074c56874aa63ff3cc8062ba81f1405ebcf426bc4cb4b64a4a6fbd60cae0a71a12195b2ddd9677d01f6c2988"],"sig":"834fc18dd712e74c2a43051fb6d4dc2ddf11ec17b728c227dbc18495c6387c6166d921c90c9002e13db1b25789300f35079a39014809c50ca8bebbd276111094d4625c4dd157ee3d310d7f95d4d63569ff493e74c1b10145555dcda26a69e123" }' -p aggsigtest@active

