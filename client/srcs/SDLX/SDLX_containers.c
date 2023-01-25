# include "../../includes/SDLX/SDLX.h"


void SDLX_ContainerPrint(SDLX_RectContainer *current,SDLX_RectContainer *parent)
{
	SDL_Log("Container %p | Child Of %p | Position (%d,%d) | Dimensions (%d,%d) | Container childCount %d | ElemChildCount %d",
		current, parent,
		current->self.boundingBox->x,
		current->self.boundingBox->y,
		current->self.boundingBox->w,
		current->self.boundingBox->h,
		current->containerCount,
		current->elemCount
	);
	for (int x = 0; x < current->containerCount; x++)
		SDLX_ContainerPrint(&current->containers[x], current);
	for (int x = 0; x < current->elemCount; x++)
	{
		SDL_Log("Elem %p Child %d Of %p | Position (%d,%d) | Dimensions (%d,%d) ",
		current,
		x,
		parent,
		current->elems[x].boundingBox->x,
		current->elems[x].boundingBox->y,
		current->elems[x].boundingBox->w,
		current->elems[x].boundingBox->h
		);
	}
}

void SDLX_ContainerElemCreate(SDLX_ContainerElem *dest, SDL_Rect *boundingBox, int margin, int widthType, int heightType)
{

	if (!boundingBox)
	{
		dest->_origin = dest->_boundingBox;
		dest->boundingBox = &dest->_boundingBox;
	}
	else
	{
		dest->boundingBox = boundingBox;
		dest->_boundingBox = *boundingBox;
		dest->_origin = *boundingBox;
	}
	dest->margin = margin;
	dest->widthTYpe = widthType;
	dest->heightTYpe = heightType;
}


void get_innerMeasurements(SDLX_RectContainer *container)
{
	SDLX_ContainerElem *elems;
	SDLX_RectContainer *childCont;
	int i;

	i = 0;
	elems = container->elems;
	childCont = container->containers;
	while (i < container->elemCount)
	{
		if (container->alignDirection == SDLX_ALIGN_VERTICAL)
		{
			container->innerWidth = MAX(elems[i].boundingBox->w + elems[i].margin, container->innerWidth);
			container->innerHeight += elems[i].boundingBox->h + elems[i].margin;
		}
		else
		{
			container->innerWidth += elems[i].boundingBox->w + elems[i].margin;
			container->innerHeight = MAX(elems[i].boundingBox->h + elems[i].margin, container->innerHeight);
		}
		// SDL_Log("Inner wifdth %d Inner Hieght %d", container->innerWidth, container->innerHeight);
		i++;
	}
	i = 0;
	while (i < container->containerCount)
	{
		if (container->alignDirection == SDLX_ALIGN_VERTICAL)
		{
			container->innerWidth = MAX(childCont[i].self.boundingBox->w + childCont[i].self.margin, container->innerWidth);
			container->innerHeight += childCont[i].self.boundingBox->h + childCont[i].self.margin;
		}
		else
		{
			container->innerWidth += childCont[i].self.boundingBox->w + childCont[i].self.margin;
			container->innerHeight = MAX(childCont[i].self.boundingBox->h + childCont[i].self.margin, container->innerHeight);
		}
		// SDL_Log("Inner wifdth %d Inner Hieght %d", container->innerWidth, container->innerHeight);
		i++;
	}
	// SDL_Log("FINAL : Inner wifdth %d Inner Hieght %d", container->innerWidth, container->innerHeight);
}

int calc_offset(SDLX_RectContainer *container, SDLX_Display *display)
{

	int measurement;
	int direction;
	int offset;

	offset = 0;
	if (container->alignDirection == SDLX_ALIGN_VERTICAL)
	{
		measurement = container->innerHeight;
		direction =container->self.boundingBox->h;
	}
	else
	{
		measurement = container->innerWidth;
		direction = container->self.boundingBox->w;
	}
	if (container->alignment == SDLX_CENTER_ALIGN)
		offset = (direction - measurement) / 2;
	else if (container->alignment == SDLX_SPACE_EVEN)
		offset = (direction - measurement) / (container->elemCount + container->containerCount + 1);
	else if (container->alignment == SDLX_RIGHT_ALIGN || container->alignment == SDLX_BOTTOM_ALIGN)
		offset = direction - measurement;
	return offset;
}
int calc_spacing(SDLX_RectContainer *container, SDLX_Display *display)
{
	int measurement;
	int direction;
	int spacing;

	spacing = 0;
	if (container->alignDirection == SDLX_ALIGN_VERTICAL)
	{
		measurement = container->innerHeight;
		direction = container->self.boundingBox->h;
	}
	else
	{
		measurement = container->innerWidth;
		direction = container->self.boundingBox->w;
	}
	if (container->alignment == SDLX_SPACE_EVEN)
		spacing = (direction - measurement) / (container->elemCount + container->containerCount + 1);
	else if (container->alignment == SDLX_SPACE_BETWEEN)
		spacing = direction - measurement;
	return spacing;
}

