#pragma once
#ifndef NDEBUG
#define GFX_CAST dynamic_cast
#else
#define GFX_CAST reinterpret_cast
#endif