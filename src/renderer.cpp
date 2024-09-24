#include "gui.hpp"
#include "logger.hpp"
#include "pointers.hpp"
#include "renderer.hpp"
#include "fonts/font_list.hpp"
#include "fonts/icon_list.hpp"

#include <imgui.h>
#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <imgui_internal.h>

#include "ui/ui_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
//CW.exe+2D4E21610
namespace big
{
	renderer::renderer()
	{
		g_renderer = this;
	}

	renderer::~renderer()
	{
		ImGui_ImplWin32_Shutdown();
		ImGui_ImplDX11_Shutdown();
		ImGui::DestroyContext();

		m_d3d_device->Release();
		m_d3d_context->Release();
		m_d3d_render_target->Release();

		g_renderer = nullptr;
	}

	void renderer::on_present()
	{
		if (g_gui.m_opened)
		{
			ImGui::GetIO().MouseDrawCursor = true;
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
		else
		{
			ImGui::GetIO().MouseDrawCursor = false;
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		g_gui.dx_on_tick();

		if (g_gui.m_opened)
		{
			g_gui.dx_on_opened();
		}

		ImGui::Render();
		m_d3d_context->OMSetRenderTargets(1, &m_d3d_render_target, NULL);
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	bool renderer::init(IDXGISwapChain* swapchain)
	{
		if (SUCCEEDED(swapchain->GetDevice(__uuidof(ID3D11Device), (void**)&m_d3d_device)))
		{
			m_d3d_device->GetImmediateContext(&m_d3d_context);

			DXGI_SWAP_CHAIN_DESC sd;
			swapchain->GetDesc(&sd);
			m_window = sd.OutputWindow;

			ID3D11Texture2D* m_back_buffer;
			swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_back_buffer);
			if (m_back_buffer && m_d3d_device)
			{
				m_d3d_device->CreateRenderTargetView(m_back_buffer, NULL, &m_d3d_render_target);
			}
			else
			{
				return false;
			}

			m_back_buffer->Release();
		}
		else
		{
			return false;
		}

		this->imgui_init();

		this->m_init = true;
		LOG(HACKER) << "Swapchain initialized.";
		g_gui.script_init();

		return true;
	}

	void renderer::imgui_init()
	{
		auto file_path = std::filesystem::path(std::getenv("appdata"));
		file_path /= "Ellohim";
		if (!std::filesystem::exists(file_path))
		{
			std::filesystem::create_directory(file_path);
		}
		else if (!std::filesystem::is_directory(file_path))
		{
			std::filesystem::remove(file_path);
			std::filesystem::create_directory(file_path);
		}
		file_path /= "imgui.ini";

		ImGuiContext* ctx = ImGui::CreateContext();

		static std::string path = file_path.make_preferred().string();
		ctx->IO.IniFilename = path.c_str();

		ImGui_ImplDX11_Init(m_d3d_device, m_d3d_context);
		ImGui_ImplWin32_Init(m_window);

		ImFontConfig font_cfg{};
		font_cfg.FontDataOwnedByAtlas = false;
		std::strcpy(font_cfg.Name, "Rubik");

		m_font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 14.f, &font_cfg);
		merge_icon_with_latest_font(14.f, false);

		g_settings->window.font_sub_title = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 18.f, &font_cfg);
		merge_icon_with_latest_font(16.f, false);

		g_settings->window.font_title = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(const_cast<std::uint8_t*>(font_rubik), sizeof(font_rubik), 24.f, &font_cfg);
		merge_icon_with_latest_font(17.f, false);

		m_monospace_font = ImGui::GetIO().Fonts->AddFontDefault();

		g_gui.dx_init();

		if (!LoadTextureFromFile(std::format("{}\\Scarlet Nexus Trainer\\Textures\\Header.png", std::getenv("appdata")).c_str(), m_d3d_device, &m_header, &m_header_size.x, &m_header_size.y))
			LOG(WARNING) << "Unable to load image header";

		if (!LoadTextureFromFile(std::format("{}\\Scarlet Nexus Trainer\\Textures\\Toggle.png", std::getenv("appdata")).c_str(), m_d3d_device, &m_toggle, &m_toggle_size.x, &m_toggle_size.y))
			LOG(WARNING) << "Unable to load image toggle";
	}

