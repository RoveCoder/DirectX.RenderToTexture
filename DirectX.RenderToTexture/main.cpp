#include <SDL.h>
#include "Renderer.h"
#include "Shader.h"
#include "Camera.h"
#include <algorithm>

#include "Crate.h"
#include "Floor.h"
#include "Pillar.h"
#include "Overlay.h"

#include "OverlayShader.h"

#include "DirectionalLightSource.h"
#include "PointLightSource.h"

#include "ShaderData.h"

int main(int argc, char** argv)
{
	// Setup SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		return -1;
	}

	// Create the window
	int window_width = 800;
	int window_height = 600;

	SDL_Window* window = SDL_CreateWindow("DirectX", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, window_width, window_height, SDL_WINDOW_MAXIMIZED | SDL_WINDOW_RESIZABLE);
	if (window == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		return -1;
	}

	// Create renderer
	Renderer* renderer = new Renderer(window);
	if (!renderer->Init())
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Renderer::Init failed", nullptr);
		return -1;
	}

	//renderer->SetLinearFilter();
	renderer->SetAnisotropicFilter();

	// Camera
	Camera* camera = new Camera(800, 600);

	// Create shader
	Shader* shader = new Shader(renderer);
	if (!shader->Create())
		return -1;

	OverlayShader* overlayShader = new OverlayShader(renderer);
	if (!overlayShader->Create())
		return -1;

	// Models
	Crate* crate = new Crate(renderer);
	if (!crate->Load())
		return -1;

	Floor* floor = new Floor(renderer);
	if (!floor->Load())
		return -1;

	Pillar* pillarLeft = new Pillar(renderer);
	if (!pillarLeft->Load())
		return -1;
	
	Pillar* pillarRight = new Pillar(renderer);
	if (!pillarRight->Load())
		return -1;

	pillarLeft->Position.x = -3.0f;
	pillarRight->Position.x = 3.0f;

	Overlay* overlay = new Overlay(renderer, camera);
	if (!overlay->Load(window_width, window_height))
		return -1;

	// Lights
	DirectionalLightSource* directionalLightSource = new DirectionalLightSource(renderer, camera);
	if (!directionalLightSource->Load())
		return -1;

	PointLightSource* pointLightSource = new PointLightSource(renderer, camera);
	if (!pointLightSource->Load())
		return -1;

	// Main loop
	bool running = true;
	while (running)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				break;

			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
					renderer->Resize(e.window.data1, e.window.data2);
					camera->Resize(e.window.data1, e.window.data2);
					break;
				}

			case SDL_MOUSEMOTION:
				if (e.motion.state == SDL_BUTTON_LMASK)
				{
					camera->Update((float)e.motion.xrel, (float)e.motion.yrel);
				}
				break;

			case SDL_MOUSEWHEEL:
				camera->UpdateFov((float)e.wheel.y);
				break;

			case SDL_KEYDOWN:
				if (e.key.keysym.scancode == SDL_SCANCODE_1)
				{
					static bool enable_wireframe = false;
					enable_wireframe = !enable_wireframe;
					renderer->EnableWireframe(enable_wireframe);
				}
				break;
			}
		}
		else
		{
			// Update lights
			LightBuffer lightBuffer;
			lightBuffer.mDirectionalLight = directionalLightSource->GetDirectionalLight();
			lightBuffer.mPointLight = pointLightSource->GetPointLight();

			lightBuffer.mDirectionalLight.mCameraPos = camera->GetPosition();
			lightBuffer.mPointLight.mCameraPos = camera->GetPosition();

			ID3D11Buffer* lightConsantBuffer = renderer->GetLightConstantBuffer();
			renderer->GetDeviceContext()->PSSetConstantBuffers(1, 1, &lightConsantBuffer);
			renderer->GetDeviceContext()->UpdateSubresource(lightConsantBuffer, 0, nullptr, &lightBuffer, 0, 0);

			//
			// Render to texture
			//
			overlay->SetRenderTargetView();

			// Draw models
			shader->Use();
			crate->Render(camera);
			floor->Render(camera);

			pillarLeft->Render(camera);
			pillarRight->Render(camera);

			directionalLightSource->Render();
			pointLightSource->Render();

			//
			// Render to backbuffer
			//
			renderer->Clear();
			renderer->SetRenderTargetView();

			// Draw models
			shader->Use();
			crate->Render(camera);
			floor->Render(camera);

			pillarLeft->Render(camera);
			pillarRight->Render(camera);

			directionalLightSource->Render();
			pointLightSource->Render();

			// Draw overlay
			overlayShader->Use();
			overlay->Render();

			// Draw scene
			renderer->Render();
		}
	}

	// Cleanup
	renderer->Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}