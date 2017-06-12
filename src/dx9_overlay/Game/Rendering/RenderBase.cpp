#include "RenderBase.h"

int RenderBase::xCalculator = 800;
int RenderBase::yCalculator = 600;

RenderBase::RenderBase(Renderer *renderer)
	: _renderer(renderer), _isMarkedForDeletion(false), _resourceChanged(false), _hasToBeInitialised(true), _firstDrawAfterReset(false)
{
}

RenderBase::~RenderBase()
{
}

void RenderBase::setPriority(int p)
{
	_priority = p;
}

int RenderBase::priority()
{
	return _priority;
}

void RenderBase::changeResource()
{
	_resourceChanged = true;
}

float RenderBase::scaleX()
{
	return (float)_renderer->screenWidth() / (float)xCalculator;
}

float RenderBase::scaleY()
{
	return (float)_renderer->screenHeight() / (float)yCalculator;
}

int RenderBase::calculatedXPos(int x)
{
	return (int)((float)x * scaleX());
}

int RenderBase::calculatedYPos(int y)
{
	return (int)((float)y * scaleY());
}

Renderer *RenderBase::renderer()
{
	return _renderer;
}


