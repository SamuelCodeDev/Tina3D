#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "Types.h"
#include "Window.h"

#include <D3DCompiler.h>
#include <dxgi1_6.h>
#include <d3d11_4.h>

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace Tina
{
    class GraphicsDesc
	{
	protected:
		D3D_FEATURE_LEVEL featureLevel;
		uint32   backBufferCount;
		uint32   antialiasing;
		uint32   quality;
		bool     vSync;
		float    bgColor[4];

	public:
		uint32 Antialiasing() const noexcept;
		uint32 Quality() const noexcept;
		void VSync(const bool state) noexcept;
	};

	inline uint32 GraphicsDesc::Antialiasing() const noexcept
	{ return antialiasing; }

	inline uint32 GraphicsDesc::Quality() const noexcept
	{ return quality; }

	inline void GraphicsDesc::VSync(const bool state) noexcept
	{ vSync = state; }

    class Graphics final : public GraphicsDesc
	{
	private:
		ID3D11Device5 * device;
		ID3D11DeviceContext4 * context;
		IDXGIFactory7 * factory;
		IDXGISwapChain1 * swapChain;

		ID3D11BlendState1 * blendState;

		ID3D11RenderTargetView1 * renderTargetView;
		ID3D11Resource ** renderTargets;

		ID3D11Texture2D * depthStencil;
		ID3D11DepthStencilState* depthStencilState;
    	ID3D11DepthStencilView* depthStencilView;

		void LogHardwareInfo();

	public:
		Graphics() noexcept;
		~Graphics();

		void Initialize(Window* window);
		void Clear() const noexcept;
		void Present() const noexcept;

		static D3D11_RECT scissorRect;
		static D3D11_VIEWPORT viewport;

		ID3D11Device5* Device() const noexcept;
		ID3D11DeviceContext4* ContextDevice() const noexcept;
	};

	inline ID3D11Device5* Graphics::Device() const noexcept
	{ return device; }
	
	inline ID3D11DeviceContext4* Graphics::ContextDevice() const noexcept
	{ return context; }

	inline void Graphics::Clear() const noexcept
	{ 
		context->ClearRenderTargetView(reinterpret_cast<ID3D11RenderTargetView*>(renderTargetView), bgColor);
		context->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
    }

	inline void Graphics::Present() const noexcept
	{
		swapChain->Present(vSync, 0);
		context->OMSetRenderTargets(1, reinterpret_cast<ID3D11RenderTargetView**>(renderTargetView), depthStencilView);
	}
}

#endif