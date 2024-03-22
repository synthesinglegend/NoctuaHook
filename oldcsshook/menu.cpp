#include "menu.h"
n_menu gn_menu;
void n_menu::render( )
{
  static bool checkbox;
  static float sliderf;
  static int slideri, dropdown;
  printconsole("begin menu render");
  zgui::poll_input( "Counter-Strike: Global Offensive" );

  if ( zgui::begin_window( "sdk for counter-strike: global offensive", { 500, 350 }, render::main_font ) )
  {
    zgui::begin_groupbox( "Example groupbox", { 468, 311 } );
    {
      zgui::checkbox( "Example checkbox", checkbox );
      zgui::slider_float( "Example slider (float)", 0.0f, 100.0f, sliderf );
      zgui::slider_int( "Example slider (int)", 0, 100, slideri );
      zgui::combobox( "Example dropdown", { "aaaa", "bbbb", "cccc", "dddd" }, dropdown );
    }
    zgui::end_groupbox( );

    zgui::end_window( );
  }
  }
  // and watermark here

void n_menu::render_ragebot( )
{
  zgui::begin_groupbox( "MAIN", { 120,90 } );
  {
    zgui::checkbox( "active", g_CVars.Aimbot.Active );
    zgui::checkbox( "automatic fire", g_CVars.Aimbot.AutoShoot );
    zgui::checkbox( "automatic wallbang", g_CVars.Aimbot.AutoWall );
  }
  zgui::end_groupbox( );
}