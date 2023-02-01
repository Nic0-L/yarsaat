#include "nvec.h"

mpz_t	*nvec_math_product_tree_recursive(mpz_t *values, size_t count)
{
	mpz_t	*result;

	//dprintf(2, "nvec_math_product_tree_recursive count : %lu\n", count);

	result = malloc(sizeof(mpz_t)); // Not optimized...
	if (result == NULL)
	{
		dprintf(2, "nvec_math_product_tree_recursive : malloc error\n");
		return (NULL);
	}
	mpz_init(*result);

	if (count == 1)
	{
		mpz_set(*result, values[0]);
		return (result);
	}

	size_t count_left = count / 2;
	size_t count_right = count_left;

	if (count % 2)
		count_right += 1;
	mpz_t *res_left = nvec_math_product_tree_recursive(values, count_left);
	mpz_t *res_right = nvec_math_product_tree_recursive(&(values[count / 2]), count_right);

	mpz_init(*result);
	mpz_mul(*result, *res_left, *res_right);

	mpz_clear(*res_left);
	mpz_clear(*res_right);
	free(res_left);
	free(res_right);

	return (result);
}

mpz_t	*nvec_math_product_tree(nvec_t *nvec)
{
	return (nvec_math_product_tree_recursive(nvec->values, nvec->size));
}