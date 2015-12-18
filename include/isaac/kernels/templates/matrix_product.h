#ifndef ISAAC_BACKEND_TEMPLATES_MPRODUCT_H
#define ISAAC_BACKEND_TEMPLATES_MPRODUCT_H

#include "isaac/kernels/templates/base.h"
#include "isaac/symbolic/expression.h"
#include "isaac/symbolic/preset.h"

namespace isaac
{
namespace templates
{

struct matrix_product_parameters : public base::parameters_type
{
  matrix_product_parameters(unsigned int simd_width
                            , unsigned int local_size_0, unsigned int KL, unsigned int local_size_1, unsigned int D
                            , unsigned int ms, unsigned int ks, unsigned int ns
                            , fetching_policy_type A_fetching_policy, fetching_policy_type B_fetching_policy
                            , unsigned int local_fetch_0, unsigned int local_fetch_1);

  unsigned int kL;
  unsigned int depth;

  unsigned int mS;
  unsigned int kS;
  unsigned int nS;

  fetching_policy_type A_fetching_policy;
  fetching_policy_type B_fetching_policy;

  unsigned int local_fetch_0;
  unsigned int local_fetch_1;

  unsigned int mL;
  unsigned int nL;

  bool prefetch;
  bool unroll_outer;
};

class matrix_product : public base_impl<matrix_product, matrix_product_parameters>
{
private:
  unsigned int temporary_workspace(math_expression const & expressions) const;
  unsigned int lmem_usage(math_expression const & expressions) const;
  unsigned int registers_usage(math_expression const & expressions) const;
  int is_invalid_impl(driver::Device const &, math_expression const &) const;
  std::string generate_impl(std::string const & suffix, math_expression const & expressions, driver::Device const & device, mapping_type const &) const;
  void enqueue_block(driver::CommandQueue & queue, int_t M, int_t N, int_t K, array_base const & A, array_base const & B, array_base const & C,
                     value_scalar const &alpha, value_scalar const &beta, driver::Program const & program, std::string const & suffix, execution_options_type const & options);
  std::vector<int_t> infos(math_expression const & expressions,  isaac::symbolic::preset::matrix_product::args &arguments) const;
public:
  matrix_product(matrix_product::parameters_type const & parameters, bool check_bound, char A_trans, char B_trans);
  std::vector<int_t> input_sizes(math_expression const & expressions) const;
  void enqueue(driver::CommandQueue & queue, driver::Program const & program, std::string const & suffix, base & fallback, execution_handler const &ctr);
private:
  const char A_trans_;
  const char B_trans_;
  expression_type type_;
  bool check_bounds_;
};

class matrix_product_nn : public matrix_product
{
public:
  matrix_product_nn(unsigned int simd, int_t ls0, int_t KL, int_t ls1, int_t D
                      , int_t ms, int_t ks, int_t ns, fetching_policy_type Afetch , fetching_policy_type Bfetch
                      , int_t lfetch0, int_t lfetch1, bool check_bound = false);
};

class matrix_product_tn : public matrix_product
{
public:
  matrix_product_tn(unsigned int simd, int_t ls0, int_t KL, int_t ls1, int_t D
                      , int_t ms, int_t ks, int_t ns, fetching_policy_type Afetch , fetching_policy_type Bfetch
                      , int_t lfetch0, int_t lfetch1, bool check_bound = false);
};


class matrix_product_nt : public matrix_product
{
public:
  matrix_product_nt(unsigned int simd, int_t ls0, int_t KL, int_t ls1, int_t D
                      , int_t ms, int_t ks, int_t ns, fetching_policy_type Afetch , fetching_policy_type Bfetch
                      , int_t lfetch0, int_t lfetch1, bool check_bound = false);
};


class matrix_product_tt : public matrix_product
{
public:
  matrix_product_tt(unsigned int simd, int_t ls0, int_t KL, int_t ls1, int_t D
                      , int_t ms, int_t ks, int_t ns, fetching_policy_type Afetch , fetching_policy_type Bfetch
                      , int_t lfetch0, int_t lfetch1, bool check_bound = false);
};

}
}

#endif