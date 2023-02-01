// gcc -W -Wall -Wextra main.c nvec.c product_tree.c -lgmp

#include "nvec.h"	

nvec_t **read_input_files(int argc, char **argv)
{
	nvec_t	**nvec_list = malloc(sizeof(nvec_t *) * argc); // More than required
	int 	nvec_counter = 0;
	int		arg_counter = 1;

	if (nvec_list == NULL)
	{
		dprintf(2, "%s : read_input_files : Malloc error (init)\n", argv[0]);
		return (NULL);
	}
	nvec_list[0] = NULL;
	while (arg_counter < argc)
	{
		if ((!strcmp(argv[arg_counter], "-ifh") || !strcmp(argv[arg_counter], "-ifr")) && arg_counter + 1 < argc)
		{
			dprintf(2, "%s : read_input_files : reading file \"%s\"\n", argv[0], argv[arg_counter + 1]);
			if (argv[arg_counter][3] == 'h') // hex or raw
				nvec_list[nvec_counter] = nvec_read_file(argv[arg_counter + 1]);
			else
				nvec_list[nvec_counter] = nvec_read_file_raw(argv[arg_counter + 1]);
			if (nvec_list[nvec_counter] == NULL)
			{
				dprintf(2, "%s : read_input_files : Malloc error (nvec_read_file)\n", argv[0]);
				return (NULL);
			}
			nvec_debug(nvec_list[nvec_counter]);
			++nvec_counter;
			nvec_list[nvec_counter] = NULL;
		}
		++arg_counter;
	}
	if (nvec_counter == 0)
	{
		dprintf(2, "%s : read_input_files : At least one input file is required\n", argv[0]);
		return (NULL);
	}
	return(nvec_list);
}

FILE *open_output_file(int argc, char **argv)
{
	FILE	*output_file_descriptor = NULL;
	int		arg_counter = 1;

	while (arg_counter < argc)
	{
		if (!strcmp(argv[arg_counter], "-of") && arg_counter + 1 < argc)
		{
			output_file_descriptor = fopen(argv[arg_counter + 1], "w");
			if (output_file_descriptor == NULL)
			{
				dprintf(2, "%s : open_output_file : cannot open %s for writing", argv[0], argv[arg_counter + 1]);
				return (NULL);
			}
			return (output_file_descriptor);
		}
		++arg_counter;
	}
	dprintf(2, "%s : open_output_file : An output file is required\n", argv[0]);
	return (NULL);
}

int main_product(char *program_name, nvec_t **input_nvecs, FILE *output_file_descriptor)
{
	dprintf(2, "%s : product : calculating product\n", program_name);
	mpz_t *result = nvec_math_product_tree(input_nvecs[0]);

	dprintf(2, "%s : product : writing result to output file\n", program_name);
	size_t output_size = mpz_out_raw(output_file_descriptor, *result);

	dprintf(2, "%s : product : wrote %lu bytes to output file\n", program_name, output_size);
	mpz_clear(*result);
	free(result);
	return (0);
}

int main_batchgcd(char *program_name, nvec_t **input_nvecs, FILE *output_file_descriptor)
{
	y_batchgcd(input_nvecs[0]);
	return (0);
	//if (input_nvecs[0] && input_nvecs[1])
}

int main_batchgcd2(char *program_name, nvec_t **input_nvecs)
{
	y_batchgcd2(input_nvecs[0], input_nvecs[1]);
	return (0);
	//if (input_nvecs[0] && input_nvecs[1])
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		dprintf(2, "Usage : %s ...\n", argv[0]);
		return (1);
	}

	nvec_t **input_nvecs;
	FILE	*output_file_descriptor;

	input_nvecs = read_input_files(argc, argv);
	if (input_nvecs == NULL)
		return (2);
	

	if (!strcmp(argv[1], "product"))
	{
		output_file_descriptor = open_output_file(argc, argv);
		if (output_file_descriptor == NULL)
			return (3);
		main_product(argv[0], input_nvecs, output_file_descriptor);
		fclose(output_file_descriptor);
	}
	if (!strcmp(argv[1], "batchgcd"))
	{
		main_batchgcd(argv[0], input_nvecs, output_file_descriptor);
	}
		if (!strcmp(argv[1], "batchgcd2"))
	{
		main_batchgcd2(argv[0], input_nvecs);
	}

	while (*input_nvecs)
		nvec_free(*input_nvecs++);

	return (0);
}