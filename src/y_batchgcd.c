#include <math.h>
#include "nvec.h"

long	y_generate_product_tree(nvec_t *nvec, nvec_t **product_tree)
{
	long	level = 0;
	nvec_t	*previous_nvec = nvec;
	char	done = 0;
	mpz_t	result;

	mpz_init(result);
	while (!done)
	{
		dprintf(2, "y_generate_product_tree : level %lu\n", level);
		product_tree[level] = nvec_init();

		size_t i = 0;
		while (i + 1 < previous_nvec->size)
		{
			mpz_mul(result, previous_nvec->values[i], previous_nvec->values[i + 1]);
			product_tree[level] = nvec_append_mpz(product_tree[level], &result);
			if (product_tree[level] == NULL)
			{
				dprintf(2, "y_generate_product_tree : malloc error (append)\n");
				return 0; // Wrong return code...
			}
			i += 2;
		}
		//if (i < previous_nvec->size)
		//	nvec_append_mpz() // If count is odd, keep the last number in the tree for later

		if (product_tree[level]->size > 1)
		{
			previous_nvec = product_tree[level];
			++level;
			product_tree[level] = NULL;
		}
		else
		{
			done = 1;
		}
	}
	dprintf(2, "y_generate_product_tree : done : size of the product : %d bytes\n", product_tree[level]->values[0][0]._mp_size * 8);
	mpz_clear(result);
	return (level);
}

void y_compute_remainder_tree_recursive(nvec_t *nvec, nvec_t **product_tree, long level, mpz_t branch, size_t n)
{
	// dprintf(2,"y_compute_remainder_tree_recursive : %ld\t%lu\n", level, n);

	mpz_t square_result;
	mpz_t mod_result;
	mpz_t *current_number;

	mpz_init(square_result);
	mpz_init(mod_result);

	if (level >= 0)
		current_number = nvec_get(product_tree[level], n);
	else
		current_number = nvec_get(nvec, n);
	mpz_pow_ui(square_result, *current_number, 2);
	mpz_mod(mod_result, branch, square_result);

	if (level >= 0)
	{
		y_compute_remainder_tree_recursive(nvec, product_tree, level - 1, mod_result, n * 2);
		y_compute_remainder_tree_recursive(nvec, product_tree, level - 1, mod_result, n * 2 + 1);
	}
	else
	{
		mpz_t div_result;
		mpz_t gcd_result;

		mpz_init(div_result);
		mpz_init(gcd_result);

		mpz_fdiv_q(div_result, mod_result, *current_number);
		mpz_gcd   (gcd_result, div_result, *current_number);

		if (mpz_cmp_ui(gcd_result, 1) && mpz_cmp(gcd_result, *current_number))
		{
			dprintf(2, "Number %ld has a common factor\n", n);
		}

		mpz_clear(div_result);
		mpz_clear(gcd_result);
	}

	mpz_clear(square_result);
	mpz_clear(mod_result);
}

void y_compute_remainder_tree(nvec_t *nvec, nvec_t **product_tree, long level)
{
	y_compute_remainder_tree_recursive(nvec, product_tree, level - 1, *nvec_get(product_tree[level], 0), 0);
	y_compute_remainder_tree_recursive(nvec, product_tree, level - 1, *nvec_get(product_tree[level], 0), 1);
}

int is_pow_of_two(size_t x)
{
	//checks whether a number is zero or not
	if (x == 0)
		return 0;

	//true till x is not equal to 1
	while (x != 1)
	{
		//checks whether a number is divisible by 2
		if (x % 2 != 0)
			return 0;
		x /= 2;
	}
	return 1;
}

void	y_batchgcd(nvec_t *nvec)
{
	nvec_t	**product_tree;
	long	level;

	if (nvec->size <= 1)
	{
		dprintf(2, "y_batchgcd : at least two numbers required");
		return;
	}
	if (!is_pow_of_two(nvec->size))
	{
		dprintf(2, "y_batchgcd : the count of provided numbers should be a power of two\n");
		return;
	}
	product_tree = malloc(sizeof(nvec_t **) * log2((double)nvec->size) + 5);
	level = y_generate_product_tree(nvec, product_tree);
	y_compute_remainder_tree(nvec, product_tree, level);
	for (long i = 0; i <= level; ++i)
		nvec_free(product_tree[i]);
}

void	y_batchgcd2(nvec_t *nvec1, nvec_t *nvec2)
{
	nvec_t	**product_tree1;
	nvec_t	**product_tree2;
	long	level1;
	long	level2;

	if (nvec1->size <= 1)
	{
		dprintf(2, "y_batchgcd : at least two numbers required");
		return;
	}
	if (!is_pow_of_two(nvec1->size))
	{
		dprintf(2, "y_batchgcd : the count of provided numbers should be a power of two\n");
		return;
	}
	product_tree1 = malloc(sizeof(nvec_t **) * log2((double)nvec1->size) + 5);
	product_tree2 = malloc(sizeof(nvec_t **) * log2((double)nvec2->size) + 5);
	level1 = y_generate_product_tree(nvec1, product_tree1);
	level2 = y_generate_product_tree(nvec2, product_tree2);
	//y_compute_remainder_tree(nvec, product_tree, level);
	dprintf(2, "y_batchgcd : gcd of the two products :\n");

	mpz_t gcd_result;

	mpz_init(gcd_result);
	mpz_gcd(gcd_result, *nvec_get(product_tree1[level1], 0), *nvec_get(product_tree2[level2], 0));
	if (mpz_cmp_ui(gcd_result, 1))
	{
		dprintf(1, "A gcd was found:\n");
		mpz_out_str(stdout, 16, gcd_result);
		putchar('\n');
	}

	dprintf(2, "y_batchgcd : done\n");
}