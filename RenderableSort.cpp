#include "RenderableSort.h"
#include "Renderable.h"

bool RenderableSort::operator() (const Renderable* renderable1, const Renderable* renderable2)
{
	return renderable1->GetOrder() < renderable2->GetOrder();
}
