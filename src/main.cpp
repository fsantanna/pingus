//  Pingus - A free Lemmings clone
//  Copyright (C) 1998-2009 Ingo Ruhnke <grumbel@gmail.com>
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <SDL.h>

#include "pingus/pingus_main.hpp"

int XXX_FROM_CEU = 0;

void ceu_sys_log (int mode, long s) {
    switch (mode) {
        case 0:
            printf("%s", (char*)s);
            break;
        case 1:
            printf("%lX", s);
            break;
        case 2:
            printf("%ld", s);
            break;
    }
}

#include "ceu_vars.h"
#include "_ceu_app.c"
tceu_app CEU_APP;

int main(int argc, char** argv)
{
  PingusMain app;

  static char CEU_DATA[sizeof(CEU_Main)];
  CEU_APP.data = (tceu_org*) &CEU_DATA;
  CEU_APP.init = &ceu_app_init;
  CEU_APP.init(&CEU_APP);

  return app.run(argc, argv);
}

/* EOF */
