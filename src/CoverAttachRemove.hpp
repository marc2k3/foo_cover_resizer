#pragma once

class CoverAttachRemove : public threaded_process_callback
{
public:
	enum class Action
	{
		Attach,
		Remove // removes all except front
	};

	CoverAttachRemove(metadb_handle_list_cref handles, const album_art_data_ptr& data, const GUID& art_guid);
	CoverAttachRemove(metadb_handle_list_cref handles);

	void on_done(HWND, bool was_aborted) override;
	void run(threaded_process_status& status, abort_callback& abort) override;

private:
	Action m_action{};
	GUID m_art_guid{};
	album_art_data_ptr m_data;
	metadb_handle_list m_handles;
};
