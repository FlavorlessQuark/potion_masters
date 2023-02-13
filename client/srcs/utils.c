#include "../includes/splendor.h"
#define NUMS "0123456789"


SDLX_RectContainer *parseUI(char *filename)
{
	SDLX_RectContainer *root;

	root = parse_UIConfig(filename);
	SDLX_ContainerUpdate(root, NULL);

	return root;
}

int	extract_num(char *str, int *number)
{
	int spn;

	spn = strcspn(str, NUMS);
	*number = atoi(str + spn);
	return spn + strspn(str + spn, NUMS);
}


void startTurn(Context *ctx)
{
	ctx->mainscreen->switchMode.enabled = SDL_TRUE;
	for (int i = 0; i < MAX_RESERVE; i++)
		ctx->mainscreen->reserved[i].enabled = SDL_TRUE;
}

void endTurn(Context *ctx)
{
	ctx->mainscreen->switchMode.enabled = SDL_FALSE;
	for (int i = 0; i < MAX_RESERVE; i++)
		ctx->mainscreen->reserved[i].enabled = SDL_FALSE;
}


void printRect(SDL_Rect *rect, char * msg)
{
	SDL_Log("[%s] Rect (%d,%d) | w: %d h: %d",
			msg,
			rect->x,
			rect->y,
			rect->w,
			rect->h
		);
}
