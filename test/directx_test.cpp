#ifndef UNICODE
#define UNICODE
#endif

#ifndef NOMINMAX
#define NOMINMAX 
#endif

#include <windows.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <wrl/client.h>

#include <iostream>

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

void initDirectX(HWND hwnd, int m_width, int m_height)
{
	Microsoft::WRL::ComPtr<IDXGIFactory4> factory;
	CreateDXGIFactory1(IID_PPV_ARGS(&factory));

	Microsoft::WRL::ComPtr<IDXGIAdapter1> hardwareAdapter;	
	Microsoft::WRL::ComPtr<ID3D12Device> m_device;
	GetHardwareAdapter(factory.Get(), &hardwareAdapter);
	D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));

	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
	
	m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

	int FrameCount = 2;

	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.BufferDesc.Width = m_width;
	swapChainDesc.BufferDesc.Height = m_height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = TRUE;	
	
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	factory->CreateSwapChain(m_commandQueue.Get(), &swapChainDesc, &swapChain);
	
	swapChain.As(&m_swapChain);
	
	factory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);
	
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();	
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_DESTROY:
			
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
		{
		}
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

	MSG msg = {};
	while (WaitMessage())
	{
		GetMessage(&msg, hwnd, 0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}
