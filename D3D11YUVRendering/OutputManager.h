// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved

#ifndef _OUTPUTMANAGER_H_
#define _OUTPUTMANAGER_H_

#include <stdio.h>
#include <windows.h>
#include <sal.h>
#include <dxgi1_2.h>
#include <d3d11.h>
#include <DirectXMath.h>

#include "warning.h"

#include "PixelShader.h"
#include "VertexShader.h"

#define NUMVERTICES 6
#define BPP         4
#define OCCLUSION_STATUS_MSG WM_USER

extern HRESULT SystemTransitionsExpectedErrors[];
extern HRESULT CreateDuplicationExpectedErrors[];
extern HRESULT FrameInfoExpectedErrors[];
extern HRESULT AcquireFrameExpectedError[];
extern HRESULT EnumOutputsExpectedErrors[];

typedef _Return_type_success_(return == DUPL_RETURN_SUCCESS) enum
{
	DUPL_RETURN_SUCCESS = 0,
	DUPL_RETURN_ERROR_EXPECTED = 1,
	DUPL_RETURN_ERROR_UNEXPECTED = 2
}DUPL_RETURN;

//
// A vertex with a position and texture coordinate
//
typedef struct _VERTEX
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT2 TexCoord;
} VERTEX;


_Post_satisfies_(return != DUPL_RETURN_SUCCESS)
DUPL_RETURN ProcessFailure(_In_opt_ ID3D11Device* Device, _In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr, _In_opt_z_ HRESULT* ExpectedErrors = nullptr);

void DisplayMsg(_In_ LPCWSTR Str, _In_ LPCWSTR Title, HRESULT hr);


//
// Handles the task of drawing into a window.
// Has the functionality to draw the mouse given a mouse shape buffer and position
//
class OUTPUTMANAGER
{
    public:
        OUTPUTMANAGER();
        ~OUTPUTMANAGER();
        DUPL_RETURN InitOutput(HWND Window, _Out_ RECT* DeskBounds);
		DUPL_RETURN CreateAccessibleSurf(RECT * DeskBounds, DXGI_FORMAT Format);
        DUPL_RETURN UpdateApplicationWindow(_Inout_ bool* Occluded);
        void CleanRefs();
        HANDLE GetSharedHandle();
        void WindowResize();

	// Vars
		ID3D11Device* m_Device;
        ID3D11Texture2D* m_SharedSurf;
		ID3D11Texture2D* m_AccessibleSurf;
		ID3D11DeviceContext* m_DeviceContext;
		IDXGISwapChain1* m_SwapChain;

    private:
    // Methods
        DUPL_RETURN MakeRTV();
        void SetViewPort(UINT Width, UINT Height);
        DUPL_RETURN InitShaders();
        DUPL_RETURN CreateSharedSurf(_Out_ RECT* DeskBounds);
        DUPL_RETURN DrawFrame();
        DUPL_RETURN ResizeSwapChain();

    // Vars
        
        IDXGIFactory2* m_Factory;
        ID3D11RenderTargetView* m_RTV;
        ID3D11SamplerState* m_SamplerLinear;
        ID3D11BlendState* m_BlendState;
        ID3D11VertexShader* m_VertexShader;
        ID3D11PixelShader* m_PixelShader;
        ID3D11InputLayout* m_InputLayout;
        HWND m_WindowHandle;
        bool m_NeedsResize;
        DWORD m_OcclusionCookie;
};

#endif
