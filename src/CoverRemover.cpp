#include "stdafx.hpp"
#include "CoverRemover.hpp"

CoverRemover::CoverRemover(metadb_handle_list_cref handles) : m_handles(handles) {}

void CoverRemover::run(threaded_process_status& status, abort_callback& abort)
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

			aaep->remove(album_art_ids::artist);
			aaep->remove(album_art_ids::cover_back);
			aaep->remove(album_art_ids::disc);
			aaep->remove(album_art_ids::icon);
			aaep->commit(abort);
		}
		catch (...) {}
	}
}
