#include "menu.h"
#include <filesystem>
#include <fstream>
#include <sstream>
n_menu gn_menu;
BOOL DirectoryExists( LPCTSTR szPath )
{
  DWORD dwAttrib = GetFileAttributes( szPath );

  return ( dwAttrib != INVALID_FILE_ATTRIBUTES &&
           ( dwAttrib & FILE_ATTRIBUTE_DIRECTORY ) );
}

bool mouse1pressed = false;
bool mouse1released = false;
bool mouse2pressed = false;
bool mouse2released = false;

bool mouse1holdpressed = false;
bool mouse1holdreleased = false;
bool mouse2holdpressed = false;
bool mouse2holdreleased = false;

    void n_menu::render( )
    {
      watermark( );
      	g_Stuff.Mouse.Wrapper();
      if ( !DirectoryExists( "C:\\Apotheosis\\" ) )
      {
        std::filesystem::create_directory( "C:\\Apotheosis\\" );
        std::ofstream output( "C:\\Apotheosis\\default.cfg" );
      }
      zgui::poll_input( "Counter-Strike Source" );    
      if ( zgui::begin_window( "Apotheosis", { 550, 625 }, render::main_font, zgui::zgui_window_flags_none ) )
      {
          static ConVar* mouse1 = g_pCvar->FindVar("cl_mouseenable");
          mouse1->SetValue(1);
      //    zgui::cu
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
      zgui::begin_groupbox( "movement", { 165, 168 } );
      {
         zgui::checkbox("bunnyhop", g_CVars.Miscellaneous.BunnyHop);
         zgui::checkbox("autostrafe", g_CVars.Miscellaneous.AutoStrafe);
         zgui::checkbox("circle strafer", g_CVars.Miscellaneous.CircleStrafe);
         zgui::slider_int("circle strafer ticks", 0, 32, g_CVars.Miscellaneous.TickValue);
         zgui::slider_int("circle strafer rotate", 0, 32, g_CVars.Miscellaneous.RotationValue);
         zgui::key_bind("circle strafer bind", g_CVars.Miscellaneous.CircleStrafeValue);
      }
      zgui::end_groupbox( );

      zgui::next_column(0, 230);

      zgui::begin_groupbox("adjustments", { 165, 195 });
      {
          zgui::checkbox("fakeduck", g_CVars.Miscellaneous.FakeDuck);
          zgui::key_bind("fakeduck bind", g_CVars.Miscellaneous.FakeDuckValue);
          zgui::checkbox("slowwalk", g_CVars.Miscellaneous.slowwalk);
          zgui::slider_int("slowwalk speed", 0, 100, g_CVars.Miscellaneous.slowwalkspeed);
          zgui::key_bind("slowwalk bind", g_CVars.Miscellaneous.slowwalkvalue);
          zgui::key_bind("fakewalk", g_CVars.Miscellaneous.FakeWalkValue);
      }
      zgui::end_groupbox();


      zgui::next_column( 174, 50 );

      zgui::begin_groupbox("misc", { 165, 145 });
      {
          zgui::checkbox("roundsay", g_CVars.Miscellaneous.RoundSay);
          zgui::checkbox("killsay", g_CVars.Miscellaneous.KillSay);
          zgui::checkbox("spam duck in air", g_CVars.Miscellaneous.AntiAim.DuckInAir);
          zgui::combobox("hitsound", std::vector<std::string>{"none", "opa", "primordial", "cod", "neverlose", "bloom"}, g_CVars.Visuals.ESP.HitSound);
          zgui::combobox("killsound", std::vector<std::string>{"none", "opa", "minecraft", "quake", "dubov"}, g_CVars.Visuals.ESP.KillSound);
      }
      zgui::end_groupbox();

      zgui::next_column(0, 210);
      zgui::begin_groupbox("exploits", { 165, 70 });
      {
          zgui::checkbox("lag exploit", g_CVars.Miscellaneous.LagExploit);
          zgui::key_bind("lag exploit bind", g_CVars.Miscellaneous.LagExploitValue);
      }
      zgui::end_groupbox();


      zgui::next_column(174, 50);
      /*zgui::begin_groupbox("Configs");
	{
		zgui::text_input("Config Name", g_config.settings.config_name);
		if (zgui::button("Create Config"))
		{
			if (!g_config.settings.config_name.empty())
			{
				g_config.save(g_utils.stringer(g_config.settings.config_name, ".json"));
				g_config.settings.config_name.clear();
				g_config.refresh();
			}
		}
		zgui::combobox("Config List", g_config.settings.config_list, g_config.settings.config_id);
		if (zgui::button("Refresh"))
			g_config.refresh();
		if (zgui::button("Save"))
			g_config.save(g_config.settings.config_list[g_config.settings.config_id]);
		if (zgui::button("Load"))
			g_config.load(g_config.settings.config_list[g_config.settings.config_id]);
	}
	zgui::end_groupbox();*/
      zgui::begin_groupbox( "configurations", { 165, 150 } );
      {

          if(zgui::button("save", { 145, 30 }));
          {
           // g_Config.Save( g_CVars.Miscellaneous.ConfigName );

          }
          if (zgui::button("load", { 145,30 }));
          {
           // g_Config.Load( g_CVars.Miscellaneous.ConfigName );
          }
          if (zgui::button("open cfg folder", { 145,30 }));
          {
           // ShellExecute( NULL, NULL, "C:\\Apotheosis", NULL, NULL, SW_SHOWNORMAL );
            g_CVars.Miscellaneous.Open = false;
          }
      }
      zgui::end_groupbox( );
    }
    void n_menu::watermark( )
    {
      const auto chrono_time = std::chrono::system_clock::to_time_t( std::chrono::system_clock::now( ) );
      const auto time = ctime( &chrono_time );
      auto fps = static_cast< int >( 1.f / g_pGlobals->frametime );
      int width, height;
      g_pEngineClient->GetScreenSize( width, height );
      render::filled_rect( width - 280, 20, 260, 28, color( 1, 1, 1, 255 ) );
     render::filled_rect( width - 279, 21, 258, 26, color( 51, 51, 51, 255 ) );
      render::filled_rect( width - 278, 22, 256, 24, color( 41, 41, 41, 255 ) );
     render::filled_rect( width - 277, 23, 254, 22, color( 51, 51, 51, 255 ) );
     render::filled_rect( width - 276, 24, 252, 20, color( 1, 1, 1, 255 ) );
      render::filled_rect( width - 275, 25, 250, 18, color( 31, 31, 31, 255 ) );
     render::filled_rect( width - 275, 42, 250, 1, color( 104, 88, 206, 255 ) );
      bool isConnected = g_pEngineClient->IsConnected( ) && g_pEngineClient->IsInGame( );
     // int ping = g_pEngineClient->GetNetChannelInfo( )->GetLatency( 0 ) * 1000;
      std::stringstream ss;
      ss << "Apotheosis" << " @ " << time << "fps: " << fps;
      render::text( width - 271, 27, color( 255, 255, 255 ), render::main_font, false, ss.str( ) );

    }
    void n_menu::render_visuals( )
    {
      
      zgui::begin_groupbox( "ESP", { 165, 168 } );
      {
        zgui::checkbox( "active", g_CVars.Visuals.ESP.Active );
        zgui::checkbox( "fill box", g_CVars.Visuals.ESP.Fillbox );
        zgui::combobox( "box type", std::vector<std::string>{"full box", "corner box"}, g_CVars.Visuals.ESP.Box );
        zgui::checkbox( "name", g_CVars.Visuals.ESP.Name );
        zgui::checkbox( "health", g_CVars.Visuals.ESP.Health );
        zgui::checkbox( "armor", g_CVars.Visuals.ESP.Armor );
        zgui::checkbox( "hitmarker", g_CVars.Visuals.ESP.Hit );
        zgui::checkbox( "aim spot", g_CVars.Visuals.ESP.AimSpot );
        //zgui::checkbox( "enemy only", g_CVars.Visuals.ESP.EnemyOnly );
      }
      zgui::end_groupbox( );

      zgui::next_column(0, 235);

      zgui::begin_groupbox("removals", { 165, 100 });
      {
          zgui::checkbox("anti smoke", g_CVars.Visuals.NoSmoke);
          zgui::checkbox("anti flash", g_CVars.Visuals.NoFlash);
          zgui::checkbox("no hands", g_CVars.Visuals.NoHands);
          zgui::checkbox("no visual recoil", g_CVars.Visuals.NoVisualRecoil);
          zgui::checkbox("no sky", g_CVars.Visuals.NoSky );
          
      }
      zgui::end_groupbox();

      zgui::next_column(174, 50);

      zgui::begin_groupbox("world", { 165, 180 });
      {
          zgui::combobox( "crosshair", std::vector<std::string>{"off", "cross", "dot", "round", "swastika"}, g_CVars.Visuals.Crosshair.Type );
          zgui::checkbox("fullbright", g_CVars.Visuals.ESP.Fullbright);
          zgui::checkbox("splash effect", g_CVars.Visuals.ESP.trails);
          zgui::slider_int("viewmodel fov", 0, 90, g_CVars.Visuals.ViewModelFOV);
          zgui::slider_float("wall transparent", 0.0f, 1.0f, g_CVars.Visuals.ASUS);
          zgui::slider_float("nightmode", 0.0f, 1.0f, g_CVars.Visuals.NightMode);
      }


      zgui::end_groupbox( );

      zgui::next_column( 174, 50 );

      zgui::begin_groupbox("colored models", { 165, 150 });
      {

      }
      zgui::end_groupbox( );

    }
 
    void n_menu::render_ragebot( )
    {
      zgui::begin_groupbox( "aimbot", { 165, 150 } );
      {
          zgui::checkbox("active#1", g_CVars.Aimbot.Active);
          zgui::checkbox("auto fire", g_CVars.Aimbot.AutoShoot);
          zgui::checkbox("silent", g_CVars.Aimbot.Silent);
          zgui::checkbox("psilent", g_CVars.Aimbot.PerfectSilent);

          zgui::slider_int("min damage", 0, 100, g_CVars.Aimbot.MinDamage);
          zgui::slider_float("hitbox scale", 0.0f, 1.0f, g_CVars.Aimbot.PointScale);

      }
      zgui::end_groupbox( );

      zgui::next_column(0, 210);

      zgui::begin_groupbox( "accuracy", { 165, 285 } );
      {
          zgui::checkbox("no spread", g_CVars.Accuracy.NoSpread);
          zgui::checkbox("force seed", g_CVars.Accuracy.ForceSeed);
          zgui::checkbox("accuracy fix", g_CVars.Accuracy.PerfectAccuracy);
          zgui::checkbox("fake correction", g_CVars.Aimbot.Resolver.Active);
          zgui::checkbox("origin correction", g_CVars.Miscellaneous.OriginCorrection);
          static int hitboxselect = 0;
          if (hitboxselect == 0) g_CVars.Aimbot.Hitbox = HITBOX_HEAD;
          else if (hitboxselect == 1) g_CVars.Aimbot.Hitbox = HITBOX_NECK;
          else if (hitboxselect == 2) g_CVars.Aimbot.Hitbox = HITBOX_CHEST;
          else if (hitboxselect == 3) g_CVars.Aimbot.Hitbox = HITBOX_STOMACH;
          
         
          zgui::combobox("hitbox", std::vector < std::string> { "Head", "Neck", "Chest", "Stomach" }, hitboxselect);
          zgui::combobox("height", std::vector<std::string>{"auto", "origin", "center", "center fixed", "highest"}, g_CVars.Aimbot.HitboxMode);
          zgui::combobox( "target sorting", std::vector<std::string>{"by distance", "by health", "next shot", "randomly"}, g_CVars.Aimbot.TargetSelection );
      }
      zgui::end_groupbox();

      zgui::next_column(174, 50);

      zgui::begin_groupbox("lag", { 165, 260 });
      {
          zgui::checkbox("active#2", g_CVars.Miscellaneous.Fakelag.Active);
          
          zgui::checkbox("in attack", g_CVars.Miscellaneous.Fakelag.InAttack);
          zgui::checkbox("in air", g_CVars.Miscellaneous.Fakelag.AirOnly);
          zgui::slider_int( "chocked ticks", 0, 16, g_CVars.Miscellaneous.Fakelag.Value );
          zgui::combobox("mode", std::vector<std::string>{"constant", "aimware", "random", "adaptive", "standart", "alternative"}, g_CVars.Miscellaneous.Fakelag.Mode);
         
        
      }
      zgui::end_groupbox();

      zgui::next_column(0, 335);

      zgui::begin_groupbox("hitbox enhancements", { 165, 245 });
      {
          zgui::checkbox("hitscan", g_CVars.Aimbot.hitscanactive );
          zgui::combobox("hitscan#1", std::vector<std::string>{"low", "medium", "high"}, g_CVars.Aimbot.HitScan);
          zgui::checkbox("body aim", g_CVars.Aimbot.BodyAim);
          zgui::key_bind("body aim bind", g_CVars.Aimbot.BodyAimValue);
          zgui::checkbox("body aim in air", g_CVars.Aimbot.baiminair);
          zgui::checkbox("body aim in crouch", g_CVars.Aimbot.baimincrouch);
          zgui::checkbox("body after hp", g_CVars.Aimbot.baimafterhp);
          zgui::slider_int("after hp", 0, 100, g_CVars.Aimbot.baimafterhpslider);
      }
      zgui::end_groupbox( );

      zgui::next_column( 174, 50 );

      zgui::begin_groupbox( "anti-hit", { 165, 530} );
      {
          zgui::checkbox("active#3", g_CVars.Miscellaneous.AntiAim.Active);
          zgui::combobox("pitch", std::vector<std::string>{"off", "normal", "inverse", "down", "up", "random", "untrusted", "lby", "custom"}, g_CVars.Miscellaneous.AntiAim.Pitch);
          zgui::combobox("yaw", std::vector<std::string>{"forward", "backward", "sideways", "custom", "jitter"}, g_CVars.Miscellaneous.AntiAim.Yaw);
          std::vector<std::string> yawvariation;
          std::vector<std::string> yawvariation_1 { "normal", "fake side 1", "fake side 2", "random" };
          std::vector<std::string> yawvariation_2 = { "additional", "adaptive", "static", "update" };
          std::vector<std::string> yawvariation_3 = { "reality", "apotheosis", "3-way" };
          switch (g_CVars.Miscellaneous.AntiAim.Yaw)
          {
          case 0: yawvariation = yawvariation_1; break;
          case 1: yawvariation = yawvariation_1; break;
          case 2: yawvariation = yawvariation_1; break;
          case 3: yawvariation = yawvariation_2; break;
          case 4: yawvariation = yawvariation_3; break;
          }

          zgui::combobox("yaw mode", yawvariation, g_CVars.Miscellaneous.AntiAim.Variation);
           zgui::slider_float( "custom pitch", 0.0f, 360.0f, g_CVars.Miscellaneous.AntiAim.PitchValue );
          
          if (g_CVars.Miscellaneous.AntiAim.Yaw == 4)
          {
              zgui::slider_float("jitter real", 0.0f, 360.0f, g_CVars.Miscellaneous.AntiAim.JitterReal);
              zgui::slider_float("jitter fake", 0.0f, 360.0f, g_CVars.Miscellaneous.AntiAim.JitterFake);
          }
          else
          {
              zgui::slider_float("custom real", 0.0f, 360.0f, g_CVars.Miscellaneous.AntiAim.RealValue);
              zgui::slider_float("custom fake", 0.0f, 360.0f, g_CVars.Miscellaneous.AntiAim.FakeValue);
          }
          zgui::checkbox("inattack pitch", g_CVars.Miscellaneous.AntiAim.Static);
          zgui::checkbox("lby breaker", g_CVars.Miscellaneous.AntiAim.lbybreaker);
          zgui::checkbox("manual", g_CVars.Miscellaneous.AntiAim.manuals);
          zgui::checkbox("enemy check", g_CVars.Miscellaneous.AntiAim.TurnOff);
          zgui::key_bind("left manual bind", g_CVars.Miscellaneous.AntiAim.LeftManualValue);
          zgui::key_bind("right manual bind", g_CVars.Miscellaneous.AntiAim.RightManualValue);
          zgui::key_bind("invert side", g_CVars.Miscellaneous.AntiAim.inverterkey);
          zgui::checkbox( "freestanding", g_CVars.Miscellaneous.AntiAim.WallDetection );
          zgui::combobox( "mode#1", std::vector<std::string> {"default", "peek fake", "peek jitter"}, g_CVars.Miscellaneous.AntiAim.WallDetectionMode );
          zgui::combobox( "at-targets", std::vector<std::string> {"off", "distance", "average"}, g_CVars.Miscellaneous.AntiAim.AtTargets );
      }
      zgui::end_groupbox( );
    }