#pragma once

#include "number/fixed.h"
#include "tls-utils/type.h"

#include <string>

namespace leaf::network::tls {

	/**
	 * Interface of different cipher suites.
	 * @note
	 * All functions in `cipher_suite` should operate on _string_.
	 */
	class cipher_suite {
	public:
		const cipher_suite_t value;

		/**
		 * Length of the string generated by `hash()`, in bytes.
		 */
		const std::size_t digest_length;

		/**
		 * Length of the key used by cipher suite, in bytes.
		 */
		const std::size_t key_length;

		/**
		 * Length of the initialization vector used by cipher suite, in bytes.
		 */
		const std::size_t iv_length;

		virtual void
		print(std::ostream&) const = 0;

		virtual void
		set_key(const number_base& secret_key) = 0;

		[[nodiscard]] virtual std::string
		encrypt(std::string_view nonce, std::string_view additional, std::string_view plain_text) const = 0;

		virtual std::string
		decrypt(std::string_view nonce, std::string_view data, std::string_view cipher_text) const = 0;

		virtual std::string
		hash(std::string_view) const = 0;

		virtual std::string
		HMAC_hash(std::string_view data, std::string_view key) const = 0;

		/**
		 * Key schedule function.
		 */
		std::string
		HKDF_expand(std::string_view key, std::string_view info, std::size_t length) const;

		/**
		 * Key schedule function.
		 */
		std::string
		HKDF_expand_label(std::string_view key, std::string_view label, std::string_view context, uint16_t length) const;

		static std::string
		HKDF_info(std::string_view label, std::string_view context, uint16_t length);

		/**
		 * Key schedule function.
		 */
		std::string
		derive_secret(std::string_view key, std::string_view label, std::string_view msg) const;

		cipher_suite(cipher_suite_t, std::size_t digest_length, std::size_t key_length, std::size_t iv_length);

		friend std::ostream&
		operator<<(std::ostream&, const cipher_suite&);

		virtual
		~cipher_suite() = default;
	};


	class unimplemented_cipher_suite: public cipher_suite {
	public:
		void
		print(std::ostream& s) const override {
			s << "0x" << std::hex << static_cast<uint16_t>(value) << " (unimplement cipher suite)";
		}

		void
		set_key(const number_base& secret_key) override {
			throw std::exception();
		}

		std::string
		encrypt(std::string_view nonce, std::string_view additional, std::string_view plain_text) const override {
			throw std::exception();
		}

		std::string
		decrypt(std::string_view nonce, std::string_view data, std::string_view cipher_text) const override {
			throw std::exception();
		}

		std::string
		hash(std::string_view view) const override {
			throw std::exception();
		}

		std::string
		HMAC_hash(std::string_view data, std::string_view key) const override {
			throw std::exception();
		}

		unimplemented_cipher_suite(cipher_suite_t c)
				: cipher_suite(c, 0, 0, 0) {
		}
	};


	cipher_suite* get_cipher_suite(std::string_view);

}
