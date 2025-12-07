#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"
#include "godot_cpp/classes/editor_property.hpp"
#include "godot_cpp/classes/text_edit.hpp"
#include "godot_cpp/classes/accept_dialog.hpp"
#include "godot_cpp/classes/button.hpp"
#include "godot_cpp/classes/translation_server.hpp"
#include "godot_cpp/classes/editor_syntax_highlighter.hpp"
#include "godot_cpp/classes/v_box_container.hpp"


using namespace godot;


class EditorPropertyDefaultMultilineText : public EditorProperty {
	GDCLASS(EditorPropertyDefaultMultilineText, EditorProperty);
	TextEdit *text = nullptr;

	AcceptDialog *big_text_dialog = nullptr;
	TextEdit *big_text = nullptr;
	Button *open_big_text = nullptr;
	VBoxContainer *button_container = nullptr;


	void _big_text_changed();
	void _text_changed();
	void _open_big_text();
	bool expression = false;
	String get_tooltip_string(const String &p_string) const;

protected:
void _notification(int p_what);

public:

	TextEdit *get_text_edit();
	TextEdit *get_fullscreen_text_edit();
	VBoxContainer *get_button_container();
	virtual void _set_read_only(bool p_read_only) override;
	virtual void _update_property() override;
	static void _bind_methods();
	EditorPropertyDefaultMultilineText(bool p_expression = false);
};