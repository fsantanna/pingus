//  $Id: pingu_particle.hxx,v 1.2 2002/06/24 22:52:59 grumbel Exp $
//
//  Pingus - A free Lemmings clone
//  Copyright (C) 1999 Ingo Ruhnke <grumbel@gmx.de>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

#ifndef HEADER_PINGUS_PARTICLES_PINGU_PARTICLE_HXX
#define HEADER_PINGUS_PARTICLES_PINGU_PARTICLE_HXX

#include "particle.hxx"

class CL_Surface;

///
class PinguParticle : public Particle
{
private:
  ///
  float size;
  ///
  float size_add;
  ///
  static CL_Surface sur;
public:
  ///
  PinguParticle();
  ///
  PinguParticle(int x, int y, float x_a, float y_a);
  
  ///
  void init(int x, int y, float x_a, float y_a);
  ///
  void draw_offset(int ofx, int ofy, float s);
  ///
  void update(float delta);
}///
;

#endif

/* EOF */
