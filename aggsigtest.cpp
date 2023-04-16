#include "aggsigtest.hpp"

void aggsigtest::testg1add(const bls_g1& op1, const bls_g1& op2, const bls_g1& res)
{
    bls_g1 r;
    bls_g1_add(op1, op2, r);
    check(r == res, "bls_g1_add test failed");
}

void aggsigtest::testg2add(const bls_g2& op1, const bls_g2& op2, const bls_g2& res)
{
    bls_g2 r;
    bls_g2_add(op1, op2, r);
    check(r == res, "bls_g2_add test failed");
}

void aggsigtest::testg1mul(const bls_g1& point, const bls_scalar& scalar, const bls_g1& res)
{
    bls_g1 r;
    bls_g1_mul(point, scalar, r);
    check(r == res, "bls_g1_mul test failed");
}

void aggsigtest::testg2mul(const bls_g2& point, const bls_scalar& scalar, const bls_g2& res)
{
    bls_g2 r;
    bls_g2_mul(point, scalar, r);
    /*
    uint64_t N = 144;
    constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    std::string s(2 + N * 2, ' ');
    s[0] = '0';
    s[1] = 'x';
    for(uint64_t i = 0; i < N; i++)
    {
        s[2 + 2*i]     = hexmap[(r[i] & 0xF0) >> 4];
        s[2 + 2*i+1]   = hexmap[ r[i] & 0x0F      ];
    }
    */
    check(r == res, "bls_g2_mul test failed");
}

void aggsigtest::testg1exp(const std::vector<bls_g1>& points, const std::vector<bls_scalar>& scalars, const bls_g1& res)
{
    check(points.size() == scalars.size(), "number of elements in points and scalars must be equal");
    bls_g1 r;
    bls_g1_exp(points, scalars, r);
    check(r == res, "bls_g1_exp test failed");
}

void aggsigtest::testg2exp(const std::vector<bls_g2>& points, const std::vector<bls_scalar>& scalars, const bls_g2& res)
{
    check(points.size() == scalars.size(), "number of elements in points and scalars must be equal");
    bls_g2 r;
    bls_g2_exp(points, scalars, r);
    check(r == res, "bls_g2_exp test failed");
}

void aggsigtest::testpairing(const std::vector<bls_g1>& g1_points, const std::vector<bls_g2>& g2_points, const bls_gt& res)
{
    check(g1_points.size() == g2_points.size(), "number of elements in g1_points and g2_points must be equal");
    bls_gt r;
    bls_pairing(g1_points, g2_points, r);
    check(r == res, "bls_pairing test failed");
}

void aggsigtest::testg1map(const bls_fp& e, const bls_g1& res)
{
    bls_g1 r;
    bls_g1_map(e, r);
    check(r == res, "bls_g1_map test failed");
}

void aggsigtest::testg2map(const bls_fp2& e, const bls_g2& res)
{
    bls_g2 r;
    bls_g2_map(e, r);
    check(r == res, "bls_g2_map test failed");
}
