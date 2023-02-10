#include "aggsigtest.hpp"

void aggsigtest::test1(const bls_g1& pk, const bls_g2& sig)
{
   bool ok = bls_verify(pk, message_1, sig);
   check(ok == true, "signature verification failed");
}

void aggsigtest::test2(const std::vector<bls_g1>& pks, const bls_g1& pk_agg)
{
   bls_g1 pa;
   bls_aggregate_public_keys(pks, &pa);
   check(std::equal(pa.begin(), pa.end(), pk_agg.begin()), "public key aggregation verification failed");
}

void aggsigtest::test3(const std::vector<bls_g2>& sigs, const bls_g2& sig_agg)
{
   bls_g2 sa;
   bls_aggregate_signatures(sigs, &sa);
   check(std::equal(sa.begin(), sa.end(), sig_agg.begin()), "signature aggregation verification failed");
}

void aggsigtest::test4(const std::vector<bls_g1>& pks, const bls_g2& sig)
{
   bool ok = bls_aggregate_verify(pks, {message_1, message_2, message_3}, sig);
   check(ok == true, "aggregate signature verification failed");
}
