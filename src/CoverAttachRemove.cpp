#include "stdafx.hpp"
#include "CoverAttachRemove.hpp"

CoverAttachRemove::CoverAttachRemove(metadb_handle_list_cref handles, const album_art_data_ptr& data, const GUID& art_guid) : m_action(Action::Attach), m_handles(handles), m_data(data), m_art_guid(art_guid) {}
CoverAttachRemove::CoverAttachRemove(metadb_handle_list_cref handles) : m_action(Action::Remove), m_handles(handles) {}

void CoverAttachRemove::on_done(HWND, bool was_aborted)
{
	if (!was_aborted && m_action == Action::Attach && m_art_guid == album_art_ids::cover_front)
	{
		standard_commands::run_context(resizer::guid_foo_cover_info_command, m_handles);
	}
}

void CoverAttachRemove::run(threaded_process_status& status, abort_callback& abort)
{
	auto lock_api = file_lock_manager::get();

	const size_t count = m_handles.get_count();
	size_t index{};
	std::set<pfc::string8> paths;

	for (auto&& handle : m_handles)
	{
		abort.check();

		const pfc::string8 path = handle->get_path();
		if (!paths.emplace(path).second) continue;

		status.set_progress(++index, count);
		status.set_item_path(path);

		album_art_editor::ptr ptr;
		if (!album_art_editor::g_get_interface(ptr, path)) continue;

		try
		{
			auto lock = lock_api->acquire_write(path, abort);
			album_art_editor_instance_ptr aaep = ptr->open(nullptr, path, abort);

			if (m_action == Action::Attach)
			{
				aaep->set(m_art_guid, m_data, abort);
			}
			else if (m_action == Action::Remove)
			{
				aaep->remove(album_art_ids::artist);
				aaep->remove(album_art_ids::cover_back);
				aaep->remove(album_art_ids::disc);
				aaep->remove(album_art_ids::icon);
			}

			aaep->commit(abort);
		}
		catch (...) {}
	}
}
