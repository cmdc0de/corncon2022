set(srcs "./app.cpp"
   "./appconfig.cpp"
	"./corncon22.cpp"
  "./menus/menu_state.cpp"
  "./menus/appbase_menu.cpp"
  "./menus/gui_list_processor.cpp"
  "./menus/setting_menu.cpp"
  "./menus/menu3d.cpp"
  "./menus/3d/renderer.cpp"
  "./menus/game_of_life.cpp"
)

idf_component_register(SRCS "${srcs}"
		INCLUDE_DIRS ../components/esp-etl/etl/include/ ../components/libesp
		)



