#include "nvec.h"

nvec_t	*nvec_init()
{
	nvec_t	*nvec = malloc(sizeof(nvec_t));

	if (nvec == NULL)
	{
		dprintf(2, "nvec_init : malloc failed\n");
		return (NULL);
	}
	nvec->values = NULL;
	nvec->size = 0;
	nvec->allocated_size = 0;
	return (nvec);
}

nvec_t	*nvec_realloc(nvec_t *nvec)
{
	size_t	size;

	if (nvec->allocated_size > nvec->size)
	{
		return (nvec);
	}
	size = nvec->allocated_size + NVEC_BLOCK_SIZE;
	// dprintf(2, "%lu\n", size);
	nvec->values = realloc(nvec->values, size * sizeof(mpz_t));
	if (nvec->values == NULL)
	{
		dprintf(2, "nvec_realloc : realloc failed\n");
		return (NULL);
	}
	nvec->allocated_size = size;
	return (nvec);
}

nvec_t	*nvec_fit(nvec_t *nvec)
{
	nvec->values = realloc(nvec->values, nvec->size * sizeof(mpz_t));
	if (nvec->values == NULL)
	{
		dprintf(2, "nvec_fit : realloc failed\n");
		return (NULL);
	}
	nvec->allocated_size = nvec->size;
	return (nvec);
}

nvec_t	*nvec_append_hex(nvec_t *nvec, char *hex_num)
{
	int i = 0;

	if (nvec->size >= nvec->allocated_size)
	{
		nvec = nvec_realloc(nvec);
		if (nvec == NULL)
		{
			dprintf(2, "nvec_append_hex : realloc failed\n");
			return (NULL);
		}
	}
	while ((hex_num[i] >= '0' && hex_num[i] <= '9')
		|| (hex_num[i] >= 'A' && hex_num[i] <= 'F')
		|| (hex_num[i] >= 'a' && hex_num[i] <= 'f'))
		++i;
	hex_num[i] = '\0';
	mpz_init(nvec->values[nvec->size]);
	mpz_set_str(nvec->values[nvec->size], hex_num, 16);
	nvec->size += 1;
	return (nvec);
}

nvec_t	*nvec_append_mpz(nvec_t *nvec, mpz_t *val)
{
	if (nvec->size >= nvec->allocated_size)
	{
		nvec = nvec_realloc(nvec);
		if (nvec == NULL)
		{
			dprintf(2, "nvec_append_mpz : realloc failed\n");
			return (NULL);
		}
	}
	mpz_init(nvec->values[nvec->size]);
	mpz_set(nvec->values[nvec->size], *val);
	nvec->size += 1;
	return (nvec);
}

mpz_t	*nvec_get(nvec_t *nvec, size_t position)
{
	if (position >= nvec->size)
		return (NULL);
	return (&(nvec->values[position]));
}

void	nvec_free(nvec_t *nvec)
{
	for (size_t i = 0; i < nvec->size; ++i)
		mpz_clear(nvec->values[i]);
	free(nvec->values);
	free(nvec);
}

void	nvec_debug(nvec_t *nvec)
{
	dprintf(2, "NVEC::DEBUG === Vector size / Allocated size : %lu / %lu\n", nvec->size, nvec->allocated_size);
}

nvec_t	*nvec_read_file(char *filename)
{
	nvec_t	*nvec;
    char*	line = NULL;
    size_t	len = 0;
	FILE	*fd;

	if (strcmp(filename, "-"))
		fd = fopen(filename, "r");
	else
		fd = stdin;

	if (fd == NULL)
	{
		dprintf(2,"nvec_read_file : fopen(%s) failed\n", filename);
		return (NULL);
	}
	nvec = nvec_init();
	if (nvec == NULL)
		return (NULL);
    while (getline(&line, &len, fd) != -1)
    {
    	nvec = nvec_append_hex(nvec, line);
    	if (nvec == NULL)
    		return (NULL);
    }
	dprintf(2, "nvec_read_file : %lu numbers loaded from \"%s\"\n", nvec->size, filename);
    free(line);
    if (fd != stdin)
		fclose(fd);
	return (nvec_fit(nvec));
}

nvec_t	*nvec_read_file_raw(char *filename)
{
	nvec_t	*nvec;
	FILE	*fd;

	if (strcmp(filename, "-"))
		fd = fopen(filename, "r");
	else
		fd = stdin;

	if (fd == NULL)
	{
		dprintf(2,"nvec_read_file : fopen(%s) failed\n", filename);
		return (NULL);
	}
	nvec = nvec_init();
	if (nvec == NULL)
		return (NULL);

	mpz_t	n;
	mpz_init(n);
	mpz_inp_raw(n, fd);
	nvec_append_mpz(nvec, &n);
	mpz_clear(n);

	dprintf(2, "nvec_read_file : %lu numbers loaded from \"%s\"\n", nvec->size, filename);
 
    if (fd != stdin)
		fclose(fd);
	return (nvec_fit(nvec));
}