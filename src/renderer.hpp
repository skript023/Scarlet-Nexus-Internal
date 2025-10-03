#pragma once
#include "common.hpp"
#include <imgui.h>

namespace big
{
	class renderer
	{
	public:
		explicit renderer();
		~renderer();

		bool m_init = false;
		bool init(IDXGISwapChain* swapchain);
		void imgui_init();

		void on_present();

		void pre_reset();
		void post_reset(IDXGISwapChain* this_);
		void merge_icon_with_latest_font(float font_size, bool FontDataOwnedByAtlas = false);

		void load_texture(ID3D11Device* resource);

		void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
		bool LoadTextureFromFile(const char* filename, ID3D11Device* d3dDevice, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
	public:
		ImFont* m_font = nullptr;
		ImFont* m_ui_manager_font = nullptr;
		ImFont* m_monospace_font = nullptr;

		HWND m_window = NULL;
	private:
		ID3D11Device* m_d3d_device = nullptr;
		ID3D11DeviceContext* m_d3d_context = nullptr;
		ID3D11RenderTargetView* m_d3d_render_target = nullptr;
	};

	inline renderer *g_renderer{};
}