	void renderer::pre_reset()
	{
		if (m_d3d_render_target)
		{
			m_d3d_context->OMSetRenderTargets(0, 0, 0);
			m_d3d_render_target->Release();
		}

		ImGui_ImplDX11_InvalidateDeviceObjects();
	}

	void renderer::post_reset(IDXGISwapChain* this_)
	{
		ImGui_ImplDX11_CreateDeviceObjects();

		ID3D11Texture2D* m_back_buffer;
		this_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_back_buffer);

		if (m_back_buffer)
		{
			m_d3d_device->CreateRenderTargetView(m_back_buffer, NULL, &m_d3d_render_target);
		}

		m_back_buffer->Release();
		m_d3d_context->OMSetRenderTargets(1, &m_d3d_render_target, NULL);
	}

	void renderer::wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (msg == WM_KEYUP && wparam == VK_INSERT)
		{
			//Persist and restore the cursor position between menu instances.
			static POINT cursor_coords{};
			if (g_gui.m_opened)
			{
				GetCursorPos(&cursor_coords);
			}
			else if (cursor_coords.x + cursor_coords.y != 0)
			{
				SetCursorPos(cursor_coords.x, cursor_coords.y);
			}

			g_gui.m_opened ^= true;
		}
		if (msg == WM_QUIT)
		{
			g_running = false;
		}
			
		g_ui_manager->check_for_input();
		g_ui_manager->handle_input();

		if (g_gui.m_opened)
		{
			ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
		}
	}

    bool renderer::LoadTextureFromFile(const char *filename, ID3D11Device *d3dDevice, ID3D11ShaderResourceView **out_srv, int *out_width, int *out_height)
    {
        // Load from disk into a raw RGBA buffer
		LOG(INFO) << "Loading texture from " << filename;

		int image_width = 0;
		int image_height = 0;
		unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
		if (image_data == NULL) 
		{
			LOG(WARNING) << "Failed to load image: " << stbi_failure_reason();

			return false;
		}

		// Create texture
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = image_width;
		desc.Height = image_height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;

		ID3D11Texture2D* pTexture = nullptr;
		D3D11_SUBRESOURCE_DATA subResource;
		subResource.pSysMem = image_data;
		subResource.SysMemPitch = desc.Width * 4; // Assuming 4 bytes per pixel
		subResource.SysMemSlicePitch = 0;

		HRESULT hr = d3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);
		if (FAILED(hr)) 
		{
			LOG(WARNING) << "Failed to create texture. HRESULT: " << hr;
			stbi_image_free(image_data); // Free image data on failure

			return false;
		}

		// Create texture view
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		hr = d3dDevice->CreateShaderResourceView(pTexture, &srvDesc, out_srv);
		if (FAILED(hr)) 
		{
			LOG(WARNING) << "Failed to create shader resource view. HRESULT: " << hr;
			pTexture->Release(); // Release texture on failure
			stbi_image_free(image_data); // Free image data
			
			return false;
		}

		pTexture->Release();
		*out_width = image_width;
		*out_height = image_height;
		stbi_image_free(image_data); // Free the image data after usage

		LOG(INFO) << "Loaded texture " << filename << " with dimensions: " << image_width << "x" << image_height;

		return true;
    }

    void renderer::merge_icon_with_latest_font(float font_size, bool FontDataOwnedByAtlas)
	{
		static const ImWchar icons_ranges[3] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

		ImFontConfig icons_config;
		icons_config.MergeMode = true;
		icons_config.PixelSnapH = true;
		icons_config.FontDataOwnedByAtlas = FontDataOwnedByAtlas;

		g_settings->window.font_icon = ImGui::GetIO().Fonts->AddFontFromMemoryTTF((void*)font_icons, sizeof(font_icons), font_size, &icons_config, icons_ranges);
	}
}
