#ifndef UNICODE
#define UNICODE
#endif

#ifndef NOMINMAX
#define NOMINMAX 
#endif

#define SK_GANESH
#define SK_DIRECT3D
#include <include/core/SkSurface.h>
#include <include/core/SkColorSpace.h>
#include <include/gpu/GrDirectContext.h>
#include <include/gpu/GrBackendSurface.h>
#include <include/gpu/d3d/GrD3DBackendContext.h>

#include <windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <wrl/client.h>

#include <iostream>

gr_cp<ID3D12Device> device;
gr_cp<ID3D12CommandQueue> queue;
gr_cp<IDXGISwapChain3> swapChain;
gr_cp<ID3D12Fence> fence;
sk_sp<GrDirectContext> context;
int bufferIndex, sampleCount; 
const int frameCount = 2;
HANDLE fenceEvent;

uint64_t fenceValues[frameCount];
gr_cp<ID3D12Resource> buffers[frameCount];
sk_sp<SkSurface> surfaces[frameCount];

void GetHardwareAdapter(IDXGIFactory4* pFactory, IDXGIAdapter1** ppAdapter)
{
	*ppAdapter = nullptr;	
	for(UINT adapterIndex = 0; ; ++adapterIndex)
	{
		IDXGIAdapter1* pAdapter = nullptr;

		if(pFactory->EnumAdapters1(adapterIndex, &pAdapter))
		{
			break;		
		}

		if(SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			*ppAdapter = pAdapter;
			return;
		}
		pAdapter->Release();
	}
}

void initDirectXBackendContext(GrD3DBackendContext* ctx)

{
	gr_cp<IDXGIFactory4> factory;
	CreateDXGIFactory1(IID_PPV_ARGS(&factory));

	gr_cp<IDXGIAdapter1> hardwareAdapter;	
	gr_cp<ID3D12Device> m_device;
	GetHardwareAdapter(factory.get(), &hardwareAdapter);
	D3D12CreateDevice(hardwareAdapter.get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	gr_cp<ID3D12CommandQueue> m_commandQueue;
	
	m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));

	ctx->fAdapter = hardwareAdapter;
	ctx->fDevice = m_device;
	ctx->fQueue = m_commandQueue;
	ctx->fProtectedContext = GrProtected::kNo;
}

void initDirectX(HWND hwnd)
{
	GrD3DBackendContext backendContext;
	
	initDirectXBackendContext(&backendContext);
	device = backendContext.fDevice;	
	queue = backendContext.fQueue;

	RECT windowRect;
	GetWindowRect(hwnd, &windowRect);
	unsigned int width = windowRect.right - windowRect.left;
	unsigned int height = windowRect.bottom - windowRect.top;

	gr_cp<IDXGIFactory4> factory;
	
	CreateDXGIFactory2(0, IID_PPV_ARGS(&factory));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	swapChainDesc.BufferCount = frameCount;
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	
	gr_cp<IDXGISwapChain1> swapChain1;
	factory->CreateSwapChainForHwnd(queue.get(), hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
	factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);

	swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain));

	bufferIndex = swapChain->GetCurrentBackBufferIndex();

	sampleCount = 1;
	
	GrD3DTextureResourceInfo info(nullptr, nullptr, D3D12_RESOURCE_STATE_PRESENT, DXGI_FORMAT_R8G8B8A8_UNORM, 1, 1, 0);

	SkSurfaceProps surfaceProps(0, kRGB_H_SkPixelGeometry);	
	
	for(int i=0; i<frameCount; i++)
	{
		swapChain->GetBuffer(i, IID_PPV_ARGS(&buffers[i]));	
		
		info.fResource = buffers[i];
		
		if(sampleCount > 1)
		{
			GrBackendTexture backendTexture(width, height, info);
			
			surfaces[i] = SkSurface::MakeFromBackendTexture(
				context.get(), 
				backendTexture, 	
				kTopLeft_GrSurfaceOrigin, 
				sampleCount, 
				kRGBA_8888_SkColorType, 
				sk_sp<SkColorSpace>(nullptr), 
				&surfaceProps);
		}
		else 
		{
		    GrBackendRenderTarget backendRT(width, height, info);
		    surfaces[i] = SkSurface::MakeFromBackendRenderTarget(
			context.get(), backendRT, kTopLeft_GrSurfaceOrigin, kRGBA_8888_SkColorType,
			sk_sp<SkColorSpace>(nullptr), &surfaceProps);
		}

	}

	for(int i=0; i<frameCount; i++)
		fenceValues[i] = 10000;

	device->CreateFence(fenceValues[bufferIndex], D3D12_FENCE_FLAG_NONE,
                                              IID_PPV_ARGS(&fence));

	
	fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

sk_sp<SkSurface> getBackbufferSurface()
{
	const uint64_t currentFenceValue = fenceValues[bufferIndex];
	bufferIndex = swapChain->GetCurrentBackBufferIndex();

	if(fence->GetCompletedValue() < fenceValues[bufferIndex])
	{
		fence->SetEventOnCompletion(fenceValues[bufferIndex], fenceEvent);

		WaitForSingleObjectEx(fenceEvent, INFINITE, FALSE);
	}

	fenceValues[bufferIndex] = currentFenceValue + 1;
	
	return surfaces[bufferIndex];
}

void onSwapBuffers()
{
	SkSurface* surface = surfaces[bufferIndex].get();

	GrFlushInfo info;
	surface->flush(SkSurface::BackendSurfaceAccess::kPresent, info);
	context->submit();
	
	swapChain->Present(1,0);
	queue->Signal(fence.get(), fenceValues[bufferIndex]);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main()
{
	const wchar_t CLASS_NAME[] = L"DirectX Test";

	HINSTANCE hInstance = GetModuleHandle(0);
	
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance; 
	wc.lpszClassName = CLASS_NAME;

	RegisterClass(&wc);

	HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"DirectX Test", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if(hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, SW_SHOW);

	initDirectX(hwnd);

	MSG msg = {};
	while (WaitMessage())
	{
		GetMessage(&msg, hwnd, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
