#include "menu.h"
n_menu gn_menu;
void n_menu::render( )
{
    printconsole("begin menu render");
  zgui::poll_input( "Counter-Strike Source" );
  if ( zgui::begin_window( "NoctuaHook", { 500,350 }, g_Font, zgui::zgui_window_flags_none ) )
  {
    if ( zgui::tab_button( "RAGEBOT", { 165, 30 }, ragebot ))
    {
      ragebot = true;
      visuals = false;
      misc = false;
      configs = false;
    }
    zgui::same_line( );
    zgui::next_column( -5, 0 );
    if ( zgui::tab_button( "VISUALS", { 165,30 }, visuals ) )
    {
      ragebot = false;
      visuals = true;
      misc = false;
    }
    zgui::same_line( );

    zgui::next_column( -5, 0 );

    if ( zgui::tab_button( "MISCELLANEOUS", { 166,30 }, misc ) )
    {
      ragebot = false;
      visuals = false;
      misc = true;
    }
    zgui::next_column( -327, 34 );
    if ( ragebot )
      render_ragebot( );
    // our tabs will be rendered here
    zgui::end_window( );
  }
  // and watermark here
}

void n_menu::render_ragebot( )
{
  zgui::begin_groupbox( "MAIN" );
  {
    zgui::checkbox( "active", g_CVars.Aimbot.Active );
    zgui::checkbox( "automatic fire", g_CVars.Aimbot.AutoShoot );
    zgui::checkbox( "automatic wallbang", g_CVars.Aimbot.AutoWall );
  }
  zgui::end_groupbox( );
}