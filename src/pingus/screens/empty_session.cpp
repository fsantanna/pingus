#include "pingus/screens/empty_session.hpp"
#include "ceu_vars.h"

EmptySession::EmptySession(void) { }

/// TODO: move to proper place!
#include "engine/display/sdl_framebuffer_surface_impl.hpp"
FramebufferSurface* load_framebuffer_sdl_surface(const Pathname& filename, 
ResourceModifier::Enum modifier)
{
  // FIXME: Implement proper cache
  try
  {
    Surface surface(filename);
    if (modifier != ResourceModifier::ROT0)
    {
      surface = surface.mod(modifier);
    }
    return new FramebufferSurface(new SDLFramebufferSurfaceImpl(surface.get_surface()));
  }
  catch(const std::exception& err)
  {
    // return a dummy surface for cases where the image file can't be found
    log_error("%1%", err.what());
    Surface surface(Pathname("images/core/misc/404.png", Pathname::DATA_PATH));
    return new FramebufferSurface(new SDLFramebufferSurfaceImpl(surface.get_surface()));
  }
}
