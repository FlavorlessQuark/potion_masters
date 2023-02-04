#include "../includes/splendor.h"

# define NUMS	"-0123456789"
# define NMATHC	"0123456789-+()x*=/ "

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

// Minor optimization but harcoding is in theory faster than loops
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
	{
		// SDL_Log("FOund parent w %d , display w %d, num %d, result %d ",
		// parent->self.boundingBox->w,
		// display->win_w,
		// num,
		// ((parent ? parent->self.boundingBox->w : display->win_w) * num ) / 100
		// );
		num = ((parent ? parent->self.boundingBox->w : display->win_w) * num ) / 100;
	}
	elem->boundingBox->w = num;
	line = line + extract_num(line, &num);
	if (line[0] == '%')
	{
		// SDL_Log("FOund parent h %d , display h %d, num %d, result %d ",
		// parent->self.boundingBox->h,
		// display->win_h,
		// num,
		// ((parent ? parent->self.boundingBox->h : display->win_h)  * num ) / 100
		// );
		num = ((parent ? parent->self.boundingBox->h : display->win_h)  * num ) / 100;
	}
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

	// SDL_Log("Container FILL %p | w %d h %d x %d y %d",
	// 			container,
	// 			container->self.boundingBox->w,
	// 			container->self.boundingBox->h,
	// 			container->self.boundingBox->x,
	// 			container->self.boundingBox->y
	// 		);
	return line;
}


char *parse_object(SDLX_RectContainer *current,SDLX_RectContainer *parent, char *line)
{
	int count;

	// SDL_Log("Start %s", line);
	fill_container(current, parent, line);
	line = strtok(NULL, "\n");

	count = 0;
	if (line && line[0] == 'c')
	{
		count = SDL_atoi(line + 1);
		current->containerCount = count;
		current->containers = SDL_calloc(count, sizeof(SDLX_RectContainer));
		line = strtok(NULL, "\n");
		for (int i = 0; i < count && line; i++)
		{
			// SDL_Log("Parse line %i %s, parent %p %d", i, line, parent, parent != NULL);
			// SDL_Log("Child %d %p of %p", i, &current->containers[i], current);
			line = parse_object(&current->containers[i], current, line);
			if (i > 0  && current != NULL)
			{
				if (current->alignDirection == SDLX_ALIGN_HORIZONTAL)
				{
					current->containers[i].self.boundingBox->x += current->containers[i - 1].self.boundingBox->x + current->containers[i - 1].self.boundingBox->w;
				}
				else
				{
					current->containers[i].self.boundingBox->y += current->containers[i - 1].self.boundingBox->y + current->containers[i - 1].self.boundingBox->h;
				}
			}
			// SDL_Log("Container  %p | w %d h %d x %d y %d",
			// 	&current->containers[i],
			// 	current->containers[i].self.boundingBox->w,
			// 	current->containers[i].self.boundingBox->h,
			// 	current->containers[i].self.boundingBox->x,
			// 	current->containers[i].self.boundingBox->y
			// );
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
			// SDL_Log("FILL Elem %d of %p : %s", i, current, line);
			line = fill_elem(&current->elems[i],current, line);
			// SDL_Log("RESULT Elem  %p | w %d h %d x %d y %d",
			// 	&current->elems[i],
			// 	current->elems[i].boundingBox->w,
			// 	current->elems[i].boundingBox->h,
			// 	current->elems[i].boundingBox->x,
			// 	current->elems[i].boundingBox->y
			// );
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
