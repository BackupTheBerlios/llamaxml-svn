#ifndef LLAMASTDINT_H
#define LLAMASTDINT_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif

#if __APPLE__
	#include <stdint.h>
	
	namespace LlamaXML {
		using ::uint16_t;
		using ::uint32_t;
	}
#else
	#include <cstdint>

	namespace LlamaXML {
		using std::uint16_t;
		using std::uint32_t;
	}
#endif

#endif
