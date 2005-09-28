#ifndef LLAMASTDINT_H
#define LLAMASTDINT_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif

#include <cstdint>

namespace LlamaXML {
	using std::uint16_t;
	using std::uint32_t;
}

#endif
