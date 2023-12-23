# include "../../includes/SDLX/SDLX.h"
// Container config format :
//  1 line per object
// Objects can be elements or containers
// Containers are objects within which elements can be arranged
// A container can have 0+ children and those children can be containers or elements
// Elements are leaf node, they have no children
//
//  First line MUST describes a container
//  Container: x,y,w%,h%|margin|alignDir,alignType
//  Element:   x,y,w%,h%|margin
//  c[amount] : container x amount
//  e[amount] : elem x amount
//
// Where :
// x , y : x and y offset in pixels
// w% , h% : width and height, specified as a % of the respective dimension of the parent object
// wType,hType: irrelevant for now, ignore
// margin: margin around the object
// alignDir : SDLX_ALIGN_VERTICAL or SDLX_ALIGN_HORIZONAL Direction of the container, the children elements will be aligned following this direction
// alignType : How the elements in the container are spaced and positionned
//      SDLX_RIGHT_ALIGN  = |o o _ _|
//      SDLX_CENTER_ALIGN = |_ o o _|
//      SDLX_LEFT_ALIGN   = |_ _ o o|
//      SDLX_TOP_ALIGN
//      SDLX_BOTTOM_ALIGN,
//      SDLX_SPACE_EVEN   = |_ o _ o _|
//      SDLX_SPACE_BETWEEN= |o _ _ _ o|
//      SDLX_ALIGN_VERTICAL,
//      SDLX_ALIGN_HORIZONTAL




// You may use tabs to align children with their parent
//  containers and elements will be parented to the container that came before

# define NUMS	"-0123456789"

static int	extract_num(char *str, int *number)
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
	line = line + extract_num(line, &elem->_origin.x);
	line = line + extract_num(line, &elem->_origin.y);


	line = line + extract_num(line, &elem->_origin.w);
	if (line[0] == '%')
		elem->heightTYpe = PERCENTAGE;
	else
		elem->heightTYpe = PIXEL;
		// num = ((parent ? parent->self.boundingBox->w : display->win_w) * num ) / 100;


	line = line + extract_num(line, &elem->_origin.h);
	if (line[0] == '%')
		elem->heightTYpe = PERCENTAGE;
	else
		elem->heightTYpe = PIXEL;
		// num = ((parent ? parent->self.boundingBox->h : display->win_h)  * num ) / 100;
	line = line + extract_num(line, &elem->margin);

	return line;
}

char *fill_container(SDLX_RectContainer *container, SDLX_RectContainer *parent, char *line)
{
	int num;

	line = fill_elem(&container->self, parent, line);
	line = line + extract_num(line, &container->alignDirection);
	line = line + extract_num(line, &container->alignment);

	return line;
}


char *parse_object(SDLX_RectContainer *current,SDLX_RectContainer *parent, char *line)
{
	int count;

	// SDL_Log("Start %s", line);
	fill_container(current, parent, line);
	line = strtok(NULL, "\n");

	if (line)
		line += strspn(line, "\t");
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

SDLX_RectContainer *SDLX_ParseConfig(char *filename)
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


void SDLX_DisplayConfig(SDL_Renderer *renderer, SDLX_RectContainer *root)
{
	SDL_SetRenderDrawColor(renderer, 255, 0,0,255);
	SDL_RenderDrawRect(renderer, &root->self.boundingBox);

	for (int i = 0; i < root->containerCount; i++)
		SDLX_DisplayConfig(renderer, &root->containers[i]);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255,255);
	for (int x = 0; x < root->elemCount; x++)
	{
		SDL_RenderDrawRect(renderer, &root->elems[x].boundingBox);
	}
	SDL_SetRenderDrawColor(renderer, 0, 0,0,255);
}

SDLX_RectContainer *SDLX_LoadConfig(char *filename)
{
	SDLX_RectContainer *root;

	root = SDLX_ParseConfig(filename);
	SDLX_ContainerUpdate(root, NULL);


	return root;
}


void SDLX_CleanupConfig(SDLX_RectContainer *container)
{
	for (int i = 0; i < container->containerCount; i++)
		SDLX_CleanupConfig(&container->containers[i]);
	if (container->containerCount > 0)
		SDL_free(container->containers);
	if (container->elemCount > 0)
		SDL_free(container->elems);
}


