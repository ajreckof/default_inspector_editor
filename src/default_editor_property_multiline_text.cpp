#include "default_editor_property_multiline_text.h"
#include "godot_cpp/classes/h_box_container.hpp"
#include "godot_cpp/classes/font.hpp"

#include "editor_string_names.h"
#include "scene_string_names.h"
#include "godot_cpp/classes/editor_interface.hpp"



String EditorPropertyDefaultMultilineText::get_tooltip_string(const String &p_string) const {
	// Trim to 100 characters to prevent the tooltip from being too long.
	constexpr int TOOLTIP_MAX_LENGTH = 100;
	return p_string.left(TOOLTIP_MAX_LENGTH).strip_edges() + String((p_string.length() > TOOLTIP_MAX_LENGTH) ? "..." : "");
}

void EditorPropertyDefaultMultilineText::_set_read_only(bool p_read_only) {
	text->set_editable(!p_read_only);
	open_big_text->set_disabled(p_read_only);
}

void EditorPropertyDefaultMultilineText::_big_text_changed() {
	text->set_text(big_text->get_text());
	// Set tooltip so that the full text is displayed in a tooltip if hovered.
	// This is useful when using a narrow inspector, as the text can be trimmed otherwise.
	text->set_tooltip_text(get_tooltip_string(big_text->get_text()));
	emit_changed(get_edited_property(), big_text->get_text(), "", true);
}

void EditorPropertyDefaultMultilineText::_text_changed() {
	text->set_tooltip_text(get_tooltip_string(text->get_text()));
	emit_changed(get_edited_property(), text->get_text(), "", true);
}

void EditorPropertyDefaultMultilineText::_open_big_text() {
	if (!big_text_dialog) {
		big_text = memnew(TextEdit);
		if (expression) {
			big_text->set_syntax_highlighter(text->get_syntax_highlighter());
			big_text->add_theme_font_override(SceneStringName(font), get_theme_font(StringName("expression"), EditorStringName(EditorFonts)));
			big_text->add_theme_font_size_override(SceneStringName(font_size), get_theme_font_size(StringName("expression_size"), EditorStringName(EditorFonts)));
		}
		big_text->connect(SceneStringName(text_changed), callable_mp(this, &EditorPropertyDefaultMultilineText::_big_text_changed));
		big_text->set_line_wrapping_mode(TextEdit::LineWrappingMode::LINE_WRAPPING_BOUNDARY);
		big_text_dialog = memnew(AcceptDialog);
		big_text_dialog->add_child(big_text);
		big_text_dialog->set_title(tr("Edit Text:"));
		add_child(big_text_dialog);
	}

	big_text_dialog->popup_centered_clamped(Size2(1000, 900) * EditorInterface::get_singleton()->get_editor_scale(), 0.8);
	big_text->set_text(text->get_text());
	big_text->grab_focus();
}

void EditorPropertyDefaultMultilineText::_update_property() {
	String t = get_edited_object()->get(get_edited_property());
	if (text->get_text() != t) {
		text->set_text(t);
		text->set_tooltip_text(get_tooltip_string(t));
		if (big_text && big_text->is_visible_in_tree()) {
			big_text->set_text(t);
		}
	}
}

void EditorPropertyDefaultMultilineText::_notification(int p_what) {
	switch (p_what) {
		case NOTIFICATION_THEME_CHANGED:
		case NOTIFICATION_ENTER_TREE: {
			Ref<Texture2D> df = get_theme_icon(StringName("DistractionFree"), EditorStringName(EditorIcons));
			open_big_text->set_button_icon(df);

			Ref<Font> font;
			int font_size;
			if (expression) {
				font = get_theme_font(StringName("expression"), EditorStringName(EditorFonts));
				font_size = get_theme_font_size(StringName("expression_size"), EditorStringName(EditorFonts));

				text->add_theme_font_override(SceneStringName(font), font);
				text->add_theme_font_size_override(SceneStringName(font_size), font_size);
				if (big_text) {
					big_text->add_theme_font_override(SceneStringName(font), font);
					big_text->add_theme_font_size_override(SceneStringName(font_size), font_size);
				}
			} else {
				font = get_theme_font(SceneStringName(font), StringName("TextEdit"));
				font_size = get_theme_font_size(SceneStringName(font_size), StringName("TextEdit"));
			}
			text->set_custom_minimum_size(Vector2(0, font->get_height(font_size) * 6));
		} break;
	}
}


void EditorPropertyDefaultMultilineText::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_text_edit"), &EditorPropertyDefaultMultilineText::get_text_edit);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "text_edit", PROPERTY_HINT_RESOURCE_TYPE, "TextEdit"), "", "get_text_edit");
	
	ClassDB::bind_method(D_METHOD("get_fullscreen_text_edit"), &EditorPropertyDefaultMultilineText::get_fullscreen_text_edit);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "fullscreen_text_edit", PROPERTY_HINT_RESOURCE_TYPE, "TextEdit"), "", "get_fullscreen_text_edit");

	ClassDB::bind_method(D_METHOD("get_button_container"), &EditorPropertyDefaultMultilineText::get_button_container);
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "button_container", PROPERTY_HINT_RESOURCE_TYPE, "VBoxContainer"), "", "get_button_container");
}

TextEdit *EditorPropertyDefaultMultilineText::get_text_edit() {
	return text;
}

TextEdit *EditorPropertyDefaultMultilineText::get_fullscreen_text_edit() {
	return big_text;
}

VBoxContainer *EditorPropertyDefaultMultilineText::get_button_container() {
	return button_container;
}

EditorPropertyDefaultMultilineText::EditorPropertyDefaultMultilineText(bool p_expression) {
	HBoxContainer *hb = memnew(HBoxContainer);
	hb->add_theme_constant_override("separation", 0);
	add_child(hb);
	set_bottom_editor(hb);
	text = memnew(TextEdit);
	text->connect(SceneStringName(text_changed), callable_mp(this, &EditorPropertyDefaultMultilineText::_text_changed));
	text->set_line_wrapping_mode(TextEdit::LineWrappingMode::LINE_WRAPPING_BOUNDARY);
	add_focusable(text);
	hb->add_child(text);
	text->set_h_size_flags(SIZE_EXPAND_FILL);

	button_container = memnew(VBoxContainer);
	button_container->set_h_size_flags(SIZE_FILL);
	button_container->set_v_size_flags(SIZE_FILL);
	button_container->set_alignment(BoxContainer::AlignmentMode::ALIGNMENT_CENTER);

	hb->add_child(button_container);

	open_big_text = memnew(Button);
	open_big_text->set_flat(true);
	open_big_text->connect(SceneStringName(pressed), callable_mp(this, &EditorPropertyDefaultMultilineText::_open_big_text));
	button_container->add_child(open_big_text);

	big_text_dialog = nullptr;
	big_text = nullptr;
	if (p_expression) {
		expression = true;
		Ref<EditorSyntaxHighlighter> highlighter;
		highlighter.instantiate();
		text->set_syntax_highlighter(highlighter);
	}
}