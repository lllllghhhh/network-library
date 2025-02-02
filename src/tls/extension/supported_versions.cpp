#include "tls-extension/extension.h"

#include "utils.h"

namespace leaf::network::tls {

	supported_versions::supported_versions(std::string_view source, context& context) {
		protocol_version_t ver;
		auto ptr = source.begin();
		switch (context.endpoint_type) {
			case context::endpoint_type_t::client: {
				message_type = msg_type_t::server_hello;
				//	selected_version
				reverse_read(ptr, ver);
				versions.push_back(ver);
				break;
			}
			case context::endpoint_type_t::server: {
				message_type = msg_type_t::client_hello;
				//	versions
				//		.size
				uint8_t versions_size;
				reverse_read(ptr, versions_size);
				//		.data
				while (versions_size) {
					reverse_read(ptr, ver);
					versions.push_back(ver);
					versions_size -= sizeof ver;
				}
				break;
			}
		}
	}

	supported_versions::supported_versions(const context& context) {
		if (context.endpoint_version < protocol_version_t::TLS1_3)
			throw std::exception();
		switch (context.endpoint_type) {
			case context::endpoint_type_t::server:
				message_type = msg_type_t::server_hello;
				break;
			case context::endpoint_type_t::client:
				message_type = msg_type_t::client_hello;
				break;
			default:
				throw std::exception();
		}
		versions.push_back(context.endpoint_version);
	}

	void supported_versions::format(std::format_context::iterator& it, const std::size_t level) const {
		it = std::ranges::fill_n(it, level, '\t');
		it = std::ranges::copy("supported_version:", it).out;
		for (auto& v: versions) {
			*it++ = '\n';
			it = std::ranges::fill_n(it, level + 1, '\t');
			it = std::format_to(it, "{}", v);
		}
	}

	supported_versions::operator raw_extension() const {
		std::string data;
		switch (message_type) {
			case msg_type_t::client_hello: {
				const std::uint8_t ver_size = versions.size() * sizeof(protocol_version_t);
				data.reserve(ver_size + 1);
				data.push_back(static_cast<char>(ver_size));
				for (auto& ver: versions)
					reverse_write(data, ver);
				break;
			}
			case msg_type_t::server_hello:
				reverse_write(data, versions.front());
			break;
		}
		return {ext_type_t::supported_versions, std::move(data)};
	}
}
