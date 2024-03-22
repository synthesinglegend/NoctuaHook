#include "menu.h"
n_menu gn_menu;
    void n_menu::render( )
    {
      zgui::poll_input( "Counter-Strike Source" );

      if ( zgui::begin_window( "Apotheosis | Alpha", { 550, 350 }, render::main_font, zgui::zgui_window_flags_none ) )

      {
        if ( zgui::button( "RAGEBOT", { 165,30 } ) )
        {
          ragebot = true;
          visuals = false;
          misc = false;
        }

        zgui::same_line( );

        zgui::next_column( -5, 0 );

        if ( zgui::button( "VISUALS", { 165,30 } ) )
        {
          ragebot = false;
          visuals = true;
          misc = false;
        }
        zgui::same_line( );

        zgui::next_column( -5, 0 );

        if ( zgui::button( "MISC", { 165,30 } ) )
        {
          ragebot = false;
          visuals = false;
          misc = true;
        }

        zgui::next_column( -350, 50 );

        if ( ragebot )render_ragebot( );
        if ( visuals )render_visuals( );
        if ( misc )render_misc( );

        zgui::end_window( );
      }

    }


    void n_menu::render_misc( )
    {
      zgui::begin_groupbox( "MISC", { 165, 150 } );
      {
      }
      zgui::end_groupbox( );

      zgui::next_column( 174, 50 );

      zgui::begin_groupbox( "MOVEMENT", { 165, 150 } );
      {

      }
      zgui::end_groupbox( );

      zgui::next_column( 174, 50 );


      zgui::begin_groupbox( "CONFIGS", { 165, 150 } );
      {

      }
      zgui::end_groupbox( );
    }

    void n_menu::render_visuals( )
    {
      zgui::begin_groupbox( "ESP", { 165, 150 } );
      {
        zgui::checkbox( "Active", g_CVars.Visuals.ESP.Active );
        zgui::checkbox( "Fill Box", g_CVars.Visuals.ESP.Fillbox );
        zgui::combobox( "Box Type", std::vector<std::string>{"Full Box", "Corner Box"}, g_CVars.Visuals.ESP.Box );
        zgui::checkbox( "Name", g_CVars.Visuals.ESP.Name );
        zgui::checkbox( "Health", g_CVars.Visuals.ESP.Health );
        zgui::checkbox( "Armor", g_CVars.Visuals.ESP.Armor );
        zgui::checkbox( "Hitmarker", g_CVars.Visuals.ESP.Hit );
        zgui::checkbox( "Enemy Only", g_CVars.Visuals.ESP.EnemyOnly );
      }
      zgui::end_groupbox( );

      zgui::next_column( 174, 50 );

      zgui::begin_groupbox( "CHAMS", { 165, 150 } );
      {

      }
      zgui::end_groupbox( );

      zgui::next_column( 174, 50 );

      zgui::begin_groupbox( "WORLD", { 165, 150 } );
      {

      }
      zgui::end_groupbox( );
    }

    void n_menu::render_ragebot( )
    {
      zgui::begin_groupbox( "AIM", { 165, 150 } );
      {

      }
      zgui::end_groupbox( );

      zgui::next_column( 174, 50 );

      zgui::begin_groupbox( "ACCURACY", { 165, 150 } );
      {

      }
      zgui::end_groupbox( );

      zgui::next_column( 174, 50 );

      zgui::begin_groupbox( "ANTI-AIM", { 165, 150 } );
      {

      }
      zgui::end_groupbox( );
    }