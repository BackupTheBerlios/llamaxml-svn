#ifndef LLAMASTDINT_H
#define LLAMASTDINT_H

#if (! __GNUC__) || __APPLE__
	#pragma once
#endif

#include <stdint.h>
#include <stdlib.h>

namespace LlamaXML {
	using ::uint16_t;
	using ::uint32_t;
	using ::int32_t;
	using ::size_t;
}

#endif
