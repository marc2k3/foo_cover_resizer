#pragma once

namespace resizer
{
	class CDialogSettings : public CDialogImpl<CDialogSettings>
	{
	public:
		CDialogSettings(bool show_size) : m_show_size(show_size) {}

		BEGIN_MSG_MAP_EX(CDialogSettings)
			MSG_WM_INITDIALOG(OnInitDialog)
			COMMAND_RANGE_HANDLER_EX(IDOK, IDCANCEL, OnCloseCmd)
			COMMAND_CODE_HANDLER_EX(EN_UPDATE, OnUpdate)
		END_MSG_MAP()

		enum { IDD = IDD_DIALOG_SETTINGS };

		BOOL OnInitDialog(CWindow, LPARAM)
		{
			m_button_ok = GetDlgItem(IDOK);
			m_combo_type = GetDlgItem(IDC_COMBO_TYPE);
			m_combo_format = GetDlgItem(IDC_COMBO_FORMAT);

			m_label_size = GetDlgItem(IDC_LABEL_SIZE);
			m_edit_quality = GetDlgItem(IDC_EDIT_QUALITY);
			m_edit_size = GetDlgItem(IDC_EDIT_SIZE);

			const size_t count = album_art_ids::num_types();
			for (size_t i = 0; i < count; ++i)
			{
				m_combo_type.AddString(pfc::wideFromUTF8(album_art_ids::query_capitalized_name(i)));
			}

			for (const auto& [id, label] : formats)
			{
				m_combo_format.AddString(label.data());
			}

			m_combo_type.SetCurSel(static_cast<int>(settings::type));
			m_combo_format.SetCurSel(static_cast<int>(settings::format));

			pfc::setWindowText(m_edit_quality, pfc::format_int(settings::quality));

			if (m_show_size)
			{
				pfc::setWindowText(m_edit_size, pfc::format_int(settings::size));
			}
			else
			{
				m_label_size.ShowWindow(SW_HIDE);
				m_edit_size.ShowWindow(SW_HIDE);
			}

			m_hooks.AddDialogWithControls(*this);
			CenterWindow();
			return TRUE;
		}

		int GetQuality()
		{
			const pfc::string8 str = pfc::getWindowText(m_edit_quality);
			return pfc::atoui_ex(str, str.get_length());
		}

		int GetSize()
		{
			const pfc::string8 str = pfc::getWindowText(m_edit_size);
			return pfc::atoui_ex(str, str.get_length());
		}

		void OnCloseCmd(UINT, int nID, CWindow)
		{
			if (nID == IDOK)
			{
				settings::type = m_combo_type.GetCurSel();
				settings::format = m_combo_format.GetCurSel();
				settings::quality = GetQuality();

				if (m_show_size)
				{
					settings::size = GetSize();
				}
			}

			EndDialog(nID);
		}

		void OnUpdate(UINT, int, CWindow)
		{
			m_button_ok.EnableWindow(GetSize() >= 200 && GetQuality() <= 100);
		}

		CButton m_button_ok;
		CComboBox m_combo_format, m_combo_type;
		CEdit m_edit_size, m_edit_quality;
		CWindow m_label_size;
		bool m_show_size{};
		fb2k::CCoreDarkModeHooks m_hooks;
	};
}
