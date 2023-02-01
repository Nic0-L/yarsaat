#ifndef NVEC_H
#define NVEC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>

#define NVEC_BLOCK_SIZE	((size_t)(65536))

typedef struct nvec_s
{
	mpz_t		*values;
	size_t		size;
	size_t		allocated_size;
}	nvec_t;

nvec_t	*nvec_init();
nvec_t	*nvec_realloc(nvec_t *nvec);
nvec_t	*nvec_fit(nvec_t *nvec);
nvec_t	*nvec_append_hex(nvec_t *nvec, char *hex_num);
nvec_t	*nvec_append_mpz(nvec_t *nvec, mpz_t *num);
mpz_t	*nvec_get(nvec_t *nvec, size_t position);
void	nvec_free(nvec_t *nvec);
void	nvec_debug(nvec_t *nvec);
nvec_t	*nvec_read_file(char *filename);
nvec_t	*nvec_read_file_raw(char *filename);

mpz_t	*nvec_math_product_tree(nvec_t *nvec);
nvec_t	*nvec_append_mpz_d(nvec_t *nvec, mpz_t val);

void	y_batchgcd(nvec_t *nvec);
void	y_batchgcd2(nvec_t *nvec1, nvec_t *nvec2);

#endif /* NVEC_H */