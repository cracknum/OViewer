#include "PlaneTexture.h"
#include <vtkObjectFactory.h>

vtkStandardNewMacro(PlaneTexture);

void PlaneTexture::PrintSelf(ostream& os, vtkIndent indent)
{
	Superclass::PrintSelf(os, indent);
}

void PlaneTexture::Load(vtkRenderer* renderer)
{
}

void PlaneTexture::ReleaseGraphicsResources(vtkWindow*)
{
}

PlaneTexture::~PlaneTexture() {}
