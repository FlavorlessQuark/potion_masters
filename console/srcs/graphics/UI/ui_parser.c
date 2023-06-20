#include "../includes/splendor.h"
// Container config format :
//  1 line per object
//  First line always describes a container
//  Container: x,y,w,h|wType,hType,margin|alignDir,alignType
//  Element:  x,y,w,h|wType,hType,margin
//  c[amount] : container x amount
//  e[amount] : elem x amount
//  containers and elements will be parented to the container that came before
int	extract_num(char *str, int *number)
{
	int spn;

	spn = strcspn(str, NUMS);
	*number = atoi(str + spn);
	return spn + strspn(str + spn, NUMS);
}

char *fetch_file(char *filename, size_t *size)
{
	FILE	*file;
	char 	*content;
	file = fopen(filename, "r");
	fseek(file, 0L , SEEK_END);

	*size= ftell(file);
	content = calloc(*size + 1, 1);

	rewind(file);
	fread(content, *size, 1, file);
	fclose(file);
	return content;
}

char *fill_elem(SDLX_ContainerElem *elem, SDLX_RectContainer *parent, char *line)
{
	int num;
	SDLX_Display *display;

	display = SDLX_DisplayGet();
	elem->boundingBox = &elem->_boundingBox;
	line = line + extract_num(line, &num);
	elem->boundingBox->x = num;
	line = line + extract_num(line, &num);
	elem->boundingBox->y = num;
	line = line + extract_num(line, &num);

	if (line[0] == '%')
		num = ((parent ? parent->self.boundingBox->w : display->win_w) * num ) / 100;

	elem->boundingBox->w = num;
	line = line + extract_num(line, &num);

	if (line[0] == '%')
		num = ((parent ? parent->self.boundingBox->h : display->win_h)  * num ) / 100;

	elem->boundingBox->h = num;
	line = line + extract_num(line, &num);
	elem->heightTYpe = num;
	line = line + extract_num(line, &num);
	elem->widthTYpe = num;
	line = line + extract_num(line, &num);
	elem->margin = num;

	return line;
}

char *fill_container(SDLX_RectContainer *container, SDLX_RectContainer *parent, char *line)
{
	int num;

	line = fill_elem(&container->self, parent, line);
	line = line + extract_num(line, &num);
	container->alignDirection = num;
	line = line + extract_num(line, &num);
	container->alignment = num;

	return line;
}


char *parse_object(SDLX_RectContainer *current,SDLX_RectContainer *parent, char *line)
{
	int count;

	// SDL_Log("Start %s", line);
	fill_container(current, parent, line);
	line = strtok(NULL, "\n");

	SDL_Log("Line before %s", line);
	if (line)
		line += strspn(line, "\t");
	SDL_Log("Line after %s", line);
	count = 0;
	if (line && line[0] == 'c')
	{
		count = SDL_atoi(line + 1);
		current->containerCount = count;
		current->containers = SDL_calloc(count, sizeof(SDLX_RectContainer));
		line = strtok(NULL, "\n");
		for (int i = 0; i < count && line; i++)
		{
			line = parse_object(&current->containers[i], current, line);
			if (i > 0  && current != NULL)
			{
				if (current->alignDirection == SDLX_ALIGN_HORIZONTAL)
					current->containers[i].self.boundingBox->x += current->containers[i - 1].self.boundingBox->x + current->containers[i - 1].self.boundingBox->w;
				else
					current->containers[i].self.boundingBox->y += current->containers[i - 1].self.boundingBox->y + current->containers[i - 1].self.boundingBox->h;
			}
		}
	}
	if (line && line[0] == 'e')
	{
		count = SDL_atoi(line + 1);
		current->elemCount = count;
		current->elems = SDL_calloc(count, sizeof(SDLX_ContainerElem));
		for (int i = 0; i < count; i++)
		{
			line = strtok(NULL, "\n");
			line = fill_elem(&current->elems[i],current, line);
		}
		line = strtok(NULL, "\n");
	}
	return line;

}

SDLX_RectContainer *parse_UIConfig(char *filename)
{
	SDLX_RectContainer *parent;
	char *file;
	size_t size = 0;

	file = fetch_file(filename, &size);
	file = strtok(file, "\n");

	parent = calloc(1, sizeof(SDLX_RectContainer));
	parse_object(parent, NULL, file);
	SDL_free(file);

	return parent;
}


