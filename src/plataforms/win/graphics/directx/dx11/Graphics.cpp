#include "Graphics.h"
#include "Error.h"
#include "Utils.h"
#include <d3d11sdklayers.h>
#include <format>
#include <bit>
using std::format;
using std::bit_cast;

#if defined(_DEBUG)
    #pragma comment( lib, "dxguid.lib") 

    template <UINT TDebugNameLength>
    inline void SetDebugName(_In_ ID3D11DeviceChild* deviceResource, _In_z_ const char (&debugName)[TDebugNameLength])
    { deviceResource->SetPrivateData(WKPDID_D3DDebugObjectName, TDebugNameLength - 1, debugName); }
#endif

namespace Tina
{
    D3D11_RECT Graphics::scissorRect = {};
    D3D11_VIEWPORT Graphics::viewport = {};

	Graphics::Graphics() noexcept :
        factory { nullptr },
        device { nullptr },
        swapChain { nullptr },
        blendState{nullptr},
        depthStencil { nullptr },
        renderTargetView{ nullptr }
	{
        featureLevel = D3D_FEATURE_LEVEL_11_0;
        backBufferCount = 2;
        antialiasing = 1;
        quality = 0;
        vSync = false;

        renderTargets = new ID3D11Resource * [backBufferCount - 1] { nullptr };
        
        ZeroMemory(bgColor, sizeof(bgColor));
        ZeroMemory(&viewport, sizeof(viewport));
        ZeroMemory(&scissorRect, sizeof(scissorRect));
	}

    Graphics::~Graphics()
    {
        if (renderTargets)
        {
            for (uint32 i = 0; i < backBufferCount - 1; ++i)
                SafeRelease(renderTargets[i]);
            delete[] renderTargets;
        }

        if (swapChain)
        {
            swapChain->SetFullscreenState(false, nullptr);
            swapChain->Release();
        }

        SafeRelease(depthStencil);
        SafeRelease(device);
        SafeRelease(factory);
        SafeRelease(blendState);
        SafeRelease(renderTargetView);
    }

    void Graphics::LogHardwareInfo()
    {
        constexpr auto BytesinMegaByte = 1048576U; // 1'048'576

        // --------------------------------------
        // Adapter card (graphics card)
        // --------------------------------------
        IDXGIAdapter4* adapter = nullptr;
        if (factory->EnumAdapters1(0, bit_cast<IDXGIAdapter1**>(&adapter)) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_ADAPTER_DESC3 desc;
            adapter->GetDesc3(&desc);
            OutputDebugStringW(format(L"---> Graphics card: {}\n", desc.Description).c_str());
        }

        IDXGIAdapter4* adapter4 = nullptr;
        if (SUCCEEDED(adapter->QueryInterface(IID_PPV_ARGS(&adapter4))))
        {
            DXGI_QUERY_VIDEO_MEMORY_INFO memInfo;
            adapter4->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &memInfo);

            string text = format("---> Video Memory (Free): {} MB\n", memInfo.Budget / BytesinMegaByte);
            text += format("---> Video Memory (Used): {} MB\n", memInfo.CurrentUsage / BytesinMegaByte);

            OutputDebugString(text.c_str());

            adapter4->Release();
        }

        // ------------------------------------------
        // Maximum Level Feature Supported by the GPU
        // ------------------------------------------

        {
            string text = format("---> Feature Level: ");

            switch (featureLevel)
            {
            case D3D_FEATURE_LEVEL_11_1: text += "11_1\n"; break;
            case D3D_FEATURE_LEVEL_11_0: text += "11_0\n"; break;
            case D3D_FEATURE_LEVEL_10_1: text += "10_1\n"; break;
            case D3D_FEATURE_LEVEL_10_0: text += "10_0\n"; break;
            case D3D_FEATURE_LEVEL_9_3:  text += "9_3\n";  break;
            case D3D_FEATURE_LEVEL_9_2:  text += "9_2\n";  break;
            case D3D_FEATURE_LEVEL_9_1:  text += "9_1\n";  break;
            }

            OutputDebugString(text.c_str());
        }