void position_elems( SDLX_RectContainer *container, SDLX_Display *display)
{
	int spacing = 0;
	int offset = 0;
	int dir;
	int x;
	int y;

	offset = calc_offset(container, display);
	spacing = calc_spacing(container, display);

	dir = container->alignDirection == SDLX_ALIGN_VERTICAL;
	x = container->self.boundingBox->x + (offset * (dir ^ 1));
	y = container->self.boundingBox->y + (offset * (dir ^ 0));
	// SDL_Log("Spacing %d Offset %d", spacing, offset);
	SDL_SetRenderDrawColor(SDLX_DisplayGet()->renderer, 255, 0, 0, 255);
	for (int i = 0; i < container->containerCount; i++)
	{
		// Override the axis of alignment ex: if vertical aligned object has a Y set, replace it to be aligned, but don't replace the X
		// SDL_Log("Box %d (%d,%d) | w: %d, h: %d",i, container->elems[i].boundingBox->x
		// , container->elems[i].boundingBox->y
		// , container->elems[i].boundingBox->w
		// , container->elems[i].boundingBox->h
		// );
		// SDL_Log(" %d X %d Y %d | Firection %d", x, y, dir);
		container->containers[i].self.boundingBox->x = x ;
		container->containers[i].self.boundingBox->y = y ;
		// container->elems[i].boundingBox->x = x + container->self.boundingBox->x;
		// container->elems[i].boundingBox->y = y + container->self.boundingBox->y;
		x += (container->containers[i].self.boundingBox->w + spacing) * (dir ^ 1);
		y += (container->containers[i].self.boundingBox->h + spacing) * (dir ^ 0);
		// SDL_Log("AFTER Elem %d/%d of %p | w %d h %d x %d y %d",
		// i, container->elemCount,
		// container,
		// container->elems[i].boundingBox->h,
		// container->elems[i].boundingBox->w,
		// container->elems[i].boundingBox->x,
		// container->elems[i].boundingBox->y
		// );
		SDL_RenderDrawRect(SDLX_DisplayGet()->renderer, container->containers[i].self.boundingBox);
		// SDL_Log("Box %d (%d,%d) | w: %d, h: %d",i, container->elems[i].boundingBox->x
		// , container->elems[i].boundingBox->y
		// , container->elems[i].boundingBox->w
		// , container->elems[i].boundingBox->h
		// );
		// SDL_Log("Elem %d %d,%d", i, container->elems[i].boundingBox->x, container->elems[i].boundingBox->y);
	}
	// x = container->self.boundingBox->x + (offset * (dir ^ 1));
	// y = container->self.boundingBox->y + (offset * (dir ^ 0));
	SDL_SetRenderDrawColor(SDLX_DisplayGet()->renderer, 255, 255, 255, 255);
	for (int i = 0; i < container->elemCount; i++)
	{
		// Override the axis of alignment ex: if vertical aligned object has a Y set, replace it to be aligned, but don't replace the X
		// SDL_Log("Box %d (%d,%d) | w: %d, h: %d",i, container->elems[i].boundingBox->x
		// , container->elems[i].boundingBox->y
		// , container->elems[i].boundingBox->w
		// , container->elems[i].boundingBox->h
		// );
		// SDL_Log(" %d X %d Y %d | Firection %d", x, y, dir);
		container->elems[i].boundingBox->x = x ;
		container->elems[i].boundingBox->y = y ;
		// container->elems[i].boundingBox->x = x + container->self.boundingBox->x;
		// container->elems[i].boundingBox->y = y + container->self.boundingBox->y;
		x += (container->elems[i].boundingBox->w + spacing) * (dir ^ 1);
		y += (container->elems[i].boundingBox->h + spacing) * (dir ^ 0);
		// SDL_Log("AFTER Elem %d/%d of %p | w %d h %d x %d y %d",
		// i, container->elemCount,
		// container,
		// container->elems[i].boundingBox->h,
		// container->elems[i].boundingBox->w,
		// container->elems[i].boundingBox->x,
		// container->elems[i].boundingBox->y
		// );
		SDL_RenderDrawRect(SDLX_DisplayGet()->renderer, container->elems[i].boundingBox);
		// SDL_Log("Box %d (%d,%d) | w: %d, h: %d",i, container->elems[i].boundingBox->x
		// , container->elems[i].boundingBox->y
		// , container->elems[i].boundingBox->w
		// , container->elems[i].boundingBox->h
		// );
		// SDL_Log("Elem %d %d,%d", i, container->elems[i].boundingBox->x, container->elems[i].boundingBox->y);
	}

}

// No dynamic size for now
// We will consider the bounding box to be the total space that will be occupied. This means that when the box is too small it will not readjust
// We can make it dynamic by first claculting the size of all the inner elements
void SDLX_ContainerUpdate(SDLX_RectContainer *container, SDLX_RectContainer *parent)
{
	int i;
	int spacing;
	SDLX_Display *display;

	i = 0;
	container->innerWidth = 0;
	container->innerHeight = 0;
	display = SDLX_DisplayGet();

	// SDL_SetRenderDrawColor(SDLX_DisplayGet()->renderer, 255, 0, 0, 255);
	// SDL_RenderDrawRect(SDLX_DisplayGet()->renderer, container->self.boundingBox);
	get_innerMeasurements(container);
	position_elems(container, display);
	while (i < container->containerCount)
	{
		SDL_Log("Conainter %i Parent %p", i, parent);
		SDLX_ContainerUpdate(&container->containers[i], container);
		i++;
	}
	SDL_SetRenderDrawColor(SDLX_DisplayGet()->renderer, 0, 0, 0, 255);
}
