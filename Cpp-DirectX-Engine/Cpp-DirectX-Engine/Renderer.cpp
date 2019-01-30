#include "Renderer.h"

using namespace DirectX;

// Initialize values in the renderer
void Renderer::Init()
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;
}

// Destructor for when the singleton instance is deleted
Renderer::~Renderer()
{ 
	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
}

// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
void Renderer::LoadShaders(ID3D11Device* device, ID3D11DeviceContext* context)
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");
}

// Draw all entities in the render list
void Renderer::Draw(ID3D11DeviceContext* context, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix)
{
	for (size_t i = 0; i < renderList.size(); i++)
	{
		// Send data to shader variables
		//  - Do this ONCE PER OBJECT you're drawing
		//  - This is actually a complex process of copying data to a local buffer
		//    and then copying that entire buffer to the GPU.  
		//  - The "SimpleShader" class handles all of that for you.
		vertexShader->SetMatrix4x4("world", renderList[i]->GetWorldMatrix());
		vertexShader->SetMatrix4x4("view", viewMatrix);
		vertexShader->SetMatrix4x4("projection", projectionMatrix);

		// Once you've set all of the data you care to change for
		// the next draw call, you need to actually send it to the GPU
		//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
		vertexShader->CopyAllBufferData();

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame...YET
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		vertexShader->SetShader();
		pixelShader->SetShader();

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex);
		UINT offset = 0;

		ID3D11Buffer* vertexBuffer = renderList[i]->GetVertexBuffer();
		ID3D11Buffer* indexBuffer = renderList[i]->GetIndexBuffer();
		context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			renderList[i]->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}
}

// Check if an entity is in the render list
bool Renderer::IsEntityInRenderList(Entity* e)
{
	return e->IsInRenderList();
}

// Add an entity to the render list
void Renderer::AddEntityToRenderList(Entity* e)
{
	renderList.push_back(e);
}

// Remove an entity from the render list
void Renderer::RemoveEntityFromRenderList(Entity* e)
{
	//Early return if render list is empty
	if (renderList.size() == 0)
		return;

	//Get the index of the entity
	size_t* index = nullptr;
	size_t i;
	for (i = 0; i < renderList.size(); i++)
	{
		if (e == renderList[i])
		{
			index = &i;
			break;
		}
	}

	//Entity is not in the render list
	if (index == nullptr)
		return;

	//If the entity is not the very last we swap it for the last one
	if (*index != renderList.size() - 1)
	{
		std::swap(renderList[*index], renderList[renderList.size() - 1]);
	}

	//Pop the last one
	renderList.pop_back();
	return;
}