#include "menu.h"
n_menu gn_menu;

void n_menu::render( )
{
  static bool checkbox;
  static float sliderf;
  static int slideri, dropdown;
  //printconsole("begin menu render");
  zgui::poll_input( "Counter-Strike Source" );

  if ( zgui::begin_window( "hook", { 500, 350 }, render::main_font, zgui::zgui_window_flags_none) )

   // render_ragebot( );

		zgui::end_window( );
	
}
  // and watermark here

