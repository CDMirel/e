#include "StringHistorySetting.h"
#include "jsonval.h"

StringHistorySetting::StringHistorySetting(const wxString& key, size_t maxItems):
	m_key(key), m_maxItems(maxItems) 
	{}

size_t StringHistorySetting:: GetItemCount(const wxJSONValue& root) const {
	if (!root.HasMember(m_key)) return 0;

	const wxJSONValue items = root.ItemAt(m_key);
	return items.Size();
}

wxString StringHistorySetting::GetItem(const wxJSONValue& root, size_t ndx) const {
	const wxJSONValue items = root.ItemAt(m_key);
	wxASSERT((int)ndx < items.Size());

	return items.ItemAt(ndx).AsString();
}

bool StringHistorySetting::AddItem(wxJSONValue& root, const wxString& item) {
	wxJSONValue& items = root[m_key];

	// Don't add duplicates
	if (items.Size() > 0) {
		const wxJSONValue last = items.ItemAt(0);
		if (last.AsString() == item) return false;
		
		// Check if there should be a duplicate lower down
		for (int i = 0; i < items.Size(); ++i) {
			if (items[i].AsString() == item) {
				items.Remove(i);
				break;
			}
		}
	}
	
	// insert the new item
	items.Insert(0, item);
	if (items.Size() > (int)m_maxItems) 
		items.Remove(items.Size()-1);

	return true;
}
