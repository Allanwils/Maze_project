#include "../headers/maze.h"


/**
 * Determine_rows - calculates the line count in a file
 * @fp: File pointer
 * Returns: number of lines in the file
 */
int count_rows(FILE *fp)
{
	char ch;
	int lines = 0;

	while (!feof(fp))
	{
		ch = fgetc(fp);
		if (ch == '\n')
		{
			lines++;
		}
	}

	fseek(fp, 0, SEEK_SET);

	return (lines + 1);
}



/**
 * in_array - verifies presence of a value in an array
 * @needle: value to search for in the array
 * @haystack: pointer to string
 * @haystack_size: size of the @haystack array
 * Returns: (0) if value not found, (1) if found
 */
int in_array(char needle, char *haystack, unsigned int haystack_size)
{
	unsigned int i;

	for (i = 0; i < haystack_size; i++)
	{
		if (haystack[i] == needle)
			return (1);
	}

	return (0);
}

/**
 * verify_line_data - validate line integrity in file
 * @max_line_count: maximum line length limit
 * @line: line to validate
 * @fp: File pointer
 * @line_number: current line number being validated
 * Return: void
 */
void validate_line_data(int max_line_count, char *line, FILE *fp,
		int line_number)
{
	char valid_digits[] = {'0', '1'};
	int i;
	unsigned int line_length = strlen(line);

	if (!line)
		return;
	if (line_length != (unsigned int)max_line_count)
	{
		free(line);
		close_file(fp);
		printf("Error line No. %d: The columns of the map should be equal.\n",
				line_number);
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < max_line_count; i++)
	{
		if ((in_array(line[i], valid_digits, max_line_count)) == 0)
		{

			printf("Error line No. %d: Invalid map values found.\n", line_number);
			exit(EXIT_FAILURE);
		}
	}
}

/**
 * merge - combines two strings
 * @s1: target string
 * @2: string to merge
 * Returns: Resultant string from merging @s1 and @s2
 */
char *concat(const char *s1, const char *s2)
{
	char *result = NULL;

	result = malloc(strlen(s1) + strlen(s2) + 1);
	if (!result)
		return (NULL);

	strcpy(result, s1);
	strcat(result, s2);
	return (result);
}

/**
 * release_map - deallocates 2D map in map_t structure
 * @map: map_t data structure with map information
 * Return: void
 */
void free_map(map_t *map)
{
	int i;

	for (i = 0; i < map->rows; i++)
		free(map->arr[i]);
	free(map->arr);
}
