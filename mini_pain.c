#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

//-------------------------------------------
#define	NO_ERROR	0
#define ERROR		1
//-------------------------------------------
#define ERR_END_OF_FILE	0
#define ERR_ARG			1
#define ERR_FILE_CRPT	2
//-------------------------------------------
#define TXT_ERR_ARG			"Error: argument"
#define TXT_ERR_FILE_CRPT	"Error: Operation file corrupted\n" 
//-------------------------------------------
#define PXL_OUTSIDE		0
#define PXL_INSIDE		1
#define PXL_ON_EDGE		2
//-------------------------------------------
typedef struct s_zone{
	int w;
	int h;
	char bg;
	int total;
} t_zone;
//-------------------------------------------
typedef struct s_circle
{
	char type;
	float x;
	float y;
	float r;
	char  c;	
} t_circle;
//-------------------------------------------
t_zone	z;
t_circle c;
char *map;
//-------------------------------------------
int ft_strlen(char *str)
{
	int i;

	i = 0;
	while(str[i] != '\0')
		i++;
	return i;
}
//-------------------------------------------
int msg_error(FILE *ptr_file, int err_type)
{
	if(err_type == ERR_ARG)
	{
		write(1, TXT_ERR_ARG, ft_strlen(TXT_ERR_ARG));
		if(ptr_file)
			fclose(ptr_file);
		return ERROR;
	}
	else if (err_type == ERR_FILE_CRPT)
	{
		write(1, TXT_ERR_FILE_CRPT, ft_strlen(TXT_ERR_FILE_CRPT));
		if(ptr_file)
			fclose(ptr_file);
		return ERROR;
	}
	else // if (err_type == ERR_END_OF_FILE)
	{
		for (int y = 0; y < z.h; y++)
		{
			write(1, &map[y * z.w], z.w);
			write(1, "\n", 1);
		if(ptr_file)
			fclose(ptr_file);
		return NO_ERROR;
		}
	}
}
//-------------------------------------------
int pixel_status(float x, float y, t_circle *c)
{
	float distance;

	distance = sqrtf(powf(x - c->x, 2) + powf(y - c->y,2));
	
	if (distance > c->r)
		return PXL_OUTSIDE;
	else if (c->r - distance < 1)
		return PXL_ON_EDGE;
	return PXL_INSIDE;
}
//-------------------------------------------
int main(int argc, char **argv)
{
	FILE *ptr_file;
	int res;
	int in_out_on;

	ptr_file = NULL;

	if (argc != 2)
		return (msg_error(ptr_file, ERR_ARG));

	if ((ptr_file = fopen(argv[1], "r")) == NULL)
		return (msg_error(ptr_file, ERR_FILE_CRPT));

	if (fscanf(ptr_file, "%d %d %c", &z.w, &z.h, &z.bg) != 3)
		return (msg_error(ptr_file, ERR_FILE_CRPT));

	if (z.w <= 0 || z.w > 300 || z.h <= 0 || z.h > 300)
		return (msg_error(ptr_file, ERR_FILE_CRPT));

	z.total = z.w * z.h;
	map = malloc (z.total * sizeof(char));
	memset(map, z.bg, z.total);

	while(1)
	{
		res = fscanf(ptr_file, "\n%c %f %f %f %c", &c.type, &c.x, &c.y, &c.r, &c.c);
		if (res == -1)
			return (msg_error(ptr_file, ERR_END_OF_FILE));
		else if(res != 5 || c.r <= 0 || (c.type != 'c' && c.type != 'C'))
			return (msg_error(ptr_file, ERR_FILE_CRPT));
		
		for (int y = 0; y < z.h; y++)
		{
			for (int x = 0; x < z.w; x++)
			{
				in_out_on = pixel_status(x, y, &c);
				if (
					(c.type == 'c' && in_out_on == PXL_ON_EDGE) 
					|| 
					(c.type == 'C' && in_out_on)
					)
					map[x + y * z.w] = c.c;
			}
		}
	}
}
//-------------------------------------------