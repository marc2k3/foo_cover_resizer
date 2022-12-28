#include "stdafx.hpp"

CoverConverterResizer::CoverConverterResizer(Action action, metadb_handle_list_cref handles, Format format, const GUID& art_guid) : m_action(action), m_handles(handles), m_format(format), m_art_guid(art_guid) {}

void CoverConverterResizer::on_done(HWND, bool was_aborted)
{
	if (!was_aborted && m_art_guid == album_art_ids::cover_front)
	{
		standard_commands::run_context(guids::foo_cover_info_command, m_handles);
	}
}

void CoverConverterResizer::run(threaded_process_status& status, abort_callback& abort)
{
	album_art_editor::ptr editor_ptr;
	album_art_extractor::ptr extractor_ptr;
	auto lock_api = file_lock_manager::get();

	const size_t count = m_handles.get_count();
	size_t index{}, success{};
	std::set<pfc::string8> paths;

	for (auto&& handle : m_handles)
	{
		abort.check();

		const pfc::string8 path = handle->get_path();
		if (!paths.emplace(path).second) continue;

		status.set_progress(++index, count);
		status.set_item_path(path);

		if (!album_art_extractor::g_get_interface(extractor_ptr, path)) continue;
		if (!album_art_editor::g_get_interface(editor_ptr, path)) continue;

		try
		{
			album_art_data_ptr data = extractor_ptr->open(nullptr, path, abort)->query(m_art_guid, abort);
			if (data.is_empty()) continue;

			const uint8_t* ptr = static_cast<const uint8_t*>(data->get_ptr());
			const uint32_t bytes = static_cast<uint32_t>(data->get_size());
			wil::com_ptr_t<IStream> stream;
			wil::com_ptr_t<IWICBitmapSource> source;
			stream.attach(SHCreateMemStream(ptr, bytes));

			if (FAILED(decode(stream.get(), source)))
			{
				FB2K_console_formatter() << Component::name << ": Unable to decode image data found in: " << path;
				continue;
			}

			if (m_action == Action::Convert)
			{
				if (FAILED(encode(m_format, source.get(), data))) continue;
			}
			else if (m_action == Action::Resize)
			{
				wil::com_ptr_t<IWICBitmapScaler> scaler;
				if (FAILED(resize(source.get(), scaler))) continue;
				if (FAILED(encode(m_format, scaler.get(), data))) continue;
			}

			if (data.is_empty()) continue;
			auto lock = lock_api->acquire_write(path, abort);
			auto instance_ptr = editor_ptr->open(nullptr, path, abort);
			instance_ptr->set(m_art_guid, data, abort);
			instance_ptr->commit(abort);
			success++;
		}
		catch (...) {}
	}
	FB2K_console_formatter() << Component::name << ": " << success << " files were updated.";
}
