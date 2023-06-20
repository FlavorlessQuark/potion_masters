#include "splendor.h"

void print_config(SDL_Renderer *renderer, SDLX_RectContainer *root)
{
	SDL_SetRenderDrawColor(renderer, 255, 0,0,255);
	SDL_RenderDrawRect(renderer, root->self.boundingBox);

	for (int i = 0; i < root->containerCount; i++)
		print_config(renderer, &root->containers[i]);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255,255);
	for (int x = 0; x < root->elemCount; x++)
	{
		SDL_RenderDrawRect(renderer, root->elems[x].boundingBox);
	}
	SDL_SetRenderDrawColor(renderer, 0, 0,0,255);
}

SDLX_RectContainer *loadConfig(char *filename)
{
	SDLX_RectContainer *root;

	root = parse_UIConfig(filename);
	SDLX_ContainerUpdate(root, NULL);

	return root;
}


void cleanupUIConfig(SDLX_RectContainer *container)
{
	for (int i = 0; i < container->containerCount; i++)
		cleanupUIConfig(&container->containers[i]);
	if (container->containerCount > 0)
		SDL_free(container->containers);
	if (container->elemCount > 0)
		SDL_free(container->elems);
}
