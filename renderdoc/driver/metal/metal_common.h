/******************************************************************************
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 Baldur Karlsson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 ******************************************************************************/

#pragma once

#include "api/replay/rdcstr.h"
#include "common/common.h"
#include "common/timing.h"
#include "official/metal-cpp.h"
#include "serialise/serialiser.h"
#include "metal_resources.h"
#include "metal_types.h"

enum class MetalChunk : uint32_t
{
  MTLCreateSystemDefaultDevice = (uint32_t)SystemChunk::FirstDriverChunk,
  MTLDevice_newDefaultLibrary,
  MTLDevice_newLibraryWithSource,
  MTLLibrary_newFunctionWithName,
  Max
};

DECLARE_REFLECTION_ENUM(MetalChunk);

// must be at the start of any function that serialises
#define CACHE_THREAD_SERIALISER() WriteSerialiser &ser = m_WrappedMTLDevice->GetThreadSerialiser();

#define SERIALISE_TIME_CALL(...)                                                                \
  {                                                                                             \
    WriteSerialiser &ser = m_WrappedMTLDevice->GetThreadSerialiser();                           \
    ser.ChunkMetadata().timestampMicro = Timing::GetTick();                                     \
    __VA_ARGS__;                                                                                \
    ser.ChunkMetadata().durationMicro = Timing::GetTick() - ser.ChunkMetadata().timestampMicro; \
  }

#define DECLARE_FUNCTION_SERIALISED(ret, func, ...) \
  ret func(__VA_ARGS__);                            \
  template <typename SerialiserType>                \
  bool CONCAT(Serialise_, func(SerialiserType &ser, ##__VA_ARGS__));

#define INSTANTIATE_FUNCTION_SERIALISED(CLASS, ret, func, ...)                     \
  template bool CLASS::CONCAT(Serialise_, func(ReadSerialiser &ser, __VA_ARGS__)); \
  template bool CLASS::CONCAT(Serialise_, func(WriteSerialiser &ser, __VA_ARGS__));

// A handy macro to say "is the serialiser reading and we're doing replay-mode stuff?"
// The reason we check both is that checking the first allows the compiler to eliminate the other
// path at compile-time, and the second because we might be just struct-serialising in which case we
// should be doing no work to restore states.
// Writing is unambiguously during capture mode, so we don't have to check both in that case.
#define IsReplayingAndReading() (ser.IsReading() && IsReplayMode(m_WrappedMTLDevice->GetState()))

#ifdef __OBJC__
#define METAL_NOT_HOOKED()                                                             \
  do                                                                                   \
  {                                                                                    \
    RDCWARN("Metal %s %s not hooked", class_getName([self class]), sel_getName(_cmd)); \
  } while((void)0, 0)
#endif

// similar to RDCUNIMPLEMENTED but without the debugbreak
#define METAL_NOT_IMPLEMENTED(...)                                            \
  do                                                                          \
  {                                                                           \
    RDCWARN("Metal '%s' not implemented -" __VA_ARGS__, __PRETTY_FUNCTION__); \
  } while((void)0, 0)

// similar to RDCUNIMPLEMENTED but for things that are hit often so we don't want to fire the
// debugbreak.
#define METAL_NOT_IMPLEMENTED_ONCE(...)                                           \
  do                                                                              \
  {                                                                               \
    static bool msgprinted = false;                                               \
    if(!msgprinted)                                                               \
      RDCDEBUG("Metal '%s' not implemented - " __VA_ARGS__, __PRETTY_FUNCTION__); \
    msgprinted = true;                                                            \
  } while((void)0, 0)
