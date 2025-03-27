#include "buffer.h"
#include "copypass.h"
#include "commandbuffer.h"
#include "transferbuffer.h"
#include "game/log.h"

CGPUCopyPass::CGPUCopyPass(std::shared_ptr<CGPUCommandBuffer> cmdBuf) : CBaseGPUObject(cmdBuf)
{
	m_handle = SDL_BeginGPUCopyPass(m_parent->GetHandle());
}

void CGPUCopyPass::End()
{
	SDL_EndGPUCopyPass(m_handle);
	m_handle = nullptr;
}

void CGPUCopyPass::UploadToBuffer(const CGPUBuffer& dest, const CGPUTransferBuffer& src, u32 size, u32 srcOffset, u32 destOffset)
{
    SDL_GPUTransferBufferLocation srcLocation = {};
    srcLocation.transfer_buffer = src.GetHandle();
    srcLocation.offset = srcOffset;
    SDL_GPUBufferRegion destRegion = {};
    destRegion.buffer = dest.GetHandle();
    destRegion.offset = destOffset;
    destRegion.size = size;
    SDL_UploadToGPUBuffer(m_handle, &srcLocation, &destRegion, false);
}

void CGPUCopyPass::UploadToTexture(
	const CGPUTexture& texture, const CGPUTransferBuffer& src, glm::ivec2 size, glm::ivec2 srcPos, glm::ivec2 destPos,
	u32 destMip, u32 destLayer)
{
    SDL_GPUTextureTransferInfo srcInfo = {};
    srcInfo.transfer_buffer = src.GetHandle();
    srcInfo.offset = 0;
    srcInfo.pixels_per_row = size.x;
    srcInfo.rows_per_layer = size.y;
    SDL_GPUTextureRegion destRegion = {};
    destRegion.w = size.x;
    destRegion.h = size.y;
    destRegion.x = destPos.x;
    destRegion.y = destPos.y;
    destRegion.mip_level = destMip;
    destRegion.layer = destLayer;
    SDL_UploadToGPUTexture(m_handle, &srcInfo, &destRegion, false);
}
