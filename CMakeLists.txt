cmake_minimum_required(VERSION 3.27)

project(gtkpoly LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# setup gtkmm
find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-4.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

add_subdirectory(lib/nanopack)

add_library(gtkpoly STATIC
        src/application.cxx
        src/messages/create_window.np.cxx
        src/messages/invoke_callback.np.cxx
        src/messages/reply_from_callback.np.cxx
        src/messages/nanopack_message_factory.np.cxx
        src/messages/widgets/create_widget.np.cxx
        src/messages/widgets/update_widget.np.cxx
        src/messages/widgets/update_widgets.np.cxx
        src/messages/widgets/make_widget.np.cxx
        src/messages/widgets/widget.np.cxx
        src/messages/widgets/center.np.cxx
        src/messages/widgets/text.np.cxx
        src/messages/widgets/row.np.cxx
        src/messages/widgets/column.np.cxx
        src/messages/widgets/font_style.np.cxx
        src/messages/widgets/button/button.np.cxx
        src/messages/widgets/button/click_event.np.cxx
        src/messages/widgets/text_field/text_field.np.cxx
        src/messages/widgets/text_field/on_value_changed.np.cxx
        src/messages/widgets/list_view/list_view.np.cxx
        src/messages/widgets/list_view/list_view_operation.np.cxx
        src/messages/widgets/list_view/list_view_batch_operations.np.cxx
        src/messages/widgets/list_view/list_view_insert_operation.np.cxx
        src/messages/widgets/list_view/list_view_delete_operation.np.cxx
        src/messages/widgets/list_view/make_list_view_operation.np.cxx
        src/messages/widgets/list_view/list_view_item.np.cxx
        src/messages/widgets/list_view/list_view_item_config.np.cxx
        src/messages/request.np.cxx
        src/messages/ok_response.np.cxx
        src/window/window.cxx
        src/window/window_manager.cxx
        src/portable_layer.cxx
        src/widget/widget_factory.cxx
        src/widget/text.cxx
        src/widget/column.cxx
        src/widget/center.cxx
        src/widget/button.cxx
        src/widget/widget_registry.cxx
        src/widget/widget_updater.cxx
        src/widget/text_field.cxx
        src/widget/row.cxx
        src/widget/spacer.cxx
        src/widget/list_view.cxx
)
target_link_libraries(gtkpoly nanopack)
target_compile_definitions(gtkpoly PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")
target_include_directories(gtkpoly PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
