#pragma once
#include "Main.h"
class n_menu
{
public:
  void render( );
  bool opened = false;
private:
  void watermark( );
  bool ragebot = true;
  bool visuals = false;
  bool misc = false;
  bool configs = false;
  void render_ragebot( );
  void render_visuals( );
  void render_misc( );
  void render_configs( );
};
extern n_menu g_nmenu;