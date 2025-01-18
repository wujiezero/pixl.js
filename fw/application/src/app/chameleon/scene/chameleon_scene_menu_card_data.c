#include "app_chameleon.h"
#include "chameleon_scene.h"

#include "mini_app_launcher.h"
#include "mini_app_registry.h"

#include "amiibo_helper.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

#include "i18n/language.h"
#include "settings.h"

#include "mui_icons.h"
#include "tag_emulation.h"
#include "tag_helper.h"

typedef enum {
    CHAMELEON_MENU_BACK,
    CHAMELEON_MENU_LOAD_DATA,
    CHAMELEON_MENU_SAVE_DATA,
    CHAMELEON_MENU_FACTORY,
} chameleon_menu_item_t;

void chameleon_scene_menu_card_data_on_event(mui_list_view_event_t event, mui_list_view_t *p_list_view,
                                             mui_list_item_t *p_item) {
    app_chameleon_t *app = p_list_view->user_data;
    chameleon_menu_item_t item = (chameleon_menu_item_t)p_item->user_data;
    switch (item) {
    case CHAMELEON_MENU_BACK:
        mui_scene_dispatcher_previous_scene(app->p_scene_dispatcher);
        break;
    case CHAMELEON_MENU_FACTORY: {
        uint8_t slot = tag_emulation_get_slot();
        tag_emulation_factory_data(slot, tag_helper_get_active_tag_type());
        tag_emulation_save();
        mui_toast_view_show(app->p_toast_view, _T(APP_CHAMELEON_CARD_DATA_FACTORY_SUCCESS));
        mui_scene_dispatcher_previous_scene(app->p_scene_dispatcher);
    } break;
    case CHAMELEON_MENU_LOAD_DATA: {
        mui_scene_dispatcher_next_scene(app->p_scene_dispatcher, CHAMELEON_SCENE_MENU_CARD_DATA_FILE_LOAD);
    } break;

    case CHAMELEON_MENU_SAVE_DATA: {
        mui_scene_dispatcher_next_scene(app->p_scene_dispatcher, CHAMELEON_SCENE_MENU_CARD_DATA_FILE_SAVE);
    } break;
    }
}

void chameleon_scene_menu_card_data_on_enter(void *user_data) {
    app_chameleon_t *app = user_data;

    mui_list_view_add_item(app->p_list_view, ICON_VIEW, _T(APP_CHAMELEON_CARD_DATA_LOAD), (void *)CHAMELEON_MENU_LOAD_DATA);
    mui_list_view_add_item(app->p_list_view, ICON_FAVORITE, _T(APP_CHAMELEON_CARD_DATA_SAVE), (void *)CHAMELEON_MENU_SAVE_DATA);
    mui_list_view_add_item(app->p_list_view, ICON_DATA, _T(APP_CHAMELEON_CARD_DATA_FACTORY), (void *)CHAMELEON_MENU_FACTORY);
    mui_list_view_add_item(app->p_list_view, ICON_BACK, _T(MAIN_RETURN), (void *)CHAMELEON_MENU_BACK);

    mui_list_view_set_selected_cb(app->p_list_view, chameleon_scene_menu_card_data_on_event);
    mui_view_dispatcher_switch_to_view(app->p_view_dispatcher, CHAMELEON_VIEW_ID_LIST);
}

void chameleon_scene_menu_card_data_on_exit(void *user_data) {
    app_chameleon_t *app = user_data;
    mui_list_view_clear_items(app->p_list_view);
}