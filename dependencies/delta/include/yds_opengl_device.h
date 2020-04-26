#ifndef YDS_OPENGL_DEVICE_H
#define YDS_OPENGL_DEVICE_H

#include "yds_window.h"
#include "yds_opengl_context.h"
#include "yds_device.h"

// External libraries forward declarations
struct SDL_Surface;

class ysOpenGLDevice : public ysDevice {
public:
    ysOpenGLDevice();
    ~ysOpenGLDevice();

    // Setup
    virtual ysError InitializeDevice();
    virtual ysError DestroyDevice();
    virtual bool CheckSupport();

    // Rendering Contexts
    virtual ysError CreateRenderingContext(ysRenderingContext **renderingContext, ysWindow *window);
    virtual ysError UpdateRenderingContext(ysRenderingContext *context);
    virtual ysError DestroyRenderingContext(ysRenderingContext *&context);
    virtual ysError SetContextMode(ysRenderingContext *context, ysRenderingContext::ContextMode mode);

    virtual ysError CreateOnScreenRenderTarget(ysRenderTarget **newTarget, ysRenderingContext *context, bool depthBuffer);
    virtual ysError CreateOffScreenRenderTarget(ysRenderTarget **newTarget, int width, int height, ysRenderTarget::Format format, int sampleCount, bool depthBuffer);
    virtual ysError CreateSubRenderTarget(ysRenderTarget **newTarget, ysRenderTarget *parent, int x, int y, int width, int height);
    virtual ysError ResizeRenderTarget(ysRenderTarget *target, int width, int height);
    virtual ysError DestroyRenderTarget(ysRenderTarget *&target);
    virtual ysError SetRenderTarget(ysRenderTarget *target);

    virtual ysError ClearBuffers(const float *clearColor);
    virtual ysError Present();

    // GPU Buffers
    virtual ysError CreateVertexBuffer(ysGPUBuffer **newBuffer, int size, char *data, bool mirrorToRam = false);
    virtual ysError CreateIndexBuffer(ysGPUBuffer **newBuffer, int size, char *data, bool mirrorToRam = false);
    virtual ysError CreateConstantBuffer(ysGPUBuffer **newBuffer, int size, char *data, bool mirrorToRam = false);
    virtual ysError UseVertexBuffer(ysGPUBuffer *buffer, int stride, int offset);
    virtual ysError UseIndexBuffer(ysGPUBuffer *buffer, int offset);
    virtual ysError UseConstantBuffer(ysGPUBuffer *buffer, int slot);
    virtual ysError EditBufferDataRange(ysGPUBuffer *buffer, char *data, int size, int offset);
    virtual ysError EditBufferData(ysGPUBuffer *buffer, char *data);
    virtual ysError DestroyGPUBuffer(ysGPUBuffer *&buffer);

    // Shaders
    virtual ysError CreateVertexShader(ysShader **newShader, const char *shaderFilename, const char *shaderName);
    virtual ysError CreatePixelShader(ysShader **newShader, const char *shaderFilename, const char *shaderName);
    virtual ysError DestroyShader(ysShader *&shader);

    // Shader Programs
    virtual ysError CreateShaderProgram(ysShaderProgram **newProgram);
    virtual ysError DestroyShaderProgram(ysShaderProgram *&shader, bool destroyShaders = false);
    virtual ysError AttachShader(ysShaderProgram *targetProgram, ysShader *shader);
    virtual ysError LinkProgram(ysShaderProgram *program);
    virtual ysError UseShaderProgram(ysShaderProgram *);

    // Input Layouts 
    virtual ysError CreateInputLayout(ysInputLayout **newLayout, ysShader *shader, ysRenderGeometryFormat *format);
    virtual ysError UseInputLayout(ysInputLayout *layout);
    virtual ysError DestroyInputLayout(ysInputLayout *&layout);

    // Textures 
    virtual ysError CreateTexture(ysTexture **texture, const char *fname);
    virtual ysError DestroyTexture(ysTexture *&texture);
    virtual ysError UseTexture(ysTexture *texture, int slot);
    virtual ysError UseRenderTargetAsTexture(ysRenderTarget *texture, int slot);

    virtual void Draw(int numFaces, int indexOffset, int vertexOffset);

    const ysOpenGLVirtualContext *UpdateContext();

protected:
    // Set the current rendering context
    void SetRenderingContext(ysRenderingContext *context);

    // Hidden Functions
    void ResubmitInputLayout();

    ysOpenGLVirtualContext *GetTransferContext();

    // Static functions
    // --------------------------------------------------------------

    // Retrieve a pixel from an SDL surface
    static unsigned int GetPixel(SDL_Surface *surface, int x, int y);

    // Get a GL type from a geometry channel format
    static int GetFormatGLType(ysRenderGeometryChannel::ChannelFormat format);

protected:
    ysOpenGLVirtualContext *m_realContext;

    bool m_deviceCreated;

protected:
    // Hidden functionality
    ysError CreateOpenGLOffScreenRenderTarget(ysRenderTarget *target, int width, int height, ysRenderTarget::Format format, int sampleCount, bool depthBuffer);
    ysError DestroyOpenGLRenderTarget(ysRenderTarget *target);

};

#endif /* YDS_OPENGL_DEVICE_H */
