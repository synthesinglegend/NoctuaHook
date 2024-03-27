#include "new gui/color.hpp"
#include <string>
#include <vector>
class n_config
{
public:
  n_config( const std::string path );
  void load_cfg( const std::string name );
  void save_cfg( const std::string name );
  void refresh_list( );
  bool unhook = false;
  std::vector<std::string> config_list;
  std::string config_name;
  int config_id = 0;
  struct
  {
    struct
    {
      bool name = true;
      bool box = true;
      bool aspectratio;
      bool wireframe;
      bool fov;
      bool fillbox;
      bool spread;
      bool armor;
      bool anginfo;
      bool money;
      bool fakelag;
      bool health = true;
      bool hitbox = true;
      bool weapon = true;
      bool bone;
      bool localesp;
      bool aimspot = true;
      bool hitmarker = true;
      bool enemyonly = true;
      bool active = true;
      bool fullbright;
      bool trails;
    }esp;
    struct
    {
      bool active;
      bool weapons;
      bool shadows;
      bool outline;
      bool handsoutl;
      bool enemeyonly;
      bool localchams;
      bool enemychams;
      bool weaponchams;
      bool handchams;
      struct
      {
        color ESPColor = color( 255, 255, 255, 255 );

        color LocalChamsColor = color( 255, 255, 255, 255 );

        color EnemyChamsColor = color( 255, 255, 255, 255 );

        color WeaponChamsColor = color( 255, 255, 255, 255 );

        color HandChamsColor = color( 255, 255, 255, 255 );
      };
      int ChamsChanger, ChamsMaterial, LocalMaterialType, EnemyMaterialType, WeaponMaterialType, HandMaterialType;
    }chams;
  }visuals;
};