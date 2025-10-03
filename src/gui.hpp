#pragma once
#include "common.hpp"
#include "class/image_dimension.hpp"

namespace big
{
	using dx_callback = std::function<void()>;
	using wndproc_callback = std::function<void(HWND, UINT, WPARAM, LPARAM)>;
	using texture_callbacks = std::function<void(ID3D11Device*)>;

	class gui
	{
		friend class dx12_impl;
		friend class dx11_impl;
		friend class renderer;

		std::map<uint32_t, dx_callback> m_dx_callbacks;
		std::vector<wndproc_callback> m_wndproc_callbacks;
		std::vector<texture_callbacks> m_texture_callbacks;
	public:
		void init();
		void dx_init();

		/**
		 * @brief Add a callback function to draw your ImGui content in
		 *
		 * @param callback Function
		 * @param priority The higher the priority the value the later it gets drawn on top
		 * @return true
		 * @return false
		 */
		bool add_dx_callback(dx_callback callback, uint32_t priority);
		/**
		 * @brief Add a callback function on wndproc
		 *
		 * @param callback Function
		 */
		void add_wndproc_callback(wndproc_callback callback);
		void add_texture(texture_callbacks callback);

		void dx_on_opened();

		void script_init();

		void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

		bool load_from_file(const char* filename, ID3D11Device* d3dDevice, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
		bool load_from_memory(const unsigned char* buffer, int buffer_size, ID3D11Device* d3dDevice, ID3D11ShaderResourceView** out_srv, int* out_width, int* out_height);
		void destroy_texture(ID3D11ShaderResourceView** tex_resources);
	public:
		ID3D11ShaderResourceView* m_header{};
		ImageDimensions m_header_size = { 0, 0 };

		ID3D11ShaderResourceView* m_toggle{};
		ImageDimensions m_toggle_size = { 0, 0 };
	};

	inline gui g_gui;
}