        // -----------------------------------------
        // Video output (monitor)
        // -----------------------------------------

        IDXGIOutput6* output = nullptr;
        if (adapter->EnumOutputs(0, bit_cast<IDXGIOutput**>(&output)) != DXGI_ERROR_NOT_FOUND)
        {
            DXGI_OUTPUT_DESC1 desc;
            output->GetDesc1(&desc);
            OutputDebugStringW(format(L"---> Monitor: {}\n", desc.DeviceName).c_str());
        }

        // ------------------------------------------
        // Video Mode (resolution)
        // ------------------------------------------

        uint32 dpi = GetDpiForSystem();
        uint32 screenWidth = GetSystemMetricsForDpi(SM_CXSCREEN, dpi);
        uint32 screenHeight = GetSystemMetricsForDpi(SM_CYSCREEN, dpi);

        DEVMODE devMode { .dmSize = sizeof(DEVMODE) };
        EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devMode);
        uint32 refresh = devMode.dmDisplayFrequency;

        OutputDebugString(
            format("--->Resolution: {} x {} {} Hz\n", 
                screenWidth, screenHeight, refresh).c_str());

        SafeRelease(adapter);
        SafeRelease(output);
    }

    void Graphics::Initialize(Window *window)
    {
        // ---------------------------------------------------
        // DXGI infrastructure and D3D device
        // ---------------------------------------------------

        uint32 createDeviceFlags{};
        uint32 factoryFlags{};
        ID3D11Debug * debug { nullptr };

    #ifdef _DEBUG
        createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
        factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    #endif

        ThrowIfFailed(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory)));

        constexpr D3D_FEATURE_LEVEL featureLevels[] =
        {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
            D3D_FEATURE_LEVEL_10_1,
            D3D_FEATURE_LEVEL_10_0,
            D3D_FEATURE_LEVEL_9_3,
            D3D_FEATURE_LEVEL_9_2,
            D3D_FEATURE_LEVEL_9_1
        };

        ThrowIfFailed(D3D11CreateDevice(nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createDeviceFlags,
            featureLevels,
            static_cast<uint32>(Countof(featureLevels)),
            D3D11_SDK_VERSION,
            bit_cast<ID3D11Device**>(&device),
            &featureLevel,
            bit_cast<ID3D11DeviceContext**>(&context)));
    
    #ifdef _DEBUG
        ThrowIfFailed(device->QueryInterface(IID_PPV_ARGS(&debug)));

        constexpr char deviceName[] = "DEV_Main";
        device->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(deviceName), deviceName);
        SetDebugName(context, "CTX_Main");

	    LogHardwareInfo();
    #endif 
        
        // ---------------------------------------------------
        // Swap Chain
        // ---------------------------------------------------

        DXGI_SWAP_CHAIN_DESC1 swapChainDesc {
            .Width = static_cast<uint32>(window->Width()),
            .Height = static_cast<uint32>(window->Height()),
            .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
            .SampleDesc {
                .Count = antialiasing,
                .Quality = quality,
            },
            .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
            .BufferCount = backBufferCount,
            .Scaling = DXGI_SCALING_STRETCH,
            .SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD,
            .Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING,
        };

        ThrowIfFailed(factory->CreateSwapChainForHwnd(
            device,
            window->Id(),
            &swapChainDesc,
            nullptr,
            nullptr,
            &swapChain));

        ThrowIfFailed(factory->MakeWindowAssociation(window->Id(), DXGI_MWA_NO_ALT_ENTER))

        // ---------------------------------------------------
        // Render Target Views
        // ---------------------------------------------------
        
        for (uint32 i = 0; i < backBufferCount - 1; ++i)
        {
            ThrowIfFailed(swapChain->GetBuffer(0, IID_PPV_ARGS(&renderTargets[i])));
            ThrowIfFailed(device->CreateRenderTargetView1(renderTargets[i], nullptr, &renderTargetView));
        }

        context->OMSetRenderTargets(1, bit_cast<ID3D11RenderTargetView**>(&renderTargetView), nullptr);

        // ---------------------------------------------------
        // Depth/Stencil View
        // ---------------------------------------------------

        D3D11_TEXTURE2D_DESC depthBufferDesc {
            .Width = static_cast<uint32>(window->Width()),
            .Height = static_cast<uint32>(window->Height()),
            .MipLevels = 1,
            .ArraySize = 1,
            .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
            .SampleDesc {
                .Count = antialiasing,
                .Quality = quality,
            },
            .Usage = D3D11_USAGE_DEFAULT,
            .BindFlags = D3D11_BIND_DEPTH_STENCIL,
            .CPUAccessFlags = 0,
            .MiscFlags = 0
        };

        ThrowIfFailed(device->CreateTexture2D(&depthBufferDesc, nullptr, &depthStencil));

	    D3D11_DEPTH_STENCIL_DESC depthStencilDesc {
            .DepthEnable = TRUE,
            .DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
            .DepthFunc = D3D11_COMPARISON_LESS,
            .StencilEnable = TRUE,
            .StencilReadMask = 0xFF,
            .StencilWriteMask = 0xFF,
            .FrontFace = {
                .StencilFailOp = D3D11_STENCIL_OP_KEEP,
                .StencilDepthFailOp = D3D11_STENCIL_OP_INCR,
                .StencilPassOp = D3D11_STENCIL_OP_KEEP,
                .StencilFunc = D3D11_COMPARISON_ALWAYS
            },
            .BackFace = {
                .StencilFailOp = D3D11_STENCIL_OP_KEEP,
                .StencilDepthFailOp = D3D11_STENCIL_OP_DECR,
                .StencilPassOp = D3D11_STENCIL_OP_KEEP,
                .StencilFunc = D3D11_COMPARISON_ALWAYS
            }
        };

        ThrowIfFailed(device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));
        context->OMSetDepthStencilState(depthStencilState, 1);

	    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc {
            .Format = DXGI_FORMAT_D24_UNORM_S8_UINT,
	        .ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D,
	        .Texture2D {
                .MipSlice = 0,
            },
        };

        ThrowIfFailed(device->CreateDepthStencilView(depthStencil, &depthStencilViewDesc, &depthStencilView));

        context->OMSetRenderTargets(1, bit_cast<ID3D11RenderTargetView**>(&renderTargetView), depthStencilView);

        // ---------------------------------------------------
        // Blend State
        // ---------------------------------------------------

        D3D11_BLEND_DESC1 blendDesc = {
            .AlphaToCoverageEnable = false,
            .IndependentBlendEnable = false,
            .RenderTarget = {
                {
                    .BlendEnable = true,
                    .SrcBlend = D3D11_BLEND_SRC_ALPHA,
                    .DestBlend = D3D11_BLEND_INV_SRC_ALPHA,
                    .BlendOp = D3D11_BLEND_OP_ADD,
                    .SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA,
                    .DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA,
                    .BlendOpAlpha = D3D11_BLEND_OP_ADD,
                    .RenderTargetWriteMask = 0x0F
                },
            },
        };

        ThrowIfFailed(device->CreateBlendState1(&blendDesc, &blendState))

        context->OMSetBlendState(blendState, nullptr, 0xffffffff);

        // ---------------------------------------------------
        // Viewport and Scissor Rect
        // ---------------------------------------------------

        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = static_cast<float>(window->Width());
        viewport.Height = static_cast<float>(window->Height());
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        context->RSSetViewports(1, &viewport);

        scissorRect = { 0, 0, window->Width(), window->Height() };

        context->RSSetScissorRects(1, &scissorRect);

        // ---------------------------------------------------
        // Background Color of Backbuffer
        // ---------------------------------------------------

        COLORREF color = window->Color();

        bgColor[0] = GetRValue(color)/255.0f;
        bgColor[1] = GetGValue(color)/255.0f;
        bgColor[2] = GetBValue(color)/255.0f;
        bgColor[3] = 1.0f;

        SafeRelease(debug);
    }
}