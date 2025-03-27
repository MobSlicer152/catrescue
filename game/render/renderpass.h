#pragma once

#include "game/game.h"
#include "gpuobj.h"

class CGPUCommandBuffer;
class CGPUBuffer;
class CGPUSampler;
class CGPUTexture;

class CGPURenderPass: public CBaseGPUObject<SDL_GPURenderPass, CGPUCommandBuffer>
{
  public:
    CGPURenderPass(std::shared_ptr<CGPUCommandBuffer> cmdBuf);
    ~CGPURenderPass();

    void BindVertexBuffers(const CGPUBuffer* buffers, u32 bufferCount, u32 firstSlot);
    void BindVertexBuffer(const CGPUBuffer& buffer, u32 slot)
    {
        BindVertexBuffers(&buffer, 1, slot);
    }

    void BindIndexBuffer(const CGPUBuffer& buffer);

    void BindFragmentSamplers(const CGPUTexture* textures, const CGPUSampler* samplers, u32 count);
    void BindFragmentSampler(const CGPUTexture& texture, const CGPUSampler& sampler)
    {
        BindFragmentSamplers(&texture, &sampler, 1);
    }

    void BindGraphicsPipeline();

    void DrawIndexed();
    void DrawIndexedIndirect();
};
