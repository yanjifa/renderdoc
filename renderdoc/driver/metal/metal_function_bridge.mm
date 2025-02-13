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

#include "metal_function.h"
#include "metal_types_bridge.h"

// Bridge for MTLFunction
@implementation ObjCBridgeMTLFunction

// ObjCBrdigeMTLFunction specific
- (id<MTLFunction>)real
{
  MTL::Function *real = self.wrappedCPP->GetReal();
  return id<MTLFunction>(real);
}

- (void)dealloc
{
  self.wrappedCPP->Dealloc();
  [super dealloc];
}

// MTLFunction : based on the protocol defined in
// Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.1.sdk/System/Library/Frameworks/Metal.framework/Headers/MTLLibrrary.h

- (NSString *)label API_AVAILABLE(macos(10.12), ios(10.0))
{
  return self.real.label;
}

- (void)setLabel:value
{
  self.real.label = value;
}

- (id<MTLDevice>)device
{
  return id<MTLDevice>(self.wrappedCPP->GetObjCBridgeMTLDevice());
}

- (MTLFunctionType)functionType
{
  return self.real.functionType;
}

- (MTLPatchType)patchType API_AVAILABLE(macos(10.12), ios(10.0))
{
  return self.real.patchType;
}

- (NSInteger)patchControlPointCount API_AVAILABLE(macos(10.12), ios(10.0))
{
  return self.real.patchControlPointCount;
}

- (NSArray<MTLVertexAttribute *> *)vertexAttributes
{
  return self.real.vertexAttributes;
}

- (NSArray<MTLAttribute *> *)stageInputAttributes API_AVAILABLE(macos(10.12), ios(10.0))
{
  return self.real.stageInputAttributes;
}

- (NSString *)name
{
  return self.real.name;
}

- (NSDictionary<NSString *, MTLFunctionConstant *> *)functionConstantsDictionary
    API_AVAILABLE(macos(10.12), ios(10.0))
{
  return self.real.functionConstantsDictionary;
}

- (id<MTLArgumentEncoder>)newArgumentEncoderWithBufferIndex:(NSUInteger)bufferIndex
    API_AVAILABLE(macos(10.13), ios(11.0))
{
  METAL_NOT_HOOKED();
  return [self.real newArgumentEncoderWithBufferIndex:bufferIndex];
}

- (id<MTLArgumentEncoder>)newArgumentEncoderWithBufferIndex:(NSUInteger)bufferIndex
                                                 reflection:(MTLAutoreleasedArgument *__nullable)reflection
    API_AVAILABLE(macos(10.13), ios(11.0))
{
  METAL_NOT_HOOKED();
  return [self.real newArgumentEncoderWithBufferIndex:bufferIndex reflection:reflection];
}

- (MTLFunctionOptions)options API_AVAILABLE(macos(11.0), ios(14.0))
{
  return self.real.options;
}

@end
