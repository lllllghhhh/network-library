#include "tls-extension/extension.h"
#include "utils.h"

namespace leaf::network::tls {

	session_ticket::session_ticket(std::string_view data)
		: data(data) {
	}

	void session_ticket::format(std::format_context::iterator& it, const std::size_t level) const {
		it = std::ranges::fill_n(it, level, '\t');
		it = std::format_to(it, "session_ticket: {}", data);
	}

	session_ticket::operator raw_extension() const {
		return {ext_type_t::session_ticket, data};
	}
}
